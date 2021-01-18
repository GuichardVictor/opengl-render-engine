#pragma once

#include "src/rendering/shader/shader.hh"


struct DefaultNoiseSettings
{
    DefaultNoiseSettings() = default;

    DefaultNoiseSettings(int _num_layers, float _lacunarity, float _persistence, float _scale, float _multiplier, float _vertical_shift, glm::vec3 _offset = {0,0,0})
        : num_layers(_num_layers), lacunarity(_lacunarity), persistence(_persistence), scale(_scale),
          multiplier(_multiplier), vertical_shift(_vertical_shift), offset(_offset)
    {}

    int num_layers = 4;
    float lacunarity = 2;
    float persistence = 0.5f;
    float scale = 0.97f;
    float multiplier = 1;
    float vertical_shift = 0;
    glm::vec3 offset = {0,0,0};

    void set_values(const std::string& uniform_name, ComputeShader& shader);
};

struct RidgidNoiseSettings
{
    RidgidNoiseSettings() = default;

    RidgidNoiseSettings(int _num_layers, float _lacunarity, float _persistence, float _scale, float _multiplier, float _power, float _gain, float _vertical_shift, float _smoothing, glm::vec3 _offset={0,0,0})
        : num_layers(_num_layers), lacunarity(_lacunarity), persistence(_persistence), scale(_scale), multiplier(_multiplier),
          power(_power), gain(_gain), vertical_shift(_vertical_shift), smoothing(_smoothing), offset(_offset)
    {}

    int num_layers = 4;
    float lacunarity = 5;
    float persistence = 0.5f;
    float scale = 1.82f;
    float multiplier = -2.84f;
    float power = 2;
    float gain = 0.5f;
    float vertical_shift = 0;
    float smoothing = 3;
    glm::vec3 offset = {0,0,0};

    void set_values(const std::string& uniform_name, ComputeShader& shader);
};

struct CraterSettings
{
    int num_craters = 50; // max 500
    glm::vec2 crater_constraints = {0.01f, 0.1f};
    float rim_steepness = 0.13f;
    float rim_width = 1.66f;
    glm::vec2 smooth_constraints = {0.4f, 1.5f};
    float distribution = 0.5f;

    int seed = 5;

    void set_values(ComputeShader& shader);

    struct CraterData
    {
        float center[4]; // one more for padding
        float setting[4]; // one more for padding
    };

    std::vector<CraterSettings::CraterData> craters_; // caching to avoid calling delete

    SSBO crater_ssbo;
    static int MAX_NUM_CRATERS;
};