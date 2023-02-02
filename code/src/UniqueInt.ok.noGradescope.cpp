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

	// Write code here to read the input file.
	while (true)
	{
		// Read a line from File
		char line[100];
		char *fileReadStatus = fgets(line, 100, inputFileStream);

		// Check if the end of file is reached - if yes, stop the loop and make the return value as EOF
		if (fileReadStatus == NULL)
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

		// Check if the line is empty
		if (line[0] == '\n')
			continue;

		// Check if the line is a valid integer
		char *endptr; // pointer to the first invalid character

		// check if the number is in the range of int type - if not, skip the line
		if (strtol(line, &endptr, 10) < INT_MIN || strtol(line, &endptr, 10) > INT_MAX)
			continue;

		// check if the number is not float using a loop - if not, skip the line
		for (int i = 0; i < strlen(line); i++)
		{
			// check if the number is not decimal - if not, skip the line
			if (line[i] == '.')
				continue;

			// check if the char is a valid digit - if not, skip the line
			if (isdigit(line[i]) == 0)
				continue;
		}

		// check if the line endptr is not pointing to the first character of the line
		// if so, the line has invalid characters - skip the line
		if (endptr == line)
			continue;

		// if endptr points to a space, skip the line
		if (isspace(*endptr) == 0)
			continue;

		// check if the line has a space in between the integer and the newline character
		int i = 0, j = strlen(line) - 1;

		// REMOVE LEADING WHITE SPACES - AT THE BEGINNING OF THE LINE
		while (i < strlen(line) && isspace(line[i]))
			i++;

		// REMOVE TRAILING WHITE SPACES - AT THE END OF THE LINE
		while (j >= 0 && isspace(line[j]))
			j--;

		// CALCULATE THE NEW LENGTH OF THE STRING
		int new_length = j - i + 1;

		// ALLOCATE A NEW CHAR ARRAY OF THE APPROPRIATE SIZE
		char new_line[new_length + 1];

		// COPY THE CHARACTERS FROM THE ORIGINAL ARRAY TO THE NEW ARRAY
		for (int k = i, l = 0; k <= j; k++, l++)
			new_line[l] = line[k];

		// ADD THE NULL CHARACTER
		new_line[new_length] = '\0';

		// SKIP THE LINE THAT HAS A SPACE IN BETWEEN THE INTEGER AND THE NEWLINE CHARACTER
		bool isThereSpace = false;
		for (int i = 0; new_line[i] != '\0'; i++)
		{
			if (new_line[i] == ' ')
			{
				isThereSpace = true;
				break;
			}
		}

		if (isThereSpace)
			continue;

		// convert the string to integer
		returnInteger = strtol(new_line, &endptr, 10);

		// If all the above conditions are satisfied, break the loop
		break;
	}

	// RETURN THE INTEGER
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
	try
	{
		// Declare a dynamic array to hold unique integers
		int *dynamicIntegers = NULL;
		int n = 0;

		// While there are integers in the input file, read the next integer and store unique integers in a dynamic array
		while (true)
		{
			int number = readNextItemFromFile(inFileStream);
			LogManager::writePrintfToLog(LogManager::Level::Status, "UniqueInt::processFile",
										 "number=%d", number);

			// Check if the end of file is reached
			if (feof(inFileStream))
				break;

			// Check if the read integer already exists in the dynamic array
			bool exists = false;
			for (int i = 0; i < n; i++)
			{
				if (dynamicIntegers[i] == number)
				{
					exists = true;
					break;
				}
			}

			// If the integer does not exist in the dynamic array, add it
			if (!exists)
			{
				// Allocate new memory and store the integer
				int *temp = new int[n + 1];
				for (int i = 0; i < n; i++)
					temp[i] = dynamicIntegers[i];

				temp[n] = number;
				n++;

				// Replace the old dynamic array with the new one
				if (dynamicIntegers != NULL)
					delete[] dynamicIntegers;

				dynamicIntegers = temp;
			}
		}

		// Sort the dynamic array
		for (int i = 0; i < n; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				if (dynamicIntegers[i] > dynamicIntegers[j])
				{
					int temp = dynamicIntegers[i];
					dynamicIntegers[i] = dynamicIntegers[j];
					dynamicIntegers[j] = temp;
				}
			}
		}

		// Write the sorted dynamic array to the output file
		for (int i = 0; i < n; i++)
			fprintf(outFileStream, "%d\n", dynamicIntegers[i]);

		// Free the dynamic array
		if (dynamicIntegers != NULL)
			delete[] dynamicIntegers;
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
