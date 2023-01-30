/*
 * hw01.cpp
 *
 *  Created on: Dec 13, 2020
 *      Author: kompalli
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
		
		char line[100]; // LINE BUFFER TO STORE THE LINE READ FROM THE INPUT FILE

		// READ ONE LINE AT A TIME FROM THE INPUT FILE AND STORE IT IN THE LINE BUFFER
		char *fileReadStatus = fgets(line, 100, inputFileStream);

		// WHEN fgets() REACHES EOF, IT RETURNS NULL - BREAK THE LOOP IF NULL IS RETURNED
		if (fileReadStatus == NULL)
		{
			returnInteger = EOF;
			break;
		}

		// REMOVE ANY LEADING AND TRAILING WHITE SPACES FROM THE LINE
		int i = 0, j = strlen(line) - 1;

		// REMOVE LEADING WHITE SPACES
		while (i < strlen(line) && isspace(line[i]))
			i++;

		// REMOVE TRAILING WHITE SPACES
		while (j >= 0 && isspace(line[j]))
			j--;

		// IF THE LINE IS EMPTY, SKIP THE LINE
		if (line[i] == '\n')
			continue;

		// POINTER TO STORE THE ADDRESS OF THE FIRST INVALID CHARACTER
		char *endptr;

		// START CONVERSION OF THE LINE TO A LONG INT FROM THE FIRST NON-WHITE SPACE CHARACTER
		long int number = strtol(line + i, &endptr, 10);

		// IF strtol() DOES NOT CONVERT ANY CHARACTER, IT RETURNS 0 AND endptr IS SET TO line + i
		// SO, IF THE LINE IS NOT A VALID INTEGER, endptr WILL BE EQUAL TO line + i
		if (number == 0 && endptr == line + i)
			continue;

		// IF THE LINE IS A VALID INTEGER, BUT OUT OF RANGE, SKIP THE LINE
		if (number < INT_MIN || number > INT_MAX)
			continue;

		// IF THE LINE IS A VALID INTEGER, BUT CONTAINS EXTRA CHARACTERS, SKIP THE LINE
		if (endptr != line + j + 1)
			continue;

		// IF THE LINE IS A VALID INTEGER, RETURN THE INTEGER
		returnInteger = number;
		break;
	}

	return returnInteger;
}

int UniqueInt::processFile(char *inputFilePath, char *outputFilePath)
{

	// CREATE A BOOLEAN ARRAY OF SIZE 2047 TO STORE THE UNIQUE NUMBERS
	bool isEncountered[2047] = {false};

	// OPEN THE INPUT FILE FOR READING
	FILE *inFileStream = fopen(inputFilePath, "r");
	if (!inFileStream)
	{
		char message[1024];
		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
		throw std::ios_base::failure(message);
	}

	// OPEN THE OUTPUT FILE FOR WRITING
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
	try
	{
		while (true)
		{
			// READ THE NEXT ITEM FROM THE INPUT FILE
			int number = readNextItemFromFile(inFileStream);

			// PRINT THE NUMBER TO CONSOLE
			std::cout << number << std::endl;

			// IF EOF REACHED, BREAK THE LOOP
			if (number == EOF)
				break;

			// IF NUMBER NOT ENCOUNTERED AT index = number + 1023, SET THE VALUE OF ARRAY TO true
			if (!isEncountered[number + 1023])
				isEncountered[number + 1023] = true;

			// IF NUMBER ENCOUNTERED BEFORE, SKIP THE NUMBER
			else
				continue;

			LogManager::writePrintfToLog(LogManager::Level::Status, "UniqueInt::processFile", "number=%d", number);
		}

		// WRITE THE UNIQUE NUMBERS TO THE OUTPUT FILE
		for (int i = 0; i < 2047; i++)
		{
			// IF THE NUMBER IS UNIQUE, WRITE IT TO THE OUTPUT FILE
			if (isEncountered[i] == true)
				fprintf(outFileStream, "%d\n", i - 1023);
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