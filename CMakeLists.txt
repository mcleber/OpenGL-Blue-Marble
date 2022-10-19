cmake_minimum_required(VERSION 3.22)

# Project Directory
project(BlueMarble)

add_executable(BlueMarble main.cpp)

# Windows
if (WIN32)

target_include_directories(BlueMarble PRIVATE deps/glm
						deps/stb
						deps/glfw/include
						deps/glew/include)

target_link_directories(BlueMarble PRIVATE deps/glfw/lib-vc2022
						deps/glew/lib/Release/x64)

target_link_libraries(BlueMarble PRIVATE glfw3.lib
						glew32.lib
						opengl32.lib)

# Post build command
add_custom_command(TARGET BlueMarble POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/glew32.dll"
				   COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
				   COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/textures" "${CMAKE_BINARY_DIR}/textures")


# Linux
else()
	find_package(glfw3 REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(OpenGL REQUIRED)

	target_include_directories(BlueMarble PRIVATE deps/glm
						deps/stb)

	target_link_libraries(BlueMarble GLEW::glew
						glfw
						OpenGL::OpenGL)

endif()
