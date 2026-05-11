#include "joystick.hpp"

Joystick::Joystick(int x_set, int y_set)
{
    XPIN = x_set;
    YPIN = y_set;
    last_x = 0;
    last_y = 0;
    current_x = 0;
    current_y = 0;
    digital_x = 0;
    digital_y = 0;

    adc_init();
    adc_gpio_init(XPIN);
    adc_gpio_init(YPIN);
}

std::array<int, 2> Joystick::sample_input()
{
    adc_select_input(0);
    int x_value = adc_read();
    adc_select_input(1);
    int y_value = adc_read();

    return std::array<int, 2>{{x_value, y_value}};
}

int Joystick::map_axis(int raw, int last_value)
{
    int mapped = 0;

    if (raw <= ADC_MIN + EDGE_LEEWAY)
    {
        mapped = -100;
    }
    else if (raw >= ADC_MAX - EDGE_LEEWAY)
    {
        mapped = 100;
    }
    else
    {
        const int centered = raw - ADC_CENTER;
        if (centered >= -CENTER_DEADZONE && centered <= CENTER_DEADZONE)
        {
            mapped = 0;
        }
        else
        {
            mapped = (raw * (200.0/ADC_MAX)) - 100;
        }
    }

    if (mapped == -100 || mapped == 0 || mapped == 100)
    {
        return mapped;
    }

    // Light smoothing in the mid-range to reduce jitter.
    return (last_value * 3 + mapped) / 4;
}

int Joystick::map_digital_axis(int analog, int last_digital)
{
    if (last_digital == 0)
    {
        if (analog >= DIGITAL_PRESS_THRESHOLD)
        {
            return 1;
        }
        if (analog <= -DIGITAL_PRESS_THRESHOLD)
        {
            return -1;
        }
        return 0;
    }

    if (last_digital > 0)
    {
        return (analog <= DIGITAL_RELEASE_THRESHOLD) ? 0 : 1;
    }

    return (analog >= -DIGITAL_RELEASE_THRESHOLD) ? 0 : -1;
}

void Joystick::set_XY()
{
    last_x = current_x;
    last_y = current_y;
    std::array<int, 2> raw = sample_input();
    current_x = map_axis(raw[0], last_x);
    current_y = map_axis(raw[1], last_y);
    digital_x = map_digital_axis(current_x, digital_x);
    digital_y = map_digital_axis(current_y, digital_y);
}