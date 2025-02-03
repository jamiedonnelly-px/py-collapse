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

void Mesh::truePairs(){
    auto max_index = faces().rows();
    size_t total_pairs = max_index * 3; // 3 pairs per face
    // pre-allocate the sizes vector
    #pragma omp single
    {
        _pairs.resize(total_pairs);
    }
    // calculate edge list here 
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        std::pair<size_t, size_t> tid_limits = partitionIndices(num_threads, tid, max_index);
        size_t pairs_start = tid_limits.first * 3;
        size_t pairs_idx = pairs_start; 
        for (auto i=tid_limits.first; i < tid_limits.second; i++){
            const auto& row = faces().row(i);
            for (auto j=0; j < row.cols(); j++){
                const auto& index_1{row(j)}, index_2{row((j+1)%3)};
                const RowVectorD& vertex_1(verts().row(index_1)), vertex_2(verts().row(index_2));
                _pairs[pairs_idx++] = Pair{
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
};

void Mesh::distancePairs(double t){
    // I could try unify with truePairs() to make an interface
    auto max_index = verts().rows();
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        std::pair<size_t, size_t> tid_limits = partitionIndices(num_threads, tid, max_index);
        std::vector<Pair> tmp_pairs;
        for (auto i=tid_limits.first; i < tid_limits.second; i++){
            for (auto j=0; j < max_index; j++){
                if (i != j){
                    const auto& v1(verts().row(i)), v2(verts().row(j));
                    double distance{vertDistance(v1, v2)};
                    if (distance <= t){
                        Pair new_pair{
                            i,
                            j,
                            v1,
                            v2,
                            false,
                            distance
                        };
                        bool _add{true};
                        for (const auto& current_pair : pairs()){
                            if (new_pair == current_pair){
                                _add = false;
                                break;
                            }
                        }
                        if (_add){
                            tmp_pairs.push_back(new_pair);
                        }
                    }
                }
            }
        }
        #pragma omp critical
        {
            _pairs.insert(
                _pairs.end(), 
                tmp_pairs.begin(), 
                tmp_pairs.end()
            );
        }
    }
};

void Mesh::getUnique() {
    std::set<Pair> s(_pairs.begin(), _pairs.end());
    _pairs = std::vector<Pair>(s.begin(), s.end());
};

void Mesh::findPairs(double t){
    truePairs();
    if (t > 0){
        distancePairs(t);
    }
    getUnique();
};