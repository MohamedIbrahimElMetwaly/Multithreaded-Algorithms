//
// Created by mohamed on 17/11/18.
//

void printMatrix(int** matrix, int row, int column);

//Read the size of matrices and value of matrices from file and assign them to 2D arrays
void readMatrix(char* ptr);

void writeMatrix(int** matrix, int rows, int columns, double timeTaken);

/*the paramter thread_e0_r1 is used specifies which type of thread is used
*if(thread_e0_r1 == 0)  thread for element
 * if(thread_e0_r1 == 1) thread for row
create thread
 */
void multiplyMatrix__thread_element(int thread_e0_r1);

//Multiply row by column to get element and put it in its coordinates in the out matrix
void* thread_element(void* parm);

//Multiply row of the first matrix with all the columns in the second matrix to get all elements of the output matrix
void* thread_row(void* parm);

//Divide the array by using thread for each divison
void* mergeSort(void* parm);

//Make the merge between subarrays
void merge(int i,int j);

//Read the array required to be sorted from file
int readArray(char* ptr);

//Function Create the main thread that mergeSort
void mergeSortByThreads(int size_of_array);

int **matrixA;
int **matrixB;
int **matrixR1;
int **matrixR2;
int *Array;
int rowsA_numb;
int columnsA_numb;
int rowsB_numb;
int columnsB_numb;
