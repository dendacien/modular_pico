#pragma once

#include "pico/stdlib.h"
#include "lib/game.hpp"
#include "hardware/adc.h"
#include <array>
#include <cstdio>

class Joystick
{
private:
    int last_x, last_y;
    int current_x, current_y;
    int digital_x, digital_y;
    int XPIN, YPIN;

    // static values related to the joystick hardware. Adjust as needed
    static constexpr int ADC_MIN = 0;
    static constexpr int ADC_MAX = 4095;
    static constexpr int ADC_CENTER = 2048;
    static constexpr int CENTER_DEADZONE = 100; // Wider center leeway around 0
    static constexpr int EDGE_LEEWAY = 20;     // Snap to full scale near each edge
    static constexpr int DIGITAL_PRESS_THRESHOLD = 60;
    static constexpr int DIGITAL_RELEASE_THRESHOLD = 35;
public:
    Joystick(int x_set, int y_set);
    std::array<int, 2> sample_input();
    void set_XY();
    int map_axis(int raw, int last_value);
    int map_digital_axis(int analog, int last_digital);
    
    // helpers
    int get_x() { return current_x; }
    int get_y() { return current_y; }
    int get_digital_x() { return digital_x; }
    int get_digital_y() { return digital_y; }
};