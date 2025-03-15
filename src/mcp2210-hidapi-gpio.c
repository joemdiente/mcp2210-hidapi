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
	chip_cfg->gp_default_val.val = rsp_buf[13];
	chip_cfg->gp_default_dir.dir = rsp_buf[15];
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
		if (chip_cfg.gp_pin_designation[gp_no] > 0x02 
			|| chip_cfg.gp_pin_designation[8] == 0x1) {
			res = -EINVAL;
			PRINT_RES("GP Pin Designation Invalid", gp_no);
			return res;
		}
		cmd_buf[5 + gp_no] = chip_cfg.gp_pin_designation[gp_no]; 
	}
	cmd_buf[14] = chip_cfg.gp_default_val.val;
	cmd_buf[16] = (unsigned char)chip_cfg.gp_default_dir.dir;
	cmd_buf[18] = chip_cfg.other_chip_settings;
	if (chip_cfg.nvram_chip_param_access_control != 0x00) {
		res = -EINVAL;
		PRINT_RES("NVRAM_CHIP_PARAM_ACCESS_CONTROL write is not supported", res);
		return res;
	}
	cmd_buf[19] = chip_cfg.nvram_chip_param_access_control = 0x00; // Read-only in user perspective.

	res = hid_write(handle, cmd_buf, 65); // Command 1
	res = hid_read(handle, rsp_buf, 65); // Response 1
	// RESPONSE Structure 
	if (res < 0) {
		PRINT_RES("hid_read error", res);
		return res;
	}
	if (rsp_buf[1] != 0x00) {
		res = -EBUSY;
		PRINT_RES("Command Completed UNSuccessfully", res);
		return res;
	} 

	return 0;
}
 /*
  *  GPIO Current Pin Direction
  */

int mcp2210_gpio_get_current_gp_dir(hid_device *handle, mcp2210_gp_dir_t *gp_dir) {
	int i;
	int res = -1;
		
	PRINT_FUN();
	 
	if (gp_dir == NULL) {
		res = -EINVAL;
		PRINT_RES("Invalid pointer", res);
	}

	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = GET_VM_GPIO_CURRENT_PIN_DIRECTION;
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

	gp_dir->dir = rsp_buf[4]; 

	return 0;
}
int mcp2210_gpio_set_current_gp_dir(hid_device *handle, mcp2210_gp_dir_t gp_dir) {
	int i;
	int res = -1;
		
	PRINT_FUN();

	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = SET_VM_GPIO_CURRENT_PIN_DIRECTION;
	cmd_buf[5] = gp_dir.dir;
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

	return 0;
}

 /*
  *  GPIO Current Pin Value
  */
  int mcp2210_gpio_get_current_gp_val(hid_device *handle, mcp2210_gp_val_t *gp_val) {
	int i;
	int res = -1;
		
	PRINT_FUN();
	 
	if (gp_val == NULL) {
		res = -EINVAL;
		PRINT_RES("Invalid pointer", res);
	}

	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = GET_VM_GPIO_CURRENT_PIN_VALUE;
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

	gp_val->val = rsp_buf[4]; 

	return 0;
}

int mcp2210_gpio_set_current_gp_val(hid_device *handle, mcp2210_gp_val_t gp_val) {
	int i;
	int res = -1;
		
	PRINT_FUN();

	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = SET_VM_GPIO_CURRENT_PIN_VALUE;
	cmd_buf[5] = gp_val.val;
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

	return 0;
}
/*
 * Examples
 * These are also used for unit testing.
 * 
 */
void gpio_get_examples(hid_device* handle) {

	printf("\r\n%s\r\n", __FUNCTION__);

	printf("GP Current Chip Settings \r\n");
	mcp2210_gpio_chip_settings_t gp_conf;
	mcp2210_gpio_get_current_chip_settings(handle, &gp_conf);

    printf("Byte 4: GP0 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[0]);
    printf("Byte 5: GP1 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[1]);
    printf("Byte 6: GP2 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[2]);
    printf("Byte 7: GP3 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[3]);
    printf("Byte 8: GP4 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[4]);
    printf("Byte 9: GP5 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[5]);
    printf("Byte 10: GP6 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[6]);
    printf("Byte 11: GP7 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[7]);
    printf("Byte 12: GP8 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[8]);
    printf("Byte 13: Default GPIO Output (low byte) - ");
	print_bits_nl(gp_conf.gp_default_val.val);
    printf("Byte 15: Default GPIO Direction (low byte) - ");
	print_bits_nl(gp_conf.gp_default_dir.dir);
    printf("Byte 17: Other Chip Settings - ");
	print_bits_nl(gp_conf.other_chip_settings);
    printf("Byte 18: NVRAM Chip Parameters Access Control - 0x%02X \r\n", gp_conf.nvram_chip_param_access_control);

	printf("GP Current GP Direction\r\n");
	mcp2210_gp_dir_t gp_dir;
	mcp2210_gpio_get_current_gp_dir(handle, &gp_dir);
    printf("Current GP Direction - ");
	print_bits_nl(gp_dir.dir);
	printf("Printing as bits: \r\n");
	printf("GP8DIR bit: Always '1");
	printf("GP7DIR bit: %u\r\n",gp_dir.gp7dir);
	printf("GP6DIR bit: %u\r\n",gp_dir.gp6dir);
	printf("GP5DIR bit: %u\r\n",gp_dir.gp5dir);
	printf("GP4DIR bit: %u\r\n",gp_dir.gp4dir);
	printf("GP3DIR bit: %u\r\n",gp_dir.gp3dir);
	printf("GP2DIR bit: %u\r\n",gp_dir.gp2dir);
	printf("GP1DIR bit: %u\r\n",gp_dir.gp1dir);
	printf("GP0DIR bit: %u\r\n",gp_dir.gp0dir);

	printf("GP Current GP Val\r\n");
	mcp2210_gp_val_t gp_val;
	mcp2210_gpio_get_current_gp_val(handle, &gp_val);
    printf("Current GP Value - ");
	print_bits_nl(gp_val.val);
	printf("Printing as bits: \r\n");
	printf("GP8 bit: Unsupported\r\n");
	printf("GP7 bit: %u\r\n",gp_val.gp7);
	printf("GP6 bit: %u\r\n",gp_val.gp6);
	printf("GP5 bit: %u\r\n",gp_val.gp5);
	printf("GP4 bit: %u\r\n",gp_val.gp4);
	printf("GP3 bit: %u\r\n",gp_val.gp3);
	printf("GP2 bit: %u\r\n",gp_val.gp2);
	printf("GP1 bit: %u\r\n",gp_val.gp1);
	printf("GP0 bit: %u\r\n",gp_val.gp0);
}
void gpio_set_examples(hid_device* handle) {

	printf("\r\n%s\r\n", __FUNCTION__);

		printf("GP Current Chip Settings \r\n");
	mcp2210_gpio_chip_settings_t gp_conf;
	mcp2210_gpio_get_current_chip_settings(handle, &gp_conf);

	gp_conf.gp_pin_designation[0] = 0x00;
	gp_conf.gp_pin_designation[1] = 0x01;
	gp_conf.gp_pin_designation[2] = 0x02;
	gp_conf.gp_pin_designation[3] = 0x02;
	gp_conf.gp_pin_designation[4] = 0x01;
	gp_conf.gp_pin_designation[5] = 0x00;
	gp_conf.gp_pin_designation[6] = 0x01;
	gp_conf.gp_pin_designation[7] = 0x02;
	gp_conf.gp_pin_designation[8] = 0x00;
	gp_conf.gp_default_val.val = 0xAA;
	gp_conf.gp_default_dir.dir = 0x55;
	gp_conf.spi_bus_release_disable = 0;
	gp_conf.interrupt_pin_mode = 0b000;
	gp_conf.remote_wakeup_enable = 0x0;
	mcp2210_gpio_set_current_chip_settings(handle, gp_conf);

	//Read Again
	mcp2210_gpio_get_current_chip_settings(handle, &gp_conf);
    printf("Byte 4: GP0 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[0]);
    printf("Byte 5: GP1 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[1]);
    printf("Byte 6: GP2 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[2]);
    printf("Byte 7: GP3 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[3]);
    printf("Byte 8: GP4 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[4]);
    printf("Byte 9: GP5 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[5]);
    printf("Byte 10: GP6 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[6]);
    printf("Byte 11: GP7 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[7]);
    printf("Byte 12: GP8 Pin Designation - 0x%02X \r\n", gp_conf.gp_pin_designation[8]);
    printf("Byte 13: Default GPIO Output (low byte) - ");
	print_bits_nl(gp_conf.gp_default_val.val);
    printf("Byte 15: Default GPIO Direction (low byte) - ");
	print_bits_nl(gp_conf.gp_default_dir.dir);
    printf("Byte 17: Other Chip Settings - ");
	print_bits_nl(gp_conf.other_chip_settings);
    printf("Byte 18: NVRAM Chip Parameters Access Control - 0x%02X \r\n", gp_conf.nvram_chip_param_access_control);

		printf("GP Current GP Direction\r\n");
	mcp2210_gp_dir_t gp_dir;
	mcp2210_gpio_get_current_gp_dir(handle, &gp_dir);

	//Toggle; Can be done since def dir was set in example above.
	gp_dir.dir ^= 0xFF;

	mcp2210_gpio_set_current_gp_dir(handle, gp_dir);
	//Read Again
    	printf("Current GP Direction - ");
	print_bits_nl(gp_dir.dir);
	printf("Printing as bits: \r\n");
	printf("GP8DIR bit: Always '1'\r\n");
	printf("GP7DIR bit: %u\r\n",gp_dir.gp7dir);
	printf("GP6DIR bit: %u\r\n",gp_dir.gp6dir);
	printf("GP5DIR bit: %u\r\n",gp_dir.gp5dir);
	printf("GP4DIR bit: %u\r\n",gp_dir.gp4dir);
	printf("GP3DIR bit: %u\r\n",gp_dir.gp3dir);
	printf("GP2DIR bit: %u\r\n",gp_dir.gp2dir);
	printf("GP1DIR bit: %u\r\n",gp_dir.gp1dir);
	printf("GP0DIR bit: %u\r\n",gp_dir.gp0dir);

	printf("GP Current GP Val\r\n");
	mcp2210_gp_val_t gp_val;
	mcp2210_gpio_get_current_gp_val(handle, &gp_val);

	gp_val.val ^= 0xFF;
	mcp2210_gpio_set_current_gp_val(handle, gp_val);
	printf("Current GP Value - ");
	print_bits_nl(gp_val.val);
	printf("Printing as bits: \r\n");
	printf("GP8 bit: Unsupported\r\n");
	printf("GP7 bit: %u\r\n",gp_val.gp7);
	printf("GP6 bit: %u\r\n",gp_val.gp6);
	printf("GP5 bit: %u\r\n",gp_val.gp5);
	printf("GP4 bit: %u\r\n",gp_val.gp4);
	printf("GP3 bit: %u\r\n",gp_val.gp3);
	printf("GP2 bit: %u\r\n",gp_val.gp2);
	printf("GP1 bit: %u\r\n",gp_val.gp1);
	printf("GP0 bit: %u\r\n",gp_val.gp0);
}
/* END OF GPIO RELATED FUNCTIONS */