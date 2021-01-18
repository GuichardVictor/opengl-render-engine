#include <iostream>

#include "helpers/lib-helpers.hh"
#include "scenes/scene.hh"

gui_information gui;
CustomScene current_scene;

void on_window_resize(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
    current_scene.camera.image_aspect = width / static_cast<float>(height);;
}

void on_mouse_move(GLFWwindow* window, double x, double y)
{
    current_scene.camera_control.update_mouse_move(current_scene.camera, window, float(x), float(y));
}

void on_scrollwheel(GLFWwindow* window, double x, double y)
{
    current_scene.camera_control.update_scroll(current_scene.camera, window, float(x), float(y));
}

void on_mouse_click(GLFWwindow* window, int button, int action, int mods)
{
    ImGui::SetWindowFocus(nullptr);
}

int main()
{
    initialize_interface(gui);

    // Set GLFW callbacks
    glfwSetCursorPosCallback(gui.window, on_mouse_move);
    glfwSetWindowSizeCallback(gui.window, on_window_resize);
    glfwSetMouseButtonCallback(gui.window, on_mouse_click);
    glfwSetScrollCallback(gui.window, on_scrollwheel);

    // Setup Scene camera
    int width, height;
    glfwGetWindowSize(gui.window, &width, &height);
    current_scene.camera.image_aspect = width / static_cast<float>(height);;

    // load shaders
    current_scene.load_shaders();

    // Run Scene Setup
    current_scene.setup();

    while (!glfwWindowShouldClose(gui.window))
    {
        TEST_OPENGL_ERROR();
        time_update(gui);

        // Clear all drawn elements.
        clear_scenes(); TEST_OPENGL_ERROR();

        // Set ImGui information
        gui_default_interface(gui);
        current_scene.set_gui();

        // Update Scene
        current_scene.update();
        // Draw Scene
        current_scene.draw();

        // Render ImGui
        ImGui::End();
        current_scene.camera_control.allow_updates = !(ImGui::IsAnyWindowFocused());
        imgui_render_frame(gui.window);

        glfwSwapBuffers(gui.window);
        glfwPollEvents();
        
        TEST_OPENGL_ERROR();
    }

    imgui_cleanup();

    glfwDestroyWindow(gui.window);
    glfwTerminate();
}