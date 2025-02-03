// #include<pybind11/pybind11.h>
// #include<pybind11/eigen.h>
//namespace py = pybind11;

#include<omp.h>
#include<iostream>
#include<Eigen/Core>

#include "pairs.hpp"
#include "mesh.hpp"
#include "types.hpp"
#include "io.hpp"

using namespace std;

int main(){

    auto pair = readOBJ("/home/jamie.donnelly/py-collapse/data/bed.obj");

    const auto& verts = pair.first;
    const auto& faces = pair.second;

    Mesh mesh = Mesh(verts, faces);

    mesh.distancePairs(0.1);

    cout << mesh.pairs().size() << endl;

    cout << "Successfully ran main!" << endl;

    return 0;
}
