/*
* From the register info pdf:
* Use the following addresses to access a register using the SPI/I2C interface. Append the address offset
*	to the port address to get the absolute address as follows:
*	• Channel 0: 0x000000 + address offset per register
*	• Channel 1: 0x200000 + address offset per register
*	• Channel 2: 0x400000 + address offset per register
*	• Channel 3: 0x600000 + address offset per register
* 
* Try to read PCS Device Identifier 1 page 375.
    2.11.1.3 PCS Device Identifier 1
        Short Name:PCS_Device_Identifier_1
        Address:0xB0002
            Table 1012 • PCS Device Identifier 1
            Bit     Name                    Access  Description                                         Default
            15:0    PCS_Device_Identifier_1 R/O     Upper 16 bits of a 32-bit unique PCS device         0x0007
                                                    identifier. Bits 3-18 of the device manufacturer's
                                                    OUI.
    2.11.1.4 PCS Device Identifier 2
        Short Name:PCS_Device_Identifier_2
        Address:0xB0003
            Table 1013 • PCS Device Identifier 2
            Bit     Name                    Access  Description                                          Default
            15:0    PCS_Device_Identifier_2 R/O     Lower 16 bits of a 32-bit unique PCS device          0x0400
                                                    identifier. Bits 19-24 of the device manufacturer's
                                                    OUI. Six-bit model number, and a four-bit
                                                    revision number.
           
*/
#include <stdio.h> // printf
#include <wchar.h> // wchar_t

#define MAX_STR 255
#include <Windows.h>
#include <stdint.h>
#include <stdbool.h>

#include "mcp2210-hidapi-debug.h"

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

typedef struct {
    uint8_t rw : 1;
    uint8_t ch_num : 2;
    uint8_t dev_num : 5;
    uint16_t reg_num : 16;
    uint32_t data : 32;
} vsc85xx_spi_slave_inst_bit_seq_t;

rc spi_32bit_read_write(uint8_t inst, uint8_t port_no, bool rd, /* (1=rd, 0=wr) */uint8_t mmd, uint16_t addr, uint32_t *value) {

    PRINT_FUN();

    vsc85xx_spi_slave_inst_bit_seq_t bit_seq;
    bit_seq.ch_num = 1;
    bit_seq.reg_num = 0x5555;
    bit_seq.data = 0xAAAAAAAA;

    printf("%u / %X / %X ", bit_seq.ch_num, bit_seq.reg_num, bit_seq.data);
    return VTSS_RC_OK;
}


/* Datasheet description

    3.14.2 SPI Slave Interface
    The VSC8257-01 device supports the serial peripheral interface (SPI) for reading and writing registers for
    high bandwidth tasks such as reading IEEE 1588 time stamp data. The SPI interface is also capable of
    accessing all status and configuration registers. The SPI slave port consists of a clock input (SCK), data
    input (MOSI), data output (MISO), and slave select input (SSN).

    Note: The SPI slave interface is the recommended interface to access status and configuration registers for the
    rest of the device.

    Drive the SSN pin low to enable the interface. The interface is disabled when SSN is high and MISO is
    placed into a high impedance state. The VSC8257-01 device captures the state of the MOSI pin on the
    rising edge of SCK. 56 data bits are captured on the MOSI pin and transmitted on the MISO pin for each
    SPI instruction. The serial data bits consist of 1 read/write command bit, 23 address bits, and 32 register
    data bits.

    The 23-bit addressing scheme consists of a 2-bit channel number, a 5-bit MDIO device number, and a
    16-bit register number. For example, the 23-bit register address for accessing the
    GPIO_0_Config_Status register in channel 1 (device number is 0x1E and register number is 0x0100) is
    Table 42 • MDIO Port Addresses Per Channel
    Channel Number Channel’s Port Address
    3 {PADDR[4:2], 11}
    2 {PADDR[4:2], 10}
    1 {PADDR[4:2], 01}
    0 {PADDR[4:2], 00}

    Functional Descriptions
    VMDS-10486 VSC8257-01 Datasheet Revision 4.1 107
    0x3E0100. The notion of device number conforms to MDIO register groupings. For example, device 2 is
    assigned to WIS registers.
    The following table shows the order in which the bits are transferred on the interface. Bit 55 is transferred
    first, and bit 0 is transferred last. This sequence applies to both the MOSI and MISO pins.
    The register data received on the MOSI pin during a write operation is the data value to be written to a
    VSC8257-01 register. Register data received on the MOSI pin during a read operation is not used, but
    must still be delivered to the device.
    The VSC8257-01 device SPI slave has a pipelined read process. Two read instructions must be sent to
    read a single register. The first read instruction identifies the register address to be read. The MISO data
    transmitted on the second read instruction contains the register contents from the address specified in
    the first instruction. While a pipelined read implementation is not the most efficient use of bandwidth to
    read a single register, it is very efficient when performing multiple back-to-back reads as would be the
    case when reading 1588's TSFIFO_* registers. The second read instruction contains the address for the
    second register to be read plus the data read from the first register. The third read instruction contains the
    address for the third register to be read plus the data read from the second register. Register reads can
    continue in this fashion indefinitely. The following illustrations show the situations where back-to-back
    read instructions are issued.
*/