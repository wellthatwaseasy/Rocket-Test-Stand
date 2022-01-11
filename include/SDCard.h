#include <Arduino.h>
/*
 * Connect the MicroSD card Adapter to the following pins:
 *
 *  SD Card |
 *  Adapter | ESP32
 *    CS       5
 *    SCK     18
 *    MOSI    23
 *    MISO    19
 *    GND     GND
 *    VCC     5V
 */
#ifndef MT_SDCARD_H
#define MT_SDCARD_H

#define SDEBUG 0

#include "FS.h"
#include "SD.h"
#include "SPI.h"

extern void testWriteFile(fs::FS &fs, const char *path, uint8_t *buf, int len);
extern void testReadFile(fs::FS &fs, const char *path, uint8_t *buf, int len);
extern void testIO(fs::FS &fs);
extern void initSDCard();

#endif