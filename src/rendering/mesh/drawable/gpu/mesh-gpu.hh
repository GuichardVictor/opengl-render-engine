#pragma once

#include "../../mesh.hh"
#include "src/helpers/lib-helpers.hh"

struct MeshData
{
    MeshData();
    MeshData(const Mesh& mesh);
    
    void clear();
    void draw();

    void update_position(const std::vector<glm::vec3>& positions);
    void update_normals(const std::vector<glm::vec3>& normals);
    void update_uvs(const std::vector<glm::vec2>& uvs);

    GLuint vao_id = 0;

    unsigned int number_triangles = 0;

    GLuint vbo_index = 0;

    GLuint vbo_position = 0;
    GLuint vbo_normal = 0;
    GLuint vbo_uvs = 0;
};

template <typename BufferType, GLuint GL_BUFFER_TYPE>
void setup_buffer(GLuint& vbo, const std::vector<BufferType>& buffer)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_BUFFER_TYPE, vbo);
    glBufferData(GL_BUFFER_TYPE, GLsizeiptr(buffer.size() * sizeof(BufferType)), &buffer[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_BUFFER_TYPE, 0);
}

template <typename BufferType, GLuint GL_BUFFER_TYPE>
void update_buffer(GLuint& vbo, const std::vector<BufferType>& buffer)
{
    if (!glIsBuffer(vbo))
        return;

    glBindBuffer(GL_BUFFER_TYPE, vbo);
    glBufferSubData(GL_BUFFER_TYPE, 0, GLsizeiptr(buffer.size() * sizeof(BufferType)), &buffer[0]);
    glBindBuffer(GL_BUFFER_TYPE, 0);
}