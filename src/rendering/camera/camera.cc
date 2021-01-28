#include "camera.hh"

#include <algorithm>
#include <cmath>

Camera::Camera()
{
    position = glm::vec3(radius,0,0);
    look_at = {0,0,0};
    up_vector = {0, 1, 0};
    
    z_near = 0.001f;
    z_far = 500.0f;
}

glm::mat4 Camera::perspective_matrix() const
{
    return glm::perspective(angle_of_view, image_aspect, z_near, z_far);
}

void Camera::update_position()
{
    // Convert to carthesian
    float x = radius * sinf(phi) * sinf(theta);
    float y = radius * cosf(phi);
    float z = radius * sinf(phi) * cosf(theta);

    position = glm::vec3(x,y,z);
}

void Camera::rotate(float dtheta, float dphi)
{
    if (up_vector.y == 1.0f)
        theta += dtheta;
    else
        theta -= dtheta;
    phi += dphi;

    if (phi > 2 * M_PI)
        phi -= 2 * M_PI;
    if (phi < -2 * M_PI)
        phi += 2 * M_PI;
    
    if (phi > 0 && phi < M_PI || phi < -M_PI && phi > -2 * M_PI)
        up_vector.y = 1.0f;
    else
        up_vector.y = -1.0f;

    update_position();
}

void Camera::pan(float dx, float dy)
{
    //FIXME
    glm::vec3 look = position;
    glm::vec3 world_up = {0,1,0};
    glm::vec3 right = glm::cross(look, world_up);
    glm::vec3 up_ = glm::cross(look, right);

    look_at = look_at + (right * dx) + (up_ * dy);
    update_position();
}

void Camera::zoom(float d)
{
    radius += d * 0.05f;

    if (radius <= 0.0f)
        radius = .01f;

    update_position();
}

void CameraControl::update_mouse_move(Camera& camera, GLFWwindow* window, float x1, float y1)
{
    if (!allow_updates)
        return;

    const bool mouse_click_left  = (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT )==GLFW_PRESS);
    const bool mouse_click_right = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS);

    if (mouse_click_left)
    {
        float dPhi = ((float)(y - y1) / 300.0f);
        float dTheta = ((float)(x - x1) / 300.0f);
        // rotate
        camera.rotate(dTheta, dPhi);

    } else if (mouse_click_right)
    {
        //camera.pan(-(x - x1), (y - y1));
    }

    x = x1;
    y = y1;
}

void CameraControl::update_scroll(Camera& camera, GLFWwindow* window, float x, float y)
{
    if (!allow_updates)
        return;

    camera.zoom(-y);
}