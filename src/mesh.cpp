#include<omp.h>
#include<set>
#include<iostream>
#include <utility>
#include <algorithm> 

#include "mesh.hpp"
#include "utils.hpp"
#include "types.hpp"

using namespace Eigen;
using namespace std;

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

bool Pair::operator<(const Pair& other) const{
    return this->cost < other.cost;
};

// // Used to find pairs with an edge between them 
// void Mesh::edgePairs() {
//     std::set<std::pair<int, int>> _pair_list;
//     auto max_index = faces().rows();
//     for (auto i = 0; i < max_index; i++){
//         const auto& row = faces().row(i);
//         for (auto j = 0; j < row.cols(); j++) {
//             const auto index_1{row(j)}, index_2{row((j+1)%3)};
//             const RowVectorD& vertex_1(verts().row(index_1));
//             const RowVectorD& vertex_2(verts().row(index_2));
//             auto result = _pair_list.insert(std::minmax(index_1, index_2));
//             if (result.second == false){
//                 continue;
//             }
//             _pairs.push_back(
//                 Pair{
//                     (size_t)index_1,
//                     (size_t)index_2,
//                     vertex_1,
//                     vertex_2,
//                     true,
//                     vertDistance(vertex_1, vertex_2)
//                 }
//             );
//         }
//     }
// }

// Find those valid pairs determined by euclidean distance threshold
// void Mesh::distancePairs(double t) {
//     auto max_index = verts().rows();    
//     // because we cant pre-allocate the vector we double #pragma 
//     #pragma omp parallel
//     {
//         // Initialise vector local to each thread
//         std::vector<Pair> local_pairs;  
//         #pragma omp for
//         for (auto i = 0; i < max_index; i++) {
//             for (auto j = 0; j < max_index; j++) {
//                 if (i != j) {
//                     const auto& v1(verts().row(i)), v2(verts().row(j));
//                     double distance{vertDistance(v1, v2)};
//                     if (distance <= t) {
//                         Pair new_pair{
//                             i,
//                             j,
//                             v1,
//                             v2,
//                             false,
//                             distance
//                         };
//                         bool _add{true};
//                         for (const auto& current_pair : pairs()) {
//                             if (new_pair == current_pair) {
//                                 _add = false;
//                                 break;
//                             }
//                         }
//                         if (_add) {
//                             local_pairs.push_back(new_pair);
//                         }
//                     }
//                 }
//             }
//         }
//         #pragma omp critical
//         {
//             _pairs.insert(
//                 _pairs.end(),
//                 local_pairs.begin(),
//                 local_pairs.end()
//             );
//         }
//     }
// }

QuadricMatrix Mesh::Kp(size_t face_index){
    QuadricMatrix _kp = QuadricMatrix::Zero();

    auto normal = _face_normals.row(face_index);

    float a = normal(0);
    float b = normal(1);
    float c = normal(2);
    float d = -1.0 * normal.dot(verts().row(faces().row(face_index)(0))); // sorry 

    _kp(0,0) = a*a; _kp(0,1) = a*b; _kp(0,2) = a*c; _kp(0,3) = a*d;
    _kp(1,0) = a*b; _kp(1,1) = b*b; _kp(1,2) = b*c; _kp(1,3) = b*d;
    _kp(2,0) = a*c; _kp(2,2) = b*c; _kp(2,2) = c*c; _kp(2,3) = c*d;
    _kp(3,0) = a*d; _kp(3,2) = b*d; _kp(3,2) = c*d; _kp(3,3) = d*d;

    return _kp;
};

QuadricMatrix Mesh::computeInitialQuadric(size_t vert_index){
    const auto& position = verts().row(vert_index);
    static auto max_face_index = faces().rows(); // number of faces
    QuadricMatrix quadric = QuadricMatrix::Zero(); // store quadric
    for (auto face_index=0; face_index < max_face_index; face_index++){
        const auto& triangle = faces().row(face_index);
        if ((vert_index == triangle(0) || vert_index == triangle(1) || vert_index == triangle(2))){
            auto kp = Kp(face_index);
            quadric += kp;
        }
    }
    return quadric;
}

std::vector<Pair> Mesh::findEdges(size_t vert_index){
    std::vector<Pair> vertex_pairs;
    static auto max_face_index = faces().rows();   
    for (auto face_index = 0; face_index < max_face_index; face_index++){
        const auto& triangle = faces().row(face_index);
        if ((vert_index == triangle(0) || vert_index == triangle(1) || vert_index == triangle(2))){
            for (auto _index : triangle){
                if (_index != vert_index){
                    RowVectorD vertex_1(verts().row(vert_index));
                    RowVectorD vertex_2(verts().row(_index));
                    vertex_pairs.push_back(
                        Pair{
                            (size_t)vert_index,
                            (size_t)_index,
                            vertex_1,
                            vertex_2,
                            true,
                            vertDistance(vertex_1, vertex_2)
                        }
                    );
                }
            }
        }
    }
    return vertex_pairs;
}

void Mesh::processVerts(double t){
    static auto max_vert_index = verts().rows();  // store n_rows of vert matrix
    omp_set_num_threads(8); // dont have a shit pc and its all good 
    #pragma omp parallel
    {   
        std::vector<Vertex> tid_verts;
        #pragma omp for
        for (auto vert_index = 0; vert_index < max_vert_index; vert_index++){
            std::vector<Pair> vertex_pairs;
            auto position = verts().row(vert_index); // find vertex
            auto quadric = computeInitialQuadric(vert_index); // compute initial vertex quadric 
            auto edge_pairs = findEdges(vert_index); // find edges for vertex            
            vertex_pairs.insert(
                vertex_pairs.end(), 
                edge_pairs.begin(),
                edge_pairs.end()
            );
            if (t>0){
                // not implemented yet
            }
            Vertex _vert{
                vert_index,
                position,
                vertex_pairs,
                quadric
            };
            tid_verts.push_back(_vert);
        }
        #pragma omp critical
        {
            _vertices.insert(
                _vertices.end(),
                tid_verts.begin(), 
                tid_verts.end()
            );
        }
    }
};

// 1015 milliseconds
// void Mesh::processVerts(double t){
//     static auto max_vert_index = verts().rows();  // store n_rows of vert matrix
//     for (auto vert_index = 0; vert_index < max_vert_index; vert_index++){
//         std::vector<Pair> vertex_pairs;
//         auto position = verts().row(vert_index); // find vertex
//         auto quadric = computeInitialQuadric(vert_index); // compute initial vertex quadric 
//         auto edge_pairs = findEdges(vert_index); // find edges for vertex            
//         vertex_pairs.insert(
//             vertex_pairs.end(), 
//             edge_pairs.begin(),
//             edge_pairs.end()
//         );
//         if (t>0){
//             // not implemented yet
//         }
//         Vertex _vert{
//             vert_index,
//             position,
//             vertex_pairs,
//             quadric
//         };
//         _vertices.push_back(_vert);
//     }
// };