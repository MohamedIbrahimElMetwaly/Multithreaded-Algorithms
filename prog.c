//
// Created by mohamed on 17/11/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "interface.h"

int readArray(char* ptr)
{
    int sizeOfArray;
    int index;
    FILE* fptr = fopen(ptr, "r");
    if(!fptr)
    {
        printf("ERROR: no such file called %d", *ptr);
    }
    fscanf(fptr,"%d\n",&sizeOfArray);

    Array = malloc(sizeOfArray * sizeof(int));

    for(index=0; index<sizeOfArray; index++)
    {
        fscanf(fptr,"%d",&Array[index]);
        getc(fptr);
    }
    return sizeOfArray;
}

void readMatrix(char* ptr)
{

    int i,r,c;

    FILE *fptr = fopen(ptr,"r");
    if(!fptr)
    {
        printf("Error: no such file !\n");
    }

    fscanf(fptr, "%d %d\n", &rowsA_numb, &columnsA_numb);
    matrixA = malloc(rowsA_numb * sizeof(int *));

    for(i = 0; i<rowsA_numb; i++)
    {
        matrixA[i] = malloc(columnsA_numb * sizeof(int));
    }

    for(r=0; r<rowsA_numb; r++)
    {
        for(c=0; c<columnsA_numb; c++)
        {
            fscanf(fptr,"%d", &matrixA[r][c]);
            getc(fptr);
        }

    }

    fscanf(fptr, "%d %d\n", &rowsB_numb, &columnsB_numb);
    matrixB = malloc(rowsB_numb * sizeof(int *));

    for(i =0; i<rowsB_numb; i++)
    {
        matrixB[i] = malloc(columnsB_numb * sizeof(int));
    }

    for(r=0; r<rowsB_numb; r++)
    {
        for(c=0; c<columnsB_numb; c++)
        {
            fscanf(fptr, "%d", &matrixB[r][c]);
            getc(fptr);
        }
    }

}

//Struct to carry the coordinates of the result element
typedef  struct positionOfElement
{
    int row;
    int column;
}positionOfElement;

void  multiplyMatrix__thread_element(int thread_e0_r1)
{
    int i,j;
   // positionOfElement *data = malloc(sizeof(positionOfElement));

    //Choose to make thread for each element
    if(thread_e0_r1 == 0)
    {
        double time_thread_element;
        clock_t startE, endE;
        startE = clock();
        pthread_t threadMatrix_element[rowsA_numb][columnsB_numb];
        //allocate space for matrix elements by threads
        matrixR1 = malloc(rowsA_numb * sizeof(int *));
        for (i = 0; i < rowsA_numb; i++) {
            matrixR1[i] = malloc(columnsB_numb * sizeof(int));
        }

        for (i = 0; i < rowsA_numb; i++) {
            for (j = 0; j < columnsB_numb; j++) {
                //Create a new thread and passing the parameters
                positionOfElement *data = malloc(sizeof(positionOfElement));
                data->row = i;
                data->column = j;
                pthread_attr_t thread_attr;
                //initialize default attributes
                pthread_attr_init(&thread_attr);
                //Create thread
               pthread_create(&threadMatrix_element[i][j], &thread_attr, thread_element, data);

            }
        }
        for (i = 0; i < rowsA_numb; i++) {
            for (j = 0; j < columnsB_numb; j++)
            {
                pthread_join(threadMatrix_element[i][j], NULL);
            }
        }
        endE = clock();
        time_thread_element = ((double)(endE - startE)) / CLOCKS_PER_SEC;
        writeMatrix(matrixR1, rowsA_numb, columnsB_numb, time_thread_element);
                printMatrix(matrixR1, rowsA_numb, columnsB_numb);
                printf("TIME TAKEN: %lf sec \n",time_thread_element);
        free(matrixR1);
    }
    //Choose to make thread for each row
    else if (thread_e0_r1 == 1)
    {
        double time_thread_row;
        clock_t startR, endR;
        startR = clock();
        pthread_t threadMatrix_row[rowsA_numb];
        matrixR2 = malloc(rowsA_numb * sizeof(int*));
        for(i=0; i<rowsA_numb; i++)
        {
            matrixR2[i] = malloc(columnsB_numb * sizeof(int));
        }
        for(i=0; i<rowsA_numb; i++)
        {
            //Create a new thread and passing the parameters
            positionOfElement *data = malloc(sizeof(positionOfElement));
                data->row = i;
                data->column = -1;

            //initialize default attributes
                pthread_attr_t thread_attr;
                pthread_attr_init(&thread_attr);

                //Create thread
            pthread_create(&threadMatrix_row[i], &thread_attr, thread_row, data);

        }
           for(i =0; i<rowsA_numb; i++)
        {
            pthread_join(threadMatrix_row[i],NULL);
        }
        endR = clock();
        time_thread_row = ((double)(endR - startR)) / CLOCKS_PER_SEC;
        writeMatrix(matrixR2, rowsA_numb, columnsB_numb, time_thread_row);
        printMatrix(matrixR2, rowsA_numb, columnsB_numb);
        printf("TIME TAKEN: %lf sec \n",time_thread_row);
        free(matrixR2);
    }

}


// thread multiplication row by column to get elements
void* thread_element(void* parm)
{
    positionOfElement* data = parm;
    int commonNumber, sum=0;
    for(commonNumber=0; commonNumber<columnsA_numb; commonNumber++)
    {
        sum+= matrixA[data->row][commonNumber] * matrixB[commonNumber][data->column];
    }
    matrixR1[data->row][data->column] = sum;
   pthread_exit(0);
}


void* thread_row(void* parm)
{
    positionOfElement* data = parm;

    int commonNumber, sum =0, col;

    for(col=0; col<columnsB_numb; col++)
    {   sum = 0;

        for(commonNumber=0; commonNumber<columnsA_numb; commonNumber++)
        {

            sum+= matrixA[data->row][commonNumber] * matrixB[commonNumber][col];
        }
        matrixR2[data->row][col] = sum;

    }
   pthread_exit(0);

}


void printMatrix(int** matrix, int row, int column)
{
    int i,j;
    for(i=0; i<row; i++)
    {
        for(j=0; j<column; j++)
        {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}

void writeMatrix(int** matrix, int rows, int columns, double timeTaken)
{   static f=0;
    int i,j;
    if(f==0)
    {
        FILE* fptrW = fopen("output.txt","w");
        for(i=0; i<rows; i++)
        {
            for(j=0; j<columns; j++)
            {
                fprintf(fptrW,"%d ",matrix[i][j]);
            }
            fprintf(fptrW,"\n");
        }
        fprintf(fptrW,"Time Taken = %lf sec\n",timeTaken);
        f = 1;
        fclose(fptrW);
    }
    else
    {
        FILE* fptrA = fopen("output.txt","a");
        for(i=0; i<rows; i++)
        {
            for(j=0; j<columns; j++)
            {
                fprintf(fptrA,"%d ",matrix[i][j]);
            }
            fprintf(fptrA,"\n");
        }
        fprintf(fptrA,"Time Taken = %lf sec\n",timeTaken);
        f = 1;
        fclose(fptrA);
    }
}




typedef  struct
{
    int start;
    int end;
}array_info_t;

void mergeSortByThreads( int size_of_array)
{
    array_info_t* array_info;
    array_info = malloc(sizeof(array_info_t));
    array_info->start = 0;
    array_info->end = size_of_array-1;

    pthread_t mainThread;
    pthread_attr_t thread_att;
    pthread_attr_init(&thread_att);
    pthread_create(&mainThread,&thread_att,mergeSort,array_info);
    pthread_join(mainThread, NULL);
}

void* mergeSort(void* parm)
{
    array_info_t* array_info = parm;
    if((array_info->end) > (array_info->start))
    {
        //initialize the parameters of both threads
        array_info_t* left_info;
        array_info_t* right_info;
        left_info = malloc(sizeof(array_info_t));
        right_info = malloc(sizeof(array_info_t));
        left_info->start = array_info->start;
        left_info->end = ((array_info->end) + (array_info->start))/2;
        right_info->start = (((array_info->end) + (array_info->start))/2)+1;
        right_info->end = array_info->end;

        //Create the 2 threads
        pthread_t left_thread, right_thread;
        pthread_attr_t threadAttr;
        pthread_attr_init(&threadAttr);
        pthread_create(&left_thread, &threadAttr, mergeSort, left_info);
        pthread_create(&right_thread, &threadAttr, mergeSort, right_info);

        //Wait for the two threads to finish
        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

    }
    merge(array_info->start,array_info->end);

}

void merge(int first_index,int last_index)
{
    int temp[last_index-first_index+1];
    int index_subArray1=first_index,index_subArray2=((first_index+last_index)/2)+1,index_temp=0,flag=0,f=first_index;

    //Check if only one elements exit
    if(first_index==last_index) {
        pthread_exit(0);
    }
    else
    {   //Fill the temp sorted array
        while(index_temp<last_index-first_index+1)
        {
            //if subarray2 is empty
            if(index_subArray2>last_index)
            {
                //Add the remaining elements of subarray1 in temp sorted array
                while(index_subArray1<=((first_index+last_index)/2))
                {
                    temp[index_temp]=Array[index_subArray1];
                    index_subArray1++;
                    index_temp++;
                }
                flag=1;
            }
            //if the subarray1 is empty
            else if(index_subArray1>((first_index+last_index)/2))
            {
                //Add the remaining elements of subarray2 in temp sorted array
                while(index_subArray2<=last_index)
                {
                    temp[index_temp]=Array[index_subArray2];
                    index_subArray2++;
                    index_temp++;
                }
                flag=1;
            }
            // if flag = 1 then both of subarrays are empty
            if(flag)
                break;

            //Compare: if element in subarray1 is greater add element of subarray2 in temp sorted array
            if(Array[index_subArray1]>Array[index_subArray2])
            {
                temp[index_temp]=Array[index_subArray2];
                index_subArray2++;
                index_temp++;
            }
            //Compare: if element in subarray2 is greater add element of subarray1 in temp sorted array
            else
            {
                temp[index_temp]=Array[index_subArray1];
                index_temp++;
                index_subArray1++;
            }

        }

        //Copy the sorted temp array into orignal array
        for(index_temp=0; index_temp<last_index-first_index+1; index_temp++)
        {
            Array[f]=temp[index_temp];
            f++;
        }

    }
    pthread_exit(0);
}