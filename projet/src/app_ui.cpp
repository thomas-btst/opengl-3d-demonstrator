#include "../include/projet/app_ui.hpp"
#include "GLFW/glfw3.h"
#include <imgui-1.91.1/imgui.h>
#include <imgui-1.91.1/backends/imgui_impl_glfw.h>
#include <imgui-1.91.1/backends/imgui_impl_opengl3.h>

#include "../include/projet/ui/radio_button.hpp"
#include "projet/light.hpp"
#include "projet/material.hpp"

AppUI::AppUI(GLFWwindow* window)
 : radioMainState(MainState::Object),
    radioObjectState(ObjectState::Drake),
    radioShapeState(ShapeState::Cube),
    radioStepState(StepState::ReliefLighting),
    material({
      glm::vec3(0.05f, 0.1f, 0.15f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      200
    }),
    light({
      glm::vec3(-1.6f, 6.0f, 1.8f),
      glm::vec3(0.8f, 0.8f, 0.8f),
      glm::vec3(0.9f, 0.9f, 0.9f),
      glm::vec3(0.7f, 0.7f, 0.7f)
    }),
    displayNormals(false),
    displaySegments(false)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void AppUI::draw() {
    // Init Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Window");

    // Draw elements
    radioMainState.addOption("Chargement d'objets", MainState::Object);
    ImGui::SameLine();
    radioMainState.addOption("Forme", MainState::Shape);
    switch (radioMainState.getSelectedValue()) {
        case MainState::Object: {
            if (ImGui::TreeNode("Objet à afficher")) {
                radioObjectState.addOption("Dragon", ObjectState::Drake);
                radioObjectState.addOption("Lapin", ObjectState::Bunny);
                radioObjectState.addOption("Hache", ObjectState::Axe);
                radioObjectState.addOption("Opossum", ObjectState::Possum);
                radioObjectState.addOption("Chaise", ObjectState::Chair);
                ImGui::TreePop();
            }
        }
            break;
        case MainState::Shape:
            if (ImGui::TreeNode("Forme à afficher")) {
                radioShapeState.addOption("Rectangle", ShapeState::Rectangle);
                radioShapeState.addOption("Cube", ShapeState::Cube);
                radioShapeState.addOption("Cylindre", ShapeState::Cylinder);
                radioShapeState.addOption("Sphère", ShapeState::Sphere);
                radioShapeState.addOption("Pyramide", ShapeState::Cone);
                ImGui::TreePop();
            }
            break;
    }
    if (ImGui::TreeNode("Etapes")) {        
        radioStepState.addOption("Affichage du maillage", StepState::Segments);
        radioStepState.addOption("Remplissage des triangles", StepState::WithoutNormals);
        radioStepState.addOption("Calcul des normales", StepState::Normals);
        radioStepState.addOption("Affichage des reliefs et des effets de lumière", StepState::ReliefLighting);
        if (radioMainState.getSelectedValue() == MainState::Object && (
                radioObjectState.getSelectedValue() == ObjectState::Axe ||
                    radioObjectState.getSelectedValue() == ObjectState::Chair ||
                    radioObjectState.getSelectedValue() == ObjectState::Possum
            ))
            radioStepState.addOption("Utilisation d'une texture", StepState::Texture);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Explications")) {
        ImGui::PushTextWrapPos(ImGui::GetWindowContentRegionMax().x);
        std::string text = "";
        switch (radioStepState.getSelectedValue()) {
            case StepState::Segments:
                text="Avant tout rendu, il faut charger les points que l'on souhaite afficher à l'écran. "
                   "On forme ensuite un maillage constitué de plusieurs triangles entre ces points. "
                   "Bien que cela ne soit pas très évident, on peut déjà distinguer la forme de l'objet.";
                break;
            case StepState::WithoutNormals:
                text="Nous remplissons maintenant le maillage avec une couleur uniforme. "
                   "On distingue toujours la forme affichée, mais elle paraît très plate. "
                   "C'est normal : nous n'avons pas encore ajouté de relief. "
                   "Pour cela, nous aurons besoin des normales.";
                break;
            case StepState::Normals: 
                text="Pour obtenir une information sur les reliefs, nous utilisons les normales. "
                   "Celles-ci peuvent être soit pré-calculées, soit calculées manuellement. "
                   "Elles sont représentées par les traits colorés à l'écran. "
                   "N'hésitez pas à zoomer pour mieux les observer !";
                break;
            case StepState::ReliefLighting: 
                text="Nous pouvons maintenant calculer les effets de lumière et ajouter des reliefs à l'aide des normales et de la position du soleil. "
                   "Pour cela, nous comparons la direction de la lumière avec la normale de chaque point. "
                   "Le rendu obtenu se rapproche fortement de la réalité.";
                break;
            case StepState::Texture: 
                text="Si une texture est fournie, nous pouvons l'afficher pour remplacer la couleur. "
                   "Lors du chargement des points, chaque point est associé à une couleur issue de la texture. "
                   "Au moment du rendu, il ne reste plus qu'à afficher la couleur correspondante pour chaque point !";
                break;
        }
        ImGui::Text("%s", text.c_str());
        ImGui::PopTextWrapPos();
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Options")) {
        ImGui::Checkbox("Afficher les normales", &displayNormals);
        ImGui::Checkbox("Afficher le maillage", &displaySegments);
        if (ImGui::TreeNode("Matériel")) {
            ImGui::ColorEdit3("Ambiante", &material.ambient.x);
            ImGui::ColorEdit3("Diffuse", &material.diffuse.x);
            ImGui::ColorEdit3("Spéculaire", &material.specular.x);
            ImGui::InputFloat("Gaussien", &material.gaussian);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Lumière")) {
            ImGui::ColorEdit3("Ambiante", &light.ambient.x);
            ImGui::ColorEdit3("Diffuse", &light.diffuse.x);
            ImGui::ColorEdit3("Spéculaire", &light.specular.x);
            ImGui::InputFloat3("Position", &light.position.x);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    // Finalize frame
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}