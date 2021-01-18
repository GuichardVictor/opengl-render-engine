#pragma once

#include <functional>

#include "src/rendering/shader/shader.hh"

class PostProcessing
{
public:
    PostProcessing() = default;
    PostProcessing(const Shader& shader); // Window Size
    PostProcessing(const Shader& shader, unsigned int width, unsigned int height); // Custom size

    void render(std::function<void(void)> render_function, std::function<void(Shader&)> shader_init);

    Shader shader;
    GLuint texture = 0;
    unsigned int width, height = 0;

private:
    // Quad info
    GLuint vao_id = 0;
    GLuint vbo_id = 0;

    // Render buffer info
    GLuint framebuffer_id = 0;
    GLuint rbo_id = 0;
    GLuint fbo_id = 0;

    void init_quad();
    void init_buffers();
};