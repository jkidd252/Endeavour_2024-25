/*
#ifndef WINDOWING_H
#define WINDOWING_H

#include <vector>

// Structure to store the window's data
struct Window {
    float** features; // dynamically allocated: [window_size][3]
};

// Function declaration
std::vector<Window> createWindows(const float data[][3], int num_rows, int window_size, int stride);

#endif
*/

#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <Arduino.h>
#include <SD.h>

#define FEATURE_DIM 3
#define WINDOW_SIZE 50
#define STRIDE 10
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
    char lineBuffer[LINE_BUFFER_SIZE];
    float buffer[WINDOW_SIZE][FEATURE_DIM];
    int samplesSeen = 0;
    bool finished = false;

    void processWindow(float window[WINDOW_SIZE][FEATURE_DIM]);
    void addSample(float sample[FEATURE_DIM]);
};

#endif