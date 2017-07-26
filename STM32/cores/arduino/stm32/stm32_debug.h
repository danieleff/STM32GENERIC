/*
  Copyright (c) 2017 Daniel Fekete

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef STM32_DEBUG_H
#define STM32_DEBUG_H

#include <stdio.h>
#include <stdarg.h>

#include "stm32_def.h"

#ifdef __cplusplus
extern "C" {
#endif

//Returns the name of the pin: "PA4" or "1 (PB3)"
char *stm32PinName(uint8_t pin);

// Returns the name of the pin: "PA4" or "1 (PB3)"
char *stm32PortPinName(GPIO_TypeDef *port, uint32_t pinMask);

// Internal: use PRINT_XXX instead
void print_log(const char *level, const char *format, const char *file, const int line, ...);

#ifdef __cplusplus
}
#endif


#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)


#define __LOG_LEVEL    0

#define __LOG_FATAL   __LOG_LEVEL >= 1
#define __LOG_ERROR   __LOG_LEVEL >= 2
#define __LOG_WARNING __LOG_LEVEL >= 3
#define __LOG_INFO    __LOG_LEVEL >= 4
#define __LOG_DEBUG   __LOG_LEVEL >= 5
#define __LOG_TRACE   __LOG_LEVEL >= 6


#define PRINT_LOG(level, format, ...) print_log(level, format, __FILENAME__, __LINE__, ##__VA_ARGS__)


#if __LOG_FATAL
#define PRINT_FATAL(...) PRINT_LOG("FATAL", __VA_ARGS__)
#else
#define PRINT_FATAL(...)
#endif

#if __LOG_ERROR
#define PRINT_ERROR(...) PRINT_LOG("ERROR", __VA_ARGS__)
#else
#define PRINT_ERROR(...)
#endif

#if __LOG_WARNING
#define PRINT_WARNING(...) PRINT_LOG("WARNING", __VA_ARGS__)
#else
#define PRINT_WARNING(...)
#endif

#if __LOG_INFO
#define PRINT_INFO(...) PRINT_LOG("INFO", __VA_ARGS__)
#else
#define PRINT_INFO(...)
#endif

#if __LOG_DEBUG
#define PRINT_DEBUG(...) PRINT_LOG("DEBUG", __VA_ARGS__)
#else
#define PRINT_DEBUG(...)
#endif

#if __LOG_TRACE
#define PRINT_TRACE(...) PRINT_LOG("TRACE", __VA_ARGS__)
#else
#define PRINT_TRACE(...)
#endif

#endif
