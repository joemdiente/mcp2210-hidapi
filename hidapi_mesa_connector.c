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

#define VSC85XX_BIT_SEQ_BYTE_COUNT 7 // Malibu bit sequence is 7 bytes.
typedef uint8_t vsc85xx_spi_slave_inst_bit_seq_t[VSC85XX_BIT_SEQ_BYTE_COUNT];

#define MALIBU_RW_BIT_OFFSET 7 
#define MALIBU_PORTNO_BIT_OFFSET 5
#define MALIBU_DEVNO_BIT_SEQ_OFFSET 0
#define MALIBU_BYTE_COUNT_PER_TRANSACTION 14

rc spi_32bit_read_write(hid_device* handle, uint8_t port_no, bool rd, /* (1=rd, 0=wr) */uint8_t dev, uint16_t reg_num, uint32_t *value) {

    PRINT_FUN();

  vsc85xx_spi_slave_inst_bit_seq_t bit_seq_1;
  vsc85xx_spi_slave_inst_bit_seq_t bit_seq_2;
  uint8_t bit_seq_all[7 * 2];
  uint8_t spi_stat = 0x0; 

  memset(&bit_seq_1, 0, sizeof(bit_seq_1));
  memset(&bit_seq_2, 0, sizeof(bit_seq_2));
  memset(&bit_seq_all, 0, sizeof(bit_seq_all));

  uint32_t i;
  // Read
  if (read) {
    // First Bit Sequence to Send.
    bit_seq_1[0] = (0 << MALIBU_RW_BIT_OFFSET);
    bit_seq_1[0] |= (port_no << MALIBU_PORTNO_BIT_OFFSET);
    bit_seq_1[0] |= (dev << MALIBU_DEVNO_BIT_SEQ_OFFSET);
    bit_seq_1[1] = (reg_num & 0xFF00) >> 8;
    bit_seq_1[2] = (reg_num & 0x00FF);
    //Don't Care "data" field

    // Second Bit Sequence to Send
    // Note: Use DEV_ID address to prevent clearing "clear-on-read" registers (Page 158)
    bit_seq_2[0] = (0 << MALIBU_RW_BIT_OFFSET);
    bit_seq_2[0] |= (0x0000 << MALIBU_PORTNO_BIT_OFFSET);
    bit_seq_2[0] |= (0x1E << MALIBU_DEVNO_BIT_SEQ_OFFSET);
    bit_seq_2[1] = 0x00; // DEV_ID
    bit_seq_2[2] = 0x00; // DEV_ID
    //Don't Care "data" field
  }
  // Write
  else {
    // First Bit Sequence to Send.
    bit_seq_1[0] = (1 << MALIBU_RW_BIT_OFFSET);
    bit_seq_1[0] |= (port_no << MALIBU_PORTNO_BIT_OFFSET);
    bit_seq_1[0] |= (dev << MALIBU_DEVNO_BIT_SEQ_OFFSET);
    bit_seq_1[1] = (reg_num >> 8);
    bit_seq_1[2] = (reg_num);
    bit_seq_1[3] = (*value & 0xFF000000) >> 24;
    bit_seq_1[4] = (*value & 0xFF0000) >> 16;
    bit_seq_1[5] = (*value & 0xFF00) >> 8;
    bit_seq_1[6] = (*value & 0xFF);

    // Second Bit Sequence to Send
    // Note: Use DEV_ID address to prevent clearing "clear-on-read" registers (Page 158)
    bit_seq_2[0] = (1 << MALIBU_RW_BIT_OFFSET);
    bit_seq_2[0] |= (0x0000 << MALIBU_PORTNO_BIT_OFFSET);
    bit_seq_2[0] |= (0x1E << MALIBU_DEVNO_BIT_SEQ_OFFSET);
    bit_seq_2[1] = 0x00; // DEV_ID
    bit_seq_2[2] = 0x00; // DEV_ID
    //Don't Care "data" field
  }

  // Combine bit_seq_1 and bit_seq_2 to issue only one mcp2210_spi_transfer_data()
  memcpy(&bit_seq_all, &bit_seq_1, sizeof(bit_seq_1));
  memcpy(&bit_seq_all[7], &bit_seq_2, sizeof(bit_seq_2));

  // Transmit Bit Sequences
  while (spi_stat != 0x10) {
      //Sent value is also being updated after received (byte_ptr_tx)
    //   spi_stat = mcp2210_spi_transfer_data(handle, bit_seq_all, MALIBU_BYTE_COUNT_PER_TRANSACTION, bit_seq_all);
  };
  // Copy Byte offset 10 (Data in the 2nd Transaction) to value 
  // regardless of operation
  *value = bit_seq_all[10] << 24;
  *value |= bit_seq_all[11] << 16;
  *value |= bit_seq_all[12] << 8;
  *value |= bit_seq_all[13];

  return VTSS_RC_OK;
}