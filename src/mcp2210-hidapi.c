/* 
 * MCP2210 C Main Source
 *
 * Author: Joemel John Diente <joemdiente@gmail.com>
 * 
 */

#include "mcp2210-hidapi.h"

#include <stdint.h>
#include <stdio.h>

/*
 * USB related Functions
 */
int mcp2210_get_usb_manufacturer(hid_device *handle) {
	int i;
	int res = -1;
		
	PRINT_FUN();
	
	// 3.1.9 GET USB MANUFACTURER NAME (Page 30)
	
	// COMMAND Structure
	cmd_buf[0] = 0x0;
	cmd_buf[1] = 0x61; //Get NVRAM Settings
	cmd_buf[2] = 0x50; //Get USB Manufacturer Name
	res = hid_write(handle, cmd_buf, 65);

	// Read RESPONSE Structure 
	res = hid_read(handle, rsp_buf, 65);
	// Print out RESPONSE Structure.
	// PRINT_BUF_RANGE(0,64);
		
	return res;
}
/* END OF USB RELATED FUNCTIONS */