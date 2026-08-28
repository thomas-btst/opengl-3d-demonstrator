#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#define STB_IMAGE_IMPLEMENTATION

#include <stbimage/stb_image.h>
#include "../include/projet/config.hpp"

#include "../include/projet/window.hpp"
#include "../include/projet/camera.hpp"
#include "../include/projet/mesh.hpp"
#include "../include/projet/shader.hpp"
#include "../include/projet/app_ui.hpp"
#include <iostream>

const unsigned WIN_WIDTH = 800;
const unsigned WIN_HEIGHT = 600;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 10.0f;

unsigned int loadTexture(std::string filename) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(std::string(_resources_directory).append("textures/"+filename).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

int main() {
    Window window(WIN_WIDTH, WIN_HEIGHT, "Projet Opengl");
    AppUI ui(window.window);

    // Shader
    Shader shader;
    shader.addShader("simple.vert", GL_VERTEX_SHADER);
    shader.addShader("simple.frag", GL_FRAGMENT_SHADER);
    shader.link();

    // Texture Shader
    Shader textureShader;
    textureShader.addShader("texture.vert", GL_VERTEX_SHADER);
    textureShader.addShader("texture.frag", GL_FRAGMENT_SHADER);
    textureShader.link();

    // Normal Shader
    Shader normalShader;
    normalShader.addShader("normal.vert", GL_VERTEX_SHADER);
    normalShader.addShader("normal.geom", GL_GEOMETRY_SHADER);
    normalShader.addShader("normal.frag", GL_FRAGMENT_SHADER);
    normalShader.link();
    
    // Without Normal Shader
    Shader withoutNormalShader;
    withoutNormalShader.addShader("without_normal.vert", GL_VERTEX_SHADER);
    withoutNormalShader.addShader("without_normal.frag", GL_FRAGMENT_SHADER);
    withoutNormalShader.link();

    // Light Shader
    Shader lightShader;
    lightShader.addShader("light.vert", GL_VERTEX_SHADER);
    lightShader.addShader("light.frag", GL_FRAGMENT_SHADER);
    lightShader.link();

    // Mesh
    Mesh drake("dragon2_small.obj");
    Mesh axe("Axe.obj");
    Mesh bunny("bunny.obj");
    Mesh possum("possum.obj");
    Mesh chair("chair.obj");

    Mesh rectangle = Mesh::Rectangle(1.5f, 3.0f);
    Mesh cube = Mesh::Cube(1.5f);
    Mesh cylinder = Mesh::Cylinder(1.0f, 2, 10);
    Mesh sphere = Mesh::Sphere(2.0f, 20, 20);
    Mesh cone = Mesh::Cone(1.5f, 2.0f, 20);

    Mesh sun("sun.obj", MeshLoadingMode::Polygon);
    Mesh grass("Grass.obj", MeshLoadingMode::Polygon);

    unsigned int axeTexture = loadTexture("Axe.png");
    unsigned int possumTexture = loadTexture("possum.png");
    unsigned int chairTexture = loadTexture("chair.jpg");

    unsigned int sunTexture = loadTexture("sun.jpg");
    unsigned int grassTexture = loadTexture("Grass.jpg");

    // Render loop
    while(!window.shouldClose()) {
        // Input
        window.processInput();

        const State state = ui.getState();

        // Rendering
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2));
        glm::mat4 view = Camera::getViewMatrix();
        glm::vec3 viewPos = Camera::getPosition();
        glm::mat4 projection = glm::perspective(Camera::getFov(), (float)window.width / (float)window.height, NEAR_PLANE, FAR_PLANE);
        Mesh* mesh = nullptr;
        switch (state.mainState) {
            case MainState::Object:
                switch (state.objectState) {
                    case ObjectState::Drake: mesh = &drake;
                        break;
                    case ObjectState::Axe: mesh = &axe;
                        model = glm::mat4(1.0f);
                        model = glm::scale(model, glm::vec3(0.0007, 0.0007, 0.0007));
                        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
                        model = glm::translate(model, glm::vec3(0.0f, -100.0f, 0.0f));
                        break;
                    case ObjectState::Chair: mesh = &chair;
                        model = glm::mat4(1.0f);
                        model = glm::scale(model, glm::vec3(0.0003, 0.0003, 0.0003));
                        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
                        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
                        break;
                    case ObjectState::Possum: mesh = &possum;
                        model = glm::mat4(1.0f);
                        model = glm::scale(model, glm::vec3(0.06, 0.06, 0.06));
                        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
                        break;
                    case ObjectState::Bunny: mesh = &bunny;
                        model = glm::rotate(model, glm::radians(145.0f), glm::vec3(1.0, 0.0, 0.0));
                        model = glm::rotate(model, glm::radians(320.0f), glm::vec3(0.0, 1.0, 0.0));
                        model = glm::rotate(model, glm::radians(230.0f), glm::vec3(0.0, 1.0, 0.0));
                        model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
                        break;
                }
                
                switch (state.objectState) {
                    case ObjectState::Axe:
                    case ObjectState::Chair:
                    case ObjectState::Possum:
                        textureShader.use();
                        textureShader.uniform(model, view, projection, viewPos, state.light, state.material);
                        switch (state.objectState) {
                            case ObjectState::Axe: textureShader.textureUniform(axeTexture);
                                break;
                            case ObjectState::Chair: textureShader.textureUniform(chairTexture);
                                break;
                            case ObjectState::Possum: textureShader.textureUniform(possumTexture);
                                break;
                            default: break;
                        }
                        break;
                    default:
                        shader.use();
                        shader.uniform(model, view, projection, viewPos, state.light, state.material);
                }
                break;
            case MainState::Shape:
                switch (state.shapeState) {
                    case ShapeState::Rectangle: mesh = &rectangle;
                        break;
                    case ShapeState::Cube: mesh = &cube;
                        break;
                    case ShapeState::Cylinder: mesh = &cylinder;
                        break;
                    case ShapeState::Sphere: mesh = &sphere;
                        break;
                    case ShapeState::Cone: mesh = &cone;
                        break;
                }
                shader.use();
                shader.uniform(model, view, projection, viewPos, state.light, state.material);
                break;
        }
        
        switch (state.stepState) {
            case StepState::Segments:
            case StepState::Normals:
            case StepState::WithoutNormals: withoutNormalShader.use();
                withoutNormalShader.uniform(model, view, projection, viewPos, state.light, state.material);
                break;
            case StepState::ReliefLighting: shader.use();
                shader.uniform(model, view, projection, viewPos, state.light, state.material);
                break;
            case StepState::Texture: break;
        }
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        if(state.stepState != StepState::Segments)
            mesh->draw();

        glDisable(GL_POLYGON_OFFSET_FILL);
        // Draw lines
        if(state.stepState == StepState::Segments || state.displaySegments) {
            withoutNormalShader.use();
            withoutNormalShader.uniform(model, view, projection, viewPos, state.light, state.material);
            mesh->draw(GL_LINE);
        }

        // Draw Normals
        if (state.displayNormals || state.stepState == StepState::Normals) {
            normalShader.use();
            normalShader.transformUniform(model, view, projection);
            mesh->draw();
        }

        // Grass
        if (state.mainState == MainState::Object) {
            glm::mat4 grassModel = glm::mat4(1.0f);
            grassModel = glm::scale(grassModel, glm::vec3(0.002, 0.002, 0.002));
            grassModel = glm::rotate(grassModel, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            grassModel = glm::translate(grassModel, glm::vec3(0.0f, 0.0f, -10.0f));
            textureShader.use();
            textureShader.uniform(
                grassModel,
                view,
                projection,
                viewPos,
                state.light,
                state.material
            );
            textureShader.textureUniform(grassTexture);
            grass.draw();
        }

        // Sun
        lightShader.use();
        lightShader.transformUniform(
            glm::scale(glm::translate(glm::mat4(1.0f), state.light.position), glm::vec3(0.001, 0.001, 0.001)),
            view,
            projection
        );
        shader.textureUniform(sunTexture);
        sun.draw();

        // Draw UI
        ui.draw();

        // Check and call events and swap the buffers
        window.swapBuffers();
        glfwPollEvents();
    }
    return 0;
}
