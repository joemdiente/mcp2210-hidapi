/* 
 * MCP2210 C GPIO Source
 *
 * Author: Joemel John Diente <joemdiente@gmail.com>
 * 
 */

#include "mcp2210-hidapi-gpio.h"

 /*
  * Current Chip Settings
  */
int mcp2210_get_current_chip_settings(hid_device *handle, uint8_t *val) {
	int res = -1;
	if (val == NULL) return -1;

	PRINT_FUN();

	buf[0] = 0x00;
	buf[1] = 0x31; //Get (VM) GPIO Current Pin Value

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
int mcp2210_set_gpio_val(hid_device *handle, uint8_t val) {
	int res = -1;
		
	PRINT_FUN();

	buf[0] = 0x00;
	buf[1] = 0x30; //Set (VM) GPIO Current Pin Value
	buf[5] = val;

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
	return 0;
}

int mcp2210_get_gpio_function(hid_device *handle, uint8_t gpio, mcp2210_gp_pin_designation_t *func) {
	int res = -1;
		
	PRINT_FUN();
	
	buf[0] = 0x00;
	buf[1] = 0x20; // Get (VM) GPIO Current Chip Settings

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
	
	switch(gpio) {
		case 0:
			*func = buf[4];
			break;
		case 1:
			*func = buf[5];
			break;
		case 2:
			*func = buf[6];
			break;
		case 3:
			*func = buf[7];
			break;
		case 4:
			*func = buf[8];
			break;
		case 5:
			*func = buf[9];
			break;
		case 6:
			*func = buf[10];
			break;
		case 7:
			*func = buf[11];
			break;
		case 8:
			*func = buf[12];
			break;
		default:
			printf("[%s] %u is not a valid gpio\r\n", __FUNCTION__, gpio);
			printf("Showing default GPIO Output Instead: %X\r\n", buf[13]);
			return res = -1;
			break;
	}

	return 0;
}

int mcp2210_set_gpio_function(hid_device *handle, uint8_t gpio, mcp2210_gp_pin_designation_t func) {
	int res = -1;
		
	PRINT_FUN();
	
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
int mcp2210_set_gpio_direction(hid_device *handle, uint8_t gpio, mcp2210_gp_pin_direction_t dir) {
	int res = -1;
		
	PRINT_FUN();
		
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
/* END OF GPIO RELATED FUNCTIONS */