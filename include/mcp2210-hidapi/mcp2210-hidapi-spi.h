/* 
 * MCP2210 C SPI Header
 *
 * Author: Joemel John Diente <joemdiente@gmail.com>
 * 
 */

#ifndef MCP2210_HIDAPI_SPI
#define MCP2210_HIDAPI_SPI

#include "mcp2210-hidapi.h"
/*
* SPI related Functions
*/
int mcp2210_get_spi_transfer_settings(hid_device *handle);


#endif // MCP2210_HIDAPI_SPI