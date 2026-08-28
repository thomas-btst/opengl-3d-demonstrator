# **R5A05**

R5A05 is a 3D educational demonstrator application written in **C++** with **Opengl**.

## **Table of Contents**

1. [Technologies Used](#technologies-used)
2. [Architecture](#architecture)
3. [Getting Started](#getting-started)
4. [Usage](#usage)
5. [Author](#author)
6. [License](#license)

## **Technologies Used**

- **Language**: [C++](https://fr.wikipedia.org/wiki/C%2B%2B)
- **API**: [OpenGL](https://www.opengl.org/)
- **Build and Dependency Management**: [CMake](https://cmake.org/)
- **IHM**: [Imgui](https://github.com/ocornut/imgui)
- **Window library**: [GLFW](https://www.glfw.org/docs/3.3/window_guide.html)
- **Others**:
    - Glad
    - stbimage

## **Architecture**

- **/src**: Contains source files.
- **/include**: Contains headers files.
- **/resources**: Contains resources like shader, objects, textures...

## **Getting started**

### Requirements

For building and running the application you need :

- **[CMake](https://cmake.org/)** installed (at least version 3.5)
- **CMake GUI** (optional)
- **[C++ compiler](https://gcc.gnu.org)** installed (like GCC or Clang)

### Steps

1. Setup CMake :
- Launch the CMake GUI.
- Enter the source code location.
- Enter the build location (exemple: /tmp/opengl).
- Click Configure, and if no errors are reported, click Generate.
Your build should now be generated in /tmp/opengl.
2. Compile the source code with make :
``` bash
cd /tmp/opengl/projet
make
```
3. Start the application :
``` bash
./projet
```

## **Usage**

#### Run the application
``` bash
./projet
```
#### Mouse Navigation
Use the mouse wheel to zoom in or zoom out.
Click and drag the mouse to rotate the camera and explore the scene.

#### Scene Control
The application features an interactive resizable menu.
This menu allows you to adjust and control various parameters of the scene.

#### Enjoy the experience:
Take your time to explore and learn how to interact with the application.
Happy learning!

## **Author**

**Name** : Thomas BATISTA  
**Institution** : IUT of Arles  
**Role** : Student developer  
**Program** : BUT Informatique  
**Contact** : thomas.batista@etu.univ-amu.fr  

## **License**

This project is developed as part of my formation.  
It is used for educational purposes only and is not intended for commercial use.