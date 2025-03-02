/*
 * MCP2210 C code using HIDAPI misc codes
 * Author: Joemel John Diente <joemdiente@gmail.com>
 */

#ifndef MCP2210_HIDAPI_MISC
#define MCP2210_HIDAPI_MISC

#include <stdint.h>

//le means little-endian
uint32_t combine_uint8_to_uint32_le(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);

//le means little-endian
uint16_t combine_uint8_to_uint16_le(uint8_t low_byte, uint8_t high_byte);

//nl means newline
void print_bits_nl(uint8_t byte);

#endif