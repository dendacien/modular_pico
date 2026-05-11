#include "lib/game.hpp"

void Game::auto_move()
{
    if (game_state != GameState::PLAYING || (moving_x == 0 && moving_y == 0))
    {
        return;
    }

    absolute_time_t now = get_absolute_time();
    uint32_t elapsed = absolute_time_diff_us(last_move_update, now);

    if (elapsed >= AUTO_MOVE_INTERVAL)
    {
        if (player.move_by(moving_x, moving_y))
        {
            if (player_at_goal()) 
            {
                player.increase_score();
            }
            last_move_update = now;
        }
        else
        {
            end_game();
        }
    }
}

void Game::update_player_input(int digital_x, int digital_y)
{
    if (game_state != GameState::PLAYING)
    {
        return;
    }

    int move_x = 0;
    int move_y = 0;

    if (std::abs(digital_x) >= std::abs(digital_y))
    {
        move_x = digital_x;
    }
    else
    {
        move_y = digital_y;
    }

    if (move_x == 0 && move_y == 0)
    {
        waiting_for_new_press = true;
    }
    // Preventing 180 move, but not end the game
    else if (std::abs(moving_x) == std::abs(move_x) && std::abs(moving_y) == std::abs(move_y))
    {
        waiting_for_new_press = true;
    }
    else
    {
        moving_x = move_x;
        moving_y = move_y;
        waiting_for_new_press = false;
    }

}

void Game::add_goal_point() 
{
    int x = 0;
    int y = 0;
    do
    {
        x = rand() % 8;
        y = rand() % 8;
    } while (x == player.x() && y == player.y() && player.intersects_body(x, y));

    goal_points.push_back(GoalPoint{x, y, 400, 0});
}

bool Game::player_at_goal()
{
    bool removed_any = false;
    for (auto it = goal_points.begin(); it != goal_points.end();)
    {
        if (it->x == player.x() && it->y == player.y())
        {
            it = goal_points.erase(it);
            removed_any = true;
            continue;
        }
        ++it;
    }
    return removed_any;
}

void Game::render_screen()
{
    uint8_t frame[8] = {0};

    if (game_state == GameState::IDLE)
    {
        absolute_time_t now = get_absolute_time();

        if (absolute_time_diff_us(last_pulse_update, now) >= 50000)
        {
            pulse_index = (pulse_index + 1) % 8;
            last_pulse_update = now;
        }

        memcpy(frame, pluse_frames[pulse_index], 8);
    }
    else if (game_state == GameState::PLAYING)
    {
        player.draw_on_frame(frame);

        if (goal_points.empty())
        {
            add_goal_point();
        }

        // Advance the brightness phase every 30ms (full dim→bright→dim cycle ~1s)
        absolute_time_t now_goal = get_absolute_time();
        if (absolute_time_diff_us(last_goal_phase_update, now_goal) >= 30000)
        {
            goal_phase = (goal_phase + 1) % 32;
            goal_brightness = goal_phase < 16 ? goal_phase : 31 - goal_phase;
            last_goal_phase_update = now_goal;
        }

        // Temporal dithering: show goal pixel only when pwm counter < brightness
        if (goal_pwm_counter < goal_brightness)
        {
            for (auto& goal : goal_points)
            {
                frame[goal.y] &= static_cast<uint8_t>(~(1u << (7u - static_cast<uint>(goal.x))));
            }
        }
        goal_pwm_counter = (goal_pwm_counter + 1) % 16;
    }
    else if (game_state == GameState::ENDING)
    {
        // Initialize frame with all pixels off (1 = off, 0 = on)
        for (int row = 0; row < 8; ++row)
        {
            frame[row] = 0xFFu;
        }

        absolute_time_t now = get_absolute_time();
        uint32_t elapsed_us = absolute_time_diff_us(animation_start_time, now);

        // Phase 0: Flash collision point (0-400ms)
        if (elapsed_us < 400000)
        {
            // Flash every 50ms
            if ((elapsed_us / 50000) % 2 == 0)
            {
                frame[collision_y] &= static_cast<uint8_t>(~(1u << (7u - collision_x)));
            }
        }
        // Phase 1: Fade body from tail to head (400-1000ms)
        else if (elapsed_us < 1000000)
        {
            uint32_t fade_elapsed = elapsed_us - 400000;
            uint32_t fade_duration = 600000;  // 600ms for complete fade
            
            // Calculate which segments to show based on fade progress
            // Segments fade from tail (back of vector) to head (front of vector)
            float fade_progress = static_cast<float>(fade_elapsed) / fade_duration;
            size_t segments_to_show = static_cast<size_t>(saved_body.size() * (1.0f - fade_progress));

            // Draw only the remaining segments
            for (size_t i = saved_body.size() - segments_to_show; i < saved_body.size(); ++i)
            {
                const auto& seg = saved_body[i];
                frame[seg.y] &= static_cast<uint8_t>(~(1u << (7u - seg.x)));
            }
        }
        // Animation complete, return to IDLE
        else
        {
            game_state = GameState::IDLE;
        }
    }

    matrix.render_display(frame);
}

void Game::start_game()
{
    game_state = GameState::PLAYING;
    player = Player(); // make player a fresh object
    goal_points.clear(); // clear the points vector
    moving_x = 0;
    moving_y = 0;
}

void Game::end_game()
{
    collision_x = player.x();
    collision_y = player.y();
    
    // Capture the player's body trail for animation
    saved_body = player.get_body();
    
    // Transition to ending animation state
    game_state = GameState::ENDING;
    animation_phase = 0;
    animation_start_time = get_absolute_time();
}
