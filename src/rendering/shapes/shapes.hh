#pragma once

#include "src/rendering/mesh/mesh.hh"

Mesh mesh_quad(const glm::vec3& p00, const glm::vec3& p10, const glm::vec3& p11, const glm::vec3& p01);
Mesh mesh_icosphere(const int resolution);