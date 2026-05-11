#include <string_view>
#include "lib/game.hpp"
#include "lib/button.hpp"
#include "lib/joystick.hpp"

int main()
{
    stdio_init_all();
    Matrix matrix(19, 20, 18);
    Game game(matrix);
    Joystick joystick(26, 27);
    Button start_button(16);

    absolute_time_t last_sample = get_absolute_time();
    srand(last_sample);

    while (true) 
    {
        game.render_screen();

        absolute_time_t now = get_absolute_time();

        if (absolute_time_diff_us(last_sample, now) >= 5000)
        {
            joystick.set_XY();
            if (start_button.pushed() && game.state() == GameState::IDLE)
            {
                game.start_game();
            }
            game.auto_move();
            game.update_player_input(joystick.get_digital_x(), joystick.get_digital_y());
            last_sample = now;
        }
    }
}
