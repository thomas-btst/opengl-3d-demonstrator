#include "../include/projet/shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "../include/projet/config.hpp"

Shader::Shader()
 : program(glCreateProgram())
{}

std::string Shader::readShader(const std::string& name){
    const std::string filename = std::string(_resources_directory).append("shaders/").append(name);
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Erreur lors de l'ouverture du fichier \"" << filename << "\"." << std::endl;
      return "";
    }

    std::stringstream sstr;
    sstr << file.rdbuf();
    file.close();
    const std::string code = sstr.str();
    return code;
}

void Shader::addShader(const std::string& filename, GLenum type) {
    const std::string shaderSource = readShader(filename);
    const char* shaderSourceChar = shaderSource.c_str();

    unsigned int shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSourceChar, NULL);
    glCompileShader(shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        shaders.push_back(shader);
    }
}

void Shader::link() {
    for(unsigned shader: shaders)
        glAttachShader(program, shader);
    glLinkProgram(program);

    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::Program::Shader::LINK_FAILED\n" << infoLog << std::endl;
    }

    for(unsigned shader: shaders)
        glDeleteShader(shader);
}

void Shader::uniformFloat(const char* name, const float value) {
    glUniform1f(glGetUniformLocation(program, name), value);

}
void Shader::uniformVec3(const char* name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
}
void Shader::uniformMat4(const char* name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::transformUniform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    uniformMat4("model", model);
    uniformMat4("view", view);
    uniformMat4("projection", projection);
}

void Shader::textureUniform(unsigned textureId) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(glGetUniformLocation(program, "baseTexture"), 0);
}

void Shader::uniform(
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& viewPos,
    const Light& light,
    const Material& material
) {
    transformUniform(model, view, projection);
    uniformVec3("viewPos", viewPos);

    uniformVec3("light.position", light.position);
    uniformVec3("light.ambient", light.ambient);
    uniformVec3("light.diffuse", light.diffuse);
    uniformVec3("light.specular", light.specular);

    uniformVec3("material.ambient", material.ambient);
    uniformVec3("material.diffuse", material.diffuse);
    uniformVec3("material.specular", material.specular);
    uniformFloat("material.shininess", material.gaussian);
}