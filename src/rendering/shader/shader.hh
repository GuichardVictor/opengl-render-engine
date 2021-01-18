#pragma once

#include "src/helpers/lib-helpers.hh"
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "ssbo.hh"

struct Shader
{
    Shader() : shader_id{0} {};
    Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    Shader(const std::string& vertex_shader_path, const std::string& geometry_shader_path, const std::string& fragment_shader_path);
    // Missing: tesselation shader and tesselation shader + geometry shader
    
    static GLuint get_current_shader();

    void set_uniform(const std::string& uniform_name, const int value);
    void set_uniform(const std::string& uniform_name, const float value);
    void set_uniform(const std::string& uniform_name, const glm::vec3& value);
    void set_uniform(const std::string& uniform_name, const glm::vec4& value);
    void set_uniform(const std::string& uniform_name, const glm::mat3& value);
    void set_uniform(const std::string& uniform_name, const glm::mat4& value, const unsigned& invert=GL_TRUE);

    void set_uniform(const std::string& uniform_name, const std::vector<glm::vec4>& values);

    void use() { glUseProgram(shader_id); TEST_OPENGL_ERROR(); }

    GLuint shader_id = 0;

    void set_buffer(const std::string& layout_name, const SSBO& buffer);

};


struct ComputeShader : Shader
{
    ComputeShader() { shader_id = 0; };
    ComputeShader(const std::string& shader_path, bool handle_include=true);

    static void print_workgroups_capabilities();

    void wait_for(GLuint barrier_type) { glMemoryBarrier(barrier_type); TEST_OPENGL_ERROR(); }
    void dispatch(int x=1, int y=1, int z=1) { glDispatchCompute(x, y, z); TEST_OPENGL_ERROR(); }
};