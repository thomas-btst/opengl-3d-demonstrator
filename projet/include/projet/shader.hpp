#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "light.hpp"
#include "material.hpp"

class Shader {
    std::vector<unsigned> shaders;
    std::string readShader(const std::string& filename);
public:
    const unsigned program;

    Shader();
    void addShader(const std::string& filename, GLenum type);
    void link();

    void uniformFloat(const char* name, const float value);
    void uniformVec3(const char* name, const glm::vec3& value);
    void uniformMat4(const char* name, const glm::mat4& value);

    void uniform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const Light&, const Material&);
    void transformUniform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
    void textureUniform(unsigned textureId);

    inline void use() {glUseProgram(program);}
};

#endif