#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    GLFWwindow* window;
    int width;
    int height;

    Window() = delete;
    Window(int width, int height, const char* title);
    ~Window();

    void processInput();
    void swapBuffers();
    bool shouldClose() const;
};

#endif