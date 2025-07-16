#include "Teensy_RawDataHandle.h"

DataHandler::DataHandler(const char* inputFileName, const char* outputFileName) {
    strncpy(inFileName, inputFileName, sizeof(inFileName));
    strncpy(outFileName, outputFileName, sizeof(outFileName));
    inFileName[sizeof(inFileName) - 1] = '\0';
    outFileName[sizeof(outFileName) - 1] = '\0';
}

bool DataHandler::begin(uint8_t cs_pin) {
    if (!SD.begin(cs_pin)) {
        Serial.println("SD init failed.");
        return false;
    }

    inputFile = SD.open(inFileName);
    if (!inputFile) {
        Serial.print("Failed to open input file: ");
        Serial.println(inFileName);
        return false;
    }

    outputFile = SD.open(outFileName, FILE_WRITE);
    if (!outputFile) {
        Serial.print("Failed to open output file: ");
        Serial.println(outFileName);
        return false;
    }

    // Overwrite previous content
    outputFile.truncate(0);
    Serial.println("DataHandler: Files opened.");
    return true;
}

bool DataHandler::update() {
    if (finished || !inputFile.available()) {
        finished = true;
        return false;
    }

    if (inputFile.fgets(lineBuffer, LINE_BUFFER_SIZE)) {
        float sample[FEATURE_DIM];
        if (sscanf(lineBuffer, "%f,%f,%f", &sample[0], &sample[1], &sample[2]) == FEATURE_DIM) {
            addSample(sample);
        } else {
            Serial.print("Skipped invalid line: ");
            Serial.println(lineBuffer);
        }
    } else {
        finished = true;
        outputFile.close();
        Serial.println("Output file closed.");
    }

    return true;
}

void DataHandler::addSample(float sample[FEATURE_DIM]) {
    // Shift buffer
    for (int i = 0; i < WINDOW_SIZE - 1; ++i) {
        for (int j = 0; j < FEATURE_DIM; ++j) {
            buffer[i][j] = buffer[i + 1][j];
        }
    }

    for (int j = 0; j < FEATURE_DIM; ++j) {
        buffer[WINDOW_SIZE - 1][j] = sample[j];
    }

    ++samplesSeen;

    if (samplesSeen >= WINDOW_SIZE && ((samplesSeen - WINDOW_SIZE) % STRIDE == 0)) {
        processWindow(buffer);
    }
}

void DataHandler::processWindow(float window[WINDOW_SIZE][FEATURE_DIM]) {
    // Flatten and write window as CSV line: t1,v1,a1,t2,v2,a2,...
    for (int i = 0; i < WINDOW_SIZE; ++i) {
        for (int j = 0; j < FEATURE_DIM; ++j) {
            outputFile.print(window[i][j], 6);
            if (i != WINDOW_SIZE - 1 || j != FEATURE_DIM - 1)
                outputFile.print(',');
        }
    }
    outputFile.println();
    outputFile.flush(); // optional, can remove for speed
}

bool DataHandler::isFinished() {
    return finished;
}