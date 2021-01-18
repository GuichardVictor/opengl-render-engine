#version 450 core

layout (triangles) in;
layout (line_strip, max_vertices=2) out;

in struct vertex_data
{
    vec4 position;
    vec4 normal;
} vertex[];



// view transform
uniform mat4 view;
// perspective matrix
uniform mat4 perspective;

uniform vec3 scale;


void main(void)
{
    mat4 M = view;

    vec4 origin = (vertex[0].position + vertex[1].position + vertex[2].position) / 3.0;
    gl_Position = M*origin;
    EmitVertex();

    vec4 normal_direction = (vec4(vertex[0].normal.xyz * scale, vertex[0].normal.w)
                            + vec4(vertex[1].normal.xyz * scale, vertex[1].normal.w)
                            + vec4(vertex[2].normal.xyz * scale, vertex[2].normal.w))
                            / 3.0;
    
    gl_Position = M*(origin + normal_direction * -0.2);
    EmitVertex();

    EndPrimitive();
}
