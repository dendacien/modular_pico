#include "led.hpp"

void LED::change_state()
{
    state = !state;
    gpio_put(PIN, state);
}

void LED::set_high()
{
    state = true;
    gpio_put(PIN, state);
}

void LED::set_low()
{
    state = false;
    gpio_put(PIN, state);
}

void LED::blink(int number_of_blinks, int sleep_time)
{
    for (int i = 0; i < number_of_blinks; i++)
    {
        change_state();
        sleep_ms(sleep_time);
        change_state();
        sleep_ms(sleep_time);
    }
}