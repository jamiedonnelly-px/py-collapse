#ifndef IO_H
#define IO_H

#include "types.hpp"

std::pair<VertMatrix, FaceMatrix> readOBJ(const std::string& filename);

#endif