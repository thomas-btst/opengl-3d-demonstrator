#ifndef APP_UI_HPP
#define APP_UI_HPP

#include <GLFW/glfw3.h>

#include "./ui/radio_button.hpp"
#include "./material.hpp"
#include "./light.hpp"


enum class MainState { Object, Shape };
enum class ObjectState { Drake, Bunny, Axe, Possum, Chair };
enum class ShapeState { Rectangle, Cube, Cylinder, Sphere, Cone };
enum class StepState { Segments, WithoutNormals, Normals, ReliefLighting, Texture };

struct State{
    MainState mainState;
    ObjectState objectState;
    ShapeState shapeState;
    StepState stepState;
    Material material;
    Light light;
    bool displayNormals;
    bool displaySegments;
};

class AppUI {
    RadioButton<MainState> radioMainState;
    RadioButton<ObjectState> radioObjectState;
    RadioButton<ShapeState> radioShapeState;
    RadioButton<StepState> radioStepState;
    Material material;
    Light light;
    bool displayNormals;
    bool displaySegments;
public:
    AppUI(GLFWwindow* window);

    inline State getState() {
        return State{
            radioMainState.getSelectedValue(), 
            radioObjectState.getSelectedValue(),
            radioShapeState.getSelectedValue(),
            radioStepState.getSelectedValue(),
            material,
            light,
            displayNormals,
            displaySegments
        };
    }
    void draw();
};

#endif
