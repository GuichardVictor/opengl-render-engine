#pragma once

#include <iostream>
#include "../../helpers/lib-helpers.hh"

struct gui_information
{
    GLFWwindow* window;
    std::string window_title;
};

void initialize_interface(gui_information& gui);
void clear_scenes();
void gui_default_interface(gui_information& gui);
void time_update(gui_information& gui);