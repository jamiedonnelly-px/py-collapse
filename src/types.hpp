#ifndef COLLAPSE_TYPES_H
#define COLLAPSE_TYPES_H

#include<Eigen/Core>

typedef Eigen::Matrix<double, Eigen::Dynamic, 3> VertMatrix;
typedef Eigen::Matrix<int, Eigen::Dynamic, 3> FaceMatrix;
typedef Eigen::Matrix<double, 1, 3> RowVectorD;
typedef Eigen::Matrix<int, 1, 3> RowVectorI;

#endif