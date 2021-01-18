#include "math.hh"

glm::vec3 slerp(const glm::vec3& start, const glm::vec3& end, float t)
{
    t = clamp<float>(t, 0, 1);
    auto dot = glm::dot(start, end);
    dot = clamp<float>(dot, -1, 1);
    float theta = std::acosf(dot) * t;
    glm::vec3 relative_vector = end - start * dot;
    relative_vector = glm::normalize(relative_vector);

    return ((start * std::cosf(theta)) + (relative_vector*std::sinf(theta)));
}

glm::vec3 random_on_sphere()
{
    float x = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX) * 2 - 1;
    float y = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX) * 2 - 1;
    float z = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX) * 2 - 1;

    if (x == y && x == z && x == 0)
        return glm::vec3(0, 1, 0);

    return glm::normalize(glm::vec3{x, y, z});
}