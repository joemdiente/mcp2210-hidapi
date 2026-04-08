/* 
 * MCP2210 C Main Source
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <https://www.gnu.org/licenses/>.
 * 
 * Author: Joemel John Diente <JoemelJohn.Diente@microchip.com>
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
int mcp2210_get_status(hid_device *handle, mcp2210_status_t *status) {
	int i;
	int res = -1;
	
	PRINT_FUN();

	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = GET_CHIP_STATUS;
	res = hid_write(handle, cmd_buf, 65);
	res = hid_read(handle, rsp_buf, 65);
	// RESPONSE Structure 
	if (res < 0) {
		PRINT_RES("hid_read error", res);
		return res;
	}
	if (rsp_buf[1] != 0x00) {
		res = -EBUSY;
		PRINT_RES("Command Completed Unsucessfully", res);
		return res;
	} 

 	// get status response.
	status->spi_bus_release_external_request_status = rsp_buf[2];
	status->spi_owner = rsp_buf[3];
	status->attempt_pw_count = rsp_buf[4];
	status->pw_guessed = rsp_buf[5];

	return 0;
}
/* END OF USB RELATED FUNCTIONS */