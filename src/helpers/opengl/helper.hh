#pragma once

#include <iostream>
#include "third_party/glad/include/glad/glad.hpp"

void opengl_print_version();
std::string opengl_error_to_string(GLenum error);

// Fabrizio Define to test opengl errors
#define TEST_OPENGL_ERROR()                                                             \
  do {		  							\
    GLenum err = glGetError(); 					                        \
    if (err != GL_NO_ERROR) std::cerr << "[OPENGL][ERROR] " << __FILE__ << ':' << __LINE__ << " - " << opengl_error_to_string(err) << std::endl;      \
  } while(0)