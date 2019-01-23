#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

void i32_to_str(int32_t n, char *buffer);
void i32_to_volstr(int32_t n, volatile char *buffer);
void ui32_to_str(uint32_t n, char *buffer);
void print_as_ascii(char *string, char *ascii_buf);

#endif
