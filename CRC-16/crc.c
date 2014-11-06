#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// prototypes
char* binToHex(char*);
char fourBitToHex(char*);
char* hexToBin(char*);
bool validate(char*);
char* xor(char*, char*);
// maybe one that divides, too
char* calcCRC(char*, char*);
bool verifyCRC(char*, char*);

int main(int argc, char** argv) {
  char someBinary[9] = "11110001";
  char otherString[15] = "0000";
  // strcat(otherString, someBinary);

  printf("%s\n", otherString);
  printf("%d", sizeof(otherString));
  // strncpy(smallString, someBinary + 4, 4);
  return 0;
}

// char* binToHex(char* binary) {
//   int numNibbles = (sizeof(binary) - 1 ) / 4
//   int i;
//   char* tempNibble;

//   for(i = 0; i < numNibbles; ++i) {
//     tempNibble = 
//   }
// }

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

