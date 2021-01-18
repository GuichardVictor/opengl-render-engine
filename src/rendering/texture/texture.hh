#pragma once

#include "src/helpers/lib-helpers.hh"
#include "image.hh"
#include <string>

namespace Texture
{
    GLuint from_file(const std::string& filename, const Image::color_type& color_type,
                     const GLuint border_s = GL_CLAMP_TO_EDGE, const GLuint border_t = GL_CLAMP_TO_EDGE);

    GLuint empty(unsigned int width, unsigned int height,
                 const GLuint border_s = GL_CLAMP_TO_EDGE, const GLuint border_t = GL_CLAMP_TO_EDGE);
};