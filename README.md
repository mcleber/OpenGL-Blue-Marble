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
* CMkake

## Installing dependencies on Linux

* OpenGL Libraries

``sudo apt update``

``sudo apt install mesa-common-dev``

* GLEW e o GLFW

``sudo apt install libglew-dev libglfw3-dev``

* Compile on Linux

``cd BlueMarble``

``mkdir build``

``cd build``

* Run CMake to generate the Makefile

``cmake ..``

* Compile and run the project

``make``

``./BlueMarble``

## links

[OpenGL® 4.5 Reference Pages](https://registry.khronos.org/OpenGL-Refpages/gl4/)

[OpenGL® 4.5 Quick Reference Card](https://www.khronos.org/files/opengl45-quick-reference-card.pdf)

[GLSL_Quick Reference](http://mew.cx/glsl_quickref.pdf)
