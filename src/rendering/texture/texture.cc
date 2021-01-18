#include "texture.hh"

#include "third_party/lodepng/lodepng.h"

#include <vector>
#include <iostream>

namespace Texture
{
    GLuint from_file(const std::string& filename, const Image::color_type& color_enum, const GLuint border_s, const GLuint border_t)
    {
        std::vector<unsigned char> data;
        unsigned int width, height = 0;

        LodePNGColorType color_type;

        if (color_enum == Image::color_type::RGB)
            color_type = LCT_RGB;
        else
            color_type = LCT_RGBA;

        auto res = lodepng::decode(data, width, height, filename, color_type);
        if (res)
        {
            std::cerr << "Could not open file: " << filename << std::endl;
            return 0;
        }

        GLuint texture_id = 0;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // Set data in GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GLsizei(width), GLsizei(height), 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set Texture settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, border_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, border_t);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        TEST_OPENGL_ERROR();

        return texture_id;
    }

    GLuint empty(unsigned int width, unsigned int height, const GLuint border_s, const GLuint border_t)
    {
        GLuint texture;
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GLsizei(width), GLsizei(height), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, border_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, border_t);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        TEST_OPENGL_ERROR();

        return texture;
    }

} // namespace Texture