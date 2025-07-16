#ifndef TEENSY_RAWDATAHANDLE_H
#define TEENSY_RAWDATAHANDLE_H

#include <Arduino.h>
#include <SD.h>

#define FEATURE_DIM 2
#define WINDOW_SIZE 3
#define STRIDE 1
#define LINE_BUFFER_SIZE 128

class DataHandler {
public:
    DataHandler(const char* inputFileName, const char* outputFileName);
    bool begin(uint8_t cs_pin);
    bool update();         // Call in loop()
    bool isFinished();

private:
    File inputFile;
    File outputFile;
    char inFileName[32];
    char outFileName[32];
    char outClassFileName[32];
    char lineBuffer[LINE_BUFFER_SIZE];
    float buffer[WINDOW_SIZE][FEATURE_DIM];
    int samplesSeen = 0;
    bool finished = false;

    void processWindow(float window[WINDOW_SIZE][FEATURE_DIM]);
    void addSample(float sample[FEATURE_DIM]);
};

#endif