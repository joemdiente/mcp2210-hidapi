/* 
* MCP2210 C GPIO Header
*
* Author: Joemel John Diente <joemdiente@gmail.com>
* 
*/
#ifndef MCP2210_HIDAPI_GPIO
#define MCP2210_HIDAPI_GPIO

#include "mcp2210-hidapi.h"

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
} mcp2210_gp_pin_designation_t;

typedef enum {
	GP_DIR_OUT = 0x00,
	GP_DIR_IN
} mcp2210_gp_pin_direction_t;

typedef struct {
	mcp2210_gp_pin_designation_t gpio_designation[7];
	uint8_t gpio_default_output;
	uint8_t gpio_default_dir;
	uint8_t other_chip_settings;
} mcp2210_chip_settings_t;

typedef struct {
	mcp2210_gp_pin_direction_t gpio_pin;
} mcp2210_gpio_dir_t;

typedef struct {
	uint8_t gpio_val;
} mcp2210_gpio_val_t;

typedef struct {
	// Command
	uint8_t data_length_tx;

	// Response 2
	uint8_t data_length_rx;
} mcp2210_spi_data_transfer_t;

int mcp2210_get_gpio_val(hid_device *handle, uint8_t *val);
int mcp2210_set_gpio_val(hid_device *handle, uint8_t val);

int mcp2210_set_gpio_direction(hid_device *handle, uint8_t gpio, mcp2210_gp_pin_direction_t dir);

int mcp2210_get_gpio_function(hid_device *handle, uint8_t gpio, mcp2210_gp_pin_designation_t *func);
int mcp2210_set_gpio_function(hid_device *handle, uint8_t gpio, mcp2210_gp_pin_designation_t func);

#endif /* MCP2210_HIDAPI_GPIO */
