#version 450 core

in  vec2  uvs;
out vec4  color;
  
uniform sampler2D scene;

void main()
{
    color = vec4(1, 0, 1, 1);
}