#include "ssbo.hh"

SSBO::SSBO(size_t size)
{
    glGenBuffers(1, &ssbo_id); TEST_OPENGL_ERROR();
    bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_COPY); TEST_OPENGL_ERROR();
    unbind();

    max_size = size;
}

GLuint SSBO::get_current_ssbo()
{
    GLint current_ssbo = 0;
    glGetIntegerv(GL_SHADER_STORAGE_BUFFER_BINDING, &current_ssbo); TEST_OPENGL_ERROR();
    
    return current_ssbo;
}