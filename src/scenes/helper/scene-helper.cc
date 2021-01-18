#include "scene-helper.hh"

#include "src/math/time.hh"


void initialize_interface(gui_information& gui)
{
    std::cout<<"*** Init GLFW ***"<<std::endl;
    glfw_init();
    std::cout<<"\t [OK] GLFW Initialized"<<std::endl;


    std::cout<<"*** Create window ***"<<std::endl;
    gui.window_title = "OpenGL Window";

    const unsigned int width  = 1280;
    const unsigned int height = 720;
    const unsigned opengl_major_version = 4;
    const unsigned opengl_minor_version = 5;

    gui.window = glfw_create_window(width, height, gui.window_title, opengl_major_version, opengl_minor_version);

    std::cout<<"\t [OK] Window Created"<<std::endl;

    std::cout<<"*** Init GLAD ***"<<std::endl;
    glad_init();
    std::cout<<"\t [OK] GLAD Initialized"<<std::endl;

    std::cout<<"*** OPENGL Information ***"<<std::endl;
    std::cout<<"======================================================="<<std::endl;
    opengl_print_version();
    std::cout<<"======================================================="<<std::endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::cout<<"*** Init imgui ***"<<std::endl;
    imgui_init(gui.window);
    std::cout<<"\t [OK] imgui Initialized"<<std::endl;
}

void time_update(gui_information& gui)
{
    bool require_fps_change = Time::update();
    if (require_fps_change)
    {
        const std::string new_window_title = gui.window_title +" ("+std::to_string(Time::fps())+" fps)";

        glfwSetWindowTitle(gui.window, new_window_title.c_str());
    }
}

void clear_scenes()
{
    // Clear buffers and reset gl depth test if it was disabled in the scene.
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void gui_default_interface(gui_information& gui)
{
    imgui_create_frame();

    ImGui::Begin("APOGL GUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
}