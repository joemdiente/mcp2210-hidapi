#include <stdio.h> // printf
#include <wchar.h> // wchar_t

#define MAX_STR 255

#include <stdio.h> // printf
#include <wchar.h> // wchar_t

#include "hidapi_winapi.h"
#include "gemini.generated.c"

#define MAX_STR 255

// Debug
#define PRINT_BUF_RANGE(x,y) 	{for (i = x; i <= y; i++) printf("buf[%d]: %X\n", i, buf[i]);}

int mcp2210_get_usb_manufacturer(hid_device *handle) {
	int i;
	int res = -1;
	unsigned char buf[65];
	
	// 3.1.9 GET USB MANUFACTURER NAME (Page 30)
	
	// COMMAND Structure
	buf[0] = 0x0;
	buf[1] = 0x61; //Get NVRAM Settings
	buf[2] = 0x50; //Get USB Manufacturer Name
	res = hid_write(handle, buf, 65);

	// Read RESPONSE Structure 
	res = hid_read(handle, buf, 65);
	// Print out RESPONSE Structure.
	for (i = 0; i < 64; i++)
		printf("buf[%d]: %X\n", i, buf[i]);
		
	return res;
}

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
	mcp2210_get_usb_manufacturer(handle);

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}

