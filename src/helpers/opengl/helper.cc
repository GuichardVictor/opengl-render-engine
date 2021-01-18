#include "helper.hh"
#include "third_party/glad/include/glad/glad.hpp"
#include <iostream>

void opengl_print_version()
{
    std::cout<<"\t [VENDOR]      : " << glGetString(GL_VENDOR) << std::endl;
    std::cout<<"\t [RENDERDER]   : " << glGetString(GL_RENDERER) << std::endl;
    std::cout<<"\t [VERSION]     : " << glGetString(GL_VERSION) << std::endl;
    std::cout<<"\t [GLSL VERSION]: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

std::string opengl_error_to_string(GLenum error)
{
    switch(error)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
    default:
        return "UNKNOWN";
    }

}