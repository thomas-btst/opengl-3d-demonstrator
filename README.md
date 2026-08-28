# OpenGL 3D Educational Demonstrator

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3%20Core-red.svg)](https://www.opengl.org/)
[![GLFW](https://img.shields.io/badge/GLFW-3.3.8-orange.svg)](https://www.glfw.org/)
[![Dear ImGui](https://img.shields.io/badge/Dear%20ImGui-1.91.1-brightgreen.svg)](https://github.com/ocornut/imgui)
[![CMake](https://img.shields.io/badge/CMake-3.5%2B-green.svg)](https://cmake.org/)

An interactive 3D educational demonstrator application developed in **C++** with **OpenGL 3.3 Core Profile**.  
This project aims to visually break down and explain the key stages of 3D image synthesis: from raw geometric structures (wireframe meshes) to final rendered scenes with lighting, textures, and specular reflections.

---

## Table of Contents

1. [Overview & Pedagogical Goals](#overview--pedagogical-goals)
2. [Key Features](#key-features)
3. [Technologies & Libraries](#technologies--libraries)
4. [Project Architecture](#project-architecture)
5. [Shaders Pipeline](#shaders-pipeline)
6. [System Requirements](#system-requirements)
7. [Installation & Building](#installation--building)
8. [Usage Guide & Controls](#usage-guide--controls)
9. [Author & Context](#author--context)
10. [License](#license)

---

## Overview & Pedagogical Goals

This project is designed as an **interactive visual learning tool** to explore the inner workings of the modern OpenGL graphics pipeline. The interface guides the user step-by-step through 5 educational stages:

1. **Wireframe Display (Mesh)**  
   Loading vertices and displaying the triangular mesh edges composing the 3D object.
2. **Triangle Filling (Flat Shading)**  
   Filling geometric primitives with a uniform color without surface normals or lighting calculations.
3. **Normals Calculation & Display**  
   Generating and displaying vector representations of surface normals (via a GLSL *Geometry Shader*) illustrating faceting and orientation.
4. **Lighting & Relief (Phong Model)**  
   Computing dynamic realistic lighting (Ambient, Diffuse, and Specular components) based on the sun's position.
5. **Texture Mapping (UV Mapping)**  
   Applying 2D textures onto the 3D mesh UV coordinates.

---

## Key Features

- **3D Model Loading (.OBJ)**: Dragon, Bunny, Axe, Possum, Chair, Sun, Grass terrain.
- **Geometric Primitives Generation**: Procedural creation of fundamental 3D shapes (Rectangle, Cube, Cylinder, Sphere, Pyramid/Cone).
- **Interactive Graphical UI (Dear ImGui)**:
  - Dynamic switching between imported 3D models and procedural primitives.
  - Step-by-step educational breakdown with integrated text explanations.
  - Toggleable wireframe mesh and normal vector overlays on demand.
  - Real-time adjustment of **Material** properties (Ambient, Diffuse, Specular colors, Gaussian exponent/shininess).
  - Real-time adjustment of **Light** properties (Ambient, Diffuse, Specular colors, and 3D sun position).
- **Smart Orbital Camera**: Mouse-driven view controls (orbital rotation around the focused object and smooth zooming).

---

## Technologies & Libraries

- **Language**: C++17
- **Graphics API**: OpenGL 3.3 (Core Profile)
- **Windowing & Events**: [GLFW 3.3.8](https://www.glfw.org/)
- **OpenGL Function Loading**: [GLAD](https://glad.dav1d.de/)
- **3D Mathematics**: [GLM 1.0.1](https://github.com/g-truc/glm) (OpenGL Mathematics)
- **User Interface (GUI)**: [Dear ImGui 1.91.1](https://github.com/ocornut/imgui)
- **Image & Texture Loading**: [stb_image](https://github.com/nothings/stb)
- **Build System**: [CMake 3.5+](https://cmake.org/)

---

## Project Architecture

```text
├── CMakeLists.txt            # Root CMake configuration
├── glengine/                 # Engine wrapper / Orbital camera abstraction
│   ├── include/glengine/     # Headers (glengine.hpp, orbitalCamera.hpp)
│   └── src/                  # 3D camera implementation
├── projet/                   # Main demonstrator application
│   ├── include/projet/       # Application headers (app_ui, camera, mesh, shader...)
│   ├── src/                  # C++ source files (main.cpp, app_ui.cpp, mesh.cpp, shader.cpp...)
│   └── resources/            # Application assets
│       ├── materials/        # Material files
│       ├── objects/          # 3D model files in .obj format (drake, bunny, axe...)
│       ├── shaders/          # GLSL shader files (.vert, .geom, .frag)
│       └── textures/         # Texture images (.png, .jpg)
├── glad/                     # GLAD OpenGL loader
├── glfw-3.3.8/               # GLFW windowing library
├── glm-1.0.1/                # GLM mathematics library
├── imgui-1.91.1/             # Dear ImGui library
└── stbimage/                 # stb_image library
```

---

## Shaders Pipeline

The application includes several **GLSL** shader programs tailored to each display mode:

- **`simple.vert / simple.frag`**: Implements the full Phong lighting model (Ambient + Diffuse + Specular).
- **`texture.vert / texture.frag`**: Combines the Phong lighting model with 2D texture mapping.
- **`normal.vert / normal.geom / normal.frag`**: Uses a *Geometry Shader* to compute and visually render lines representing surface normal vectors.
- **`without_normal.vert / without_normal.frag`**: Renders shapes and wireframes without lighting calculations (flat uniform color).
- **`light.vert / light.frag`**: Emissive shader dedicated to rendering the Sun object representing the light source.

---

## System Requirements

To build and run the application, you need:

- A **C++ Compiler** supporting C++17 (GCC, Clang, or MSVC)
- **CMake** (version 3.5 or higher)
- **Graphics drivers** compatible with OpenGL 3.3 Core Profile
- **System GL development libraries** (e.g., `libgl-dev` on Debian/Ubuntu, Mesa on Linux)

---

## Installation & Building

### 1. Clone or download the repository

```bash
git clone <repository-url>
cd projet
```

### 2. Command line compilation (Recommended)

Use CMake to generate build files inside a dedicated `build` directory:

```bash
# Configure and generate build files
cmake -B build -S .

# Build the project
cmake --build build
```

*(Optional) Classic Make method:*
```bash
mkdir build && cd build
cmake ..
make
```

### 3. Running the application

Launch the generated executable from the root directory:

```bash
./build/projet/projet
```

---

## Usage Guide & Controls

### Mouse Navigation
- **Left Click + Drag**: Rotate the camera in orbit around the focused object.
- **Mouse Scroll Wheel**: Zoom in / Zoom out of the 3D scene.

### ImGui Control Panel
- **Object / Shape Selection**: Switch between complex 3D models (.obj) and basic geometric primitives.
- **Steps**: Select an educational step (Wireframe, Filling, Normals, Lighting, Texture).
- **Explanations**: Read contextual explanations describing the theory behind the active step.
- **Options**:
  - Check *Display normals* to overlay surface normal vectors.
  - Check *Display mesh* to overlay wireframe edges.
  - Dynamically adjust colors and properties for **Materials** and **Light** (3D position of the Sun).

### Keyboard
- **Escape (`ESC`)**: Exit the application.

---

## Author & Context

- **Author**: Thomas BATISTA  
- **Institution**: IUT d'Arles (Aix-Marseille University)  
- **Degree**: BUT Informatique (Year 3 / S5)  
- **Role**: Student Developer  
- **Contact**: `thomas.batista@etu.univ-amu.fr`

---

## License

This project is developed as part of an academic coursework.  
It is intended for educational purposes only.
