#include "settings.hh"

#include "src/math/math.hh"

void DefaultNoiseSettings::set_values(const std::string& uniform_name, ComputeShader& shader)
{
    std::vector<glm::vec4> values = {
        {offset.x, offset.y, offset.z, num_layers},
        {persistence, lacunarity, scale, multiplier},
        {vertical_shift, 0, 0, 0}
    };

    shader.set_uniform(uniform_name, values);
}

void RidgidNoiseSettings::set_values(const std::string& uniform_name, ComputeShader& shader)
{
    std::vector<glm::vec4> values = {
        {offset.x, offset.y, offset.z, num_layers},
        {persistence, lacunarity, scale, multiplier},
        {power, gain, vertical_shift, smoothing}
    };

    shader.set_uniform(uniform_name, values);
}

// Craters size are more small than large
static float lower_biased_random(float distribution)
{
    if (distribution == 1) { return 0; }

    float value = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);

    float k = glm::clamp<float>(1 - distribution, 0, 1);
    k = k * k * k - 1;

    return glm::clamp<float>((value + value * k) / (value * k + 1), 0, 1);
}

void CraterSettings::set_values(ComputeShader& shader)
{
    std::srand(seed);
    if (crater_ssbo.ssbo_id == 0)
    {
        // Init ssbo
        crater_ssbo = SSBO(sizeof(CraterSettings::CraterData) * CraterSettings::MAX_NUM_CRATERS);
    }
    
    CraterSettings::MAX_NUM_CRATERS = std::abs(CraterSettings::MAX_NUM_CRATERS);
    num_craters = std::abs(num_craters);

    if (num_craters == 0)
    {
        shader.set_uniform("num_craters", int(num_craters));
        return;
    }
    if (num_craters >= CraterSettings::MAX_NUM_CRATERS)
        num_craters = CraterSettings::MAX_NUM_CRATERS - 1;

    std::vector<CraterSettings::CraterData> craters(num_craters);
    
    for (size_t i = 0; i < static_cast<size_t>(num_craters); i++)
    {
        float t = lower_biased_random(distribution);

        float size = lerp(crater_constraints.x, crater_constraints.y, t);
        float floor_height = lerp(-1.2f, -1.0f, t + lower_biased_random(0.3f));
        float smooth = lerp(smooth_constraints.x, smooth_constraints.y, t);

        glm::vec3 crater_center = random_on_sphere();

        craters[i] = {{crater_center.x, crater_center.y, crater_center.z, 0}, {size, floor_height, smooth, 0}};
    }

    craters_ = craters;
    
    crater_ssbo.update_data<CraterData>(craters_);

    shader.set_uniform("num_craters", num_craters);
    shader.set_uniform("rim_steepness", rim_steepness);
    shader.set_uniform("rim_width", rim_width);
    shader.set_buffer("crater_buffer", crater_ssbo);
}

int CraterSettings::MAX_NUM_CRATERS = 500;