#ifndef MESH_H
#define MESH_H  

#include<Eigen/Core>
#include<Eigen/Dense>
#include<vector>
#include<set>

#include "types.hpp"
#include "utils.hpp"

struct Pair{    
    size_t vertex_1_index, vertex_2_index;
    RowVectorD vertex_1; 
    RowVectorD vertex_2;
    bool hasEdge; // if this is a true edge
    double distance; // distance between verts 
    double cost; // this will be used to sort the pairs 

    // Constructor that sorts indices
    Pair(size_t v1_idx, size_t v2_idx, 
         RowVectorD v1, RowVectorD v2,
         bool has_edge = false, double dist = 0.0, double c = 0.0) 
        : vertex_1(v1_idx < v2_idx ? v1 : v2)
        , vertex_2(v1_idx < v2_idx ? v2 : v1)
        , hasEdge(has_edge)
        , distance(dist)
        , cost(c) {
        vertex_1_index = std::min(v1_idx, v2_idx);
        vertex_2_index = std::max(v1_idx, v2_idx);
    }
    bool operator==(const Pair& other) const;
    bool operator!=(const Pair& other) const;
    bool operator<(const Pair& other) const;
};

struct Vertex{
    size_t index;
    RowVectorD position;
    std::vector<Pair> pairs; // set of valid pairs associated
    QuadricMatrix quadric;
};

class Mesh{
    public:
        Mesh(VertMatrix v, FaceMatrix f) : _vert_matrix(v), _face_matrix(f){
            _face_normals = faceNormals(v, f);
        }
        ~Mesh(){} // eigen will cleanup;
        const VertMatrix& verts() const { return _vert_matrix; }
        const FaceMatrix& faces() const { return _face_matrix; }
        const std::vector<Pair>& pairs() const { return _pairs; }
        const std::vector<Vertex>& vertList() const { return _vertices; }
        void setVerts(VertMatrix v){
            _vert_matrix = v;
        };
        void setFaces(FaceMatrix f){
            _face_matrix = f;
        };
        void processVerts(double t=0);

    private:
        VertMatrix _vert_matrix;
        FaceMatrix _face_matrix;
        VertMatrix _face_normals;
        std::vector<Pair> _pairs;
        std::vector<Vertex> _vertices;
        // void edgePairs(); // finds all edges
        // std::vector<Pair> distancePairs(double t=0); // pairs by distance t
        // void getUnique(); // removes duplicated edges
        QuadricMatrix Kp(size_t face_index);
        std::vector<Pair> findEdges(size_t vert_index);
        QuadricMatrix computeInitialQuadric(size_t vert_index);
};


#endif