#pragma once

#include "third_party/glad/include/glad/glad.hpp"
#include <GLFW/glfw3.h>
#include <string>

GLFWwindow* glfw_create_window(int width, int height, const std::string& title, int opengl_version_major, int opengl_version_minor, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
void glfw_init();
void glad_init();