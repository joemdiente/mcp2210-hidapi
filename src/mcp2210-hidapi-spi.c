/* 
 * MCP2210 C SPI Source
 *
 * Author: Joemel John Diente <joemdiente@gmail.com>
 * 
 */
#include "mcp2210-hidapi-spi.h"

/*
 * SPI related Functions
 */
int mcp2210_get_spi_transfer_settings(hid_device *handle) {
	int i;
	int res = -1;
		
	PRINT_FUN();
	
	//3.2.1 GET (VM) SPI TRANSFER SETTINGS (Page 34)
	// COMMAND Structure 
	buf[0] = 0x00;
	buf[1] = 0x41; //Get (VM) SPI Transfer Settings
	res = hid_write(handle, buf, 65);

	res = hid_read(handle, buf, 65);
	// Read RESPONSE Structure 
	if (res == -1) {
		printf("Get SPI Transfer Settings Failed\r\n");
	}

	// Interpret
	printf("Size in Bytes of SPI Transfer Structure: %d \r\n", buf[2]);

	//Example: Bit rate = 3,000,000 bps = 002D C6C0 buf[4] = 0xC0; buf[7] = 0x00
	uint32_t bitrate;
	bitrate = combine_uint8_to_uint32_le(buf[7], buf[6], buf[5], buf[4]);
	printf("Bit rate: %ld \r\n", (long) bitrate);

	// Idle Chip Select Value
	printf("Idle Chip Select Value (CS7:CS0): 0b");
	print_bits_nl(buf[8]);

	// Active Chip Select Value
	printf("Active Chip Select Value (CS7:CS0): 0b");
	print_bits_nl(buf[10]);

	// Chip Select to Data Delay
	uint16_t cs_to_dd;
	cs_to_dd = combine_uint8_to_uint16_le(buf[12],buf[13]);
	cs_to_dd *= 100;	//quanta of 100us
	printf("Chip Select to Data Delay: %X\r\n", cs_to_dd);

	// Last Data Byte to CS (de-asserted) Delay
	uint16_t ldb_to_cs;
	ldb_to_cs = combine_uint8_to_uint16_le(buf[14],buf[15]);
	ldb_to_cs *= 100;	//quanta of 100us
	printf("Last Data Byte to CS (de-asserted) Delay: %X\r\n", ldb_to_cs);

	// Delay Between Subsequent Data Bytes
	uint16_t subseq_data_delay;
	subseq_data_delay = combine_uint8_to_uint16_le(buf[16],buf[17]);
	subseq_data_delay *= 100;	//quanta of 100us
	printf("Delay Between Subsequent Data Bytes: %X\r\n", subseq_data_delay);

	// Bytes to Transfer per SPI Transaction
	uint16_t bytes_per_transfer;
	bytes_per_transfer = combine_uint8_to_uint16_le(buf[18],buf[19]);
	bytes_per_transfer *= 100;	//quanta of 100us
	printf("Bytes to Transfer per SPI Transaction: %X\r\n", bytes_per_transfer);

	// SPI Mode
	printf("SPI Mode: 0x%X \r\n", buf[20]);
	return res;
}
/* END OF SPI RELATED FUNCTIONS */