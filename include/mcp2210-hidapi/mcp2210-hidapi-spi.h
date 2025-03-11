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

#define GET_VM_SPI_TRANSFER_SETTINGS 0x41 // Datasheet 3.2.1
#define SET_VM_SPI_TRANSFER_SETTINGS 0x40 // Datasheet 3.2.2

/* SPI Transfer Settings */
typedef struct {
    uint8_t transfer_size;
    uint32_t bitrate;
    uint16_t idle_cs_val;
    uint16_t active_cs_val;
    uint16_t cs_to_data_dly;
    uint16_t last_data_byte_to_cs;
    uint16_t dly_bw_subseq_data_byte;
    uint16_t byte_to_tx_per_transfer;
    uint8_t mode;
} mcp2210_spi_transfer_settings_t;
int mcp2210_spi_get_transfer_settings(hid_device *handle, mcp2210_spi_transfer_settings_t *cfg) ;
int mcp2210_spi_set_transfer_settings(hid_device *handle, mcp2210_spi_transfer_settings_t cfg) ;

/* */


// Examples
void spi_get_examples(hid_device* handle);
void spi_set_examples(hid_device* handle);
#endif // MCP2210_HIDAPI_SPI