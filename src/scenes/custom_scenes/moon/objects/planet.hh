#pragma once

#include "../settings/settings.hh"
#include "src/rendering/mesh/mesh.hh"
#include "src/rendering/mesh/drawable/mesh-renderer.hh"

struct Planet : public MeshRenderer
{
    Planet(const Mesh& mesh, const Shader& shader) : MeshRenderer(mesh, shader) {}
    Planet(const Mesh& mesh) : MeshRenderer(mesh) {}
    Planet() : MeshRenderer() {}

    DefaultNoiseSettings noise_settings = DefaultNoiseSettings(4, 2, 0.5f, 0.97f, 1, 0);
    RidgidNoiseSettings  ridgid_noise_1 = RidgidNoiseSettings(4, 5, 0.5f, 1.82f, 1.84f, 2, 0.5f, 0, 3);
    RidgidNoiseSettings  ridgid_noise_2 = RidgidNoiseSettings(5, 2, 0.5f, 2, 0.5f, 3, 1, 0, 0);
    CraterSettings crater_settings;

    // Triplanar settings
    float texture_scaling = 1;
    float blend_sharpness = 1;
    bool use_texture = true;
    bool fixed_light = false;
    glm::vec3 fixed_light_position = {0, 1, 0};
};