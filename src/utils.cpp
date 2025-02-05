#include <omp.h>
#include<iostream>
#include<chrono>

#include "utils.hpp"

using namespace std;
using namespace Eigen::placeholders;

double vertDistance(const RowVectorD& r1, const RowVectorD& r2){
    // compute distance between vectors
    return sqrt((r1 - r2).transpose().dot((r1-r2)));
}

VertMatrix faceNormals(const VertMatrix& verts, const FaceMatrix& faces) {
    // compute the face normals during matrix initialisation;
    VertMatrix normals(faces.rows(), 3);
    
    const auto t1 = verts(faces.col(0).array(), all);
    const auto t2 = verts(faces.col(1).array(), all);
    const auto t3 = verts(faces.col(2).array(), all);
    
    // edge matrices
    auto U = (t2 - t1);
    auto V = (t3 - t1);

    // plane normals
    normals.col(0) = U.col(1).array() * V.col(2).array() - U.col(2).array() * V.col(1).array();
    normals.col(1) = U.col(2).array() * V.col(0).array() - U.col(0).array() * V.col(2).array();
    normals.col(2) = U.col(0).array() * V.col(1).array() - U.col(1).array() * V.col(0).array();
    
    normals.rowwise().normalize();

    return normals;
}