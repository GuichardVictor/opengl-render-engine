#pragma once

#include "src/helpers/lib-helpers.hh"
#include <string>
#include <iostream>
#include <vector>

struct SSBO
{
    SSBO() { ssbo_id = 0; }

    SSBO(size_t size);

    template <typename T>
    void add_data(const std::vector<T>& data)
    {
        size_t addon_size = sizeof(T) * data.size();
        
        if (current_size + addon_size > max_size)
        {
            std::cerr << "[SSBO] Attempted to add more data than capable. Skipping." << std::endl;
            return;
        }
        
        bind();
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, GLsizeiptr(current_size), GLsizeiptr(addon_size), &data[0]);

        current_size += addon_size;
    }

    template <typename T>
    void update_data(const std::vector<T>& data, int offset=0)
    {
        size_t addon_size = sizeof(T) * data.size();
        
        if (offset + addon_size > max_size)
        {
            std::cerr << "[SSBO] Attempted to add more data than capable. Skipping." << std::endl;
            return;
        }

        bind();
        current_size = offset;
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, GLsizeiptr(current_size), GLsizeiptr(addon_size), &data[0]);

        current_size += addon_size;
    }

    template <typename T>
    std::vector<T> retreive_data(size_t count, GLintptr offset=0)
    {
        if (ssbo_id == 0) { std::cerr << "Retreiving from uninitialized buffer." << std::endl; std::exit(1); }

        bind();
        std::vector<T> content(count);
        glGetNamedBufferSubData(ssbo_id, offset, GLsizeiptr(count * sizeof(T)), &content[0]); TEST_OPENGL_ERROR();
        unbind();

        return content;
    }

    static GLuint get_current_ssbo();
    void set_layout(const std::string& layout_name) const;

    void bind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id); TEST_OPENGL_ERROR(); }
    static void unbind() { glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); TEST_OPENGL_ERROR(); }

    GLuint ssbo_id = 0;
    size_t max_size = 0;
    size_t current_size = 0;
};