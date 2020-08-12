#include "main.h"

LiquidCrystal lcd(0x27);
TickerArduino ticker(distance, 700);
Ultrasonic ultrasonicMachine(2, 4);
Ultrasonic ultrasonicRain(7, 8);

uint8_t machineHeightTotal    = 136;
uint8_t machineHeightOverflow = 124;
uint8_t machineHeightMinimum  = 21;
uint8_t machineHeightSensor   = 12;
uint8_t machinePreProgress    = 0;
uint8_t machineNcBlink        = 0;

uint8_t rainHeightTotal    = 145;
uint8_t rainHeightOverflow = 129;
uint8_t rainHeightMinimum  = 14;
uint8_t rainHeightSensor   = 16;
uint8_t rainPreProgress    = 0;
uint8_t rainNcBlink        = 0;

void setup() {
    Wire.begin();
    Serial.begin(115200);
    Serial.println("MeasureWater running");

    delay(100);
    lcd.begin(16, 2);

    for (uint8_t i = 0; i < 5; ++i)
        lcd.createChar(i, progressChar[i]);

    lcd.createChar(MACHINE_CHAR, machineChar);
    lcd.createChar(RAIN_CHAR, rainChar);

    delay(100);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("WATER");
    lcd.setCursor(9, 1);
    lcd.print("LEVEL");
    delay(1000);

    ticker.start();
}

void loop() {
    ticker.update();
}

void distance() {
    uint8_t machineDistance = ultrasonicMachine.read();
    uint8_t rainDistance    = ultrasonicRain.read();
    uint8_t machineProgress = 0;
    uint8_t rainProgress    = 0;
    uint8_t totalProgress   = 70;

    bool machineError = false;
    bool rainError = false;

    if (machineDistance > machineHeightTotal + 10) {
        machineError = true;
        machinePreProgress = 0;
    } else if (machineDistance < machineHeightSensor - 3) {
        machineError = true;
        machinePreProgress = totalProgress;
    } else if (machineDistance >= machineHeightSensor) {
        machineDistance -= machineHeightSensor;
    } else {
        machineDistance = 0;
    }

    if (rainDistance > rainHeightTotal + 10) {
        rainError = true;
        rainPreProgress = 0;
    } else if (rainDistance < rainHeightSensor - 3) {
        rainError = true;
        rainPreProgress = totalProgress;
    } else if (rainDistance >= rainHeightSensor) {
        rainDistance -= rainHeightSensor;
    } else {
        rainDistance = 0;
    }

    if (!machineError) {
        if (machineDistance >= machineHeightOverflow)
            machineDistance = machineHeightOverflow;

        machineDistance = machineHeightOverflow - machineDistance;
        machineProgress = machineDistance * totalProgress / machineHeightOverflow;
    }

    if (!rainError) {
        if (rainDistance >= rainHeightOverflow)
            rainDistance = rainHeightOverflow;

        rainDistance = rainHeightOverflow - rainDistance;
        rainProgress = rainDistance * totalProgress / rainHeightOverflow;
    }

    if (machinePreProgress != machineProgress || rainPreProgress != rainProgress) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write(byte(MACHINE_CHAR));

        if (machineError) {
            machineNcBlink = machineNcBlink == 0 ? 1 : 0;

            if (machineNcBlink == 1) {
                lcd.setCursor(8, 0);
                lcd.print("--");
            }
        } else {
            progress(0, machineProgress, totalProgress);
        }

        lcd.setCursor(0, 1);
        lcd.write(byte(RAIN_CHAR));

        if (rainError) {
            rainNcBlink = rainNcBlink == 0 ? 1 : 0;

            if (rainNcBlink == 1) {
                lcd.setCursor(8, 1);
                lcd.print("--");
            }
        } else {
            progress(1, rainProgress, totalProgress);
        }

        machinePreProgress = machineProgress;
        rainPreProgress    = rainProgress;
    }
}

void progress(uint8_t row, uint8_t current, uint8_t total) {
    uint8_t dotSize    = 5;
    uint8_t colTotal   = ceil((double)total   / (double)dotSize);
    uint8_t colCurrent = ceil((double)current / (double)dotSize);

    lcd.home();
    lcd.setCursor(16 - colTotal, row);

    uint8_t colDot = 0;
    uint8_t idxDot = 0;

    for (uint8_t i = 1; i <= colTotal; ++i) {
        if (i > colCurrent)
            break;

        colDot = i * dotSize;

        if (colDot >= current) {
            idxDot = colDot - current;
            idxDot = dotSize - idxDot - 1;
        } else {
            idxDot = dotSize - 1;
        }

        lcd.write(byte(idxDot));
    }
}
