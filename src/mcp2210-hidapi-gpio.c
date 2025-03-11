/* 
 * MCP2210 C GPIO Source
 *
 * Author: Joemel John Diente <joemdiente@gmail.com>
 * 
 */

#include "mcp2210-hidapi-gpio.h"

 /*
  *  GPIO Current Chip Settings
  */
int mcp2210_gpio_get_current_chip_settings(hid_device *handle, mcp2210_gpio_chip_settings_t *chip_cfg) {
	int i;
	int res = -1;
		
	PRINT_FUN();
	 
	if (chip_cfg == NULL) {
		res = -EINVAL;
		PRINT_RES("Invalid pointer", res);
	}

	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = GET_VM_GPIO_CURRENT_CHIP_SETTINGS;
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

	uint8_t gp_no = 0;
	for (gp_no = 0; gp_no <= 8; gp_no ++) {
		chip_cfg->gp_pin_designation[gp_no] = rsp_buf[4 + gp_no]; 
	}
	chip_cfg->gpio_default_output = rsp_buf[13];
	chip_cfg->gpio_default_dir.dir = rsp_buf[15];
	chip_cfg->other_chip_settings = rsp_buf[17];
	chip_cfg->nvram_chip_param_access_control = rsp_buf[18];

	// PRINT_BUF_RANGE(rsp_buf, 0, 19); // Debug only
	return 0;
}

int mcp2210_gpio_set_current_chip_settings(hid_device *handle, mcp2210_gpio_chip_settings_t chip_cfg) {
	int i;
	int res = -1;
		
	PRINT_FUN();

	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = SET_VM_GPIO_CURRENT_CHIP_SETTINGS; //Get (VM) GPIO Current Pin Value

	uint8_t gp_no = 0;
	for (gp_no = 0; gp_no <= 8; gp_no ++) {
		cmd_buf[5 + gp_no] = chip_cfg.gp_pin_designation[gp_no]; 
	}
	cmd_buf[14] = chip_cfg.gpio_default_output;
	cmd_buf[16] = (unsigned char)chip_cfg.gpio_default_dir.dir;
	cmd_buf[18] = chip_cfg.other_chip_settings;

	res = hid_write(handle, cmd_buf, 65); // Command 1
	res = hid_read(handle, rsp_buf, 65); // Response 1
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

	return 0;
}

// Examples
void gpio_set_examples(hid_device* handle) {

}
void gpio_get_examples(hid_device* handle) {
	mcp2210_gpio_chip_settings_t chip_settings;

	mcp2210_gpio_get_current_chip_settings(handle, &chip_settings);

    printf("Byte 4: GP0 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[0]);
    printf("Byte 5: GP1 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[1]);
    printf("Byte 6: GP2 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[2]);
    printf("Byte 7: GP3 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[3]);
    printf("Byte 8: GP4 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[4]);
    printf("Byte 9: GP5 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[5]);
    printf("Byte 10: GP6 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[6]);
    printf("Byte 11: GP7 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[7]);
    printf("Byte 12: GP8 Pin Designation - 0x%02X \r\n", chip_settings.gp_pin_designation[8]);
    printf("Byte 13: Default GPIO Output (low byte) - ");
	print_bits_nl(chip_settings.gpio_default_output);
    printf("Byte 15: Default GPIO Direction (low byte) - ");
	print_bits_nl(chip_settings.gpio_default_dir.dir);
    printf("Byte 17: Other Chip Settings - ");
	print_bits_nl(chip_settings.other_chip_settings);
    printf("Byte 18: NVRAM Chip Parameters Access Control - ");
	print_bits_nl(chip_settings.nvram_chip_param_access_control);
}

/* END OF GPIO RELATED FUNCTIONS */