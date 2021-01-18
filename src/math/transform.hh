#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct Transform
{
    glm::vec3 translation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::mat3 rotation = glm::mat3(1);
};