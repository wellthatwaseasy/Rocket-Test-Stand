///////////////////////////////////////////////////////////
// Rocket Test Stand alpha
//  got 
//    Scale
//  working on
//    SDCard, RTC
// loooking forward to
//    FTPServer, HTMServer, wired Display and controls
///////////////////////////////////////////////////////////
#include <Arduino.h>
#include "scale.h"
#include "SDCard.h"

double units[20];     // Units are used to average noise or spikes 
#define Avg_Count 3   // amount of averaging the equation below does the averaging and leaves 2 decimals
#define Avg_Units (floor((units[0]+units[1]+units[2])*10.0) / 30.0) //+units[3]+units[4]+units[5]+units[6]+units[7]+units[8]+units[9]

int i;
HX711 scale;
unsigned long LastTime = 0;
bool Isfileopen = false;

float calibration_factor = 440; // this calibration factor is adjusted according to my load cell
/**********************************************
* Subroutines
***********************************************/
/*
* Waste of time, to let the peripherals warm up
*/
void CPUSpeed() {
  uint32_t startMicros,TMicros;
  startMicros = micros();
  for(uint32_t i=0;i<1000000;i++){ __asm__("nop\n\t"); }
  TMicros = micros()-startMicros;
  Serial.print("\n**********************************\nIt took ");Serial.print(TMicros);
  Serial.println("uS to perform 1 Million NOP's\n**********************************\n\n");
}
/*
* What would a Rocket Launch be without a Count Down?
*/
void CountDown(int Count=5){
  do{
    delay(1000); Serial.println(Count);
  }while(Count--);
  Serial.println("\n\n\n\n");
}

void setup() {
  Serial.begin(115200);
  delay(200);
  CPUSpeed();  
  SetupScale(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);        //Start the HX711 interface
  scale.set_scale(calibration_factor);                    //Adjust to this calibration factor
  initSDCard();                                           //Setup the SD Card Adapter

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Place the weight on the scale");   i=0; //just for test
  CountDown(3);
}

void loop() {
  char StrBuf[255];
  unsigned long Duration; 
  //double newtons;
  File file;

  if(i < 100){          //just for testing, will be to when the burn is complete
    if(!Isfileopen) {
      file = OpenFile(SD, "/testfilespd.txt", "w");
      if(file) Isfileopen = true;
    }
    units[i%Avg_Count] = floor(100.0 * scale.get_units(UNITS_PER_READ)+0.05) / 100.0; // rounds to 2 decimals 
    units[i%Avg_Count] = units[i%Avg_Count] < 0 ? 0 :units[i%Avg_Count];
    Duration = millis() - LastTime;
    LastTime = millis();
    //newtons = Avg_Units * 0.0098;   // a bit north of the tropic of cancer, maybe
    sprintf(StrBuf, "%lu,%f,%lu,%f\n",LastTime,Avg_Units,Duration,Avg_Units * 0.0098); // create a line of readings
    if(Isfileopen) file.write((uint8_t *)StrBuf,strlen(StrBuf));    // write to the SD
    Serial.print(StrBuf);
    delay(11);  //just for testing
    //11ms of loop left with UNITS_PER_READ set to 1(80hz ~12.5ms), if you notice duration above 12, lower this
    i++;
  }else if(Isfileopen) {
    CloseFile(file);
    Serial.println("File Closed");
    Isfileopen = false;
  }
}
