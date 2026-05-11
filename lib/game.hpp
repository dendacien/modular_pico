#pragma once

#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"
#include "lib/matrix_display.hpp"
#include "lib/player.hpp"

// Forward declaration for Segment (defined in player.hpp)
struct Segment;

enum class GameState
{
    IDLE,     // Defaults to 0
    PLAYING,  // Defaults to 1
    ENDING    // End-game animation
};

struct GoalPoint 
{
    int x, y;
    uint16_t lifetime_ms; // Counts down
    uint8_t animation_frame;
};

class Game
{
private:
    Matrix& matrix;
    Player player;
    std::vector<GoalPoint> goal_points;
    GameState game_state;
    uint8_t pluse_frames[8][8] = {
        // Frame 0: center dot
        {0b00000000,0b00000000,0b00000000,0b00011000,0b00011000,0b00000000,0b00000000,0b00000000},
        // Frame 1: small plus
        {0b00000000,0b00000000,0b00011000,0b00111100,0b00111100,0b00011000,0b00000000,0b00000000},
        // Frame 2: bigger diamond-ish
        {0b00000000,0b00011000,0b00111100,0b01111110,0b01111110,0b00111100,0b00011000,0b00000000},
        // Frame 3: near full
        {0b00011000,0b00111100,0b01111110,0b11111111,0b11111111,0b01111110,0b00111100,0b00011000},
        // Frame 4: full-ish square
        {0b00111100,0b01111110,0b11111111,0b11111111,0b11111111,0b11111111,0b01111110,0b00111100},
        // Frame 5: contract
        {0b00011000,0b00111100,0b01111110,0b11111111,0b11111111,0b01111110,0b00111100,0b00011000},
        // Frame 6: contract more
        {0b00000000,0b00011000,0b00111100,0b01111110,0b01111110,0b00111100,0b00011000,0b00000000},
        // Frame 7: back to small plus
        {0b00000000,0b00000000,0b00011000,0b00111100,0b00111100,0b00011000,0b00000000,0b00000000}
    };
    uint pulse_index;
    absolute_time_t last_pulse_update;
    absolute_time_t last_move_update;
    absolute_time_t last_goal_phase_update;
    uint8_t goal_phase;     // 0-31, triangle wave index
    uint8_t goal_brightness; // 0-15, current PWM duty cycle
    uint8_t goal_pwm_counter; // 0-15, increments each render call
    bool waiting_for_new_press;
    int moving_x = 0;
    int moving_y = 0;
    uint32_t AUTO_MOVE_INTERVAL = 250000; // microseconds (500ms)
    // End-game animation state
    int collision_x, collision_y;
    std::vector<Segment> saved_body;
    absolute_time_t animation_start_time;
    uint8_t animation_phase;  // 0: flash collision, 1: fade body

public:
    Game(Matrix& m)
        : matrix(m),
          game_state(GameState::IDLE),
          pulse_index(0),
          last_pulse_update(get_absolute_time()),
          last_move_update(get_absolute_time()),
          last_goal_phase_update(get_absolute_time()),
          goal_phase(0),
          goal_brightness(0),
          goal_pwm_counter(0),
          waiting_for_new_press(true),
          collision_x(0),
          collision_y(0),
          animation_start_time(get_absolute_time()),
          animation_phase(0) {}
    void render_screen();
    void auto_move();
    void update_player_input(int digital_x, int digital_y);
    void start_game();
    void add_goal_point();
    bool player_at_goal();
    void end_game();
    // helpers
    GameState state() { return game_state; }
};