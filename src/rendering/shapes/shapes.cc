#include "shapes.hh"
#include "src/rendering/shapes/mesh-sphere.hh"

static Mesh& mesh_fill_empty(Mesh& mesh)
{
    if (mesh.positions.size() == 0)
        return mesh;
    
    if (mesh.normal.size() < mesh.positions.size())
        mesh.normal = std::vector<glm::vec3>(mesh.positions.size(), {0,0,0});
    if (mesh.uvs.size() < mesh.positions.size())
        mesh.uvs = std::vector<glm::vec2>(mesh.positions.size(), {0,0});
    if (mesh.triangles.size() < mesh.positions.size())
        mesh.triangles = std::vector<glm::uvec3>(mesh.positions.size(), {0,0,0});

    return mesh;
}

Mesh mesh_quad(const glm::vec3& p00, const glm::vec3& p10, const glm::vec3& p11, const glm::vec3& p01)
{
    // Compute Plane Normal
    const glm::vec3 normal = glm::normalize(glm::cross(glm::normalize(p10 - p00), glm::normalize(p01 - p00)));

    Mesh quad;
    quad.positions = {p00, p10, p11, p01};
    quad.normal = {normal,normal,normal,normal};
    quad.uvs = {{0,0}, {1,0}, {1,1}, {0,1}};
    quad.triangles = {{0,1,2}, {0,2,3}};

    return quad;
}

Mesh mesh_icosphere(const int resolution)
{
    auto sphere = IcoSphere(resolution);
    Mesh mesh;

    mesh.positions = sphere.vertices;
    
    mesh.triangles = std::vector<glm::uvec3>(sphere.triangles.size());
    for (size_t i = 0; i < sphere.triangles.size(); i++)
        mesh.triangles[i] = {sphere.triangles[i].x, sphere.triangles[i].y, sphere.triangles[i].z};
    
    mesh.uvs = std::vector<glm::vec2>(mesh.positions.size(), {0,0});
    
    // Need normal computation
    Mesh::recalculate_normals(mesh, true);

    return mesh;
}