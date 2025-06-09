/* 
* MCP2210 C Debug Header
*
* Author: Joemel John Diente <joemdiente@gmail.com>
* 
*/

#ifndef MCP2210_HIDAPI_DEBUG
#define MCP2210_HIDAPI_DEBUG
// Debug
#define DEBUG_MCP2210 1
#define DEBUG_MCP2210_SHOW_ADVANCED
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
#define PRINT_BUF_RANGE(x,y) {}
#endif // debug mcp2210

/* End of User Configuration */

#endif /* MCP2210_HIDAPI_DEBUG */