/*
 * MCP2210 C code using HIDAPI misc codes
 * Author: Joemel John Diente <joemdiente@gmail.com>
 */
#ifndef MCP2210_HIDAPI_MISC
#define MCP2210_HIDAPI_MISC
 
#include <stdint.h>
#include <stdio.h>

#include "mcp2210-hidapi-misc.h"

//le means little-endian
uint32_t combine_uint8_to_uint32_le(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3) {
	uint32_t bitrate = 0;

	// Reassemble the bytes into a 32-bit value in little-endian order
	bitrate = (uint32_t)byte3 | ((uint32_t)byte2 << 8) | ((uint32_t)byte1 << 16) | ((uint32_t)byte0 << 24);

	return bitrate;
}

//le means little-endian
uint16_t combine_uint8_to_uint16_le(uint8_t byte0, uint8_t byte1) {
	uint16_t result = 0;

	// Combine the bytes in little-endian order
	result = (uint16_t)byte0 | ((uint16_t)byte1 << 8);

	return result;
}

// Inverse of combine_uint8_to_uint32_le
void split_uint32_to_uint8_le(uint32_t value, uint8_t* byte0, uint8_t* byte1, uint8_t* byte2, uint8_t* byte3) {
    *byte0 = (uint8_t)(value >> 24);
    *byte1 = (uint8_t)(value >> 16);
    *byte2 = (uint8_t)(value >> 8);
    *byte3 = (uint8_t)(value);
}

// Inverse of combine_uint8_to_uint16_le
void split_uint16_to_uint8_le(uint16_t value, uint8_t*byte0, uint8_t* byte1) {
    *byte0 = (uint8_t)(value);
    *byte1 = (uint8_t)(value >> 8);
}

//nl means newline
void print_bits_nl(uint8_t byte) {
	for (int i = 7; i >= 0; i--) {
		// Extract the i-th bit using bitwise AND and right shift
		int bit = (byte >> i) & 1;
		printf("%d", bit);
	}
	printf("\r\n"); // Add a newline for readability
}

// Function to set a specific bit to 1 (8-bit)
uint8_t set_bit(uint8_t number, uint8_t bit_position) {
    if (bit_position > 7) { // Check for valid bit position (0-7 for 8-bit integers)
        return number; // Or handle the error as needed
    }
    uint8_t mask = 1 << bit_position;
    return number | mask;
}

// Function to clear a specific bit to 0 (8-bit)
uint8_t clear_bit(uint8_t number, uint8_t bit_position) {
    if (bit_position > 7) {
        return number; // Or handle the error
    }
    uint8_t mask = ~(1 << bit_position);
    return number & mask;
}

// Function to toggle a bit (8-bit)
uint8_t toggle_bit(uint8_t number, uint8_t bit_position) {
    if (bit_position > 7) {
        return number;
    }
    uint8_t mask = 1 << bit_position;
    return number ^ mask;
}

// Function to check the value of a bit (8-bit)
uint8_t check_bit(uint8_t number, uint8_t bit_position) {
    if (bit_position > 7) {
        return -1; // Or handle the error
    }
    uint8_t mask = 1 << bit_position;
    return (number & mask) != 0;
}

#endif