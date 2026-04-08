/*
 * MCP2210 C code using HIDAPI misc codes
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <https://www.gnu.org/licenses/>.
 * 
 * Author: Joemel John Diente <JoemelJohn.Diente@microchip.com>
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