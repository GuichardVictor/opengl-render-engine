#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

struct Mesh
{

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec2> uvs;

    std::vector<glm::uvec3> triangles;

    static void recalculate_normals(Mesh& mesh, bool invert=false)
    {
        mesh.normal = std::vector<glm::vec3>(mesh.positions.size(), {0,0,0});

        for (auto& triangle : mesh.triangles)
        {
            auto& v1 = mesh.positions[triangle.x];
            auto& v2 = mesh.positions[triangle.y];
            auto& v3 = mesh.positions[triangle.z];

            glm::vec3 u = v1 - v3;
            glm::vec3 v = v1 - v2;
            auto face_normal = glm::cross(u, v);

            face_normal = glm::normalize(face_normal);
            mesh.normal[triangle.x] += face_normal;
            mesh.normal[triangle.y] += face_normal;
            mesh.normal[triangle.z] += face_normal; 
        }
        
        std::transform(mesh.normal.begin(), mesh.normal.end(), mesh.normal.begin(), [](glm::vec3& n) { return glm::normalize(n); });
    }
};