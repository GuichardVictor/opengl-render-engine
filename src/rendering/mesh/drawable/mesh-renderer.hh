#pragma once

#include "../mesh.hh"
#include "gpu/mesh-gpu.hh"
#include "src/math/transform.hh"
#include "src/rendering/shader/shader.hh"
#include "src/rendering/camera/camera.hh"
#include "src/rendering/texture/texture.hh"

#include <functional>

struct ShadingInfo
{
    float ambiant = 0.2f;
    float diffuse = 0.7f;
    float specular = 0.5f;
    int specular_exponent = 64;
};

struct MeshUniform
{
    // Main info
    Transform transform;
    glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f);

    // Shading Info
    ShadingInfo shading;
};

struct MeshRenderer
{
    MeshRenderer();
    MeshRenderer(const Mesh& mesh, const Shader& shader_ = Shader());

    void clear();
    void draw(const Camera& camera);
    void draw(const Camera& camera, Shader& custom_shader);

    void draw(const Camera& camera, std::function<void(Shader&)> setup);

    void draw(const Camera& camera, Shader& shader, std::function<void(Shader&)> setup);

    MeshData data;
    Shader shader;
    MeshUniform uniform;
    GLuint texture = 0;
};