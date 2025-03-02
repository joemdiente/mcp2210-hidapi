/* 
 * MCP2210 C code using HIDAPI
 *
 * Author: Joemel John Diente <joemdiente@gmail.com>
 * 
 */

#include "hidapi.h"
#include "hidapi_winapi.h"

#include "mcp2210-hidapi.h"

// Check OS
#define WINDOWS
#ifdef WINDOWS
#include <Windows.h>
#elif (LINUX)

#endif 

#if (DEBUG_MCP2210 == 1)
#define PRINT_BUF_RANGE(x,y) 	{ if (buf == NULL) {} else {int i = 0; for (i = x; i <= y; i++) printf("buf[%d]: %X\n", i, buf[i]);} }
#define PRINT_RES(y,z) 	{ printf("[%s] %s: %X\r\n", __FUNCTION__, y, z); }
#else 
#define PRINT_BUF_RANGE(x,y) {}
#define PRINT_RES(y,z) {}
#endif

static unsigned char buf[65];

 /*
 * GPIO related Functions
 */
int mcp2210_get_gpio_val(hid_device *handle, int gpio, int *val) {
	int res = -1;
	if (val == NULL) return -1;

	buf[0] = 0x00;
	buf[1] = 0x31; //Set Current Chip Settings

	//HID API
	if (hid_write(handle, buf, 65) == -1) {
		PRINT_RES("write failed", res);
		return -1;
	}
	if (hid_read(handle, buf, 65) == -1) {
		PRINT_RES("read failed", res);
		return -1;
	}
	// PRINT_BUF_RANGE(0,64);
	if (buf[1] != 0x00){
		PRINT_RES("command unsucessful", buf[2]);
		return -1;
	}

	// Store to val from Response
	*val = buf[4];

	return 0;
}
int mcp2210_set_gpio_direction(hid_device *handle, int gpio, int dir) {
	int res = -1;
	
	buf[0] = 0x00;
	buf[1] = 0x32; // Set (VM) GPIO Current Pin Direction

	if (gpio > 7 || dir > 1) {
		printf("[%s] invalid gpio/dir %u/%u \r\n", __FUNCTION__, gpio, dir);
	}
	buf[5] = 0x00 | (dir << gpio);

	//HID API
	if (hid_write(handle, buf, 65) == -1) {
		PRINT_RES("write failed", res);
		return -1;
	}
	// PRINT_BUF_RANGE(0,64);
	if (hid_read(handle, buf, 65) == -1) {
		PRINT_RES("read failed", res);
		return -1;
	}
	// PRINT_BUF_RANGE(0,64);
	if (buf[1] != 0x00){
		PRINT_RES("command unsucessful", buf[2]);
		return -1;
	}

	return 0;
}
int mcp2210_set_gpio_function(hid_device *handle, int gpio, mcp2210_gp_pin_designation_t func) {
	int res = -1;

	buf[0] = 0x00;
	buf[1] = 0x21; //Set Current Chip Settings
	
	switch(gpio) {
		case 0:
			buf[5] = func;
			break;
		case 1:
			buf[6] = func;
			break;
		case 2:
			buf[7] = func;
			break;
		case 3:
			buf[8] = func;
			break;
		case 4:
			buf[9] = func;
			break;
		case 5:
			buf[10] = func;
			break;
		case 6:
			buf[11] = func;
			break;
		case 7:
			buf[12] = func;
			break;
		case 8:
			if (func == 0x1) {
				printf("[%s] %i function is not supported\r\n",__FUNCTION__, func);
				return res = -1;
			}
			buf[13] = func;
			break;
		default:
			printf("[%s] %u is not a valid gpio\r\n", __FUNCTION__, gpio);
			return res = -1;
			break;
	}
	// Set GPIO Default Output to Low For Now
	buf[14] = 0x00;

	//HID API
	if (hid_write(handle, buf, 65) == -1) {
		PRINT_RES("write failed", res);
		return -1;
	}
	// PRINT_BUF_RANGE(0,64);
	if (hid_read(handle, buf, 65) == -1) {
		PRINT_RES("read failed", res);
		return -1;
	}
	// PRINT_BUF_RANGE(0,64);
	if (buf[1] != 0x00){
		PRINT_RES("command unsucessful", buf[2]);
		return -1;
	}

	return 0;
}

/*
 * USB related Functions
 */
int mcp2210_get_usb_manufacturer(hid_device *handle) {
	int i;
	int res = -1;
	
	// 3.1.9 GET USB MANUFACTURER NAME (Page 30)
	
	// COMMAND Structure
	buf[0] = 0x0;
	buf[1] = 0x61; //Get NVRAM Settings
	buf[2] = 0x50; //Get USB Manufacturer Name
	res = hid_write(handle, buf, 65);

	// Read RESPONSE Structure 
	res = hid_read(handle, buf, 65);
	// Print out RESPONSE Structure.
	// PRINT_BUF_RANGE(0,64);
		
	return res;
}
/*
 * SPI related Functions
 */
int mcp2210_get_spi_transfer_settings(hid_device *handle) {
	int i;
	int res = -1;

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
	printf("SPI Mode: 0x%X \r\n", buf[20]);
	return res;
}