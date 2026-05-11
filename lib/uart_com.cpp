#include "uart_com.hpp"

// UART constructor
UART::UART(std::string_view uart_id, uint baud_rate, int tx_pin, int rx_pin)
{
	if (uart_id == "usb" || uart_id == "USB")
	{
		is_usb = true;
		while(!stdio_usb_connected())
		{
			sleep_ms(10);
		}
		return;
	} 
	else if (uart_id == "uart1" || uart_id == "UART1")
	{
		uart_inst = uart1;
	} 
	else if (uart_id == "uart0" || uart_id == "UART0")
	{
		uart_inst = uart0;
	} 
	else
	{
		// Error handling uses 'panic' XD
		panic("UART ID must be usb, uart0, or uart1");
	}

	uart_init(uart_inst, baud_rate);
	
	if (tx_pin >= 0)
	{
		gpio_set_function(tx_pin, GPIO_FUNC_UART);
	}
	if (rx_pin >= 0)
	{
		gpio_set_function(rx_pin, GPIO_FUNC_UART);
	}
}

// Write text plus newline
void UART::write_line(const char *text) const
{
	if (is_usb)
	{
		printf("%s\n", text);
	} 
	else
	{
		uart_puts(uart_inst, text);
		uart_puts(uart_inst, "\n");
	}
}

// Just write the text, no newline
void UART::write(const char *text) const
{
	if (is_usb)
	{
		printf("%s", text);
	} 
	else
	{
		uart_puts(uart_inst, text);
	}
}

// Char array buffer is passed as an argument to avoid lifetime issues and avoids heap allocation
// returns the size of the final string
uint UART::read_line(char *out_buf, size_t out_buf_size)
{
	// Can't fill a null array
	if (out_buf == nullptr || out_buf_size == 0)
	{
		return 0;
	}

	uint i = 0; // Index for the char array
	int ch;
	while (i < out_buf_size) {
		if (is_usb)
		{
			ch = getchar();
		} 
		else 
		{
			while (!uart_is_readable(uart_inst))
			{
				tight_loop_contents(); // Mark intionally spinning here
			}
			ch = uart_getc(uart_inst);
		}
		if (ch == 0) continue; // Skip NULLs
		if (ch == '\n' || ch == '\r') break;
		out_buf[i] = (char)ch;
		i++;
	}
	out_buf[i] = '\0'; // Ending the array with \0 completes the char array into a string

	return i;
}