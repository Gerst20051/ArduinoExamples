#ifndef ENVELOP2048_INT8_H_
#define ENVELOP2048_INT8_H_

#include "Arduino.h"
#include <avr/pgmspace.h>

#define ENVELOP2048_NUM_CELLS 2048
#define ENVELOP2048_SAMPLERATE 2048

const char __attribute__((progmem)) ENVELOP2048_DATA []  =
        {
                0, 1, 5, 8, 13, 22, 36, 53, 69, 82, 91,
                97, 103, 107, 113, 119, 128, 139, 151, 162, 171, 177, 181, 184, 186, 189, 192,
                195, 199, 203, 208, 212, 217, 221, 225, 228, 232, 236, 240, 242, 241, 238, 232,
                227, 223, 221, 221, 221, 221, 220, 219, 217, 216, 215, 214, 213, 212, 210, 209,
                206, 204, 203, 201, 200, 200, 199, 199, 199, 199, 198, 198, 198, 197, 197, 196,
                195, 194, 194, 194, 194, 194, 195, 194, 194, 194, 194, 193, 193, 193, 193, 192,
                192, 191, 191, 190, 188, 187, 185, 183, 181, 180, 178, 176, 174, 172, 171, 170,
                169, 169, 169, 169, 169, 169, 168, 167, 166, 165, 163, 161, 160, 159, 159, 159,
                159, 159, 159, 158, 158, 158, 158, 158, 158, 158, 157, 157, 156, 156, 155, 154,
                153, 153, 152, 152, 152, 151, 148, 145, 143, 142, 141, 140, 140, 140, 141, 142,
                144, 144, 145, 145, 144, 144, 144, 144, 145, 145, 145, 145, 145, 145, 145, 145,
                145, 145, 144, 144, 144, 144, 143, 143, 142, 142, 140, 139, 138, 137, 136, 136,
                136, 136, 136, 136, 135, 135, 135, 134, 134, 134, 134, 133, 133, 133, 133, 132,
                132, 132, 132, 132, 132, 132, 131, 131, 131, 131, 131, 131, 131, 130, 130, 130,
                130, 129, 128, 127, 126, 125, 124, 124, 124, 124, 124, 124, 124, 123, 123, 122,
                122, 121, 121, 121, 121, 121, 120, 120, 120, 120, 120, 121, 121, 121, 121, 120,
                120, 120, 120, 119, 119, 119, 118, 118, 117, 116, 115, 113, 112, 112, 111, 111,
                111, 111, 111, 111, 112, 112, 112, 112, 112, 111, 111, 111, 111, 110, 109, 108,
                108, 107, 106, 106, 105, 105, 104, 104, 103, 103, 103, 103, 103, 103, 104, 105,
                105, 105, 105, 105, 104, 104, 104, 103, 102, 101, 99, 98, 97, 97, 97, 97, 96,
                96, 96, 96, 96, 97, 97, 97, 96, 96, 96, 95, 95, 95, 95, 94, 94, 94, 94, 94, 93,
                93, 93, 93, 93, 92, 92, 92, 92, 92, 91, 91, 91, 91, 90, 90, 90, 90, 90, 89, 89,
                89, 89, 89, 88, 88, 88, 88, 88, 87, 87, 87, 87, 87, 86, 86, 86, 86, 86, 85, 85,
                85, 85, 85, 84, 84, 84, 84, 84, 83, 83, 83, 83, 83, 82, 82, 82, 82, 82, 81, 81,
                81, 81, 81, 81, 80, 80, 80, 80, 80, 79, 79, 79, 79, 79, 78, 78, 78, 78, 78, 78,
                77, 77, 77, 77, 77, 76, 76, 76, 76, 76, 76, 75, 75, 75, 75, 75, 74, 74, 74, 74,
                74, 74, 73, 73, 73, 73, 73, 73, 72, 72, 72, 72, 72, 72, 71, 71, 71, 71, 71, 71,
                70, 70, 70, 70, 70, 70, 69, 69, 69, 69, 69, 69, 69, 68, 68, 68, 68, 68, 68, 67,
                67, 67, 67, 67, 67, 66, 66, 66, 66, 66, 66, 65, 65, 65, 65, 65, 65, 65, 64, 64,
                64, 64, 64, 64, 63, 63, 63, 63, 63, 63, 62, 62, 62, 62, 62, 62, 61, 61, 61, 61,
                61, 61, 60, 60, 60, 60, 60, 60, 60, 59, 59, 59, 59, 59, 59, 59, 58, 58, 58, 58,
                58, 58, 58, 57, 57, 57, 57, 57, 57, 56, 56, 56, 56, 56, 56, 55, 55, 55, 55, 55,
                55, 55, 54, 54, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 52, 52, 52, 52, 52, 52,
                51, 51, 51, 51, 51, 51, 50, 50, 50, 50, 50, 50, 50, 49, 49, 49, 49, 49, 49, 49,
                48, 48, 48, 48, 48, 48, 48, 47, 47, 47, 47, 47, 47, 47, 47, 47, 46, 46, 46, 46,
                46, 46, 46, 46, 46, 46, 46, 46, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 44,
                44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 43, 43, 43, 43, 43, 43, 43, 43, 43,
                43, 43, 43, 43, 43, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 41, 41, 41, 41,
                41, 41, 41, 41, 41, 41, 41, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 39, 39,
                39, 39, 39, 39, 39, 39, 39, 39, 39, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
                38, 38, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 36, 36, 36, 36, 36,
                36, 36, 36, 36, 36, 36, 36, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
                35, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 33, 33, 32, 33, 33, 34,
                33, 33, 32, 33, 33, 34, 33, 33, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
                32, 32, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 29, 29,
                29, 29, 29, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
                28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
                28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
                29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
                29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
                29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
                31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
                31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 29, 29, 29,
                29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
                29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
                28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27,
                27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26,
                26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 25, 25,
                25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 24, 24,
                24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 23,
                23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 22, 22, 22,
                22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 21, 21, 21, 21, 21,
                21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 20, 20, 20, 20, 20, 20, 20,
                20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
                19, 19, 19, 19, 19, 19, 19, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
                18, 18, 18, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 15, 15,
                15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 14, 14, 14, 14, 14, 14,
                14, 14, 14, 14, 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
                13, 13, 13, 13, 13, 13, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
                12, 12, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 10,
                10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9,
                9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6,
                6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3,
                3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

#endif /* ENVELOP2048_INT8_H_ */