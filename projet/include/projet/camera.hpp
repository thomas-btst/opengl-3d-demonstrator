#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glengine/orbitalCamera.hpp>

enum class MousePressedButton { NONE, LEFT, RIGHT, MIDDLE };

class Camera {
    static GLEngine::OrbitalCamera camera;

    static MousePressedButton mouseButtonState;
    static bool firstMouse;
    static float lastX;
    static float lastY;
public:
    Camera() = delete;

    static void onMouseButton(GLFWwindow* window, int button, int action, int mods);
    static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
    static void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
    inline static glm::mat4 getViewMatrix() {return camera.getViewMatrix();}
    inline static glm::vec3 getPosition() {return camera.getPosition();}
    inline static float getFov() {return camera.getFov();}
};

#endif