#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

class Button 
{
private:
    uint PIN;
public:
    explicit Button(int button_pin) : PIN(button_pin) {
        gpio_init(PIN);
        gpio_set_dir(PIN, GPIO_IN);
        gpio_pull_up(PIN);
    }
    bool pushed() { return !gpio_get(PIN); }
};