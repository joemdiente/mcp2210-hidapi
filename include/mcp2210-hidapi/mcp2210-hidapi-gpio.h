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

#define GET_VM_GPIO_CURRENT_CHIP_SETTINGS 0x20
#define SET_VM_GPIO_CURRENT_CHIP_SETTINGS 0x21

typedef enum {
	GP_FUNC_GPIO = 0x0,
	GP_FUNC_CHIP_SELECTS = 0x1,
	GP_FUNC_DEDICATED_FUNCTION_PIN = 0x02
} mcp2210_gp_pin_designation_t;

typedef	union { 
	uint8_t dir;
	struct {
		uint8_t gp0dir : 1; // Bits for gpio_default_dir
		uint8_t gp1dir : 1;
		uint8_t gp2dir : 1;
		uint8_t gp3dir : 1;
		uint8_t gp4dir : 1;
		uint8_t gp5dir : 1;
		uint8_t gp6dir : 1;
		uint8_t gp7dir : 1;
	};
} mcp2210_gpio_dir_t;

// mcp2210_gpio_current_chip_setting
typedef struct {
	mcp2210_gp_pin_designation_t gp_pin_designation[9]; // Max number of GPx + 1
    union {
        uint8_t gpio_default_output; // For whole-byte access
        struct {
            uint8_t gp0 : 1; // Define each bit as a field
            uint8_t gp1 : 1;
            uint8_t gp2 : 1;
            uint8_t gp3 : 1;
            uint8_t gp4 : 1;
            uint8_t gp5 : 1;
            uint8_t gp6 : 1;
            uint8_t gp7 : 1;
        };
    };
	mcp2210_gpio_dir_t gpio_default_dir;
	union {
		uint8_t other_chip_settings;
		struct {
			uint8_t spi_bus_release_disable : 1; // bit 0; 1 means do not release; 0 means release.
			uint8_t interrupt_pin_mode : 3; 
			uint8_t remote_wakeup_enable : 1; 
			uint8_t reserved : 3;
		};
	};
	uint8_t nvram_chip_param_access_control;
} mcp2210_gpio_chip_settings_t;


int mcp2210_gpio_get_current_chip_settings(hid_device *handle, mcp2210_gpio_chip_settings_t *chip_cfg);
int mcp2210_gpio_set_current_chip_settings(hid_device *handle, mcp2210_gpio_chip_settings_t chip_cfg);

int mcp2210_gpio_get_current_gpio_dir(hid_device *handle, mcp2210_gpio_dir_t current_dir);

// Examples
void gpio_get_examples(hid_device* handle);
void gpio_set_examples(hid_device* handle);

#endif /* MCP2210_HIDAPI_GPIO */
