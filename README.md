### UniqueInt

This is a C++ program that reads a file and returns the first unique integer and outputs them into a file. The program takes two command line arguments: the name of the input file and the name of the output file. The input file contains a list of integers, one per line. The output file contains the first unique integer in the input file. If there is no unique integer in the input file, the output file should contain nothing.

Its two main functions are: readNextItemFromFile and processFile

1. readNextItemFromFile

This function must take as input a file stream that is open, read the next line
and return an integer if it is valid.

a. If there are any lines with no inputs, those lines must be skipped.
b. If there are any lines that contain a non-integer input, those lines must be skipped. If end of file is reached, it must throw an end of file error

2. processFile

This is a function that reads an input file and give the output as needed. It takes as input the name of the input file and the name of the output file. It opens the input file and output file and call the readNextItemFromFile function to read the input file. It writes the first unique integer to the output file. If there is no unique integer in the input file, the output file should contain nothing.

a. If the input file cannot be read throw an error of type ios_base::failure
b. If the output file cannot be generated, then throw an error of type ios_base::failure


##### __Explanation:__

   * The function takes a file stream inputFileStream as parameter.
   * It first checks if the input file stream is null or not. If it's null, it throws an error message "Cannot open input file for reading".
   * The function then reads one line at a time from the file stream using fgets and stores it in a line buffer.
   * It removes any leading and trailing white spaces using two while loops.
   * It then checks if the line is empty or not and skips it if it's empty.
   * It uses strtol function to convert the line to a long int type. If strtol returns 0 and the pointer endptr points to the same position, it means the line is not a valid integer, so the function skips the line.
   * If the converted number is outside the range of int, the function skips the line.
   * If the pointer endptr does not point to the end of the line, the function skips the line.
   * If all the checks pass, the function stores the converted number in returnInteger and returns it.

   ##### __Usage:__

    * Compile the program using the command `g++ UniqueInt.cpp -o UniqueInt`
    * Run the program using the command `./UniqueInt input.txt output.txt`
    * The program will read the input file and write the first unique integer to the output file.