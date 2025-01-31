#include<iostream>

#include "pairs.hpp"


bool IndexPair::operator==(IndexPair& other);

std::vector<ValidPair> valid_pairs(Mesh mesh){
    // implementing pair-finding without tolerenace; simple edge contraction
    const auto& verts = mesh.verts();
    const auto& faces = mesh.faces();

    

    for (int i; i < faces.rows(); i++){
        
    };

    for (int i; i < verts.rows(); i++ ){
        const auto& row = verts.row(i);
    };
};

// tester print function
void pairs_test(Mesh mesh){
    const auto& verts = mesh.verts();
    for (int i; i < verts.rows(); i++ ){
        const auto& row = verts.row(i);
        std::cout << row << std::endl;
    };
};


