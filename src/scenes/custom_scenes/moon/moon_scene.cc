#include "moon_scene.hh"

#include <array>
#include <algorithm>
#include <chrono>

// https://eliasdaler.github.io/using-imgui-with-sfml-pt2/
static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
    auto& vector = *static_cast<std::vector<std::string>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx).c_str();
    return true;
};


void CustomScene::set_gui()
{
    bool changes = false;

    ImGui::Text("%s", "Shape Editor Settings");
    ImGui::Text("Last Sphere Update Time: %dms", last_update_time);
    ImGui::Checkbox("Debug", &debug_mode);
    ImGui::Checkbox("Display Plane", &display_ground);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    if (ImGui::CollapsingHeader("Shade settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Use Texture##shade_settings", &planet.use_texture);
        ImGui::SliderFloat("Scale##shade_settings", &planet.texture_scaling, 0.5f, 10.0f);
        ImGui::SliderFloat("Sharpness##shade_settings", &planet.blend_sharpness, 0.1f, 5.0f);

        static int current_item = 0;
        static int previous_item = current_item;
        ImGui::ListBox("Texture", &current_item, vector_getter,
                       static_cast<void*>(&texture_keys), static_cast<int>(texture_keys.size()));
        
        if (previous_item != current_item)
        {
            previous_item = current_item;
            auto it = textures.begin();
            std::advance(it, current_item);

            planet.texture = it->second;
        }
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    if (ImGui::CollapsingHeader("Noise settings"))
    {
        ImGui::SliderInt("Num Layers##noise_settings", &planet.noise_settings.num_layers, 1, 10); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Lacunarity##noise_settings", &planet.noise_settings.lacunarity, -5.0f, 5.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Persistence##noise_settings", &planet.noise_settings.persistence, .01f, 1.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Scale##noise_settings", &planet.noise_settings.scale, .01f, 2.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Multiplier##noise_settings", &planet.noise_settings.multiplier, -2.0f, 2.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::DragFloat("VerticalShift##noise_settings", &planet.noise_settings.vertical_shift, 0.01f); changes |= ImGui::IsItemDeactivated();
        ImGui::DragFloat3("Sample Offset##noise_settings", &planet.noise_settings.offset.x, 0.01f); changes |= ImGui::IsItemDeactivated();
    }

    if (ImGui::CollapsingHeader("Ridgid settings"))
    {
        ImGui::SliderInt("Num Layers##ridgid1_settings", &planet.ridgid_noise_1.num_layers, 1, 10); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Lacunarity##ridgid1_settings", &planet.ridgid_noise_1.lacunarity, -5.0f, 5.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Persistence##ridgid1_settings", &planet.ridgid_noise_1.persistence, .01f, 1.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Scale##ridgid1_settings", &planet.ridgid_noise_1.scale, .01f, 2.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Multiplier##ridgid1_settings", &planet.ridgid_noise_1.multiplier, -2.0f, 2.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::DragFloat("VerticalShift##ridge1_settings", &planet.ridgid_noise_1.vertical_shift, 0.01f); changes |= ImGui::IsItemDeactivated();
        ImGui::DragFloat3("Sample Offset##ridge1_settings", &planet.ridgid_noise_1.offset.x, 0.01f); changes |= ImGui::IsItemDeactivated();
    }

    if (ImGui::CollapsingHeader("Ridgid settings 2"))
    {
        ImGui::SliderInt("Num Layers##ridgid2_settings", &planet.ridgid_noise_2.num_layers, 1, 10); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Lacunarity##ridgid2_settings", &planet.ridgid_noise_2.lacunarity, -5.0f, 5.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Persistence##ridgid2_settings", &planet.ridgid_noise_2.persistence, .01f, 1.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Scale##ridgid2_settings", &planet.ridgid_noise_2.scale, .01f, 2.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Multiplier##ridgid2_settings", &planet.ridgid_noise_2.multiplier, -2.0f, 2.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::DragFloat("VerticalShift##ridge2_settings", &planet.ridgid_noise_2.vertical_shift, 0.01f); changes |= ImGui::IsItemDeactivated();
        ImGui::DragFloat3("Sample Offset##ridge2_settings", &planet.ridgid_noise_2.offset.x, 0.01f); changes |= ImGui::IsItemDeactivated();
    }

    if (ImGui::CollapsingHeader("Crater Settings"))
    {
        changes |= ImGui::InputInt("Crater Seed##crater_settings", &planet.crater_settings.seed);
        ImGui::SliderInt("Num Craters##crater_settings", &planet.crater_settings.num_craters, 0, 400); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat2("Crater Size Constraints##crater_settings", &planet.crater_settings.crater_constraints.x, 0.0005f, 0.1f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Rim Steepness##crater_settings", &planet.crater_settings.rim_steepness, 0, 1.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Rim Width##crater_settings", &planet.crater_settings.rim_width, 0, 3.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat("Distribution##crater_settings", &planet.crater_settings.distribution, 0.2f, 1.0f); changes |= ImGui::IsItemDeactivated();
        ImGui::SliderFloat2("Smoothing Constraints##crater_settings", &planet.crater_settings.smooth_constraints.x, 0.0f, 1.0f); changes |= ImGui::IsItemDeactivated();
    }

    if (changes)
    {
        // Ensure constraints are correctly sets
        if (planet.crater_settings.crater_constraints.x > planet.crater_settings.crater_constraints.y)
            std::swap(planet.crater_settings.crater_constraints.x, planet.crater_settings.crater_constraints.y);
        if (planet.crater_settings.smooth_constraints.x > planet.crater_settings.smooth_constraints.y)
            std::swap(planet.crater_settings.smooth_constraints.x, planet.crater_settings.smooth_constraints.y);

        // Reset sphere mesh info
        sphere_mesh.positions = sphere_mesh_cached.positions;
        sphere_mesh.normal = sphere_mesh_cached.normal;

        // Update sphere mesh
        update_sphere_heights();

        // Update GPU Data
        update_gpu_sphere_data();
    }
}

void CustomScene::update_gpu_sphere_data()
{
    planet.data.update_normals(sphere_mesh.normal);
    planet.data.update_position(sphere_mesh.positions);
}

void CustomScene::update_sphere_heights()
{
    const int NUM_WORK_GROUP = 512;

    SSBO ssbo(sizeof(glm::vec4) * sphere_mesh.positions.size());

    std::vector<glm::vec4> ssbo_data;
    
    std::transform(sphere_mesh.positions.begin(), sphere_mesh.positions.end(),
                   std::back_inserter(ssbo_data),
                   [](glm::vec3 v) {
                       return glm::vec4(v.x, v.y, v.z, 0);
                   });
    
    ssbo.add_data<glm::vec4>(ssbo_data);

    if (cp_shader.shader_id != Shader::get_current_shader())
        cp_shader.use();

    planet.noise_settings.set_values("noise_settings", cp_shader);
    planet.ridgid_noise_1.set_values("ridgid_noise_settings", cp_shader);
    planet.ridgid_noise_2.set_values("ridgid_noise_settings_2", cp_shader);
    planet.crater_settings.set_values(cp_shader);

    cp_shader.set_uniform("vertices_count", (int)(sphere_mesh.positions.size()));
    cp_shader.set_buffer("vertex_position_height", ssbo);

    ssbo.bind(); // Ensure it is bound
    
    auto start = std::chrono::high_resolution_clock::now();

    cp_shader.dispatch(NUM_WORK_GROUP);
    cp_shader.wait_for(GL_SHADER_STORAGE_BARRIER_BIT);
    
    auto heights = ssbo.retreive_data<glm::vec4>((int)(sphere_mesh.positions.size()));

    // Unbind ssbo buffers
    SSBO::unbind();

    std::transform(heights.begin(), heights.end(), sphere_mesh.positions.begin(), sphere_mesh.positions.begin(), [](glm::vec4& v, glm::vec3& p) { return v.w * p;});

    Mesh::recalculate_normals(sphere_mesh, true);
    
    auto end = std::chrono::high_resolution_clock::now();
    last_update_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void CustomScene::setup()
{
    const int RESOLUTION = 7; // higher than 7 will take a lot of time to re compute normals
    sphere_mesh = mesh_icosphere(RESOLUTION);
    sphere_mesh_cached = Mesh(sphere_mesh);

    update_sphere_heights();

    textures.insert({
        "moon_noise", Texture::from_file("./assets/moon_noise.png", Image::color_type::RGB, GL_REPEAT, GL_REPEAT)
    });
    textures.insert({
        "rock_texture", Texture::from_file("./assets/rock_texture_2.png", Image::color_type::RGB, GL_REPEAT, GL_REPEAT)
    });
    textures.insert({
        "checker", Texture::from_file("./assets/checker.png", Image::color_type::RGB, GL_REPEAT, GL_REPEAT)
    });

    transform(textures.begin(), textures.end(), back_inserter(texture_keys), [](std::pair<std::string, GLuint> p) { return p.first; });

    planet = Planet(sphere_mesh, default_shader);
    planet.uniform.shading.specular = 0;
    planet.uniform.transform.scale = {1.0, 0.8, 1.0}; // planet y scale is reduced to give the oval shape of a planet.
    planet.texture = textures["moon_noise"];

    quad = MeshRenderer(mesh_quad({-1, 0, -1}, {1, 0, -1}, {1, 0, 1}, {-1, 0, 1}), wireframe_shader);
    quad.uniform.color = {0.8, 0.8, 0.8};
}

void CustomScene::draw()
{
    if (debug_mode)
    {
        planet.uniform.color = {1, 1, 0.3f};
        planet.draw(camera, normal_disp_shader);
        planet.uniform.color = {0.7, 0.7, 0.7};
        planet.draw(camera, wireframe_shader);
    } else 
    {
        planet.draw(camera, [this](Shader& shader) {
            shader.set_uniform("texture_scaling", this->planet.texture_scaling);
            shader.set_uniform("texture_sharpness", this->planet.blend_sharpness);
            shader.set_uniform("use_texture", (int)this->planet.use_texture);
        });
    }

    if (display_ground)
        quad.draw(camera);
}

void CustomScene::load_shaders()
{
    ComputeShader::print_workgroups_capabilities();

    default_shader = Shader("./shaders/triplanar/triplanar.vert.glsl", "./shaders/triplanar/triplanar.frag.glsl");
    wireframe_shader = Shader("./shaders/wireframe/wireframe.vert.glsl", "./shaders/wireframe/wireframe.geo.glsl", "./shaders/wireframe/wireframe.frag.glsl");
    normal_disp_shader = Shader("./shaders/normals/normal.vert.glsl", "./shaders/normals/normal.geo.glsl", "./shaders/normals/normal.frag.glsl");
    cp_shader = ComputeShader("./shaders/compute/height_generator.compute.glsl");    
}