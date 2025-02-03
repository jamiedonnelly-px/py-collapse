// #include<pybind11/pybind11.h>
// #include<pybind11/eigen.h>
//namespace py = pybind11;

#include<omp.h>
#include<iostream>
#include<Eigen/Core>
#include <chrono>

#include "pairs.hpp"
#include "mesh.hpp"
#include "types.hpp"
#include "io.hpp"

using namespace std;

int main(){

    auto pair = readOBJ("/home/jamie.donnelly/py-collapse/data/ankylosaurus.obj");

    const auto& verts = pair.first;
    const auto& faces = pair.second;

    Mesh mesh = Mesh(verts, faces);

    auto start = std::chrono::high_resolution_clock::now();
    mesh.findPairs();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    cout << mesh.pairs().size() << endl;

    cout << "Successfully ran main!" << endl;

    return 0;
}
