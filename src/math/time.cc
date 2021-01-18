#include "time.hh"

#include <GLFW/glfw3.h>

double Time::previous_time = 0;
double Time::previous_fps_time = 0;
int Time::frame_count = 0;
int Time::fps_ = 0;
double Time::deltaTime_ = 0;

bool Time::update()
{
    frame_count++;
    const auto current_time = glfwGetTime();
    deltaTime_ = current_time - previous_time;

    if (current_time - previous_fps_time > fps_update)
    {
        fps_ = static_cast<int>((double)frame_count / (current_time-previous_fps_time));

        frame_count = 0;
        previous_fps_time = current_time;
        return true;
    }

    previous_time = current_time;

    return false;
}