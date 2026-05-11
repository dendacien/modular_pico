#pragma once

#include <cstdint>
#include <stdio.h>
#include "pico/stdlib.h"

class Matrix
{
private:
    uint STcp, SHcp, DS;
public:
    explicit Matrix(uint st_cp, uint sh_cp, uint ds_pin);
    void shift_out(uint8_t value, bool msb_first = true);
    void render_display(uint8_t datArray[8]);
    // Helpers
    void hold_stcp() { gpio_put(STcp, false); }
    void pull_stcp() { gpio_put(STcp, true); }
};