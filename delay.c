#include <proc/pic18f4520.h>
#include "config.h"
#include "delay.h"

void timed(unsigned char ms) {
    volatile unsigned char i, j, k, w;

    for (w = 0; w < ms; w++) {
        for (i = 0; i < 100; i++) {
            for (j = 0; j < 41; j++) {
                for (k = 0; k < 3; k++);
            }
        }
    }
}
