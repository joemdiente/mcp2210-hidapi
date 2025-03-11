#include "hidapi.h"
#include "include/mcp2210-hidapi/mcp2210-hidapi.h"

#include <stdio.h> // printf
#include <wchar.h> // wchar_t

#define MAX_STR 255
#include <Windows.h>

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
		spi_get_examples(handle);
		spi_set_examples(handle);
	} // Examples

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}
