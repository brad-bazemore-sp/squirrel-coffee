#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "error.h"


/* a utility error function */

void error(int fatal, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (fatal) {
        exit(1);
    }
}
