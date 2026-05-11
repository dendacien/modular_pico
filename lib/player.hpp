#pragma once

#include <vector>
#include <stdio.h>
#include "pico/stdlib.h"

struct Segment
{
    int x;
    int y;
};

class Player
{
private:
    int head_x, head_y;
    int max_x, max_y;
    uint score = 0;
    std::vector<Segment> body;
    uint8_t body_pwm_counter = 0;
    static constexpr uint8_t BODY_BRIGHTNESS = 1; // x/4 - 1 = 25% brightness
public:
    Player() : head_x(4), head_y(4), max_x(7), max_y(7), body{{4, 4}} {};
    void draw_on_frame(uint8_t frame[]);
    bool move(int new_x, int new_y);
    bool move_by(int dx, int dy);
    void increase_score() { score += 1; }
    uint get_score() const { return score; }
    int x() const { return head_x; }
    int y() const { return head_y; }
    bool intersects_body(int new_x, int new_y);
    std::vector<Segment> get_body() const { return body; }
};