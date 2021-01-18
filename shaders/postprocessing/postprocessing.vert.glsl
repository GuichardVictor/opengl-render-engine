#version 450 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 uvs;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f); 
    uvs = vertex.zw;
} 