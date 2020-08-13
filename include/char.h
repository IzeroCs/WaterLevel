#ifndef CHAR_H
#define CHAR_H

#include <Arduino.h>

#define PROGRESS_NONE_CHAR 5
#define MACHINE_CHAR       6
#define RAIN_CHAR          7

byte machineChar[]      = {B00000,B10001,B11111,B11111,B11111,B11111,B00000,B00000};
byte rainChar[]         = {B00000,B01110,B11111,B11111,B00010,B01000,B00010,B00000};
byte progressNoneChar[] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B11111};

byte progressChar[5][8] = {
    {B00000,B10000,B10000,B00000,B10000,B10000,B00000,B11111},
    {B00000,B11000,B11000,B00000,B11000,B11000,B00000,B11111},
    {B00000,B11100,B11100,B00000,B11100,B11100,B00000,B11111},
    {B00000,B11110,B11110,B00000,B11110,B11110,B00000,B11111},
    {B00000,B11111,B11111,B00000,B11111,B11111,B00000,B11111}
};

#endif
