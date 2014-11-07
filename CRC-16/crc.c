#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// prototypes
char* padBinaryString(char*);
void printBinaryString(char*);
char* binToHex(char*);
char fourBitToHex(char*);
char* hexToBin(char*);
bool validate(char*);
char* xor(char*, char*);
// maybe one that divides, too
char* calcCRC(char*, char*);
bool verifyCRC(char*, char*);

int main(int argc, char** argv) {
  char* someBinary = argv[1];
  char* paddedBinary = padBinaryString(someBinary);

  // echo the binary to the console
  printf("The binary string is:\n");
  printBinaryString(paddedBinary);
  printf("\nIts hexadecimal equivalent is: %s\n", binToHex(paddedBinary));

  // strncpy(smallString, someBinary + 4, 4);
  return 0;
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

