#include <stdio.h> // printf
#include <wchar.h> // wchar_t

#define MAX_STR 255
#include <Windows.h>
#include <stdint.h>
#include <stdbool.h>

#include "mcp2210-hidapi-debug.h"
#include "mcp2210-hidapi-spi.h"
/**
 * From mesa/include/vtss_init_api.h
 * 
 * \brief SPI 32 bit read/write function
 *
 * \param inst    [IN] Vitesse API instance.
 * \param port_no [IN] Port number.
 * \param read    [IN] Read/Write.
 * \param dev     [IN] MMD device number.
 * \param reg_num [IN] Register offset.
 * \param data [IN|OUT] Pointer to the data to be written to SPI Slave, if doing write operation.
 *                      Pointer to the data read from SPI Slave, if doing read operation.
 *
 * \return Return code.
 **/
typedef int rc;

#define VTSS_RC_OK 0;
#define VTSS_RC_ERROR -1;

typedef struct __attribute__((__packed__)) {
    bool rw : 1;
    uint8_t port_no : 2;
    uint8_t  dev : 5;
    uint16_t  reg_num;
    uint32_t   value;
} vsc85xx_spi_slave_inst_bit_seq_t;

typedef struct {
    uint8_t byte[8];
} vsc85xx_spi_slave_inst_buf_t;

rc spi_32bit_read_write(hid_device* handle, uint8_t inst, uint8_t port_no, bool rd, /* (1=rd, 0=wr) */uint8_t dev, uint16_t reg_num, uint32_t *value) {

    PRINT_FUN();

    uint8_t i = 0;
    vsc85xx_spi_slave_inst_bit_seq_t bit_seq_tx;
    vsc85xx_spi_slave_inst_bit_seq_t bit_seq_rx;

    memset(&bit_seq_tx, 0, sizeof(bit_seq_tx));
    memset(&bit_seq_rx, 0, sizeof(bit_seq_rx));   

    unsigned char *byte_ptr_tx = (unsigned char *)&bit_seq_tx;
    unsigned char *byte_ptr_rx = (unsigned char *)&bit_seq_rx;
    
    // Test Bit Sequence (Software test only)
    bool sw_test = true;
    if (sw_test) { 
        bit_seq_tx.rw = rd;     //0b1 = Write
        bit_seq_tx.port_no = port_no; //0b01 = Channel/Port 1
        bit_seq_tx.dev = dev; //0b11110 = Device Number 1E
        bit_seq_tx.reg_num = reg_num; // Dummy Register Value
        bit_seq_tx.value = *value; // Dummy Data Value
    }
    // Verify in Terminal (Software Test)
    for (size_t i = sizeof(bit_seq_tx); i > 0; i--) {
        printf("ptr_adr: %X, %02X \r\n", &byte_ptr_tx[i - 1], byte_ptr_tx[i - 1]);
    }
    
    // Test using MCP2210 EVB and Logic Sal
    bool hw_test = true; 
    if (hw_test) {
        // mcp2210-hidapi-spi initialization
        mcp2210_spi_transfer_settings_t spi_cfg;
        mcp2210_spi_get_transfer_settings(handle, &spi_cfg);
        spi_cfg.bitrate = 3000000;
        spi_cfg.active_cs_val = clear_bit(spi_cfg.active_cs_val, GP0); // Active Low
        spi_cfg.idle_cs_val = set_bit(spi_cfg.active_cs_val, GP0); // Idle High
        spi_cfg.cs_to_data_dly = 1; // assert - data out 100us
        spi_cfg.last_data_byte_to_cs = 1; // de-assert - last data 100us
        spi_cfg.dly_bw_subseq_data_byte = 1; // 100 us
        spi_cfg.byte_to_tx_per_transact = sizeof(bit_seq_tx);
        spi_cfg.mode = SPI_MODE_0;
        mcp2210_spi_set_transfer_settings(handle, spi_cfg);

        mcp2210_gpio_chip_settings_t gp_cfg;
        mcp2210_gpio_get_current_chip_settings(handle, &gp_cfg);
        gp_cfg.gp_pin_designation[0] = GP_FUNC_CHIP_SELECTS; // CS0
        gp_cfg.spi_bus_release_disable = 0; // Released in each transfer
        mcp2210_gpio_set_current_chip_settings(handle, gp_cfg);

        printf("Sending Test Bit Sequence: \r\n", sizeof(bit_seq_tx));
        uint8_t spi_stat = 0x0; 
        while (spi_stat != 0x10) {
            spi_stat = mcp2210_spi_transfer_data(handle, byte_ptr_tx, sizeof(bit_seq_tx), byte_ptr_rx);
            if (spi_stat == 0x20) {
                printf(" USB In Progress wait for Data TX\r\n");
                Sleep(1000); // 'S'leep is windows thing in ms.
            }
        };

        printf("Print Received Data\r\n", sizeof(byte_ptr_rx));
        for (size_t i = sizeof(bit_seq_rx); i > 0; i--) {
            printf("ptr_adr: %X, %02X \r\n", &byte_ptr_rx[i - 1], byte_ptr_rx[i - 1]);
        }
    }

    return VTSS_RC_OK;
}