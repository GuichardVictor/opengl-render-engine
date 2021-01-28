#include "obj-parser.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <array>
#include <map>

#ifdef WIN32 || _WIN32 || __WIN32
    #define sscanf sscanf_s
#endif

glm::uvec3 extract_face_index(std::string const& word, const ObjParser::obj_info_type& type)
{
    glm::uvec3 indices = {0,0,0};


    if(type == ObjParser::obj_info_type::vertex)
        sscanf(word.c_str(), "%d", &indices[0]);
    else if( type==ObjParser::obj_info_type::vertex_texture )
        sscanf(word.c_str(), "%d/%d", &indices[0], &indices[1]);
    else if( type==ObjParser::obj_info_type::vertex_normal )
        sscanf(word.c_str(), "%d//%d", &indices[0], &indices[2]);
    else if( type==ObjParser::obj_info_type::vertex_texture_normal )
        sscanf(word.c_str(), "%d/%d/%d", &indices[0], &indices[1], &indices[2]);

    for(int k=0; k<3; ++k)
        indices[k]--;    // obj indices starts at 1

    return indices;

}

std::vector<std::vector<glm::uvec3>> extract_faces(const std::string& filename, const ObjParser::obj_info_type& type)
{
    std::ifstream stream(filename);

    std::vector<std::vector<glm::uvec3>> faces;

    while (stream.good())
    {
        std::string buffer;
        std::getline(stream, buffer);
        if (buffer.size() == 0)
            continue;
        
        std::stringstream tokens(buffer);
        std::string key;

        tokens >> key;

        if (key.size() == 0 || key[0] == '#')
            continue;
        if (key != "f")
            continue;
        
        std::string word;
        std::vector<glm::uvec3> current_face;
        size_t idx = 0;
        while(tokens) {
            tokens >> word;

            if(tokens){
                glm::vec3 face_index = extract_face_index(word, type);
                current_face.push_back(face_index);
            }
        }
        faces.push_back(current_face);
    }

    return faces;
}

std::tuple<std::vector<glm::vec3>, std::vector<glm::vec2>, std::vector<glm::vec3>>
extract_informations(const std::string& filename)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::uvec3> connectivity;

    std::ifstream stream(filename);

    while (stream.good())
    {
        std::string buffer;
        std::getline(stream, buffer);
        if (buffer.size() == 0)
            continue;
        
        std::stringstream tokens(buffer);
        std::string key;

        tokens >> key;

        if (key.size() == 0 || key[0] == '#')
            continue;
        if (key == "v")
        {
            glm::vec3 p;
            tokens >> p.x >> p.y >> p.z;
            positions.push_back(p);
        } else if (key == "vn")
        {
            glm::vec3 p;
            tokens >> p.x >> p.y >> p.z;
            normals.push_back(p);
        } else if (key == "vt")
        {
            glm::vec2 uv;
            tokens >> uv.x >> uv.y;
            uvs.push_back({uv.x, 1.0f-uv.y});
        }
    }

    return {positions, uvs, normals};
}

std::vector<std::array<glm::uvec3, 3>> triangulate(std::vector<std::vector<glm::uvec3>>& faces)
{
    std::vector<std::array<glm::uvec3, 3>> correct_faces;

    const size_t N_faces = faces.size();

    for (size_t k = 0; k < N_faces; k++)
    {
        const auto& current_face = faces[k];
        const int N_info = int(current_face.size());

        for (int i = 0; i < N_info-2; i++)
        {
            const glm::uvec3& f0 = current_face[0];
            const glm::uvec3& f1 = current_face[i+1];
            const glm::uvec3& f2 = current_face[i+2];

            correct_faces.push_back({f0, f1, f2});
        }
    }

    return correct_faces;
}

Mesh create_mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals,
                 const std::vector<std::array<glm::uvec3, 3>>& faces, const ObjParser::obj_info_type& type)
{
    Mesh m;

    auto comp = ObjParser::uvec3_comparator(int(positions.size()));
    std::map<glm::uvec3, int, ObjParser::uvec3_comparator> connectivity_map(comp); // triangle to triangle index

    for (size_t k = 0; k < faces.size(); k++)
    {
        auto& triangle = faces[k];
        glm::uvec3 new_triangle;

        for (int i = 0; i < 3; i++)
        {
            const glm::uvec3& index = triangle[i];
            auto const it = connectivity_map.find(index);
            if (it != connectivity_map.end())
            {
                new_triangle[i] = it->second;
                continue;
            }

            // Get Position index
            const size_t offset = m.positions.size();
            connectivity_map[index] = int(offset);
            new_triangle[i] = int(offset);

            const int position = index[0];

            m.positions.push_back(positions[position]);

            // Do the same for uvs and normals

            if (type == ObjParser::obj_info_type::vertex_texture || type == ObjParser::obj_info_type::vertex_texture_normal)
            {
                const int uv = index[1];
                m.uvs.push_back(uvs[uv]);
            }
            if (type == ObjParser::obj_info_type::vertex_normal || type == ObjParser::obj_info_type::vertex_texture_normal)
            {
                const int normal = index[2];
                m.normal.push_back(normals[normal]);
            }
        }
        m.triangles.push_back(new_triangle);
    }

    return m;
}


Mesh ObjParser::load_mesh(const std::string& filename)
{
    // Extract positions, uvs and normals
    auto [positions, uvs, normals] = extract_informations(filename);
    ObjParser::obj_info_type type = ObjParser::obj_info_type::vertex;;

    if (positions.size() == 0)
    {
        std::cerr << "File: " << filename << ": has no vertices.";
        std::exit(1);
    }

    // From positions, uvs and normals we can deduce the face type
    if (uvs.size() > 0 && normals.size() > 0)
        type = ObjParser::obj_info_type::vertex_texture_normal;
    else if (uvs.size() > 0)
        type = ObjParser::obj_info_type::vertex_texture;
    else if (normals.size() > 0)
        type = ObjParser::obj_info_type::vertex_normal;

    auto obj_faces = extract_faces(filename, type);
    
    auto faces = triangulate(obj_faces); // if faces were not triangles

    // We have vertices, uvs, normals and triangulate faces (if faces were not triangluated)
    // We need to make sure that uvs and normals are unique for each vertices
    auto mesh = create_mesh(positions, uvs, normals, faces, type);

    return mesh;
}