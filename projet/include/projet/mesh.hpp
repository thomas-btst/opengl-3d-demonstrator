#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "vertex.hpp"
#include "shader.hpp"

enum struct MeshLoadingMode {Triangle, Polygon};

class Mesh {
    // Render data
    unsigned int VAO, VBO, EBO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    void loadTriangle(std::stringstream& stream, std::vector<unsigned>& tmpTexIndices, std::vector<unsigned>& tmpNormalIndices);
    void loadPolygon(std::stringstream& stream, std::vector<unsigned>& tmpTexIndices, std::vector<unsigned>& tmpNormalIndices);

    void computeNormals();
    void setupMesh();

    public:
        // Mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;

        Mesh() = delete;
        Mesh(const char* filename, MeshLoadingMode mode = MeshLoadingMode::Triangle);

        void draw(GLenum mode = GL_FILL);

        static Mesh Rectangle(float width, float height);
        static Mesh Cube(float side);
        static Mesh Cylinder(float radius, float height, int segments);
        static Mesh Sphere(float radius, int longitude_segments, int latitude_segments);
        static Mesh Cone(float radius, float height, int segments);
};

#endif