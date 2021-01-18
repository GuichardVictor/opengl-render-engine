#pragma once

#include "helper/scene-helper.hh"
#include "src/rendering/camera/camera.hh"

struct Scene
{
    void load_shaders();
    void setup();
    
    void update();
    void draw();

    void set_gui();

    Camera camera;
    CameraControl camera_control;
};

#include "custom_scenes/custom_scene.hh"