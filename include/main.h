#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Ultrasonic.h>
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include <TickerArduino.h>

#include "char.h"

void distance();
void progress(uint8_t row, uint8_t current, uint8_t total);

#endif
