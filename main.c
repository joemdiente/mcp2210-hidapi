#include "hidapi.h"

#include <stdio.h> // printf
#include <wchar.h> // wchar_t

#define MAX_STR 255
#include <Windows.h>

#include "mcp2210-hidapi.h"
#include "mcp2210-hidapi-gpio.h"
#include "mcp2210-hidapi-spi.h"

// Main Test Code
int main(int argc, char* argv[]) {
	int res;
	unsigned char buf[65];
	wchar_t wstr[MAX_STR];
	hid_device *handle;

	// Initialize the hidapi library
	res = hid_init();

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x4d8, 0xDE, NULL);
	if (!handle) {
		printf("Unable to open device\n");
		hid_exit();
 		return 1;
	}
	printf("Opened MCP2210\r\n");

	// Examples
	{
		// Read the Manufacturer String
		res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
		printf("Manufacturer String: %ls\n", wstr);

		// Read the Product String
		res = hid_get_product_string(handle, wstr, MAX_STR);
		printf("Product String: %ls\n", wstr);

		// Read the Serial Number String
		res = hid_get_serial_number_string(handle, wstr, MAX_STR);
		printf("Serial Number String: (%d) %ls\n", wstr[0], wstr);

		// Read Indexed String 1
		res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
		printf("Indexed String 1: %ls\n", wstr);

		// Setup a transfer and transmit a data
		spi_transfer_example(handle);

		spi_get_examples(handle); // example get/read SPI command/response
		spi_set_examples(handle); // example set/write SPI command/response

		gpio_get_examples(handle); // example get/read GPIO command/response
		gpio_set_examples(handle); // example set/write GPIO command/response

	} 
	// End of Examples

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}
