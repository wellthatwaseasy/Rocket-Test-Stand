#include <Arduino.h>
#include "scale.h"
#include "SDCard.h"

float units[20];
#define Avg_Units ((units[0]+units[1]+units[2]+units[3]+units[4]+units[5]+units[6]+units[7]+units[8]+units[9])/10)
#define Avg_Count 10
int i;
float ounces;
HX711 scale;
char StrBuf[255];

float calibration_factor = 440; // this calibration factor is adjusted according to my load cell
/**********************************************
* Subroutines
***********************************************/

void CPUSpeed() {
  uint32_t startMicros,TMicros;
  startMicros = micros();
  for(uint32_t i=0;i<1000000;i++){ __asm__("nop\n\t"); }
  TMicros = micros()-startMicros;
  Serial.print("\n**********************************\nIt took ");Serial.print(TMicros);
  Serial.println("uS to perform 1 Million NOP's\n**********************************\n\n");
  i=0;
}

void setup() {
  Serial.begin(115200);
  CPUSpeed();  
  SetupScale(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  initSDCard();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Place the weight on the scale\n5");
  delay(1000);Serial.println("4");
  delay(1000);Serial.println("3");
  delay(1000);Serial.println("2");
  delay(1000);Serial.println("1");
  delay(1000);Serial.println("\n\n\n\n");
  for(int ii=0; ii<Avg_Count;ii++) units[ii] = scale.get_units(2);  
}

void loop() {
  if(i < 500){
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGHtage level)
    units[i%Avg_Count] = scale.get_units(UNITS_PER_READ);
    ounces = Avg_Units * 0.035274;
    sprintf(StrBuf, "%lu,%f,%f,%f\n",millis(),Avg_Units,units[i%Avg_Count],ounces);
    //testWriteFile(SD,"/testfilespd.txt", (uint8_t *)StrBuf,strlen(StrBuf));
    Serial.print(StrBuf);
    //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    i++;
  }
}
