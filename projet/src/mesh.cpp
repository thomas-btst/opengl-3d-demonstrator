#include "../include/projet/mesh.hpp"

#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../include/projet/config.hpp"
#include "glm/fwd.hpp"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices)
 : vertices(_vertices), indices(_indices)
{
    setupMesh();
}

Mesh::Mesh(const char* filename, MeshLoadingMode mode)
 : vertices(), indices()
{
    std::cout << "Ouverture du fichier objet " << filename << std::endl; 
    // Read object file
    std::ifstream file(std::string(_resources_directory).append("objects/").append(filename));
    if (!file.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier \"" << filename << "\"." << std::endl;
        return;
    }
    std::vector<glm::vec2> tmpTexCoord;
    std::vector<glm::vec3> tmpNormal;
    std::vector<unsigned> tmpTexIndices, tmpNormalIndices;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream stream(line);
        char c = stream.get();
        switch (c) {
            case 'v': {
                char c2 = stream.get();
                switch (c2){
                    case ' ': {
                        float x, y, z;
                        stream >> x >> y >> z;
                        vertices.push_back(Vertex{
                            glm::vec3(x, y, z),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                        });
                        break;
                    }
                    case 't': {
                        float x, y;
                        stream >> x >> y;
                        tmpTexCoord.push_back(glm::vec2(x, y));
                        break;
                    }
                    case 'n': {
                        float x, y, z;
                        stream >> x >> y >> z;
                        tmpNormal.push_back(glm::vec3(x, y, z));
                        break;
                    }
                    default: std::cerr << "Avertissement lors de la lecture du fichier: Opérateur '" << c << c2 << "' inconnu." << std::endl;
                }
                break;
            }
            case 'f': {
                switch (mode) {
                    case MeshLoadingMode::Triangle: loadTriangle(stream, tmpTexIndices, tmpNormalIndices);
                        break;
                    case MeshLoadingMode::Polygon: loadPolygon(stream, tmpTexIndices, tmpNormalIndices);
                        break;
                }
                break;
            }
            case 'm': {
                std::string tllib;
                stream >> tllib;
                if ('m'+tllib != "mtllib") {
                    std::cerr << "Avertissement lors de la lecture du fichier: Opérateur '" << c << tllib << "' inconnu." << std::endl;
                }
                break;
            }
            case 'u': break;
            case 'o': {
                // std::string name;
                // stream >> name;
                // std::cout << "Loading '" << name << "' object." << std::endl;
                break;
            }
            case '\n': break;
            case '\r': break;
            case '#': break;
            default: std::cerr << "Avertissement lors de la lecture du fichier: Opérateur '" << c << "' inconnu." << std::endl;
        }
    }
    // Compute tex coords
    for (size_t i = 0; i < tmpTexIndices.size(); i++) {
        vertices[indices[i]].TexCoords = tmpTexCoord[tmpTexIndices[i]];
    }

    if (tmpNormalIndices.empty())
        computeNormals();
    else
        for (size_t i = 0; i < tmpNormalIndices.size(); i++)
            vertices[indices[i]].Normal = tmpNormal[tmpNormalIndices[i]];

    setupMesh();
}

void Mesh::loadTriangle(std::stringstream& stream, std::vector<unsigned>& tmpTexIndices, std::vector<unsigned>& tmpNormalIndices) {
    unsigned int indice;
    for (unsigned i = 0; i < 3; i++) {
        stream >> indice;
        indices.push_back(--indice);
        if (stream.peek() == '/') {
            char sep;
            unsigned int texCoord;
            stream >> sep >> texCoord;
            tmpTexIndices.push_back(--texCoord);
            if (stream.peek() == '/') {
                unsigned int normal;
                stream >> sep >> normal;
                tmpNormalIndices.push_back(--normal);
            }
        }
    }
}

void Mesh::loadPolygon(std::stringstream& stream, std::vector<unsigned>& tmpTexIndices, std::vector<unsigned>& tmpNormalIndices) {
    unsigned int indice;
    std::vector<unsigned int> faceIndices;
    for (unsigned i = 0; i < 4; i++) {
        stream >> indice;
        faceIndices.push_back(--indice);
        if (stream.peek() == '/') {
            char sep;
            unsigned int texCoord;
            stream >> sep >> texCoord;
            tmpTexIndices.push_back(--texCoord);
            if (stream.peek() == '/') {
                unsigned int normal;
                stream >> sep >> normal;
                tmpNormalIndices.push_back(--normal);
            }
        }
    }

    indices.push_back(faceIndices[0]);
    indices.push_back(faceIndices[1]);
    indices.push_back(faceIndices[2]);

    indices.push_back(faceIndices[0]);
    indices.push_back(faceIndices[2]);
    indices.push_back(faceIndices[3]);

    const unsigned tmp = tmpTexIndices[tmpTexIndices.size() - 1];
    tmpTexIndices[tmpTexIndices.size() - 1] = tmpTexIndices[tmpTexIndices.size() - 4];
    tmpTexIndices.push_back(tmpTexIndices[tmpTexIndices.size()-2]);
    tmpTexIndices.push_back(tmp);

    const unsigned tmpNorm = tmpNormalIndices[tmpNormalIndices.size() - 1];
    tmpNormalIndices[tmpNormalIndices.size() - 1] = tmpNormalIndices[tmpNormalIndices.size() - 4];
    tmpNormalIndices.push_back(tmpNormalIndices[tmpNormalIndices.size()-2]);
    tmpNormalIndices.push_back(tmpNorm);
}

void Mesh::computeNormals() {
    // Compute normal
    for (unsigned int i = 0; i < indices.size(); i += 3) {
        unsigned int i1 = indices[i];
        unsigned int i2 = indices[i+1];
        unsigned int i3 = indices[i+2];

        // Vertices
        glm::vec3 v1 = vertices[i1].Position;
        glm::vec3 v2 = vertices[i2].Position;
        glm::vec3 v3 = vertices[i3].Position;

        // Compute normal
        glm::vec3 n = glm::cross(v3 - v1, v3 - v2);

        vertices[i1].Normal += n;
        vertices[i2].Normal += n;
        vertices[i3].Normal += n;
    }

    // Normalize
    for (Vertex& vertex: vertices) {
        vertex.Normal = glm::normalize(vertex.Normal);
    }
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex tex coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}


void Mesh::draw(GLenum mode) 
{
    glBindVertexArray(VAO);

    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

Mesh Mesh::Rectangle(float width, float height) {
    std::vector<Vertex> vertices;
    for (float x = -0.5; x < 1; x++)
        for (float y = -0.5; y < 1; y++) {
            vertices.push_back(Vertex{
                glm::vec3(x*width, y*height, 0.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f)
            });
            vertices.push_back(Vertex{
                glm::vec3(x*width, y*height, 0.001f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f)
            });
        }
            

    std::vector<unsigned> indices = {
        0, 2, 4,
        1, 3, 5,
        6, 2, 4,
        7, 3, 5,
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::Cube(float side) {
    float halfSide = side / 2;
    std::vector<Vertex> vertices = {
        Vertex{glm::vec3(-halfSide, -halfSide, -halfSide), glm::vec3(0.0f,  0.0f, -1.0f)},
        Vertex{glm::vec3(halfSide, -halfSide, -halfSide), glm::vec3(0.0f,  0.0f, -1.0f)},
        Vertex{glm::vec3(halfSide,  halfSide, -halfSide), glm::vec3(0.0f,  0.0f, -1.0f)},
        Vertex{glm::vec3(-halfSide,  halfSide, -halfSide), glm::vec3(0.0f,  0.0f, -1.0f)},

        Vertex{glm::vec3(-halfSide, -halfSide,  halfSide), glm::vec3(0.0f,  0.0f, 1.0f)},
        Vertex{glm::vec3(halfSide, -halfSide,  halfSide), glm::vec3(0.0f,  0.0f, 1.0f)},
        Vertex{glm::vec3(halfSide,  halfSide,  halfSide), glm::vec3(0.0f,  0.0f, 1.0f)},
        Vertex{glm::vec3(-halfSide,  halfSide,  halfSide), glm::vec3(0.0f,  0.0f, 1.0f)},

        Vertex{glm::vec3(-halfSide,  halfSide,  halfSide), glm::vec3(-1.0f,  0.0f,  0.0f)},
        Vertex{glm::vec3(-halfSide,  halfSide, -halfSide), glm::vec3(-1.0f,  0.0f,  0.0f)},
        Vertex{glm::vec3(-halfSide, -halfSide, -halfSide), glm::vec3(-1.0f,  0.0f,  0.0f)},
        Vertex{glm::vec3(-halfSide, -halfSide,  halfSide), glm::vec3(-1.0f,  0.0f,  0.0f)},

        Vertex{glm::vec3(halfSide,  halfSide,  halfSide), glm::vec3(1.0f,  0.0f,  0.0f)},
        Vertex{glm::vec3(halfSide,  halfSide, -halfSide), glm::vec3(1.0f,  0.0f,  0.0f)},
        Vertex{glm::vec3(halfSide, -halfSide, -halfSide), glm::vec3(1.0f,  0.0f,  0.0f)},
        Vertex{glm::vec3(halfSide, -halfSide,  halfSide), glm::vec3(1.0f,  0.0f,  0.0f)},

        Vertex{glm::vec3(-halfSide, -halfSide, -halfSide), glm::vec3(0.0f, -1.0f,  0.0f)},
        Vertex{glm::vec3(halfSide, -halfSide, -halfSide), glm::vec3(0.0f, -1.0f,  0.0f)},
        Vertex{glm::vec3(halfSide, -halfSide,  halfSide), glm::vec3(0.0f, -1.0f,  0.0f)},
        Vertex{glm::vec3(-halfSide, -halfSide,  halfSide), glm::vec3(0.0f, -1.0f,  0.0f)},

        Vertex{glm::vec3(-halfSide,  halfSide, -halfSide), glm::vec3(0.0f,  1.0f,  0.0f)},
        Vertex{glm::vec3(halfSide,  halfSide, -halfSide), glm::vec3(0.0f,  1.0f,  0.0f)},
        Vertex{glm::vec3(halfSide,  halfSide,  halfSide), glm::vec3(0.0f,  1.0f,  0.0f)},
        Vertex{glm::vec3(-halfSide,  halfSide,  halfSide), glm::vec3(0.0f,  1.0f,  0.0f)}
    };

    std::vector<unsigned> indices = {
        2, 1, 0,
        0, 3, 2,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        14, 13, 12,
        12, 15, 14,

        16, 17, 18,
        18, 19, 16,

        22, 21, 20,
        20, 23, 22
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::Cylinder(float radius, float height, int segments) {
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;

    float halfHeight = height / 2.0f;
    float angleStep = 2.0f * M_PI / segments;

    for (int i = 0; i <= segments; ++i) {
        float angle = i * angleStep;
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;

        vertices.push_back(Vertex{
            glm::vec3(x, -halfHeight, z),
            glm::vec3(x, 0.0f, z),
            glm::vec3(0.0f, 0.0f, 0.0f)
        });

        vertices.push_back(Vertex{
            glm::vec3(x, -halfHeight, z),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        });

        vertices.push_back(Vertex{
            glm::vec3(x, halfHeight, z),
            glm::vec3(x, 0.0f, z),
            glm::vec3(0.0f, 0.0f, 0.0f)
        });

        vertices.push_back(Vertex{
            glm::vec3(x, halfHeight, z),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        });
    }

    int centerBottomIndex = vertices.size();
    vertices.push_back(Vertex{
        glm::vec3(0.0f, -halfHeight, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    });

    int centerTopIndex = vertices.size();
    vertices.push_back(Vertex{
        glm::vec3(0.0f, halfHeight, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    });

    for (int i = 0; i < segments; ++i) {
        int p0 = i * 4;
        int p1 = p0 + 2;
        int p2 = (p0 + 4) % (segments * 4);
        int p3 = (p1 + 4) % (segments * 4);

        indices.push_back(p0);
        indices.push_back(p1);
        indices.push_back(p2);

        indices.push_back(p1);
        indices.push_back(p3);
        indices.push_back(p2);
    }

    for (int i = 0; i < segments; ++i) {
        int p0 = i * 4 + 1;
        int p1 = (p0 + 4) % (segments * 4);
        indices.push_back(centerBottomIndex);
        indices.push_back(p1);
        indices.push_back(p0);
    }

    for (int i = 0; i < segments; ++i) {
        int p0 = i * 4 + 3;
        int p1 = (p0 + 4) % (segments * 4);
        indices.push_back(centerTopIndex);
        indices.push_back(p0);
        indices.push_back(p1);
    }

    return Mesh(vertices, indices);
}

Mesh Mesh::Sphere(float radius, int longitude_segments, int latitude_segments) {
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;

    for (int lat = 0; lat <= latitude_segments; ++lat) {
        float phi = M_PI * lat / latitude_segments;
        float y = radius * cos(phi);
        float sin_phi = sin(phi);

        for (int lon = 0; lon <= longitude_segments; ++lon) {
            float theta = 2.0f * M_PI * lon / longitude_segments;
            float x = radius * sin_phi * cos(theta);
            float z = radius * sin_phi * sin(theta);

            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

            vertices.push_back(Vertex{
                glm::vec3(x, y, z),
                normal,
                glm::vec3(0.0f, 0.0f, 0.0f)
            });
        }
    }

    for (int lat = 0; lat < latitude_segments; ++lat) {
        for (int lon = 0; lon < longitude_segments; ++lon) {
            int current = lat * (longitude_segments + 1) + lon;
            int next = current + longitude_segments + 1;

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    return Mesh(vertices, indices);
}

Mesh Mesh::Cone(float radius, float height, int segments) {
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;

    float slope = radius / height;

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        glm::vec3 horizontal(x, 0.0f, z);
        glm::vec3 vertical(0.0f, height, 0.0f);
        glm::vec3 normal = glm::normalize(horizontal + vertical * slope);

        vertices.push_back(Vertex{
            glm::vec3(x, 0.0f, z),
            normal,
            glm::vec3(0.0f, 0.0f, 0.0f)
        });

        vertices.push_back(Vertex{
            glm::vec3(x, 0.0f, z),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        });
    }

    int baseCenterIndex = vertices.size();
    vertices.push_back(Vertex{
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    });

    int apexIndex = vertices.size();
    vertices.push_back(Vertex{
        glm::vec3(0.0f, height, 0.0f),
        glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::vec3(0.0f, 0.0f, 0.0f)
    });

    for (int i = 0; i < segments; ++i) {
        int next = (i * 2 + 2) % (segments * 2);
        indices.push_back(i * 2);
        indices.push_back(next);
        indices.push_back(apexIndex);
    }

    for (int i = 0; i < segments; ++i) {
        int next = (i * 2 + 3) % (segments * 2);
        indices.push_back(i * 2 + 1);
        indices.push_back(next);
        indices.push_back(baseCenterIndex);
    }

    return Mesh(vertices, indices);
}