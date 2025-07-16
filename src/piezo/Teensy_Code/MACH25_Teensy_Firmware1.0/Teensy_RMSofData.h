#ifndef TEENSY_RMSOFDATA_H
#define TEENSY_RMSOFDATA_H

#include <Arduino.h>
#include <SD.h>

#define LINE_BUFFER 128

class DataRMS {
public:
    DataRMS(const char* inputFileName, const char* outputFileName);
    bool begin(uint8_t cs_pin);
    bool update();         // Call in loop()
    bool isFinished();

private:
    File inputFile;
    File outputFile;
    char inFileName[32];
    char outFileName[32];
    char lineBuffer[LINE_BUFFER];
    bool finished = false;

    void processLine(const char* line);
};

#endif