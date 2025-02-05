#ifndef UTILS_H
#define UTILS_H

#include<omp.h>
#include<utility>
#include<vector>
#include<Eigen/Core>
#include<Eigen/Dense>

#include "types.hpp"

using namespace Eigen;

double vertDistance(const RowVectorD& r1, const RowVectorD& r2);

VertMatrix faceNormals(const VertMatrix& verts, const FaceMatrix& faces);

#endif