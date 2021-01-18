#pragma once

class Time
{
public:
    static bool update();
    static float deltaTime() { return (float)deltaTime_; };
    static int fps() { return fps_; };
private:
    static double previous_fps_time;
    static int frame_count;
    static constexpr double fps_update = 3.0; // Seconds
    static int fps_;

    static double previous_time;
    static double deltaTime_;
};