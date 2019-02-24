//
// Created by mohamed on 17/11/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "interface.h"

int main()
{   int size_of_array,i;
    //clock_t startR, startE, endR, endE;
   // double time_thread_element, time_thread_row;
    char *ptr = "input.txt";
    readMatrix(ptr);
    printf("Matrix A:\n");
    printMatrix(matrixA, rowsA_numb, columnsA_numb);
    printf("--------------------------------------------------------------\n");
    printf("Matrix B:\n");
    printMatrix(matrixB, rowsB_numb, columnsB_numb);
    printf("==============================================================\n");
    printf("OUTPUT OF MATRIX MULTIPLICATION USING THREAD FOR EACH ELEMENT:\n");
   // startE = clock();
    multiplyMatrix__thread_element(0);
  //  endE = clock();
  //  time_thread_element = ((double)(endE - startE)) / CLOCKS_PER_SEC;
 //   printf("Time taken = %lf sec",time_thread_element );

    printf("\n\n");

    printf("OUTPUT OF MATRIX MULTIPLICATION USING THREAD FOR EACH ROW:\n");
  //  startR = clock();
   multiplyMatrix__thread_element(1);
 //  endR = clock();
 //  time_thread_row = ((double)(endR - startR)) / CLOCKS_PER_SEC;
 //  printf("Time taken = %lf sec",time_thread_row);
printf("==============================================================\n");
size_of_array = readArray("inputArray.txt");

printf("Unsorted Array:\n");
//The unsorted array
    for(i=0; i<10; i++)
    {
        printf("%d ",Array[i]);
    }
printf("\n");
 mergeSortByThreads(size_of_array);
  printf("Sorted Array:\n");
 for(i=0; i<10; i++)
 {
     printf("%d ",Array[i]);
 }

    return 0 ;
}