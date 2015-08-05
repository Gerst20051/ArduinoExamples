#ifndef SAW_ANALOGUE512_INT8_H_
#define SAW_ANALOGUE512_INT8_H_

#include "Arduino.h"
#include <avr/pgmspace.h>

#define SAW_ANALOGUE512_NUM_CELLS 512
#define SAW_ANALOGUE512_SAMPLERATE 512

const char __attribute__((progmem)) SAW_ANALOGUE512_DATA []  =
        {
                23, 68, 102, 119, 120, 111, 100,
                92, 91, 95, 101, 106, 106, 102, 97, 93, 92, 94, 97, 100, 100, 97, 94, 91, 90,
                91, 93, 95, 95, 93, 91, 89, 88, 88, 90, 91, 91, 90, 87, 86, 85, 85, 86, 87, 87,
                86, 84, 82, 82, 82, 83, 83, 83, 82, 81, 79, 78, 78, 79, 80, 80, 79, 77, 76, 75,
                75, 76, 76, 76, 75, 74, 73, 72, 72, 72, 73, 72, 72, 71, 69, 69, 68, 69, 69, 69,
                68, 67, 66, 65, 65, 65, 66, 66, 65, 64, 63, 62, 62, 62, 62, 62, 62, 61, 60, 59,
                58, 59, 59, 59, 58, 57, 56, 55, 55, 55, 55, 55, 55, 54, 53, 52, 52, 52, 52, 52,
                51, 51, 50, 49, 49, 49, 49, 49, 48, 47, 47, 46, 45, 45, 45, 45, 45, 44, 43, 43,
                42, 42, 42, 42, 42, 41, 40, 39, 39, 39, 39, 39, 38, 38, 37, 36, 36, 35, 35, 35,
                35, 34, 34, 33, 32, 32, 32, 32, 32, 31, 31, 30, 29, 29, 29, 29, 29, 28, 27, 27,
                26, 26, 26, 26, 26, 25, 24, 23, 23, 23, 23, 23, 22, 22, 21, 20, 20, 20, 19, 19,
                19, 19, 18, 17, 17, 16, 16, 16, 16, 16, 15, 14, 14, 13, 13, 13, 13, 13, 12, 11,
                11, 10, 10, 10, 10, 10, 9, 8, 8, 7, 7, 7, 7, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2,
                1, 1, 1, 1, 0, 0, -1, -1, -2, -2, -2, -2, -3, -3, -4, -4, -5, -5, -5, -5, -5,
                -6, -7, -7, -8, -8, -8, -8, -8, -9, -10, -10, -11, -11, -11, -11, -11, -12, -13,
                -13, -14, -14, -14, -14, -14, -15, -15, -16, -17, -17, -17, -17, -17, -18, -18,
                -19, -19, -20, -20, -20, -20, -20, -21, -22, -22, -22, -23, -23, -23, -23, -24,
                -25, -25, -25, -25, -25, -26, -26, -27, -27, -28, -28, -28, -28, -28, -29, -30,
                -30, -31, -31, -31, -31, -31, -32, -32, -33, -33, -34, -34, -34, -34, -34, -35,
                -36, -36, -36, -37, -36, -37, -37, -38, -38, -39, -39, -39, -39, -39, -40, -40,
                -41, -42, -42, -42, -42, -42, -42, -43, -44, -44, -45, -45, -45, -45, -45, -46,
                -46, -47, -47, -47, -47, -47, -48, -48, -49, -50, -50, -50, -50, -50, -50, -51,
                -52, -52, -53, -53, -53, -52, -53, -53, -54, -55, -55, -55, -55, -55, -55, -56,
                -57, -57, -58, -58, -58, -58, -58, -58, -59, -60, -60, -61, -60, -60, -60, -61,
                -62, -63, -63, -63, -63, -63, -63, -63, -64, -65, -66, -66, -65, -65, -65, -66,
                -67, -68, -68, -68, -68, -67, -67, -68, -69, -70, -71, -71, -70, -70, -70, -70,
                -71, -73, -73, -73, -73, -72, -72, -72, -74, -75, -76, -76, -75, -74, -74, -75,
                -76, -78, -79, -79, -78, -77, -76, -76, -78, -80, -82, -82, -80, -78, -77, -78,
                -80, -84, -86, -86, -83, -79, -76, -77, -82, -89, -95, -95, -83, -59, -22,
        };

#endif	/* SAW_ANALOGUE512_INT8_H_ */
