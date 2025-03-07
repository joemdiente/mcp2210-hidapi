#include "hidapi.h"

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
		int i = 0;
		uint8_t val = 0xFF;

		mcp2210_get_usb_manufacturer(handle);
		mcp2210_get_spi_transfer_settings(handle);

		/* GPIO Example
		 * Set all GP to GPIOS
		 * Set all GP to Output except GP5
		 */
		for (i = 0; i < 7; i++) {
			mcp2210_set_gpio_function(handle, i, GP_FUNC_GPIO);
			mcp2210_set_gpio_direction(handle, i, 0);				//Output
			if (i == 5) mcp2210_set_gpio_direction(handle, 5, 1);	//Input
		}

		// Set GPIOs Output Value
		mcp2210_set_gpio_val(handle, 0xA5);

		// Read Back (Expected Output: 0x84 if GP5 is High)
		// while(1) {
		for (i = 0; i < 3; i++) {
			if (mcp2210_get_gpio_val(handle, &val)) {
				printf("get gpio val failed \r\n");
			}
			
			printf("GPIOs (GP7:GP0) Val: %X \r\n", val);
		}

		// Change GP0:3 5 times with 500ms and 250ms high-low.
		for (i = 0; i < 5; i ++) {
			mcp2210_set_gpio_val(handle, 0x55);
		}

		// SPI Example
		
	} // Examples

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}
