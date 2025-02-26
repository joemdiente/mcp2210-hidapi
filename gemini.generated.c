/*
 * These functions are generated from gemini.google.com
 * I only modified them slightly.
 */

#include <stdint.h>
#include <stdio.h>

//le means little-endian
uint32_t combine_uint8_to_uint32_le(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    uint32_t bitrate = 0;

    // Reassemble the bytes into a 32-bit value in little-endian order
    bitrate = (uint32_t)byte3 | ((uint32_t)byte2 << 8) | ((uint32_t)byte1 << 16) | ((uint32_t)byte0 << 24);

    return bitrate;
}

//le means little-endian
uint16_t combine_uint8_to_uint16_le(uint8_t low_byte, uint8_t high_byte) {
    uint16_t result = 0;

    // Combine the bytes in little-endian order
    result = (uint16_t)low_byte | ((uint16_t)high_byte << 8);

    return result;
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