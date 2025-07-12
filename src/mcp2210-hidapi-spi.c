/* 
 * MCP2210 C SPI Source
 *
 * Author: Joemel John Diente <joemdiente@gmail.com>
 * 
 */

#include "mcp2210-hidapi-spi.h"

/* 
 * Keep track of transfer size 
 */
uint32_t spi_transfer_size = 0;
/*
 * SPI related Functions
 */
int mcp2210_spi_get_transfer_settings(hid_device *handle, mcp2210_spi_transfer_settings_t *cfg)  {
	int i;
	int res = -1;
		
	PRINT_FUN();
	 
	if (cfg == NULL || handle == NULL) {
		res = -EINVAL;
		PRINT_RES("Invalid pointer", res);
	}
	// COMMAND Structure 
	memset(cmd_buf, 0, 65);
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
		return res;
	} 

	cfg->transfer_size = rsp_buf[2];
	cfg->bitrate = combine_uint8_to_uint32_le(rsp_buf[7], rsp_buf[6], 
								rsp_buf[5], rsp_buf[4]);
	cfg->idle_cs_val = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[9], rsp_buf[8]);
	cfg->active_cs_val = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[11], rsp_buf[10]);
	cfg->cs_to_data_dly = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[13], rsp_buf[12]) * 100;  
	cfg->last_data_byte_to_cs = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[15], rsp_buf[14]) * 100;
	cfg->dly_bw_subseq_data_byte= (uint32_t)combine_uint8_to_uint16_le(rsp_buf[17], rsp_buf[16])  * 100;
	cfg->byte_to_tx_per_transact = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[19], rsp_buf[18]);
	spi_transfer_size = (uint32_t)combine_uint8_to_uint16_le(rsp_buf[19], rsp_buf[18]); //Global
	cfg->mode = (uint32_t)rsp_buf[20];

	return 0;
}

int mcp2210_spi_set_transfer_settings(hid_device *handle, mcp2210_spi_transfer_settings_t cfg) {
	int i;
	int res = -1;
		
	PRINT_FUN();

	// COMMAND Structure 
	memset(cmd_buf, 0, 65);
	cmd_buf[0] = 0x00;
	cmd_buf[1] = SET_VM_SPI_TRANSFER_SETTINGS;

	split_uint32_to_uint8_le(cfg.bitrate, &cmd_buf[5], &cmd_buf[6], &cmd_buf[7], &cmd_buf[8]);
	split_uint16_to_uint8_le(cfg.idle_cs_val, &cmd_buf[9], &cmd_buf[10]);
	split_uint16_to_uint8_le(cfg.active_cs_val, &cmd_buf[11], &cmd_buf[12]);
	split_uint16_to_uint8_le((cfg.cs_to_data_dly)/100, &cmd_buf[13], &cmd_buf[14]); //Quanta of 100 us
	split_uint16_to_uint8_le((cfg.last_data_byte_to_cs)/100, &cmd_buf[15], &cmd_buf[16]); //Quanta of 100 us
	split_uint16_to_uint8_le((cfg.dly_bw_subseq_data_byte)/100, &cmd_buf[17], &cmd_buf[18]); //Quanta of 100 us
	split_uint16_to_uint8_le(cfg.byte_to_tx_per_transact, &cmd_buf[19], &cmd_buf[20]);
	cmd_buf[21] = cfg.mode;

	res = hid_write(handle, cmd_buf, 65); // Command 1
	res = hid_read(handle, rsp_buf, 65); // Response 1
	// RESPONSE Structure 
	if (res < 0) {
		PRINT_RES("hid_read error", res);
		return res;
	}

	switch(rsp_buf[1]) {
		case 0xF8: 
			res = -EBUSY;
			PRINT_RES("RSP 2: USB Transfer in Progress", res);
			return res;
			break;
		case 0xFB:
			res = -EPERM;
			PRINT_RES("Blocked Access", res); // Note: Everything in this code is RAM (VM) only, this should not happen!
			return res;
			break;
		case 0x00:
			res = 0;
			PRINT_RES("RSP 1: Command Completed Sucessfully", res);
			return res;
			break;

		default:
			break;
	} 

	return 0;
}

int mcp2210_spi_transfer_data(hid_device *handle, uint8_t* tx_data, size_t tx_size, uint8_t* rx_data) {
	int i;
	int res = -1;
	int rx_size = 0; //Note: this is RX size received from the device. Should be always less than spi_transfer_size
	PRINT_FUN();
	 
	if (tx_data == NULL || rx_data == NULL || handle == NULL) {
		res = -EINVAL;
		PRINT_RES("Invalid pointer", res);
	}

	// COMMAND Structure 
	memset(cmd_buf, 0, 65);
	cmd_buf[0] = 0x00;
	cmd_buf[1] = SPI_TRANSFER_DATA;
	if (tx_size > 60) {
		res = -ERANGE;
		PRINT_RES("TX Data must not exceed > 60 bytes", res);
		return res;
	}
	else if (tx_size > spi_transfer_size) {
		res = -ERANGE;
		PRINT_RES("Data is more than current transfer size", res);
		return res;
	}
	cmd_buf[2] = tx_size;
	memcpy(&cmd_buf[5], tx_data, tx_size);
	res = hid_write(handle, cmd_buf, 65);
	res = hid_read(handle, rsp_buf, 65);
	// RESPONSE Structure 
	if (res < 0) {
		PRINT_RES("hid_read error", res);
		return res;
	}
	
	// Transfer SPI Data Logic Flow (Page 55)
	switch (rsp_buf[1]) {
		case SPI_DATA_ACCEPTED:
			res = 0;
			//Continue to next code.
			break;
		case SPI_BUS_BUSY:
			res = rsp_buf[1];
			return res;
			break;
		case SPI_XFER_IN_PROG:
			res = rsp_buf[1];
			return res;
			break;
		default:
			break;
	} 
	
	// SPI_DATA_ACCEPTED -> Transfer Progress (Tx buffer and Rx buffer)
	switch (rsp_buf[3]) { 
		case SPI_XFER_STARTED_RX_NDATA:
			res = rsp_buf[3];
			return res;
			break;
		case SPI_XFER_NDONE_RX_AVAIL:
			res = rsp_buf[3];
			return res;
			break;
		case SPI_XFER_DONE_TX_NONE:
			//Transfer is done; Implicitly no more RX byte
			rx_size = (int) rsp_buf[2];
			if (rx_size != 0)	{	//If no RX, then skip.
				memcpy(rx_data, &rsp_buf[4], rx_size);
			}
			res = rsp_buf[3];
			return res;
			break;
		default:
			res = 0;
			break;
	}

	return res;
}
int mcp2210_spi_cancel_transfer(hid_device *handle, mcp2210_status_t *status) {
	int i;
	int res = -1;
	
	PRINT_FUN();

	// COMMAND Structure 
	memset(cmd_buf, 0, 65);
	cmd_buf[0] = 0x00;
	cmd_buf[1] = SPI_CANCEL_TRANSFER;
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
		return res;
	} 

 	// Note: Cancel the Current SPI Transfer response is similar to get status response.
	status->spi_bus_release_external_request_status = rsp_buf[2];
	status->spi_owner = rsp_buf[3];
	status->attempt_pw_count = rsp_buf[4];
	status->pw_guessed = rsp_buf[5];

	return 0;
}
int mcp2210_spi_request_bus_release(hid_device *handle) {
	int i;
	int res = -1;
	
	PRINT_FUN();
	// Note: only if SPI BUS request pin (GP8 is used)
	// COMMAND Structure 
	memset(cmd_buf, 0, 65);
	cmd_buf[0] = 0x00;
	cmd_buf[1] = SPI_REQUEST_BUS_RELEASE;
	res = hid_write(handle, cmd_buf, 65);
	res = hid_read(handle, rsp_buf, 65);
	// RESPONSE Structure 
	if (res < 0) {
		PRINT_RES("hid_read error", res);
		return res;
	}
	if (rsp_buf[1] != 0x00 || rsp_buf[1] == 0xF8) {
		res = -EBUSY;
		PRINT_RES("SPI Bus Not Released - SPI transfer in process", rsp_buf[1]);
		return res;
	} 
	// Above check can also capture this (using else) but just for alignment add here.
	if (rsp_buf[1] == 0x00) {
		res = 0;
		PRINT_RES("Command Completed Successfully – SPI bus released", res);
		return res;
	}
	
	return 0;
}
/*
 * Examples
 * These are also used for unit testing.
 * 
 */
void spi_set_examples(hid_device* handle) {
	
	printf("\r\n%s\r\n", __FUNCTION__);

	printf("SPI Current Transfer Settings\r\n");
	mcp2210_spi_transfer_settings_t xfer_settings;

	// Get settings first!
	mcp2210_spi_get_transfer_settings(handle, &xfer_settings);
	// Then change the settings.
	xfer_settings.bitrate = 3000000;
	// Then set the settings.
	mcp2210_spi_set_transfer_settings(handle, xfer_settings);


}
void spi_get_examples(hid_device* handle) {

	printf("\r\n%s\r\n", __FUNCTION__);

	printf("SPI Current Transfer Settings\r\n");
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
	printf("Bytes to Transfer per SPI Transaction: %X\r\n", xfer_settings.byte_to_tx_per_transact);

	// SPI Mode
	printf("SPI Mode: 0x%X \r\n", xfer_settings.mode);

}
void spi_transfer_example(hid_device *handle) {
	
	printf("\r\n%s\r\n", __FUNCTION__);

	/* 
	 * Setup Transfer 
	 *
	 */ 

	printf("SPI Transfer SPI Data\r\n");

	mcp2210_status_t stat;
	mcp2210_get_status(handle, &stat);
	printf("SPI Owner: 0x%X \r\n", stat.spi_owner);
	printf("SPI bus rel ext req stat: 0x%X \r\n", stat.spi_bus_release_external_request_status);

	printf("SPI TXFER Settings\r\n");
	mcp2210_spi_transfer_settings_t spi_cfg;
	mcp2210_spi_get_transfer_settings(handle, &spi_cfg);
	spi_cfg.bitrate = 3000000; // 3 MHz
	spi_cfg.active_cs_val = clear_bit(spi_cfg.active_cs_val, GP0); // Active Low
	spi_cfg.idle_cs_val = set_bit(spi_cfg.active_cs_val, GP0); // Idle High
	spi_cfg.cs_to_data_dly = 1; // assert - data out 100us
	spi_cfg.last_data_byte_to_cs = 1; // de-assert - last data 100us
	spi_cfg.dly_bw_subseq_data_byte = 1; // 100 us
	spi_cfg.byte_to_tx_per_transact = 56; // 56 bytes per transfer - Malibu PHY Max Bytes
	spi_cfg.mode = SPI_MODE_0;
	mcp2210_spi_set_transfer_settings(handle, spi_cfg);

	printf("Set GP0 to CS0 Settings\r\n");
	mcp2210_gpio_chip_settings_t gp_cfg;
	mcp2210_gpio_get_current_chip_settings(handle, &gp_cfg);
	gp_cfg.gp_pin_designation[0] = GP_FUNC_CHIP_SELECTS; // CS0
	gp_cfg.gp_pin_designation[1] = GP_FUNC_GPIO;
	gp_cfg.gp_pin_designation[2] = GP_FUNC_GPIO;
	gp_cfg.gp_pin_designation[3] = GP_FUNC_GPIO;
	gp_cfg.gp_pin_designation[4] = GP_FUNC_GPIO; 
	gp_cfg.gp_pin_designation[5] = GP_FUNC_GPIO; 
	gp_cfg.gp_pin_designation[6] = GP_FUNC_GPIO;
	gp_cfg.gp_pin_designation[7] = GP_FUNC_GPIO; 
	gp_cfg.gp_default_dir.dir = 0x00; 
	gp_cfg.gp_default_val.val = 0x00;
	gp_cfg.spi_bus_release_disable = 0; // Released in each transfer
	mcp2210_gpio_set_current_chip_settings(handle, gp_cfg);

	//Prepare Data
#define TX_SZ 32
	uint8_t tx_data[TX_SZ];
	uint8_t rx_data[60];
	uint8_t rx_size = 0;
	uint8_t i;
	// Fill Up Data 
	for (i = 0; i < TX_SZ; i++) {
		tx_data[i] = i;
	}
	memset(rx_data, 0, 60);

	uint8_t spi_stat = 0x0;

	printf("Print Transmit Data\r\n", strlen(tx_data));
	PRINT_BUF_RANGE(tx_data, 0, TX_SZ);

	while (spi_stat != 0x10) {
		// printf("txfer data \r\n");
		spi_stat = mcp2210_spi_transfer_data(handle, &tx_data[0], TX_SZ, &rx_data[0]);
		if (spi_stat == 0x20) {
			printf(" USB In Progress wait for Data TX\r\n");
			Sleep(1000); // 'S'leep is windows thing in ms.
		}
	};
	printf("Print Received Data\r\n", sizeof(rx_data));
	PRINT_BUF_RANGE(rx_data, 0, 60);

	do {
		mcp2210_get_status(handle, &stat);
		printf("SPI Owner: 0x%X \r\n", stat.spi_owner);
	}
	while (stat.spi_owner == 0x01);
	printf("SPI bus rel ext req stat: 0x%X \r\n", stat.spi_bus_release_external_request_status);

	// mcp2210_spi_cancel_transfer(handle, &stat);
	// mcp2210_get_status(handle, &stat);
	// printf("SPI Owner: 0x%X \r\n", stat.spi_owner);
	// printf("SPI bus rel ext req stat: 0x%X \r\n", stat.spi_bus_release_external_request_status);
	int res = 0;
	printf("End , %d\r\n", res);
}
/* END OF SPI RELATED FUNCTIONS */