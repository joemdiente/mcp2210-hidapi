#include "include/hidapi.h"

#include <stdio.h> // printf
#include <wchar.h> // wchar_t

#define MAX_STR 255

#define DEBUG_MCP2210 1
#include "mcp2210-hidapi.h"

// Debug

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

	// Examples
	{
		printf("Running Examples \r\n");
		mcp2210_get_usb_manufacturer(handle);
		mcp2210_get_spi_transfer_settings(handle);

		//GPIO Example
		mcp2210_set_gpio_function(handle, 6, GP_FUNC_GPIO);
		mcp2210_set_gpio_direction(handle, 6, 1);

		int i = 0;
		int val = 0xFF;
		while(1) {
			if (mcp2210_get_gpio_val(handle, 6, &val)) {
				printf("get gpio val failed \r\n");
			}
			
			printf("GP6 Val: %X \r\n", val);
		}
	}

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}
