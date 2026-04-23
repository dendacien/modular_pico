#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

class Button 
{
private:
    uint PIN;
public:
    explicit Button(int button_pin) : PIN(button_pin) {
        
    }

};