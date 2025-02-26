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

int mcp2210_get_spi_transfer_settings(hid_device *handle) {
	int i;
	int res = -1;
	unsigned char buf[65];

	//3.2.1 GET (VM) SPI TRANSFER SETTINGS (Page 34)
	// COMMAND Structure 
	buf[0] = 0x00;
	buf[1] = 0x41; //Get (VM) SPI Transfer Settings
	res = hid_write(handle, buf, 65);

	res = hid_read(handle, buf, 65);
	// Read RESPONSE Structure 
	if (res == -1) {
		printf("Get SPI Transfer Settings Failed\r\n");
	}

	// Interpret
	printf("Size in Bytes of SPI Transfer Structure: %d \r\n", buf[2]);

	//Example: Bit rate = 3,000,000 bps = 002D C6C0 buf[4] = 0xC0; buf[7] = 0x00
	uint32_t bitrate;
	bitrate = combine_uint8_to_uint32_le(buf[7], buf[6], buf[5], buf[4]);
	printf("Bit rate: %ld \r\n", (long) bitrate);

	// Idle Chip Select Value
	printf("Idle Chip Select Value (CS7:CS0): 0b");
	print_bits_nl(buf[8]);

	// Active Chip Select Value
	printf("Active Chip Select Value (CS7:CS0): 0b");
	print_bits_nl(buf[10]);

	// Chip Select to Data Delay
	uint16_t cs_to_dd;
	cs_to_dd = combine_uint8_to_uint16_le(buf[12],buf[13]);
	cs_to_dd *= 100;	//quanta of 100us
	printf("Chip Select to Data Delay: %X\r\n", cs_to_dd);

	// Last Data Byte to CS (de-asserted) Delay
	uint16_t ldb_to_cs;
	ldb_to_cs = combine_uint8_to_uint16_le(buf[14],buf[15]);
	ldb_to_cs *= 100;	//quanta of 100us
	printf("Last Data Byte to CS (de-asserted) Delay: %X\r\n", ldb_to_cs);

	// Delay Between Subsequent Data Bytes
	uint16_t subseq_data_delay;
	subseq_data_delay = combine_uint8_to_uint16_le(buf[16],buf[17]);
	subseq_data_delay *= 100;	//quanta of 100us
	printf("Delay Between Subsequent Data Bytes: %X\r\n", subseq_data_delay);

	// Bytes to Transfer per SPI Transaction
	uint16_t bytes_per_transfer;
	bytes_per_transfer = combine_uint8_to_uint16_le(buf[18],buf[19]);
	bytes_per_transfer *= 100;	//quanta of 100us
	printf("Bytes to Transfer per SPI Transaction: %X\r\n", bytes_per_transfer);

	// SPI Mode
	printf("SPI Mode: 0x%X", buf[20]);
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
	mcp2210_get_spi_transfer_settings(handle);

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}

