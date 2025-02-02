// #include<pybind11/pybind11.h>
// #include<pybind11/eigen.h>
//namespace py = pybind11;

#include<iostream>
#include<Eigen/Core>

#include "pairs.hpp"
#include "mesh.hpp"
#include "types.hpp"

int main(){

    int n_verts = 5;
    VertMatrix verts = Eigen::MatrixXd::Random(n_verts, 3);
    FaceMatrix faces = Eigen::MatrixXi::Random(n_verts * 2, 3);
    Mesh mesh = Mesh(verts, faces);

    std::cout << "Eigen version: " 
              << EIGEN_WORLD_VERSION << "."
              << EIGEN_MAJOR_VERSION << "."
              << EIGEN_MINOR_VERSION << std::endl;


    return 0;
}
