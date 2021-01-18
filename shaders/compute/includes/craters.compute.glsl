#version 450 core

struct Crater
{
    vec3 center;
    // padding
    float padding_0;

    float radius;
    float floor_height;
    float smoothness;

    // padding
    float padding_1;
};

layout (std430, binding = 0) buffer crater_buffer
{
    Crater craters[];
};

uniform int num_craters;
uniform float floor_height;
uniform float rim_steepness;
uniform float rim_width;


// Smooth minimum of two values, controlled by smoothing factor k
// When k = 0, this behaves identically to min(a, b)
float smooth_min(float a, float b, float k)
{
    k = max(0, k);
    // https://www.iquilezles.org/www/articles/smin/smin.htm
    float h = max(0, min(1, (b - a + k) / (2 * k)));
    return a * h + b * (1 - h) - k * h * (1 - h);
}

// Smooth maximum of two values, controlled by smoothing factor k
// When k = 0, this behaves identically to max(a, b)
float smooth_max(float a, float b, float k)
{
    k = min(0, -k);
    float h = max(0, min(1, (b - a + k) / (2 * k)));
    return a * h + b * (1 - h) - k * h * (1 - h);
}


float compute_crater_depth(vec3 vertex_pos)
{
    float crater_height = 0;

    for (int i = 0; i < num_craters; i++)
    {
        // Compute Crater Streignth on the vertex position
        float x = length(vertex_pos - craters[i].center) / max(craters[i].radius, 0.0001);

        float cavity_shape = x * x - 1;

        float rimX = min(x - 1 - rim_width, 0);
        float rim_shape = rim_steepness * rimX * rimX;

		float crater_shape = smooth_max(cavity_shape, craters[i].floor_height, craters[i].smoothness);
		crater_shape = smooth_min(crater_shape, rim_shape, craters[i].smoothness);

        crater_height += crater_shape * craters[i].radius;
    }

    return crater_height;
}