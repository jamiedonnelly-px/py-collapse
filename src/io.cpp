#include <Eigen/Core>
#include <Eigen/Dense>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "io.hpp"


std::pair<VertMatrix, FaceMatrix> readOBJ(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<Eigen::Vector3d> vertices;
    std::vector<Eigen::Vector3i> faces;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {  // Vertex
            double x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(Eigen::Vector3d(x, y, z));
        }
        else if (type == "f") {  // Face
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            
            // Handle both formats: "1" and "1/1/1"
            auto getVertexIndex = [](const std::string& str) -> int {
                return std::stoi(str.substr(0, str.find("/"))) - 1;  // OBJ is 1-based
            };

            faces.push_back(Eigen::Vector3i(
                getVertexIndex(v1),
                getVertexIndex(v2),
                getVertexIndex(v3)
            ));
        }
    }

    // Convert to Eigen matrices
    VertMatrix V(vertices.size(), 3);
    for (size_t i = 0; i < vertices.size(); ++i) {
        V.row(i) = vertices[i];
    }

    FaceMatrix F(faces.size(), 3);
    for (size_t i = 0; i < faces.size(); ++i) {
        F.row(i) = faces[i];
    }

    return {V, F};
}