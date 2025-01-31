#ifndef MESH_H
#define MESH_H  

#include<Eigen/Core>

#include "types.hpp"

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

    private:
        VertMatrix _verts;
        FaceMatrix _faces;
};

#endif