#include "mesh-renderer.hh"

MeshRenderer::MeshRenderer()
    : data()
{}

MeshRenderer::MeshRenderer(const Mesh& mesh, const Shader& shader_)
    : data(mesh), shader{shader_}
{}

void MeshRenderer::clear()
{
    data.clear();
}

void MeshRenderer::draw(const Camera& camera)
{
    this->draw(camera, this->shader);
}

void MeshRenderer::draw(const Camera& camera, Shader& shader)
{
    this->draw(camera, shader, [](Shader& shader) {});
}

void MeshRenderer::draw(const Camera& camera, std::function<void(Shader&)> setup)
{
    draw(camera, this->shader, setup);
}

void MeshRenderer::draw(const Camera& camera, Shader& shader, std::function<void(Shader&)> setup)
{
    if (shader.shader_id == 0)
    {
        std::cerr << "Drawing without shader. Skipping." << std::endl;
        return;
    }

    GLuint current_shader = Shader::get_current_shader();
    if (current_shader != shader.shader_id)
    {
        shader.use();
    }

    if (texture != 0 && glIsTexture(texture))
    {
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    // Transform Uniforms
    shader.set_uniform("rotation", uniform.transform.rotation); TEST_OPENGL_ERROR();
    shader.set_uniform("translation", uniform.transform.translation); TEST_OPENGL_ERROR();
    shader.set_uniform("scaling", uniform.transform.scale); TEST_OPENGL_ERROR();

    // Object Color Uniforms
    shader.set_uniform("color", uniform.color); TEST_OPENGL_ERROR();

    // Object Shading Uniforms
    shader.set_uniform("ambiant", uniform.shading.ambiant); TEST_OPENGL_ERROR();
    shader.set_uniform("diffuse", uniform.shading.diffuse); TEST_OPENGL_ERROR();
    shader.set_uniform("specular", uniform.shading.specular); TEST_OPENGL_ERROR();
    shader.set_uniform("specular_exponent", uniform.shading.specular_exponent); TEST_OPENGL_ERROR();

    // Set Camera Info
    shader.set_uniform("perspective", camera.perspective_matrix(), GL_FALSE); TEST_OPENGL_ERROR();
    shader.set_uniform("view", camera.view_matrix(), GL_FALSE); TEST_OPENGL_ERROR();
    shader.set_uniform("camera_position", camera.position); TEST_OPENGL_ERROR();

    // User additional uniforms / data
    setup(shader); TEST_OPENGL_ERROR();

    data.draw();

    glBindTexture(GL_TEXTURE_2D, 0);
}