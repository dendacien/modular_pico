#include "lib/uart_com.hpp"

int main()
{
    stdio_init_all();
    UART serial("usb");

    serial.write_line("Echo");
    char line[64]; // Char array for line of output

    while (true) 
    {
        serial.read_line(line, sizeof(line));
        printf("%s\n", line);
    }
}
