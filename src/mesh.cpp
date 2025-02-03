#include<omp.h>
#include<set>
#include<iostream>

#include "mesh.hpp"
#include "utils.hpp"

using namespace Eigen;
using namespace std;

// this is not a valid sorting, just to determine uniqueness. 
bool Pair::operator<(const Pair& other) const {
    return distance < other.distance;
}

bool Pair::operator==(const Pair& other) const{
    bool indices_match = (vertex_1_index == other.vertex_1_index && 
                        vertex_2_index == other.vertex_2_index) ||
                        (vertex_1_index == other.vertex_2_index && 
                        vertex_2_index == other.vertex_1_index);

    const double epsilon = 1e-10;
    bool vertices_match = (vertex_1.isApprox(other.vertex_1, epsilon) && 
                            vertex_2.isApprox(other.vertex_2, epsilon)) ||
                        (vertex_1.isApprox(other.vertex_2, epsilon) && 
                            vertex_2.isApprox(other.vertex_1, epsilon));

    return indices_match && vertices_match;
};

bool Pair::operator!=(const Pair& other) const{
    return !(*this == other);
};

void Mesh::truePairs() {
    auto max_index = faces().rows();
    size_t total_pairs = max_index * 3;  // 3 pairs per face
    
    // Pre-allocate the vector
    _pairs.resize(total_pairs);
    
    #pragma omp parallel for
    for (size_t i = 0; i < max_index; i++) {
        const auto& row = faces().row(i);
        size_t pairs_idx = i * 3;  // Each face gets 3 consecutive pairs
        
        for (auto j = 0; j < row.cols(); j++) {
            const auto& index_1{row(j)}, index_2{row((j+1)%3)};
            const RowVectorD& vertex_1(verts().row(index_1)), vertex_2(verts().row(index_2));
            _pairs[pairs_idx + j] = Pair{
                index_1,
                index_2,
                vertex_1,
                vertex_2,
                true,
                vertDistance(vertex_1, vertex_2)
            };
        }
    }
}

void Mesh::distancePairs(double t) {
    auto max_index = verts().rows();    
    #pragma omp parallel
    {
        std::vector<Pair> local_pairs;  // Local to each thread
        #pragma omp for
        for (size_t i = 0; i < max_index; i++) {
            for (auto j = 0; j < max_index; j++) {
                if (i != j) {
                    const auto& v1(verts().row(i)), v2(verts().row(j));
                    double distance{vertDistance(v1, v2)};
                    if (distance <= t) {
                        Pair new_pair{
                            i,
                            j,
                            v1,
                            v2,
                            false,
                            distance
                        };
                        bool _add{true};
                        for (const auto& current_pair : pairs()) {
                            if (new_pair == current_pair) {
                                _add = false;
                                break;
                            }
                        }
                        if (_add) {
                            local_pairs.push_back(new_pair);
                        }
                    }
                }
            }
        }
        
        #pragma omp critical
        {
            _pairs.insert(
                _pairs.end(),
                local_pairs.begin(),
                local_pairs.end()
            );
        }
    }
}

void Mesh::getUnique() {
    // First parallel sort
    #pragma omp parallel
    {
        #pragma omp single
        {
            std::sort(_pairs.begin(), _pairs.end());
        }
    }
    
    // Then parallel unique
    std::vector<Pair> unique_pairs;
    #pragma omp parallel
    {
        std::vector<Pair> thread_unique;
        bool print{true};
        #pragma omp for
        for(size_t i = 0; i < _pairs.size(); i++) {
            #pragma omp critical
            {   
                if (print){
                    cout << omp_get_thread_num() << " start index " << i << endl;
                }
                print = false;
            }
            if(i == 0 || !(_pairs[i] == _pairs[i-1])) {
                thread_unique.push_back(_pairs[i]);
            }
        }
        
        #pragma omp critical
        {
            unique_pairs.insert(
                unique_pairs.end(), 
                thread_unique.begin(), 
                thread_unique.end()
            );
        }
    }
    _pairs = std::move(unique_pairs);
}

void Mesh::findPairs(double t){
    truePairs();
    if (t > 0){
        distancePairs(t);
    }
    getUnique();
};