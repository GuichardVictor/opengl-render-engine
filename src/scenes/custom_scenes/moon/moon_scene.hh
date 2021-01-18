#pragma once

#include "src/scenes/scene.hh"

#include "src/rendering/shapes/shapes.hh"
#include "src/rendering/mesh/drawable/mesh-renderer.hh"
#include "src/rendering/mesh/loader/obj-parser.hh"

#include "objects/planet.hh"

#include "settings/settings.hh"

struct CustomScene : Scene
{
    // Required
    void set_gui();
    void draw();
    void setup();
    void load_shaders();

    // Additional

    Mesh sphere_mesh;
    Mesh sphere_mesh_cached;

    MeshRenderer quad;
    Planet planet;

    Shader default_shader;
    Shader wireframe_shader;
    Shader normal_disp_shader;
    ComputeShader cp_shader;

    void update_sphere_heights();
    void update_gpu_sphere_data();

    long long last_update_time = 0;
    bool debug_mode = false;
};