#include "../include/projet/camera.hpp"

#include <imgui.h>

GLEngine::OrbitalCamera Camera::camera(glm::vec3(0.3f, 0.4f, 3.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
MousePressedButton Camera::mouseButtonState = MousePressedButton::NONE;
bool Camera::firstMouse = true;
float Camera::lastX = 0.0f;
float Camera::lastY = 0.0f;

void Camera::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
  if (action == GLFW_RELEASE || ImGui::GetIO().WantCaptureMouse) {
      mouseButtonState = MousePressedButton::NONE;
  }
  else {
    switch (button) {
      case GLFW_MOUSE_BUTTON_LEFT: mouseButtonState = MousePressedButton::LEFT;
        break;
      case GLFW_MOUSE_BUTTON_RIGHT: mouseButtonState = MousePressedButton::RIGHT;
        break;
      case GLFW_MOUSE_BUTTON_MIDDLE: mouseButtonState = MousePressedButton::MIDDLE;
        break;
    }
  }
}

void Camera::onMouseMove(GLFWwindow* window, double xpos, double ypos) {
  if (mouseButtonState == MousePressedButton::NONE) {
    lastX = (float)xpos;
    lastY = (float)ypos;
  }
  else {
    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;
    switch (mouseButtonState) {
      case MousePressedButton::LEFT: camera.orbit(xoffset, yoffset);
        break;
      case MousePressedButton::RIGHT:
        camera.track(xoffset);
        camera.pedestal(yoffset);
        break;
      case MousePressedButton::MIDDLE: camera.dolly(yoffset);
        break;
      default: break;
    }
  }
}

void Camera::onMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
  if (!ImGui::GetIO().WantCaptureMouse) {
    camera.zoom((float)yoffset);
  }
}