#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texture_uv;

out struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
} fragment;


// model transformation
uniform vec3 translation = vec3(0.0, 0.0, 0.0);                      // user defined translation
uniform mat3 rotation = mat3(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0); // user defined rotation
uniform vec3 scaling = vec3(1.0, 1.0, 1.0);                          // user defined scaling


// view transform
uniform mat4 view;
// perspective matrix
uniform mat4 perspective;

void main()
{
    // scaling matrix
    mat4 S = mat4(scaling.x,0.0,0.0,0.0, 0.0, scaling.y,0.0,0.0, 0.0,0.0, scaling.z,0.0, 0.0,0.0,0.0,1.0);
    // 4x4 rotation matrix
    mat4 R = mat4(rotation);
    // 4D translation
    vec4 T = vec4(translation,0.0);


    fragment.color = vec4(1.0, 1.0, 1.0, 1.0);
    fragment.texture_uv = texture_uv;

    fragment.normal = R*normal;
    vec4 position_transformed = R*S*position + T;

    fragment.position = position_transformed;
    gl_Position = view * position_transformed;
}
