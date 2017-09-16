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

#include "unistd.h"

#include "STM32System.h"

#include "cmsis_gcc.h"

static const int print_fileno = 3;

static Print *print = NULL;
Stream *stdoutStream = NULL;
Stream *stderrStream = NULL;

int stm32SetPrintOutput(Print *p) {
    if (p == NULL) {
        print = NULL;
        return 0;
    }

    if (isInterrupt() && print != NULL) {
        return 0;
    }

    while(print != NULL);
    print = p;

    return print_fileno;
}

extern "C" int _write( int file, char *ptr, int len ) {

    if (file == STDOUT_FILENO) {

        if (stdoutStream) stdoutStream->write(ptr, len);

    } else if (file == STDERR_FILENO) {

        if (stderrStream) stderrStream->write(ptr, len);
        if (stderrStream) stderrStream->flush();

    } else if (file == print_fileno) {

        if (print != NULL)print->write(ptr, len);

    } else {
        // TODO show error
    }

}
