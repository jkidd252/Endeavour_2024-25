#include "Teensy_RMSofData.h"

DataRMS::DataRMS(const char* inputFileName, const char* outputFileName) {
    size_t inLen = strlen(inputFileName);
    if (inLen >= sizeof(inFileName)) inLen = sizeof(inFileName) - 1;
    memcpy(inFileName, inputFileName, inLen);
    inFileName[inLen] = '\0';

    size_t outLen = strlen(outputFileName);
    if (outLen >= sizeof(outFileName)) outLen = sizeof(outFileName) - 1;
    memcpy(outFileName, outputFileName, outLen);
    outFileName[outLen] = '\0';   
}

bool DataRMS::begin(uint8_t cs_pin) {
    inputFile = SD.open(inFileName);
    if (!inputFile) {
        return false;
    }

    outputFile = SD.open(outFileName, FILE_WRITE);
    if (!outputFile) {
        return false;
    }
    
    outputFile.println("# START");
    outputFile.flush();
    return true;
}

bool DataRMS::isFinished() {
    return finished;
}

bool DataRMS::update() {
    if (finished || !inputFile.available()) {
        finished = true;
        inputFile.close();
        outputFile.close();
        return false;
    }

    String line = inputFile.readStringUntil('\n');
    line.trim();  // remove any trailing whitespace

    if (line.length() > 0) {
        processLine(line.c_str());
    } else {
        finished = true;
        inputFile.close();
        outputFile.close();
    }

    return true;
}

void DataRMS::processLine(const char* line) {
    float time, P1, P2, P3, Ax, Ay, Az;
    int parsed = sscanf(line, "%f %f %f %f %f %f %f", &time, &P1, &P2, &P3, &Ax, &Ay, &Az);

    if (parsed == 7) {
        float rmsPiezo = sqrt((P1*P1 + P2*P2 + P3*P3) / 3.0f);

        outputFile.print(time, 6);
        outputFile.print(" ");
        outputFile.println(rmsPiezo, 6);
        outputFile.flush();
    }
}