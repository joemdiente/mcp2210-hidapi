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
uint16_t combine_uint8_to_uint16_le(uint8_t byte0, uint8_t byte1);

// Inverse of combine_uint8_to_uint32_le
void split_uint32_to_uint8_le(uint32_t value, uint8_t* byte0, uint8_t* byte1, uint8_t* byte2, uint8_t* byte3);

// Inverse of combine_uint8_to_uint16_le
void split_uint16_to_uint8_le(uint16_t value, uint8_t* byte0, uint8_t* byte1);

//nl means newline
void print_bits_nl(uint8_t byte);

//Bit-wise operations
uint8_t set_bit(uint8_t number, uint8_t bit_position);
uint8_t clear_bit(uint8_t number, uint8_t bit_position);
uint8_t toggle_bit(uint8_t number, uint8_t bit_position);
uint8_t check_bit(uint8_t number, uint8_t bit_position);

#endif