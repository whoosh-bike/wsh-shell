#ifndef __SHELL_H
#define __SHELL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool Shell_Init(const char* pcHostName, const char* pcLogin, const char* pcPass);
void Shell_SendChar(char ch);

#endif /* __SHELL_H */
