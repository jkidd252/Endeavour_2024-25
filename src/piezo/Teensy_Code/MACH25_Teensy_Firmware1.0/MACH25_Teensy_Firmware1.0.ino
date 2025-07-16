#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include "Teensy_RawDataHandle.h"
#include "Teensy_RMSofData.h"

const int ledPin = 13;
const int chipSelect = BUILTIN_SDCARD; 
File dataFile_raw;
File dataFile_rms;
File dataFile_spare;

Adafruit_MMA8451 mma = Adafruit_MMA8451();
float x = 0, y = 0, z = 0;
bool stopLogging = false; 

DataHandler dataHandler("data_rms.txt", "windowed_data.txt", "Classes.txt");
DataRMS dataRMS( "data.txt", "data_rms.txt" );
////////////////////////////////////////
unsigned long RunTime = 60000 * 60; // 600000 = 10mins
// time needed prior to start.
////////////////////////////////////////

/*
int piezo1 = 17;
int piezo2 = 16;
int piezo3 = 39;
int piezo4 = 38;
*/
///////////////////////// PIEZO 2,3,4 ARE USED IN RMS & 1 IS SAVED TO SEPERATE FILE.
unsigned long piezo1 = A3;
//unsigned long piezo2 = A2;
unsigned long piezo3 = A15;
unsigned long piezo4 = A14;
//////////////////////////
unsigned long previousMillis = 0;

void setup() {
  //Serial.begin(15200);
  analogReadResolution(10);
  mma.begin();
  mma.setRange(MMA8451_RANGE_8_G);
  pinMode(ledPin, OUTPUT);

  if ( mma.getRange() != 2 ) {
  // CHECK THAT IMU IS SET AT 8G MAX
    while(1) {
      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
      delay(1000);
    }
  }

  if (!SD.begin(chipSelect)) {
  //  CHECK THAT SD CARD IS AVAILABLE 
    while (1) {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
  }
 
  dataFile_raw = SD.open("data.txt", FILE_WRITE);
  dataFile_spare = SD.open("portA3_data.txt", FILE_WRITE);
  dataFile_raw.println("# START");
  dataFile_spare.println("# START");
  digitalWrite(ledPin, HIGH);
}

float RMS_begin = 0;

void loop() {
  previousMillis = millis();

  /*
  // NOT RUN DUE TO RISK OF EARLY TRIGGER.
  delay(10);

  while( ( millis() - previousMillis < thresh_time ) && !( (RMS_begin <= 0.59) && (0.55 <= RMS_begin) ) ) {
    int i = 0;
    //Serial.println( millis() - previousMillis < thresh_time);
    //Serial.println( (RMS_begin <= 0.59) && (0.55 <= RMS_begin) );
    float RMS_event;
    float sum = 0;
    while (i < 3) {
      sensors_event_t event;
      mma.getEvent(&event);
      RMS_event = sqrt( (event.acceleration.x*event.acceleration.x + event.acceleration.y*event.acceleration.y + event.acceleration.z*event.acceleration.z)/3 ) / 9.81;
      delay(10);
      i++;
      sum += RMS_event;
    }
    RMS_begin = sum / 3;
    //Serial.println(RMS_begin);

  }
  */
  //Serial.println("Started");
 // previousMillis = millis();
  

  while ( millis() - previousMillis < RunTime ) {   // add new condition for accel threshhold and to start a timer. => potenitally with time and accel.?!?
    //dataFile = SD.open(fileName, FILE_WRITE);
    //Serial.println(dataFile_raw);
    if (dataFile_raw) {
      //Serial.println("Run");
      dataFile_raw.print(millis());
      dataFile_raw.print(" ");
      dataFile_raw.print(analogRead(A2));
      dataFile_raw.print(" ");
      dataFile_raw.print(analogRead(piezo3));
      dataFile_raw.print(" ");
      dataFile_raw.print(analogRead(piezo4));
      dataFile_raw.print(" ");
      dataFile_spare.print(analogRead(piezo1));

      sensors_event_t event;
      mma.getEvent(&event);

      dataFile_raw.print( event.acceleration.x );
      dataFile_raw.print(" ");
      dataFile_raw.print( event.acceleration.y );
      dataFile_raw.print(" ");
      dataFile_raw.print( event.acceleration.z );

      dataFile_raw.print("\n");
      dataFile_spare.print("\n");
      delay(10);
    }
  }
  dataFile_spare.close();
  dataFile_raw.close();
  
  if (!dataRMS.begin(chipSelect)) {
  // INITIALISING THE FUNCTION TO PUT RAW DATA INTO WINDOWS.
        while (1);
       // Serial.println("RMS Fail");
  }

  while (!dataRMS.isFinished()) {
        dataRMS.update();
        //Serial.println("RMS");
  }

  if (!dataHandler.begin(chipSelect)) {
  // INITIALISING THE FUNCTION TO PUT RAW DATA INTO WINDOWS.
        while (1);
        //Serial.println("ML fail");
    }

  while (1) {
   if (dataHandler.isFinished()) {
    //Serial.println("Finished");
      while (1) {
      digitalWrite(ledPin, LOW);
      delay(1000);
      digitalWrite(ledPin, HIGH);
      delay(2000);
      // looped finish state 
      }
    }
    //Serial.println("ML");
    dataHandler.update();  // Processes one line at a time
  }
}

