/*
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin ?? -> HX711 CLK  DEFINE IN SCALE.H
 Arduino pin ?? -> HX711 DOUT  DEFINE IN SCALE.H
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 
*/

#include "scale.h"

/**********************************************
* Subroutines
***********************************************/

void SetupScale(const int Dout,const int Sck) {
  scale.begin(Dout, Sck);
  scale.set_scale();
  scale.tare();                             //Reset the scale to 0
  long zero_factor = scale.read_average(100);  //Get a baseline reading
  
  Serial.print("\nHX711 calibrated at ");
  Serial.print(calibration_factor);
  Serial.print(" - Reading ");
  Serial.print(UNITS_PER_READ);
  Serial.print(" units per cycle ");
  Serial.print(" - Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  Serial.println("");  
}