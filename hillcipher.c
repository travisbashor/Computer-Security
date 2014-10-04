#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct _Matrix {
  int rows, cols;
  int** grid;
} Matrix;

// prototypes
int** alloc2DArray(int, int);
void printMatrix(Matrix*);
Matrix* allocMatrix();
Matrix* assignMatrixFromFile(char*);
char* collectABC(char*);
void printMessage(char*);
void padMessage(char**, Matrix*);
Matrix* hillProduct(Matrix*, Matrix*);
int dotProduct(int**, int**, int, int);
Matrix* createColumn(char*, int);
char* encipherText(char*, Matrix*);

int main(int argc, char *argv[]) {
  Matrix* myMatrix;
  char *input = NULL;
  char *output = NULL;

  if(argc == 3) {
    // generate matrix from file
    myMatrix = assignMatrixFromFile(argv[1]);

    // look for some zeroes
    printMatrix(myMatrix);

    // collect message from file
    input = collectABC(argv[2]);
    padMessage(&input, myMatrix);

    // encipher the text
    output = encipherText(input, myMatrix);

    // echo message to screen
    printMessage(input);
    printMessage(output);   

  }
  else {
    printf("You must enter the names of 2 files.\n");
  }
}

// allocate memory for a 2D-array
int** alloc2DArray(int rows, int cols) {
  // allocate memory for the rows
  int **new2DArray = (int **)calloc(rows, sizeof(int *));
  int i;

  // create the columns
  for(i = 0; i < rows; ++i) {
    new2DArray[i] = (int *)calloc(cols, sizeof(int));
  }

  return new2DArray;
}

// allocate memory for a Matrix struct
Matrix* allocMatrix() {
  Matrix* myMatrix = (Matrix *)malloc(sizeof(Matrix));
  myMatrix->grid = NULL;
  myMatrix->rows = 0;
  myMatrix->cols = 0;

  return myMatrix;
}

// print a matrix of given dimensions
void printMatrix(Matrix* someMatrix) {
  int i, j;
  printf("\n");
  // row by row, print the matrix
  for(i = 0; i < someMatrix->rows; ++i) {
    for(j = 0; j < someMatrix->cols; ++j) {
      printf("%d ", someMatrix->grid[i][j]);
    }
    printf("\n");
  }
}

// create a matrix from the key file
Matrix* assignMatrixFromFile(char* fileName) {
  int sideLength, i, j;
  int **myArray;
  Matrix* myMatrix = allocMatrix();
  FILE *ifp = fopen(fileName, "r");

  // assign side length from file
  fscanf(ifp, "%d", &sideLength);

  // create matrix with given dimensions
  myArray = alloc2DArray(sideLength, sideLength);

  // assign values to matrix
  for(i = 0; i < sideLength; ++i) {
    for(j = 0; j < sideLength; ++j) {
      if(j == sideLength - 1) {
        fscanf(ifp, "%d", &myArray[i][j]);
      }
      else {
        fscanf(ifp, "%d %*c", &myArray[i][j]);
      }
    }
  }

  fclose(ifp);
  myMatrix->grid = myArray;
  myMatrix->rows = sideLength;
  myMatrix->cols = sideLength;

  return myMatrix;
}

// collect ordered alphabetic characters from file
char* collectABC(char* fileName) {
  FILE* ifp = fopen(fileName, "r");
  char* message = (char *)calloc(10000,sizeof(char));
  int i = 0;
  char this;
  
  // scan through, picking up abc/ABCs
  while(fscanf(ifp, "%c", &this) != EOF) {
    if(this >= 'a' && this <= 'z') {
      message[i] = this;
      ++i;
    }
    else if(this >= 'A' && this <= 'Z') {
      // convert to lower
      this += ('a' - 'A');
      message[i] = this;
      ++i;
    }
  }
  fclose(ifp);

  return message;
}

// pad message with x's. 
void padMessage(char** text, Matrix* someMatrix) {
  int i = 0;

  // bring i to the end of the message
  while((*text)[i] != 0) {
    ++i;  
  }

  // pad any residual space up to a multiple of the matrix's cols
  while(i % someMatrix->cols != 0) {
    (*text)[i] = 'x';
    ++i;
  }
}

// print text
void printMessage(char* text) {
  int i;
  printf("\n");

  // run through, only printing non-zero-valued chars
  for(i = 0; i < 10000; ++i) {
    if(text[i] != 0) {
      printf("%c", text[i]);
    }
  }
  printf("\n");
}

// encipher the plaintext, fragment by fragment
char* encipherText(char* text, Matrix* keyMatrix) {
  int i, j; 
  Matrix* tempMatrix = NULL;
  Matrix* cipherColumn = NULL;
  char *output = calloc(10000, sizeof(char));

  for(i = 0; i < 10000; i += keyMatrix->rows) {
    if(text[i] != 0) {
      tempMatrix = createColumn(&text[i], keyMatrix->rows);
      for(j = 0; j < keyMatrix->rows; ++j) {
        output[i + j] = (hillProduct(keyMatrix, tempMatrix)->grid)[j][0];
      }
    }
  }

  return output;
}

// create a column of fragmented text from the plaintext for multiplication against the key
Matrix* createColumn(char* text, int rowNum) {
  int i;
  Matrix* newColumn = allocMatrix();
  newColumn->cols = 1;
  newColumn->rows = rowNum;
  newColumn->grid = alloc2DArray(newColumn->rows, newColumn->cols);

  for(i = 0; i < rowNum; ++i) {
    (newColumn->grid)[i][0] = text[i];
  }

  return newColumn;
}

// auxillary function for encipherText(), which performs matrix multiplication
Matrix* hillProduct(Matrix* keyMatrix, Matrix* columnVector) {
  int i;
  Matrix* C = allocMatrix();
  C->cols = 1;
  C->rows = keyMatrix->rows;
  C->grid = alloc2DArray(C->rows, C->cols);

  for(i = 0; i < C->rows; ++i) {
    (C->grid)[i][0] = 
      (dotProduct(keyMatrix->grid, columnVector->grid, i, columnVector->rows) % 26 + 'a');
  }

  return C;
}

// auxillary function for hillProduct()
int dotProduct(int** rowVector, int** columnVector, int rowSelector, int numTerms) {
  int i;
  int aDotB = 0;
  
  for(i = 0; i < numTerms; ++i) {
    aDotB += ((rowVector[rowSelector][i]) * (columnVector[i][0] - 'a') );
  }

  return aDotB;
} 