#include<vector>

#include "mesh.hpp"
#include "types.hpp"

class IndexPair{
    public:
        IndexPair() : vert1() {};
        bool operator==(IndexPair& other);

    private:
        RowVectorD vert1; 
        RowVectorD vert2;

};

class VertexPair{

    public:
        VertexPair(RowVectorD v1, RowVectorD v2) : _v1(v1), _v2(v2){};
        int i1, i2;

    private:
        RowVectorD _v1;
        RowVectorD _v2;
};

std::vector<VertexPair> vertex_pairs(Mesh& mesh);

void _pair_test(Mesh& mesh);