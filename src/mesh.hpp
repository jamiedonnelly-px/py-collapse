#ifndef MESH_H
#define MESH_H  

#include<Eigen/Core>
#include<vector>

#include "types.hpp"

struct Pair{    
    size_t vertex_1_index, vertex_2_index;
    RowVectorD vertex_1, vertex_2;
    bool hasEdge; // if this is a true edge
    double distance; // distance between verts 
    // ASIDE:
    // there's something to this above ^^ 
    // keep this idea of edge distance in mind
    bool operator==(const Pair& other) const;
    bool operator!=(const Pair& other) const;
    bool operator<(const Pair& other) const;
};
struct Vertex{
    size_t index;
    RowVectorD position;
    std::vector<Pair> pairs; // set of valid pairs associated
};

class Mesh{
    public:
        Mesh(VertMatrix v, FaceMatrix f) : _verts(v), _faces(f){}
        ~Mesh(){} // eigen will cleanup;
        const VertMatrix& verts() const { return _verts; }
        const FaceMatrix& faces() const { return _faces; }
        const std::vector<Pair>& pairs() const { return _pairs; }
        void setVerts(VertMatrix v){
            _verts = v;
        };
        void setFaces(FaceMatrix f){
            _faces = f;
        };
        void findPairs(double t=0);

    private:
        VertMatrix _verts;
        FaceMatrix _faces;
        std::vector<Pair> _pairs;
        void truePairs();
        void distancePairs(double t=0);
        void getUnique();
};

#endif