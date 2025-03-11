/* 
* MCP2210 C Mainc code Header
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

// Debug
#define DEBUG_MCP2210 1
// #define DEBUG_MCP2210_SHOW_FUNCTION

/* End of User Configuration */

/* Check Operating System */
#ifdef WINDOWS
#include "hidapi_winapi.h"
#include <Windows.h>
#include <errno.h>      // Use errno.h
#elif (LINUX)
//To be defined.
#endif 

/* Debug utilities */
#if (DEBUG_MCP2210 == 1) // debug mcp2210
#define PRINT_BUF_RANGE(x,y,z) 	{ if (x == NULL) {printf("invalid buffer\r\n");} else {int i = 0; for (i = y; i <= z; i++) printf("buffer[%d]: %X\n", i, x[i]);} }
#define PRINT_RES(y,z) 	{ printf("[%s] %s: %X\r\n", __FUNCTION__, y, z); }

#ifdef DEBUG_MCP2210_SHOW_FUNCTION
#define PRINT_FUN() { printf("[Entered %s] \r\n", __FUNCTION__); }
#else //Show function
#define PRINT_FUN() {}
#endif //Show Function

#else // debug mcp2210
#define PRINT_BUF_RANGE(x,y) {}
#define PRINT_RES(y,z) {}
#endif // debug mcp2210

// Main Buffer
static unsigned char buf[65];
static unsigned char cmd_buf[65];
static unsigned char rsp_buf[65];

// Functions
#include "mcp2210-hidapi-gpio.h"
#include "mcp2210-hidapi-spi.h"
#include "mcp2210-hidapi-misc.h"

/*
* USB related Functions
*/
int mcp2210_get_usb_manufacturer(hid_device *handle);

#endif /* MCP2210_HIDAPI */