#include<iostream>

#include "pairs.hpp"


std::vector<VertexPair> valid_pairs(Mesh mesh){
    // implementing pair-finding without tolerenace; simple edge contraction
    const auto& verts = mesh.verts();
    const auto& faces = mesh.faces();

    std::vector<VertexPair> all_pairs;

    


    for (int i; i < faces.rows(); i++){
        RowVectorI face = faces.row(i);
        std::cout << face << std::endl;
        // for (int j; j<3; j++){
        //     all_pairs.push_back();
        // }
    };

    // for (int i; i < verts.rows(); i++ ){
    //     const auto& row = verts.row(i);
    // };
};

// tester print function
void pairs_test(Mesh mesh){
    const auto& verts = mesh.verts();
    for (int i; i < verts.rows(); i++ ){
        const auto& row = verts.row(i);
        std::cout << row << std::endl;
    };
};


