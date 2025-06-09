/* 
* MCP2210 C Main code Header
*
* Author: Joemel John Diente <joemdiente@gmail.com>
* 
*/

#ifndef MCP2210_HIDAPI
#define MCP2210_HIDAPI

/* HIDAPI Headers */
#include "../hidapi/hidapi.h"

 /* 
 * Required User Configuration:
 * Define "WINDOWS" or "LINUX" (tbd)
 * Define DEBUG_xxx
 * 
 */
#define WINDOWS

#include "mcp2210-hidapi-debug.h"

/* Check Operating System */
#ifdef WINDOWS
#include "hidapi_winapi.h"
#include <Windows.h>
#include <errno.h>      // Use errno.h
#elif (LINUX)
//To be defined.
#endif 

/* Include Standard Libraries */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

// Main Buffer
static unsigned char cmd_buf[65];
static unsigned char rsp_buf[65];

typedef enum {
    SPI_BUS_OWNER_NO_OWNER = 0x00,
    SPI_BUS_OWNER_USB_BRIDGE = 0x01,
    SPI_BUS_OWNER_EXTERNAL_HOST = 0x02
} mcp2210_spi_bus_owner_t;

typedef struct {
    uint8_t spi_bus_release_external_request_status;
    mcp2210_spi_bus_owner_t spi_owner;
    uint8_t attempt_pw_count;
    uint8_t pw_guessed;
} mcp2210_status_t;

/* Implemented */
/*
 * USB related Functions
 * These will be put to another source code in the future
 */
int mcp2210_get_usb_manufacturer(hid_device *handle);

/* Unimplemented */
/*
 * MCP2210 Status related Functions
 */
#define GET_CHIP_STATUS 0x10
int mcp2210_get_status(hid_device *handle, mcp2210_status_t *status);

// Functions
#include "mcp2210-hidapi-spi.h"
#include "mcp2210-hidapi-gpio.h"
#include "mcp2210-hidapi-misc.h"

#endif /* MCP2210_HIDAPI */