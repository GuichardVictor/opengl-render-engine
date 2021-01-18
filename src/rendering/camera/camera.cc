#include "camera.hh"

#include <algorithm>

Camera::Camera()
{
    position = glm::vec3(-50,5,-50);
    look_at = {0,0,0};
    up_vector = {0, 1, 0};
    
    z_near = 0.01f;
    z_far = 500.0f;
}

glm::mat4 Camera::perspective_matrix() const
{
    return glm::perspective(angle_of_view, image_aspect, z_near, z_far);
}

void Camera::rotate(float dtheta, float dphi)
{
    //FIXME
    theta += dtheta;
    phi += dphi;

    // Convert to carthesian
    float radius = glm::length(position - look_at);
    float x = radius * std::sinf(phi) * std::sinf(theta);
    float y = radius * std::cosf(phi);
    float z = radius * std::sinf(phi) * std::cosf(theta);

    position = glm::vec3(x,y,z);
}

void Camera::pan(float dx, float dy)
{
    //FIXME
    glm::vec3 look = position;
    glm::vec3 world_up = {0,1,0};
    glm::vec3 right = glm::cross(look, world_up);
    glm::vec3 up_ = glm::cross(look, right);

    look_at = look_at + (right * dx) + (up_ * dy);
}

void Camera::zoom(float d)
{
    auto dir = glm::normalize(position - look_at);
    position += dir * d;
}

void CameraControl::update_mouse_move(Camera& camera, GLFWwindow* window, float x1, float y1)
{
    if (!allow_updates)
        return;
    // Homogenous coordinates
    glm::vec4 position = glm::vec4(camera.position.x, camera.position.y, camera.position.z, 1);
    glm::vec4 pivot = glm::vec4(camera.look_at.x, camera.look_at.y, camera.look_at.z, 1);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    const float w = static_cast<float>(width);
    const float h = static_cast<float>(height);

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
        // camera.pan(-(x - x1), (y - y1));
    }

    x = x1;
    y = y1;
}

void CameraControl::update_scroll(Camera& camera, GLFWwindow* window, float x, float y)
{
    if (!allow_updates)
        return;

    const float scroll_speed = 0.1f; 
    auto direction = glm::normalize(camera.look_at - camera.position);    
    camera.position += direction * y * scroll_speed;
}