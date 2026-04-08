/* 
 * MCP2210 C Debug Header
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
 * 
 */

#ifndef MCP2210_HIDAPI_DEBUG
#define MCP2210_HIDAPI_DEBUG
// Debug
// #define DEBUG_MCP2210
// #define DEBUG_MCP2210_SHOW_ADVANCED
// #define DEBUG_MCP2210_SHOW_FUNCTION

/* Debug utilities */
#ifdef DEBUG_MCP2210 // debug mcp2210
#define PRINT_BUF_RANGE(buf,y,z) {if (buf == NULL) {printf("DEBUG: invalid buffer\r\n");} \
                    else { printf("DEBUG: print buffer\r\n"); \
                        int i = 0; for (i = y; i <= z; i++) \
                        printf("buffer[%d]: 0x%X\n", i, buf[i]);} }
#ifdef DEBUG_MCP2210_SHOW_ADVANCED
#define PRINT_RES(y,z) 	{ printf("[DEBUG: %s] %s: 0x%X\r\n", __FUNCTION__, y, z); }
#define PRINT_FUN() { printf("[DEBUG: Entered %s] \r\n", __FUNCTION__); }
#else //Show function
#define PRINT_FUN() {}
#define PRINT_RES(y,z) {}
#endif //Show Function

#else // debug mcp2210
#define PRINT_BUF_RANGE(x,y,z) {}
#define PRINT_FUN() {}
#define PRINT_RES(y,z) {}
#endif // debug mcp2210

/* End of User Configuration */

#endif /* MCP2210_HIDAPI_DEBUG */