#include "../include/projet/window.hpp"

#include <imgui.h>
#include <stdexcept>

#include "../include/projet/camera.hpp"
#include "GLFW/glfw3.h"

Window::Window(int _width, int _height, const char* title)
    : width(_width), height(_height)
{
    // Init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetMouseButtonCallback(window, Camera::onMouseButton);
    glfwSetCursorPosCallback(window, Camera::onMouseMove);
    glfwSetScrollCallback(window, Camera::onMouseScroll);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);
}

Window::~Window() {
    glfwTerminate();
}

void Window::processInput() {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void Window::swapBuffers() {
    return glfwSwapBuffers(window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}