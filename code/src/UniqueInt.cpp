/*
 * hw01.cpp
 *
 *  Created on: Dec 13, 2020
 *      Author: kompalli
 */

/*
 * UniqueInt.cpp
 *
 * Purpose: Reading a list of integers from an input file, then 
 * Generate an output file having a list of unique integers present in the input file
 *
 * Submitted on: February 6, 2023
 * Co-Author: Niyomwungeri Parmenide ISHIMWE <parmenin@andrew.cmu.edu>
 */

#include "UniqueInt.h"

int UniqueInt::readNextItemFromFile(FILE *inputFileStream)
{
    if (!inputFileStream)
    {
        char message[35] = "Cannot open input file for reading";
        throw std::ios_base::failure(message);
    }

    int returnInteger;

    // READ ONE LINE AT A TIME FROM THE INPUT FILE - UNTIL EOF IS REACHED
    while (true)
    {
        char lineBuffer[1000]; // LINE BUFFER TO STORE THE LINE READ FROM THE INPUT FILE

        // READ ONE LINE AT A TIME FROM THE INPUT FILE AND STORE IT IN THE LINE BUFFER
        char *oneLineReading;

        // WHEN fgets() REACHES EOF, IT RETURNS NULL - BREAK THE LOOP IF NULL IS RETURNED
        if (!(oneLineReading = fgets(lineBuffer, 1000, inputFileStream)))
        {
            // IF fgets() RETURNS NULL, CHECK IF THE INPUT FILE HAS REACHED EOF
            if (feof(inputFileStream))
                break; // BREAK THE LOOP IF EOF IS REACHED

            // IF fgets() RETURNS NULL, BUT THE INPUT FILE HAS NOT REACHED EOF, THROW AN EXCEPTION
            else
            {
                char message[35] = "Cannot read from input file";
                throw std::ios_base::failure(message);
            }
        }

        // REMOVE ANY LEADING AND TRAILING WHITE SPACES FROM THE LINE
        int start = 0, end = strlen(lineBuffer) - 1;

        // REMOVE LEADING WHITE SPACES (AT THE BEGINNING OF THE LINE)
        while (start < strlen(lineBuffer) && isspace(lineBuffer[start]))
            start++;

        // REMOVE TRAILING WHITE SPACES (AT THE END OF THE LINE)
        while (end >= 0 && isspace(lineBuffer[end]))
            end--;

        // IF THE LINE IS EMPTY, SKIP THE LINE
        if (lineBuffer[start] == '\n')
            continue;

        // POINTER TO STORE THE ADDRESS OF THE FIRST INVALID CHARACTER
        char *firstInvalidChar;

        // START CONVERSION OF THE LINE TO A LONG INT FROM THE FIRST NON-WHITE SPACE CHARACTER
        long int number = strtol(lineBuffer + start, &firstInvalidChar, 10);

        // IF strtol() DOES NOT CONVERT ANY CHARACTER, IT RETURNS 0 AND firstInvalidChar IS SET TO line + i
        // SO, IF THE LINE IS NOT A VALID INTEGER, firstInvalidChar WILL BE EQUAL TO line + i
        if (number == 0 && firstInvalidChar == lineBuffer + start)
            continue;

        // IF THE LINE IS A VALID INTEGER, BUT OUT OF RANGE, SKIP THE LINE
        if (number < INT_MIN || number > INT_MAX)
            continue;

        // IF THE LINE IS A VALID INTEGER, BUT CONTAINS EXTRA CHARACTERS, SKIP THE LINE
        if (firstInvalidChar != lineBuffer + end + 1)
            continue;

        // IF THE LINE IS A VALID INTEGER, RETURN THE INTEGER
        returnInteger = number;
        break;
    }

    return returnInteger;
}

int UniqueInt::processFile(char *inputFilePath, char *outputFilePath)
{

    FILE *inFileStream = fopen(inputFilePath, "r");
    if (!inFileStream)
    {
        char message[1024];
        sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
        throw std::ios_base::failure(message);
    }

    FILE *outFileStream = fopen(outputFilePath, "w");
    if (!outFileStream)
    {
        char message[1024];
        sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
        throw std::ios_base::failure(message);
    }
    LogManager::writePrintfToLog(LogManager::Level::Status,
                                 "UniqueInt::processFile",
                                 "Starting the processing inFile=%s", inputFilePath);

    // READ THE INPUT FILE UNTIL EOF IS REACHED
    try
    {
        // CREATE A BOOLEAN ARRAY OF SIZE 2047 TO STORE THE UNIQUE NUMBERS
        bool isEncountered[2047] = {false}; // INDEX 0 => -1023, INDEX 1023 => 0, INDEX 2046 => 1023

        // READING THE INPUT FILE UNTIL EOF IS REACHED
        while (true)
        {

            // READ ONE INTEGER AT A TIME FROM THE INPUT FILE
            int number = readNextItemFromFile(inFileStream);
            LogManager::writePrintfToLog(LogManager::Level::Status, "UniqueInt::processFile",
                                         "number=%d", number);

            // IF EOF IS REACHED, BREAK THE LOOP
            if (feof(inFileStream))
                break;

            // IF THE NUMBER IS UNIQUE, SET THE CORRESPONDING BOOLEAN ARRAY ELEMENT TO TRUE
            if (!isEncountered[number + 1023])
                isEncountered[number + 1023] = true;
        }

        // WRITE THE UNIQUE NUMBERS TO THE OUTPUT FILE
        for (int index = 0; index < 2047; index++)
        {
            // IF THE NUMBER IS UNIQUE, WRITE IT TO THE OUTPUT FILE
            if (isEncountered[index] == true)
                fprintf(outFileStream, "%d\n", index - 1023);
        }
    }

    catch (std::invalid_argument &e)
    {
        LogManager::writePrintfToLog(LogManager::Level::Status,
                                     "UniqueInt::processFile",
                                     "invalid_argument exception caught=%s\n", e.what());
        if (strcmp(e.what(), "EoF reached") == 0)
        {
            LogManager::writePrintfToLog(LogManager::Level::Status,
                                         "UniqueInt::processFile",
                                         "OK to continue");
        }
        else
        {
            return -1;
        }
    }

    fflush(outFileStream);
    fclose(outFileStream);
    fclose(inFileStream);
}