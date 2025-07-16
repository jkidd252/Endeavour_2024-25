#include "Teensy_RawDataHandle.h"

DataHandler::DataHandler(const char* inputFileName, const char* outputFileName, const char* outputClassFileName) {
    size_t inLen = strlen(inputFileName);
    if (inLen >= sizeof(inFileName)) inLen = sizeof(inFileName) - 1;
    memcpy(inFileName, inputFileName, inLen);
    inFileName[inLen] = '\0';

    size_t outLen = strlen(outputFileName);
    if (outLen >= sizeof(outFileName)) outLen = sizeof(outFileName) - 1;
    memcpy(outFileName, outputFileName, outLen);
    outFileName[outLen] = '\0';   

    size_t outClassLen = strlen(outputClassFileName);
    if (outClassLen >= sizeof(outClassFileName)) outClassLen = sizeof(outClassFileName) - 1;
    memcpy(outClassFileName, outputClassFileName, outClassLen);
    outClassFileName[outClassLen] = '\0';   
}

bool DataHandler::begin(uint8_t cs_pin) {
    if (!SD.begin(cs_pin)) {
        return false;
    }

    inputFile = SD.open(inFileName);
    if (!inputFile) {
        return false;
    }

    outputFile = SD.open(outFileName, FILE_WRITE);
    if (!outputFile) {
        return false;
    }

    ClassFile = SD.open(outClassFileName, FILE_WRITE);
    if (!ClassFile) {
        return false;
    }

    outputFile.println("# START");
    ClassFile.println("# START");
    //Serial.println("DataHandle Start");
    return true;
}

bool DataHandler::update() {
    if (finished || !inputFile.available()) {
        finished = true;
        return false;
    }

    int len = 0;
    while (inputFile.available()) {
        char c = inputFile.read();
        if (c == '\n' || len >= LINE_BUFFER_SIZE - 1) {
            break;
        }
        if (c != '\r') {  // Ignore carriage returns (Windows-style)
            lineBuffer[len++] = c;
        }
    }
    lineBuffer[len] = '\0';
    if (len > 0) {
        float sample[FEATURE_DIM];
        if (sscanf(lineBuffer, "%f %f", &sample[0], &sample[1]) == FEATURE_DIM) {
            addSample(sample);
        }

    } else {
        finished = true;
        outputFile.close();
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

    samplesSeen++;

    if (samplesSeen >= WINDOW_SIZE) {
        processWindow(buffer);
    }
}

void DataHandler::processWindow(float window[WINDOW_SIZE][FEATURE_DIM]) {
    for (int i = 0; i < WINDOW_SIZE; ++i) {
        for (int j = 0; j < FEATURE_DIM; ++j) {
            outputFile.print(window[i][j], 6);
            if (j < FEATURE_DIM - 1)
                outputFile.print(' ');
        }
        outputFile.println();  // newline after each row
    }

    outputFile.println();
    outputFile.flush(); // optional, can remove for speed

    // === Flatten the window into 1D input array ===
    float input[WINDOW_SIZE * FEATURE_DIM];
    int idx = 0;
    for (int i = 0; i < WINDOW_SIZE; ++i) {
        for (int j = 0; j < FEATURE_DIM; ++j) {
            input[idx++] = window[i][j];
        }
    }

    // === Run Model on window ===
    int predicted_class = run_model_inference(input);
    // === Write classification to class output file ===
    ClassFile.println(predicted_class);
    ClassFile.flush();
}

bool DataHandler::isFinished() {
    return finished;
}