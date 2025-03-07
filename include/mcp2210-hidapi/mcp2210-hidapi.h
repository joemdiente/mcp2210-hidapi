/* 
* MCP2210 C code using HIDAPI Header
*
* Author: Joemel John Diente <joemdiente@gmail.com>
* 
*/
#ifndef MCP2210_HIDAPI
#define MCP2210_HIDAPI

/* HIDAPI Headers */
#include "../hidapi/hidapi.h"

/* MCP2210-HIDAPI Headers */
#include "mcp2210-hidapi-misc.h"
#include <stdio.h> // printf

/*
* GPIO related Functions
*/
typedef enum {
	GP_FUNC_GPIO = 0x0,
	GP_FUNC_CHIP_SELECTS = 0x1,
	GP_FUNC_DEDICATED_FUNCTION_PIN = 0x02
} mcp2210_gp_pin_designation_t ;

int mcp2210_set_gpio_val(hid_device *handle, uint8_t val);
int mcp2210_get_gpio_val(hid_device *handle, uint8_t *val);
int mcp2210_set_gpio_direction(hid_device *handle, uint8_t gpio, uint8_t dir);
int mcp2210_set_gpio_function(hid_device *handle, uint8_t gpio, mcp2210_gp_pin_designation_t func);

/*
* USB related Functions
*/
int mcp2210_get_usb_manufacturer(hid_device *handle);

/*
* SPI related Functions
*/
int mcp2210_get_spi_transfer_settings(hid_device *handle);

#endif /* MCP2210_HIDAPI */
