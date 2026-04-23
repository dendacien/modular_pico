#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

// Class LED
// Description: Class to set up and manipulate LEDs
// Arguments: Int - GPIO pin
// Functions:
//   change_state() - changes the state of the pin to the opposite
//   set_high() - set the pin to high
//   set_low() - set the pin to low
//   blink(int number_of_blinks, int sleep_time) - defaults to 1 and 500ms
//      must set number_of_blinks if you want to change the sleep time
//      blinks the LED the number of times given and waits the given milliseconds
class LED 
{
private:
    bool state = false;
    uint PIN;
public:
    explicit LED(int pin_num) : PIN(pin_num) {
        gpio_init(PIN);
        gpio_set_dir(PIN, GPIO_OUT);
    }
    void change_state();
    void set_high();
    void set_low();
    void blink(int number_of_blinks = 1, int sleep_time = 500);
};