#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// prototypes
void validateInput(int, char*, char*);
char* removeOffset(char*);
void displayObservedCRC(char*);
void displayCalculations(char*);
void displayValues(char*, char*);
int findNewIndex(char*);
char* hexCharToBin(char);
char* padBinaryString(char*);
void printBinaryString(char*);
char* binToHex(char*);
char fourBitToHex(char*);
char* hexToBin(char*);
bool validate(char*);
char* xor(char*, char*);
char* calcCRC(char*, char*, int);
bool verifyCRC(char*, char*);
char* assignHexFromFile(char*);
char* appendZeroes(char*);
char* gatherOffsetFromFile(char*);
void printArgumentError();
void printFileError();
void printFlagError();
void printValidationError();
void runCalculation(char*, char*);
void runVerification(char*, char*);

int main(int argc, char** argv) {
  char* polynomial = "10100101010001101";
  char* flagValue;
  char* inputFile;

  validateInput(argc, argv[1], argv[2]);
  
  // assign flagValue and inputFile from keyboard entry
  flagValue = argv[1];
  inputFile = argv[2];

  // run in calculation mode
  if(strcmp(flagValue, "c") == 0) {
    runCalculation(inputFile, polynomial);
  }
  // run in verification mode
  else {
    runVerification(inputFile, polynomial);
  }

  return 0;
}

// check that user input adheres to basic requirements
void validateInput(int numArguments, char* firstArgument, char* secondArgument) {
  // error-checking tree
  if(numArguments != 3) {
    printArgumentError();
    exit(0);
  }
  else if(strcmp(firstArgument, "c") != 0 && strcmp(firstArgument, "v") != 0) {
    printFlagError();
    exit(0);
  }
  else if(!fopen(secondArgument, "r")) {
    printFileError();
    exit(0);
  }
  else if(!validate(secondArgument)) {
    printValidationError();
    exit(0);
  }
}

// run the program in calculation mode
void runCalculation(char* inputFile, char* polynomial) {
  char* hexVal = (char *)calloc(41, sizeof(char));
  char* computedCRC = (char *)calloc(5, sizeof(char));

  // assign the hex value from the file
  hexVal = assignHexFromFile(inputFile);

  // declare the contents of the input file in hex and binary
  displayValues(hexVal, polynomial);
  printf("We will append 16 zeroes to the binary input.\n\n");

  // calculate CRC-16
  computedCRC = calcCRC(appendZeroes(hexToBin(hexVal)), polynomial, 1);

  // display computed CRC
  displayCalculations(computedCRC);
}

// run the program in verification mode
void runVerification(char* inputFile, char* polynomial) {
  char* hexVal = (char *)calloc(41, sizeof(char));
  char* computedCRC = (char *)calloc(5, sizeof(char));
  bool pass;

  // assign the hex value from the file
  hexVal = assignHexFromFile(inputFile);

  // declare the contents of the input file in hex and binary
  displayValues(hexVal, polynomial);

  // declare the observed 16-bit crc
  displayObservedCRC(inputFile);

  // verify the crc
  pass = verifyCRC(hexToBin(hexVal), polynomial);

  // compute the CRC
  computedCRC = calcCRC(appendZeroes(hexToBin(removeOffset(hexVal))), polynomial, 0);

  // display result of calculations
  displayCalculations(computedCRC);
  printf("\n");

  // display pass/fail
  printf("Did the CRC check pass? (Yes or No): ");
  if(pass) {
    printf("Yes\n\n");
  }
  else {
    printf("No\n\n");
  }

}

// remove the observed CRC-16 from the hex value in the input file
char* removeOffset(char* hexVal) {
  char* newHex = (char *)calloc(37, sizeof(char));
  int numDigits = strlen(hexVal);
  int i;

  for(i = 0; i < numDigits - 4; ++i) {
    newHex[i] = hexVal[i];
  }

  return newHex;
}

// declare the CRC-16 attached to the value in the file
void displayObservedCRC(char* inputFile) {
  char* offset = (char *)calloc(5, sizeof(char));
  offset = gatherOffsetFromFile(inputFile);

  printf("The 16-bit CRC observed at the end of the file (bin):\n");
  printBinaryString(hexToBin(offset));
  printf("\n");
  printf("The 16-bit CRC observed at the end of the file (hex):\n%s\n\n", offset);
}

// show the computed CRC in binary and hex
void displayCalculations(char* computedCRC) {
  printf("The computed CRC for the input file (bin):\n");
  printBinaryString(computedCRC);
  printf("\n");
  printf("The computed CRC for the input file (hex):\n%s\n", binToHex(computedCRC));
}

// display the hex and binary versions of the values to the consol
void displayValues(char* hexVal, char* polynomial) {
  // declare the contents of the input file in hex and binary
  printf("The input file (hex):\n%s\n\n", hexVal);
  printf("The input file (bin):\n");
  printBinaryString(hexToBin(hexVal));
  printf("\n\n");

  // declare the polynomial used for calculation
  printf("The polynomial that was used (binary bit string):\n");
  printBinaryString(polynomial);
}

// error message for non-hexadecimal digits found in file
void printValidationError() {
  printf("The file specified contains non-hexadecimal digits.\n");
}

// print error message for wrong number of arguments
void printArgumentError() {
  printf("You must enter 2 arguments:\n");
  printf("1) 'c' or 'v' for calculate or verify a CRC-16, respectively\n");
  printf("2) the name of a file (such as input.txt) to be used");
}

// print error message for input file
void printFileError() {
  printf("The system cannot find the file specified.\n");
}
  
// print error message for wrong flag entry
void printFlagError() {
  printf("You must choose either a 'c' or a 'v' for your first argument.\n");
}

// verify the validity of the crc appended to the message
bool verifyCRC(char* binaryString, char* polynomial) {
  char* properResult = "0000000000000000";

  if(strcmp(calcCRC(binaryString, polynomial, 1), properResult) == 0) {
    return true;
  }
  else {
    return false;
  }
}

// calculates the crc-16 value for the given binaryString, polynomial pair
char* calcCRC(char* binaryString, char* polynomial, int displayResults) {
  if(displayResults == 1) {
    printf("The binary string difference after each XOR step of the CRC calculation:\n\n");
    printBinaryString(binaryString);
  }
  int i = 0;
  char* result = (char *)calloc(strlen(binaryString) + 1, sizeof(char));

  // initialize result as the binary string before any operations
  result = binaryString;
  while(i < strlen(binaryString) - 17) {
    i = findNewIndex(result);
    strncpy(result + i, xor(polynomial, result + i), strlen(polynomial));
    if(displayResults == 1) {
      printBinaryString(result);
    }
  }

  if(displayResults == 1) {
    printf("\n");
  }
  return (result + (strlen(binaryString) - 16));
}

// finds the next non-zero digit during polynomial division
int findNewIndex(char* binaryString) {
  int index;

  for(index = 0; index < strlen(binaryString) - 17; ++index) {
    if(binaryString[index] != '0') {
      break;
    }
  }

  return index;
}

// append 16 zeroes to our binary string
char* appendZeroes(char* binaryString) {
  char* newBinary = (char *)calloc(strlen(binaryString) + 17, sizeof(char));
  char* sixteenZeroes = "0000000000000000";

  strcpy(newBinary, binaryString);
  strcat(newBinary, sixteenZeroes);

  return newBinary;
}

// shows the result of XOR on two binary strings
char* xor(char* binA, char* binB) {
  int i;
  char* binC = (char *)calloc(strlen(binA) + 1, sizeof(char));

  // compare the digits of A and B to determine the result for C
  for(i = 0; i < strlen(binA); ++i) {
    if(binA[i] == binB[i]) {
      binC[i] = '0';
    }
    else {
      binC[i] = '1';
    }
  }

  return binC;
}

// auxillary function for verification mode
bool validate(char* inputFile) {
  int i;
  char* fileContents = (char *)calloc(41, sizeof(char));
  int fileLength;
  FILE* ifp = fopen(inputFile, "r");
  bool goodFile = true;

  // gather contents of input file
  fscanf(ifp, "%s", fileContents);
  fileLength = strlen(fileContents);

  for(i = 0; i < fileLength; ++i) {
    if(fileContents[i] < '0' || fileContents[i] > 'f') {
      goodFile = false;
    }
    else if(fileContents[i] > '9' && fileContents[i] < 'A') {
      goodFile = false;
    }
    else if(fileContents[i] > 'F' && fileContents[i] < 'a') {
      goodFile = false;
    }
  }

  return goodFile;
}

// pulls the offset from the file when in verification mode
char* gatherOffsetFromFile(char* inputFile) {
  char* fileContents = (char *)calloc(41, sizeof(char));
  char* offset = (char *)calloc(5, sizeof(char));
  int fileLength;
  FILE* ifp = fopen(inputFile, "r");

  // gather contents
  fscanf(ifp, "%s", fileContents);
  fileLength = strlen(fileContents);

  // pull last four digits from file
  strncpy(offset, (fileContents + fileLength - 4), 4);

  fclose(ifp);
  return offset;
}

// return the hexadecimal value in the file
char* assignHexFromFile(char* fileName) {
  char* hex = calloc(41, sizeof(char));
  FILE* ifp = fopen(fileName, "r");

  fscanf(ifp, "%s", hex);
  fclose(ifp);
  return hex;
}

// translate a binary string into hex
char* binToHex(char* binary) {
  char* hexadecimalValue = (char *)calloc(41, sizeof(char));
  char* tempNibble = (char *)calloc(5, sizeof(char));
  int numNibbles = strlen(binary) / 4;
  int i;

  // four bits at a time, assign the hexadecimal digits of our hex value
  for(i = 0; i < numNibbles; ++i) {
    strncpy(tempNibble, binary + (4 * i), 4);
    hexadecimalValue[i] = fourBitToHex(tempNibble);
  }

  return hexadecimalValue;
}

// convert a string of hexadecimal digits into a binary string
char* hexToBin(char* hex) {
  int i, numDigits;
  numDigits = strlen(hex);
  char* binaryEquivalent = (char *)calloc(161, sizeof(char));

  // digit-by-digit, convert hex characters into binary nibbles
  for(i = 0; i < numDigits; ++i) {
    strcat(binaryEquivalent, hexCharToBin(hex[i]));
  }

  return binaryEquivalent;
}

// formatted print for humans
void printBinaryString(char* binaryString) {
  int i;
  // TODO: implement character limit on lines 
  for(i = 0; i < strlen(binaryString); ++i) {
    if(i != 0 && i % 4 == 0 ) {
      printf(" ");
    }
    printf("%c", binaryString[i]);
  }
  printf("\n");
} 

// prepend zeroes to a binary string so that it has groups of 4 bits
char* padBinaryString(char* binary) {
  char* newBinary = (char *)calloc(160, sizeof(char));
  int numDigits = strlen(binary);
  int i;

  // if we have groups of 4 bits throughout, no need to pad
  if((numDigits % 4) != 0) {
    for(i = 0; i < (4 - numDigits % 4); ++i) {
      newBinary[i] = '0';
    }
  }

  // join newBinary with binary to form the padded string
  strcat(newBinary, binary);
  return newBinary;
}

// converts four bits of a binary string to a hexadecimal character
char fourBitToHex(char* fourBits) {
  char hexChar;

  if(strcmp(fourBits, "0000") == 0) {
    hexChar = '0';
  }
  else if(strcmp(fourBits, "0001") == 0) {
    hexChar = '1';
  }
  else if(strcmp(fourBits, "0010") == 0) {
    hexChar = '2';
  }
  else if(strcmp(fourBits, "0011") == 0) {
    hexChar = '3';
  }
  else if(strcmp(fourBits, "0100") == 0) {
    hexChar = '4';
  }
  else if(strcmp(fourBits, "0101") == 0) {
    hexChar = '5';
  }
  else if(strcmp(fourBits, "0110") == 0) {
    hexChar = '6';
  }
  else if(strcmp(fourBits, "0111") == 0) {
    hexChar = '7';
  }
  else if(strcmp(fourBits, "1000") == 0) {
    hexChar = '8';
  }
  else if(strcmp(fourBits, "1001") == 0) {
    hexChar = '9';
  }
  else if(strcmp(fourBits, "1010") == 0) {
    hexChar = 'A';
  }
  else if(strcmp(fourBits, "1011") == 0) {
    hexChar = 'B';
  }
  else if(strcmp(fourBits, "1100") == 0) {
    hexChar = 'C';
  }
  else if(strcmp(fourBits, "1101") == 0) {
    hexChar = 'D';
  }
  else if(strcmp(fourBits, "1110") == 0) {
    hexChar = 'E';
  }
  else if(strcmp(fourBits, "1111") == 0) {
    hexChar = 'F';
  }

  return hexChar;
}

// convert a hex character to its 4-bit binary equivalent
char* hexCharToBin(char hexValue) {
  char* nibble = calloc(5, sizeof(char));

  if(hexValue == '0') {
    nibble = "0000";
  }
  else if(hexValue == '1') {
    nibble = "0001";
  }
  else if(hexValue == '2') {
    nibble = "0010";
  }
  else if(hexValue == '3') {
    nibble = "0011";
  }
  else if(hexValue == '4') {
    nibble = "0100";
  }
  else if(hexValue == '5') {
    nibble = "0101";
  }
  else if(hexValue == '6') {
    nibble = "0110";
  }
  else if(hexValue == '7') {
    nibble = "0111";
  }
  else if(hexValue == '8') {
    nibble = "1000";
  }
  else if(hexValue == '9') {
    nibble = "1001";
  }
  else if(hexValue == 'A' || hexValue == 'a') {
    nibble = "1010";
  }
  else if(hexValue == 'B' || hexValue == 'b') {
    nibble = "1011";
  }
  else if(hexValue == 'C' || hexValue == 'c') {
    nibble = "1100";
  }
  else if(hexValue == 'D' || hexValue == 'd') {
    nibble = "1101";
  }
  else if(hexValue == 'E' || hexValue == 'e') {
    nibble = "1110";
  }
  else if(hexValue == 'F' || hexValue == 'f') {
    nibble = "1111";
  }

  return nibble;
}