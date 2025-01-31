// #include<pybind11/pybind11.h>
// #include<pybind11/eigen.h>
//namespace py = pybind11;

#include "pairs.hpp"
#include "mesh.hpp"
#include "types.hpp"

int main(){

    int n_verts = 5;
    VertMatrix verts = Eigen::MatrixXd::Random(n_verts, 3);
    FaceMatrix faces = Eigen::MatrixXi::Random(n_verts * 2, 3);
    Mesh mesh = Mesh(verts, faces);

    _pair_test(mesh);

    return 0;
}
