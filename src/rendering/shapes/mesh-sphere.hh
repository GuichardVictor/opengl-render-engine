#pragma once

#include <vector>
#include <array>
#include <map>
#include <glm/glm.hpp>
#include <algorithm>

#include "src/math/math.hh"

/*
* This part is a readaptation of: https://github.com/libigl/eigen/blob/master/demos/opengl/icosphere.cpp
* Code is simplified, cleaned and readapted for glm and modern c++.
*/

/*  Taken from: https://fr.wikipedia.org/wiki/Icosa%C3%A8dre
**
**  Static icosahedron informations
*/
const float X=.525731112119133606f;
const float Z=.850650808352039932f;
const float N=0.f;

static const std::vector<glm::vec3> icosahedron_vertices =
{
  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
  {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
};

static const std::vector<glm::vec3> icosahedron_triangles =
{
  {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
  {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
  {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
  {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
};


class IcoSphere
{
public:
    IcoSphere(int resolution_)
    {
        resolution = resolution_;
        vertices = icosahedron_vertices;
        triangles = icosahedron_triangles;

        for (int i = 0; i < resolution; i++)
        {
            triangles = IcoSphere::subdivide_faces(vertices, triangles);
        }
    }

    int resolution;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> triangles;

private:

    using index_lookup_type = std::map<std::pair<size_t, size_t>, size_t>;

    static size_t vertex_for_edge(index_lookup_type& lookup_table, std::vector<glm::vec3>& vertices, const size_t first, size_t second)
    {
        index_lookup_type::key_type key(first, second);
        if (key.first>key.second)
            std::swap(key.first, key.second);

        auto inserted = lookup_table.insert({key, vertices.size()});

        if (inserted.second)
        {
            // New middle point
            auto new_v = glm::normalize(vertices[first] + vertices[second]);
            vertices.push_back(new_v);
        }

        return inserted.first->second;
    }

    static std::vector<glm::vec3> subdivide_faces(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& triangles)
    {
        index_lookup_type lookup_table;
        std::vector<glm::vec3> triangle_subdivisions;

        for (auto&& triangle : triangles)
        {
            std::array<size_t, 3> middle;
            for (int edge = 0; edge < 3; edge++)
            {
                middle[edge] = vertex_for_edge(lookup_table, vertices, (size_t)(triangle[edge]), (size_t)(triangle[(edge+1)%3]));
            }

            triangle_subdivisions.push_back({triangle[0], middle[0], middle[2]});
            triangle_subdivisions.push_back({triangle[1], middle[1], middle[0]});
            triangle_subdivisions.push_back({triangle[2], middle[2], middle[1]});
            triangle_subdivisions.push_back({middle[0], middle[1], middle[2]});
        }

        return triangle_subdivisions;
    }
};