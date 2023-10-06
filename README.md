# Blue Marble project made in C++ and OpenGL

Studying computer graphics with OpenGL.

This is not a static project. It is possible to control the camera using the WASD keys and with the left mouse button the Yaw and Pitch movements.

Clouds move around the planet.

There is also diffuse and specular lighting (Phong Model).

This is my result of the Introduction to Computer Graphics Course by Thales Sabino.

<img src="https://github.com/mcleber/Cpp-OpenGL-Blue-Marble/blob/main/screenshot/BlueMarbleImage01.jpg" width="450" height="354">

<img src="https://github.com/mcleber/Cpp-OpenGL-Blue-Marble/blob/main/screenshot/BlueMarbleImage02.jpg" width="450" height="354">

## Dependencies

* GLEW
* GLFW
* GLM
* STB Image
* CMake

## Installing dependencies on Linux

* OpenGL Libraries

``sudo apt update``

``sudo apt install mesa-common-dev``

* GLEW and GLFW

``sudo apt install libglew-dev libglfw3-dev``

- STB Image and GLM

``sudo apt install libstb-dev libglm-dev``

- CMake

``sudo apt install cmake``

``cmake --version``

## Build, compile and run the program

``cd BlueMarble``

``mkdir build``

``cd build``

``cmake ..``

``make``

``./BlueMarble``

## links

[CMake](https://cmake.org)

[GLEW](https://glew.sourceforge.net)

[GLFW](https://www.glfw.org)

[STB Image](https://github.com/nothings/stb)

[GLM](https://github.com/g-truc/glm)

[OpenGL® 4.5 Reference Pages](https://registry.khronos.org/OpenGL-Refpages/gl4/)

[OpenGL® 4.5 Quick Reference Card](https://www.khronos.org/files/opengl45-quick-reference-card.pdf)

[GLSL_Quick Reference](http://mew.cx/glsl_quickref.pdf)
