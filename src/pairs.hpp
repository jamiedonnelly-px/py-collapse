#include<vector>

#include "mesh.hpp"
#include "types.hpp"

class IndexPair{

    public:
        bool operator==(IndexPair& other);
}

class ValidPair{

    public:
        ValidPair(RowVectorD v1, RowVectorD v2) : _v1(v1), _v2(v2){};
        int i1, i2;

    private:
        RowVectorD _v1;
        RowVectorD _v2;
};

std::vector<ValidPair> valid_pairs(Mesh& mesh);

void _pair_test(Mesh& mesh);