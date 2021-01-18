#include "postprocessing.hh"
#include "src/rendering/texture/texture.hh"


PostProcessing::PostProcessing(const Shader& shader)
{
    auto window = glfwGetCurrentContext();
    glfwGetWindowSize(window, (int*)&width, (int*)&height);

    init_buffers();
    init_quad();
}


PostProcessing::PostProcessing(const Shader& shader, unsigned int width, unsigned int height)
{
    this->width = width;
    this->height= height;

    init_buffers();
    init_quad();
}

void PostProcessing::render(std::function<void(void)> render_function, std::function<void(Shader&)> shader_init)
{
    if (framebuffer_id == 0 || !glIsFramebuffer(framebuffer_id))
        return;
    if (fbo_id == 0 || !glIsFramebuffer(fbo_id))
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

    // Clear FrameBuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    render_function();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint current_shader = Shader::get_current_shader();
    if (current_shader != shader.shader_id)
    {
        shader.use();
    }

    shader_init(shader);

    if (texture == 0 || !glIsTexture(texture))
        return;

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (vao_id == 0 || !glIsVertexArray(vao_id))
        return;

    glBindVertexArray(vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessing::init_quad()
{
    float quad_data[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    glBindVertexArray(vao_id);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    TEST_OPENGL_ERROR();
}

void PostProcessing::init_buffers()
{
    glGenFramebuffers(1, &framebuffer_id);
    glGenFramebuffers(1, &fbo_id);
    glGenRenderbuffers(1, &rbo_id);
    // initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height); // allocate storage for render buffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo_id);

    TEST_OPENGL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
    texture = Texture::empty(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0); // attach texture to framebuffer as its color attachment
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    TEST_OPENGL_ERROR();
}