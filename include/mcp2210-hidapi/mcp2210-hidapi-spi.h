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

/* SPI Transfer Settings */
typedef enum {
    SPI_MODE_0 = 0x00,
    SPI_MODE_1 = 0x01,
    SPI_MODE_2 = 0x02,
    SPI_MODE_3 = 0x03,
} mcp2210_spi_mode_t;

typedef struct {
    uint8_t transfer_size;
    uint32_t bitrate;
    uint16_t idle_cs_val;
    uint16_t active_cs_val;
    uint16_t cs_to_data_dly;
    uint16_t last_data_byte_to_cs;
    uint16_t dly_bw_subseq_data_byte;
    uint16_t byte_to_tx_per_transact;
    mcp2210_spi_mode_t mode;
} mcp2210_spi_transfer_settings_t;

enum {
    SPI_DATA_ACCEPTED = 0x00,
    SPI_BUS_BUSY = 0xF7,
    SPI_XFER_IN_PROG = 0xF8,
    SPI_XFER_STARTED_RX_NDATA = 0x20,
    SPI_XFER_NDONE_RX_AVAIL = 0x30,
    SPI_XFER_DONE_TX_NONE = 0x10,
};
/* Implemented */
#define GET_VM_SPI_TRANSFER_SETTINGS 0x41 // Datasheet 3.2.1
#define SET_VM_SPI_TRANSFER_SETTINGS 0x40 // Datasheet 3.2.2
int mcp2210_spi_get_transfer_settings(hid_device *handle, mcp2210_spi_transfer_settings_t *cfg) ;
int mcp2210_spi_set_transfer_settings(hid_device *handle, mcp2210_spi_transfer_settings_t cfg) ;

/* Not yet implemented */
#define SPI_TRANSFER_DATA 0x42
int mcp2210_spi_transfer_data(hid_device *handle, uint8_t* tx_data, size_t tx_size, uint8_t *rx_data);

#define SPI_CANCEL_TRANSFER 0x11
int mcp2210_spi_cancel_transfer(hid_device *handle, mcp2210_status_t *status);

#define SPI_REQUEST_BUS_RELEASE 0x80
int mcp2210_spi_request_bus_release(hid_device *handle);

// Examples
void spi_get_examples(hid_device* handle);
void spi_set_examples(hid_device* handle);
void spi_transfer_example(hid_device *handle);
#endif // MCP2210_HIDAPI_SPI