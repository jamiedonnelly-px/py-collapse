// #include<pybind11/pybind11.h>
// #include<pybind11/eigen.h>
//namespace py = pybind11;

#include<omp.h>
#include<iostream>
#include<Eigen/Core>
#include<Eigen/Dense>
#include <chrono>

#include "mesh.hpp"
#include "types.hpp"
#include "io.hpp"

using namespace std;

int main(){

    cout << EIGEN_WORLD_VERSION << "." << EIGEN_MAJOR_VERSION << "." << EIGEN_MINOR_VERSION << endl;

    std::pair<VertMatrix, FaceMatrix> pair = readOBJ("/home/jamie.donnelly/py-collapse/data/ankylosaurus.obj");

    const VertMatrix& verts = pair.first;
    const FaceMatrix& faces = pair.second;

    cout << "Number of rows: " <<  verts.rows() << endl;

    auto start = chrono::high_resolution_clock::now();
    Mesh mesh = Mesh(verts, faces);
    mesh.processVerts();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    cout << "Number of verts: " << mesh.vertList().size() << endl;
    cout << "Successfully ran main!" << endl;

    return 0;
}
