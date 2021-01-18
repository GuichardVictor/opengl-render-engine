#pragma once

#include <glm/glm.hpp>
#include <algorithm>

template<typename T>
T clamp(const T& value, T min, T max)
{
    T new_value = value;
    if (value > max)
        new_value = max;
    if (value < min)
        new_value = min;

    return new_value;
}

template<typename T>
T clamp(T& value, T min, T max)
{
    T new_value = value;
    if (value > max)
        new_value = max;
    if (value < min)
        new_value = min;

    return new_value;
}

glm::vec3 slerp(const glm::vec3& start, const glm::vec3& end, float t);

// C++20 only so implementation is required
static float lerp(float a, float b, float t)
{
    if (t > 1)
        t = 1;
    if (t < 0)
        t = 0;

    return a + t * (b - a);
}

glm::vec3 random_on_sphere();