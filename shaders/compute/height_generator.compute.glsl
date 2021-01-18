#version 450 core

#include "includes/noise.compute.glsl"
#include "includes/craters.compute.glsl"

// Depending on resolution, vertices and heights can get quite huge
// Shared memory on low end gpu might not be able to handle it.
layout (std430, binding = 1) buffer vertex_position_height
{
    vec4 data[]; // vec4 for alignment: xyz: vertex position, w: height
};

uniform int vertices_count;

uniform vec4 noise_settings[3];
uniform vec4 ridgid_noise_settings[3];
uniform vec4 ridgid_noise_settings_2[3];


const float elevation_multiplier = 0.01;


layout (local_size_x = 512, local_size_y = 1, local_size_z = 1) in;
void main()
{
    const uint id = gl_GlobalInvocationID.x;
    if (id >= vertices_count) { return; }

    vec3 vertex = data[id].xyz;

    float crater_height = compute_crater_depth(vertex);

    float base_noise = noise(vertex, noise_settings); // Base noise elevation
    
    float ridge_noise   = smoothed_ridgid_noise(vertex, ridgid_noise_settings); // Second layer with different settings
    float ridge_noise_2 = smoothed_ridgid_noise(vertex, ridgid_noise_settings_2); // Third layer with different settings

    float noise_value = (base_noise + ridge_noise + ridge_noise_2) * elevation_multiplier;

    data[id].w = 1 + noise_value + crater_height;
}