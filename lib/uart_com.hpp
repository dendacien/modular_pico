#pragma once

#include <string_view> // light weight version of string
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/uart.h"

// Class UART
// description: Class that handles serial communication, both by USB and GPIO pins
// arguments: string - UART ID
//            uint - baud rate, defaults to 115200
//            int - tx_pin, default is -1 and will be ignored, set to -1 if only wanting to set RX
//            int - rx_pin, default is -1 and will be ignored
// functions: write_line (char *text) - string of text to be sent
//            read_line (char *out_buf, uint out_buf_size) - read a line of text into the passed string
class UART 
{
private:
    bool is_usb = false;
    uart_inst_t *uart_inst = nullptr;
public:
    explicit UART(std::string_view uart_id, uint baud_rate = 115200, int tx_pin = -1, int rx_pin = -1);
    void write_line(const char *text) const;
    uint read_line(char *out_buf, uint out_buf_size);
};