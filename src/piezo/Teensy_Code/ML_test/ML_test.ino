#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>

const int ledPin = 13;

const int chipSelect = BUILTIN_SDCARD; 
File dataFile;

int piezo1 = 24;
int piezo2 = 25;
int piezo3 = 26;  // not 

Adafruit_MMA8451 mma = Adafruit_MMA8451();
float x = 0, y = 0, z = 0;
bool stopLogging = false; 

unsigned long previousMillis = 0;
const long interval = 500;



//////////////////////////////////// ML SETUP /////////////////////////////////////////////
//#include <eloquent_tensorflow_cortexm.h>
#include <Arduino.h>
#include "model_CNN_test.h"
#include <tflm_cortexm.h>  // run time for Arm Cortex-M

#include <eloquent_tinyml.h>
//#include <eloquent_tinyml/tensorflow.h>


#define WINDOW_SIZE 3
#define FEATURES 3
#define TF_NUM_INPUTS (WINDOW_SIZE * FEATURES)  // 9
#define TF_NUM_OUTPUTS 8
#define TENSOR_ARENA_SIZE 100*1024 // 16-Bits (uint16_t) maxed out at 65536  (400 * 1024)

#define TF_NUM_OPS 6
// defined by the number of unique operations included in the model
// i.e in this model CONV_2D, RELU, MAX_POOL_2D, AVERAGE_POOL_2D, FULLY_CONNECTED, SOFTMAX  are used = 6 operations
#define STEP 1   // Overlap = WINDOW_SIZE - STEP

Eloquent::TF::Sequential<TF_NUM_OPS, TENSOR_ARENA_SIZE> tf;
//Eloquent::TF::Sequential<TF_NUM_OPS, ARENA_SIZE> tf;


float window_buffer[TF_NUM_INPUTS];

void setup() {
  Serial.begin(115200);
  analogReadResolution(10);
  //tf.setNumInputs(TF_NUM_INPUTS);
  //tf.setNumOutputs(TF_NUM_OUTPUTS);
  //tf.resolver.AddFullyConnected();
  
  while (!tf.begin(model_CNN_test_tflite)) {
      Serial.println("FAIL");
      delay(1000);
  }
  
  // tf.begin(model_CNN_tflite);

  
  mma.begin();
  mma.setRange(MMA8451_RANGE_8_G);
  pinMode(ledPin, OUTPUT);
  
  if (mma.getRange() != 2) { // Checking to see if the IMU is set to 8g limit. :)
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
  
  digitalWrite(ledPin, HIGH);
}

void RunModel() {
  dataFile = SD.open("data.txt");
  if (!dataFile) {
    while (true){
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    };
  };
}


void loop() {
  // put your main code here, to run repeatedly:

}

