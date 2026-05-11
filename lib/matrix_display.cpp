#include "matrix_display.hpp"

Matrix::Matrix(uint st_cp, uint sh_cp, uint ds_pin)
{
    STcp = st_cp;
    SHcp = sh_cp;
    DS = ds_pin;
    gpio_init(STcp);
    gpio_init(SHcp);
    gpio_init(DS);
    gpio_set_dir(STcp, GPIO_OUT);
    gpio_set_dir(SHcp, GPIO_OUT);
    gpio_set_dir(DS, GPIO_OUT);
    gpio_put(STcp, false);
    gpio_put(SHcp, false);
    gpio_put(DS, false);
}

void Matrix::shift_out(uint8_t value, bool msb_first)
{
    for (int i = 0; i < 8; ++i)
    {
        int bit_index = msb_first ? (7 - i) : i;
        bool bit_value = ((value >> bit_index) & 0x01u) != 0;

        gpio_put(DS, bit_value);
        gpio_put(SHcp, true);
        busy_wait_us_32(1);
        gpio_put(SHcp, false);
        busy_wait_us_32(1);
    }
}

void Matrix::render_display(uint8_t datArray[8])
{
    for (int i = 0; i < 8; i++)
    {
        // Blank the display before switching rows to reduce edge ghosting.
        hold_stcp();
        shift_out(0xFFu);
        shift_out(0x00u);
        pull_stcp();

        hold_stcp();
        shift_out(datArray[i]);
        shift_out(static_cast<uint8_t>(0x80u >> i));
        pull_stcp();
        sleep_us(100);
    }
}
