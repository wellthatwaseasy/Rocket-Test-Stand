/*
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 6 -> HX711 CLK
 Arduino pin 5 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 
*/
#ifndef SCALE_HX711
#define SCALE_HX711

#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 16
#define LOADCELL_SCK_PIN 17
#define UNITS_PER_READ 3

#define LED_BUILTIN 2

extern HX711 scale;

extern float calibration_factor ;

extern void SetupScale(const int Dout,const int Sck);

#endif