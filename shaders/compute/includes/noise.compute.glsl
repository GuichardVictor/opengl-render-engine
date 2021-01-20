#version 450 core
#include "simplex_noise.compute.glsl"

float noise(vec3 value, vec4 parameters[3])
{
    // Noise Parameters
    vec3 offset = parameters[0].xyz; // Offset to simulate a displacement
    int num_layers = int(parameters[0].w);
    float persistence = parameters[1].x;
    float lacunarity = parameters[1].y;
    float scale = parameters[1].z;
	float multiplier = parameters[1].w;
	float vertical_shift = parameters[2].x; // parameters[2].yzw is for padding

    // Sum up noise layers
	float noise_value = 0;
    float amplitude = 1;
    float frequency = scale;
    float ridgeWeight = 1;

    for (int i = 0; i < num_layers; i ++)
    {
        noise_value += snoise(value * frequency + offset) * amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

	return noise_value * multiplier + vertical_shift;    

}

// vec4 for alignement + more gpu efficient
float ridgid_noise(vec3 value, vec4 parameters[3])
{
    // Noise Parameters
    vec3 offset = parameters[0].xyz; // Offset to simulate a displacement
    int num_layers = int(parameters[0].w);
    float persistence = parameters[1].x;
    float lacunarity = parameters[1].y;
    float scale = parameters[1].z;
	float multiplier = parameters[1].w;
	float power = parameters[2].x;
    float gain = parameters[2].y;
    float vertical_shift = parameters[2].z; // parameters[2].w is for padding

	// Sum up noise layers
	float noise_value = 0;
    float amplitude = 1;
    float frequency = scale;
    float ridgeWeight = 1;

    for (int i = 0; i < num_layers; i ++)
    {
        float current_noise_value = 1 - abs(snoise(value * frequency + offset));
        current_noise_value = pow(abs(current_noise_value), power);
        current_noise_value *= ridgeWeight;
        ridgeWeight = clamp(current_noise_value * gain, 0.0, 1.0);

        noise_value += current_noise_value * amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

	return noise_value * multiplier + vertical_shift;    
}


// Same as rigid noise but multi sampled around the value point and return the average.
// Helps reducing large spikes
float smoothed_ridgid_noise(vec3 value, vec4 parameters[3])
{
    vec3 direction = normalize(value);

    // Get Axis vectors to correctly apply offsets
    vec3 axisA = cross(direction, vec3(0,1,0));
    vec3 axisB = cross(direction, axisA);

    float offset_dst = parameters[2].w * 0.01; // Smoothness offset

    float sample0 = ridgid_noise(value, parameters);
    float sample1 = ridgid_noise(value - axisA * offset_dst, parameters);
    float sample2 = ridgid_noise(value + axisA * offset_dst, parameters);
    float sample3 = ridgid_noise(value - axisB * offset_dst, parameters);
    float sample4 = ridgid_noise(value + axisB * offset_dst, parameters);

    return (sample0 + sample1 + sample2 + sample3 + sample4) / 5;
}