#ifndef MESH_H
#define MESH_H  

#include<Eigen/Core>
#include<vector>

#include "types.hpp"

struct Vertex{
    size_t index;
    RowVectorD position;
};

struct Edge{    
    size_t vertex_1_index, vertex_2_index;
    RowVectorD vertex_1, vertex_2;
};
class Mesh{
    public:
        Mesh(VertMatrix v, FaceMatrix f) : _verts(v), _faces(f){}
        ~Mesh(){} // eigen will cleanup;
        const VertMatrix& verts() const { return _verts; }
        const FaceMatrix& faces() const { return _faces; }
        void setVerts(VertMatrix v){
            _verts = v;
        };
        void setFaces(FaceMatrix f){
            _faces = f;
        };
        void calculateEdges();

    private:
        VertMatrix _verts;
        FaceMatrix _faces;
        std::vector<Edge> edges;
};

#endif