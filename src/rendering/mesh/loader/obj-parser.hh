#pragma once

#include <string>

#include "../mesh.hh"

struct ObjParser
{
    enum class obj_info_type
    {
        vertex, // The obj file only defines vertices
        vertex_texture, // The obj file defines vertices and uvs
        vertex_texture_normal, // The obj file defines vertices, uvs and normals
        vertex_normal // The obj file defines vertices and normals
    };

    static Mesh load_mesh(const std::string& filename);

    struct uvec3_comparator
    {
        long int N;
        uvec3_comparator(int size) : N{size} {}
        
        bool operator() (const glm::uvec3& lhs, const glm::uvec3& rhs) const
        {
            const long int offset_lhs = long(lhs.x) + N * (long(lhs.y) + N * long(lhs.z));
            const long int offset_rhs = long(rhs.x) + N * (long(rhs.y) + N * long(rhs.z));
            return offset_lhs < offset_rhs;
        }
    };
};