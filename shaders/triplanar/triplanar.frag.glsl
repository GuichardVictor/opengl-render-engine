#version 450 core

in struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
} fragment;

uniform sampler2D texture_sampler;

out vec4 FragColor;

uniform vec3 camera_position;
uniform vec3 color     = vec3(1.0, 1.0, 1.0);
uniform float color_alpha = 1.0;
uniform float ambiant  = 0.7;
uniform float diffuse  = 0.8;
uniform float specular = 0.5;
uniform int specular_exponent = 128;

uniform float texture_scaling = 1;
uniform float texture_sharpness = 1;
uniform int use_texture;

uniform vec3 light = vec3(0.0, 1.0, 0.0);

vec3 pow_vec(vec3 v, float e)
{
    return vec3(pow(v.x, e), pow(v.y, e), pow(v.z, e));
}

vec3 blendNormal(vec3 normal)
{
	vec3 blending = normal * normal;

    blending = pow_vec(blending, texture_sharpness);

	blending /= vec3(blending.x + blending.y + blending.z);
	return blending;
}

vec4 triplanarMapping (sampler2D texture, vec3 normal, vec3 position, float scale)
{
    vec3 normalBlend = blendNormal(normal);

    vec3 center = position - 0.5;
    center *= scale;
    center += 0.5;

	vec4 x_color = texture2D(texture, center.yz);
	vec4 y_color = texture2D(texture, center.xz);
	vec4 z_color = texture2D(texture, center.xy);

    return (x_color * normalBlend.x + y_color * normalBlend.y + z_color * normalBlend.z);
}

void main()
{
    vec3 n = normalize(fragment.normal.xyz);
    vec3 u = normalize(light-fragment.position.xyz);
    vec3 r = reflect(u,n);
    vec3 t = normalize(fragment.position.xyz-camera_position);


    float diffuse_value  = diffuse * clamp(dot(u,n), 0.0, 1.0);
    float specular_value = specular * pow(clamp(dot(r,t), 0.0, 1.0), specular_exponent);


    vec3 white = vec3(1.0);

    vec4 color_texture = vec4(1.0, 1.0, 1.0, 1.0);
    
    // avoid if statement which is not good in gpu
    color_texture = mix(color_texture, triplanarMapping(texture_sampler, n.xyz, fragment.position.xyz, texture_scaling), use_texture);

    vec3 c = (ambiant+diffuse_value)*color.rgb * color_texture.rgb + specular_value*white;

    FragColor = vec4(c, 1);
}