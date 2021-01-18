#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/helpers/lib-helpers.hh"

#ifndef M_PI
    #define M_PI 3.14159265358979323846f
#endif

struct Camera
{
    Camera();

    Camera(glm::vec3& position_, glm::vec3 look_at_, glm::vec3 up = {0, 1, 0})
        : position{position_}, look_at{look_at_}, up_vector{up}
    {}

    glm::mat4 view_matrix() const { return glm::lookAt(position, look_at, up_vector); }
    glm::mat4 perspective_matrix() const;

    glm::vec3 view_direction() const { return -glm::transpose(view_matrix())[2]; }
    glm::vec3 right_vector() const { return glm::transpose(view_matrix())[0]; }

    void rotate(float dtheta, float dphi);
    void pan(float dx, float dy);
    void zoom(float d);
    void update_position();

    glm::vec3 position;
    glm::vec3 look_at;
    glm::vec3 up_vector;

    float theta = 0;
    float phi = 0;
    float radius = 5;

    // Perspective info
#ifdef GLM_FORCE_RADIANS 
    float angle_of_view = 60.0f * M_PI / 180;
#else
    float angle_of_view = 120.0f;
#endif
    float image_aspect = 1;
    float z_near = 0;
    float z_far = 1;

};

class CameraControl
{
public:
    void update_mouse_move(Camera& camera, GLFWwindow* window, float x1, float y1);
    void update_scroll(Camera& camera, GLFWwindow* window, float xoffset, float yoffset);

    bool allow_updates = true;

private:
    float x = 0;
    float y = 0;
};