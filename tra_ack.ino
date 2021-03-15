
#include <PString.h>
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
////////////////////////////////////////////////////
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;
float i ;
char buffer2[10];
  /////////////////////
  unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 100; // send once per second
////////////////////////////////
void setup() {
  radio.begin();
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPayloadSize(12 * sizeof(byte));
  radio.setAutoAck(true);
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  //radio.setPALevel(RF24_PA_MAX);
  Serial.begin(115200);

  ///////////////////////////////////////////////////////
  LoadCell.begin();
  float calibrationValue = EEPROM.get(calVal_eepromAdress, calibrationValue);
  Serial.println(calibrationValue);
  unsigned long stabilizingtime = 1000; 
  boolean _tare = true;
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}



void loop() {
  loadcelldata();
  currentMillis = millis();
  
    if (currentMillis - prevMillis >= txIntervalMillis) {
        senddata();
    }
  
  //showdata();

}

void senddata(){
      PString(buffer2, sizeof(buffer2),i);
      Serial.println(buffer2);
      radio.stopListening();
      delay(100);
      radio.write((byte *)&buffer2, sizeof(buffer2));
  
  
  }

/*void showdata(){
  Serial.print("Load_cell output val: ");
  Serial.println(i);
  }*/
void loadcelldata(){
   static boolean newDataReady = 0;
  const int serialPrintInterval = 0; 

  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      //Serial.println(t);
      newDataReady = 0;
      t = millis();
    }
  }
}
