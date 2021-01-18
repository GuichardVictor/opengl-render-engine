#include "mesh-gpu.hh"

#include <glm/glm.hpp>

MeshData::MeshData()
    : vao_id(0), number_triangles(0), vbo_index(0), vbo_position(0), vbo_normal(0), vbo_uvs(0)
{}

static void set_buffer_layout(GLuint vbo, GLuint layout_id, GLuint data_size)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(layout_id);
    glVertexAttribPointer(layout_id, data_size, GL_FLOAT, GL_FALSE, 0, nullptr );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

MeshData::MeshData(const Mesh& mesh)
{
    // If Empty Mesh leave
    if (mesh.positions.size() == 0)
        return;
    
    // Create Buffers
    // Setup Position Buffer
    setup_buffer<glm::vec3, GL_ARRAY_BUFFER>(vbo_position, mesh.positions);
    // Setup Normal Buffer
    setup_buffer<glm::vec3, GL_ARRAY_BUFFER>(vbo_normal, mesh.normal);
    // Setup UVs
    setup_buffer<glm::vec2, GL_ARRAY_BUFFER>(vbo_uvs, mesh.uvs);
    // Setup triangles indexes
    setup_buffer<glm::uvec3, GL_ELEMENT_ARRAY_BUFFER>(vbo_index, mesh.triangles);

    // Create Vertex Arrays
    number_triangles = (unsigned int)(mesh.triangles.size());
    
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // Set at corresponding engine defined layout
    set_buffer_layout(vbo_position, 0, 3);
    set_buffer_layout(vbo_normal, 1, 3);
    set_buffer_layout(vbo_uvs, 2, 2);

    glBindVertexArray(0);
}

void MeshData::clear()
{
    // GPU Mesh Data was never initialized.
    if (number_triangles == 0 && vao_id == 0)
        return;
    
    glDeleteBuffers(1,&vbo_position);
    glDeleteBuffers(1,&vbo_normal);
    glDeleteBuffers(1,&vbo_uvs);
    glDeleteBuffers(1,&vbo_index); 
}

void MeshData::draw()
{
    if (number_triangles == 0 && vao_id == 0)
        return;

    bool is_valid_gpu_data = glIsVertexArray(vao_id) && glIsBuffer(vbo_index);

    if (!is_valid_gpu_data)
        return;

    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index); TEST_OPENGL_ERROR();
    glDrawElements(GL_TRIANGLES, GLsizei(number_triangles * 3), GL_UNSIGNED_INT, nullptr); TEST_OPENGL_ERROR();
    glBindVertexArray(0);
}

void MeshData::update_position(const std::vector<glm::vec3>& positions)
{
    update_buffer<glm::vec3, GL_ARRAY_BUFFER>(vbo_position, positions);
}


void MeshData::update_normals(const std::vector<glm::vec3>& normals)
{
    update_buffer<glm::vec3, GL_ARRAY_BUFFER>(vbo_normal, normals); 
}


void MeshData::update_uvs(const std::vector<glm::vec2>& uvs)
{
    update_buffer<glm::vec2, GL_ARRAY_BUFFER>(vbo_uvs, uvs);
}