#version 450 core

in struct fragment_data
{
    vec4 position;
} fragment;


uniform vec3 color = vec3(1.0, 1.0, 1.0);
out vec4 FragColor;

void main()
{
    FragColor = vec4(color.rgb, 1.0);
}
