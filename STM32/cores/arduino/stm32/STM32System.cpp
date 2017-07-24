#include "unistd.h"

#include "STM32System.h"

#include "cmsis_gcc.h"

static const int print_fileno = 3;

static Print *print;

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

        Serial.write(ptr, len);

    } else if (file == STDERR_FILENO) {

        Serial.write(ptr, len);
        Serial.flush();

    } else if (file == print_fileno) {

        if (print != NULL)print->write(ptr, len);

    } else {
        // TODO show error
    }

}
