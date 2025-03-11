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
int mcp2210_spi_get_transfer_settings(hid_device *handle, mcp2210_spi_transfer_settings_t *cfg)  {
	int i;
	int res = -1;
		
	PRINT_FUN();
	 
	if (cfg == NULL) {
		res = -EINVAL;
		PRINT_RES("Invalid pointer", res);
	}
	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = 0x41; // 3.2.1 GET (VM) SPI TRANSFER SETTINGS 
	res = hid_write(handle, cmd_buf, 65);

	res = hid_read(handle, rsp_buf, 65);
	// RESPONSE Structure 
	if (res < 0) {
		PRINT_RES("hid_read error", res);
		return res;
	}
	if (rsp_buf[1] != 0x00) {
		res = -EBUSY;
		PRINT_RES("Command Completed Unsucessfully", res);
	} 

	cfg->transfer_size = rsp_buf[2];
	cfg->bitrate = combine_uint8_to_uint32_le(rsp_buf[7], rsp_buf[6], 
								rsp_buf[5], rsp_buf[4]);
	cfg->idle_cs_val = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[9], rsp_buf[8]);
	cfg->active_cs_val = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[11], rsp_buf[10]);
	cfg->cs_to_data_dly = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[13], rsp_buf[12]) * 100;  
	cfg->last_data_byte_to_cs = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[15], rsp_buf[14]) * 100;
	cfg->dly_bw_subseq_data_byte= (uint32_t)combine_uint8_to_uint16_le(rsp_buf[17], rsp_buf[16])  * 100;
	cfg->byte_to_tx_per_transfer = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[19], rsp_buf[18]);
	cfg->mode = (uint32_t)rsp_buf[20];

	return 0;
}

int mcp2210_spi_set_transfer_settings(hid_device *handle, mcp2210_spi_transfer_settings_t cfg) {
	int i;
	int res = -1;
		
	PRINT_FUN();

	// COMMAND Structure 
	cmd_buf[0] = 0x00;
	cmd_buf[1] = SET_VM_SPI_TRANSFER_SETTINGS;
	cmd_buf[2] = 0x00; //Reserved
	cmd_buf[3] = 0x00; //Reserved
	cmd_buf[4] = 0x00; //Reserved
	split_uint32_to_uint8_le(cfg.bitrate, &cmd_buf[8], &cmd_buf[7], &cmd_buf[6], &cmd_buf[5]);
	split_uint16_to_uint8_le(cfg.idle_cs_val, &cmd_buf[10], &cmd_buf[9]);
	split_uint16_to_uint8_le(cfg.active_cs_val, &cmd_buf[12], &cmd_buf[11]);
	split_uint16_to_uint8_le((cfg.cs_to_data_dly)/100, &cmd_buf[14], &cmd_buf[13]); //Quanta of 100 us
	split_uint16_to_uint8_le((cfg.last_data_byte_to_cs)/100, &cmd_buf[16], &cmd_buf[15]); //Quanta of 100 us
	split_uint16_to_uint8_le((cfg.dly_bw_subseq_data_byte)/100, &cmd_buf[18], &cmd_buf[17]); //Quanta of 100 us
	split_uint16_to_uint8_le(cfg.byte_to_tx_per_transfer, &cmd_buf[20], &cmd_buf[19]);
	cmd_buf[21] = cfg.mode;

	res = hid_write(handle, cmd_buf, 65); // Command 1
	res = hid_read(handle, rsp_buf, 65); // Response 1
	// RESPONSE Structure 
	if (res < 0) {
		PRINT_RES("hid_read error", res);
		return res;
	}
	if (rsp_buf[1] != 0x00) {
		res = -EBUSY;
		PRINT_RES("Command Completed Unsucessfully", res);
	} 


	return 0;
}

/* SPI EXAMPLES */
void spi_set_examples(hid_device* handle) {
	mcp2210_spi_transfer_settings_t xfer_settings;

	// Get settings first!
	mcp2210_spi_get_transfer_settings(handle, &xfer_settings);
	// Then change the settings.
	xfer_settings.bitrate = 3000000;
	// Then set the settings.
	mcp2210_spi_set_transfer_settings(handle, xfer_settings);
}
void spi_get_examples(hid_device* handle)
{
	mcp2210_spi_transfer_settings_t xfer_settings;
	// Interpret
	mcp2210_spi_get_transfer_settings(handle, &xfer_settings);
	printf("Size in Bytes of SPI Transfer Structure: %d \r\n", xfer_settings.transfer_size);

	//Example: Bit rate = 3,000,000 bps = 002D C6C0 buf[4] = 0xC0; buf[7] = 0x00
	printf("Bit rate: %ld \r\n", (long) xfer_settings.bitrate);

	// Idle Chip Select Value
	printf("Idle Chip Select Value (CS7:CS0): 0b");
	print_bits_nl(xfer_settings.idle_cs_val);

	// Active Chip Select Value
	printf("Active Chip Select Value (CS7:CS0): 0b");
	print_bits_nl(xfer_settings.active_cs_val);

	// Chip Select to Data Delay
	printf("Chip Select to Data Delay: %X\r\n", xfer_settings.cs_to_data_dly);

	// Last Data Byte to CS (de-asserted) Delay
	printf("Last Data Byte to CS (de-asserted) Delay: %X\r\n", xfer_settings.last_data_byte_to_cs);

	// Delay Between Subsequent Data Bytes
	printf("Delay Between Subsequent Data Bytes: %X\r\n", xfer_settings.dly_bw_subseq_data_byte);

	// Bytes to Transfer per SPI Transaction
	printf("Bytes to Transfer per SPI Transaction: %X\r\n", xfer_settings.byte_to_tx_per_transfer);

	// SPI Mode
	printf("SPI Mode: 0x%X \r\n", xfer_settings.mode);
}
/* END OF SPI RELATED FUNCTIONS */