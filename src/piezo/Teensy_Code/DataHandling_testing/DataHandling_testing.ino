#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include "Teensy_RawDataHandle.h"

const int ledPin = 13;
const int chipSelect = BUILTIN_SDCARD; 
File dataFile_raw;
File dataFile_org;

Adafruit_MMA8451 mma = Adafruit_MMA8451();
float x = 0, y = 0, z = 0;
bool stopLogging = false; 

DataHandler dataHandler("data.txt", "windowed_data.txt");

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  analogReadResolution(10);

  mma.begin();
  mma.setRange(MMA8451_RANGE_8_G);

  pinMode(ledPin, OUTPUT);

  if ( mma.getRange() != 2 ) {  // likely caused a reset => DO NOT USE THIS IN FUTURE
    while(1) {
      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
      delay(1000);
    }
  }

  if (!SD.begin(chipSelect)) {
    while (1) {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
  }
  dataFile_raw = SD.open("data.txt", FILE_READ);   // uncomment in final version, make file to write data to.
  dataFile_org = SD.open("windowed_data.txt", FILE_WRITE);

  if (!dataHandler.begin(chipSelect)) {
        Serial.println("Data handler init failed!");
        while (1);
    }

  digitalWrite(ledPin, HIGH);

}

void loop() {
  // Data Collection Loops. etc.
  
  while (1) {
   if (dataHandler.isFinished()) {
      Serial.println("Finished processing file.");
      while (1);
      // looped finish state 
    }
    dataHandler.update();  // Processes one line at a time
  }
  
}

