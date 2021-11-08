/**
 * @file life.c
 * @brief Game of Life functions
 * @details Contains initializations, calculation of living and dead cells,
 * matrices, etc.
 * @author Benjamin Wilson
 * @date Fall 2021
 * @todo Idk even know what's happening here yet.
 * @bug none
 */

#include "life.h"

/**
 * Dynamically allocates a matrix/grid into the heap and sets every cell
 * be dead (numerically 0).
 * @param row The number of rows for the matrix
 * @param col The number of columns for the matrix
 * @return Matrix row and column pointers.
 */
unsigned char **init_matrix(int row, int col)
{
    /* for loop counters/iterations */
    int i, j;

    /* Matrix initialization */
    unsigned char **matrix;

    /* Mallocs the number of rows */
    matrix = malloc(row * sizeof(unsigned char *));

    /* Error check the malloc */
    if(!matrix)
    {
        printf("Computer out of memory...\n");
        exit(ENOMEM);
    }

    /* Creates columns for each row */
    for(i = 0; i < row; i++)
    {
        /* Malloc the number of columns in each row */
        matrix[i] = malloc(col * sizeof(unsigned char));

        /* Error check the malloc */
        if(!matrix[i])
        {
            /* Frees all the columns if it fails */
            for(j = 0; j < i; j++)
                free(matrix[j]);
            /* Frees the rows if it fails */
            free(matrix);
            printf("Computer out of memory...\n");
            exit(ENOMEM);
        }
    }

    /* If it does not fail, then each cell is equal to 0 (dead) */
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
            matrix[i][j] = '0';
    }

    return matrix;
}

/**
 * Prints matrix
 * @param row number of rows
 * @param col number of col
 */
void print_matrix(unsigned char **matrix, int row, int col)
{
    int i, j;
    for(i = 0; i < row; i++)
        for(j = 0; j < col; j++)
        {
            if(j < col - 1)
                printf("%d\t", matrix[i][j]);
            else
                printf("%d\n", matrix[i][j]);
        }
}


unsigned char **next_gen(unsigned char **matrix, int row, int col, char edge)
{
    /* Iterations */
    int i, j, y, z;
    /* Neihbor "Alive" counter */
    int count = 0;

    switch(edge)
    {
    /* Hedge First */
    case('h'):
        /* Adds dead cell edges for logic of a tmp matrix */
        unsigned char **tmp = init_matrix(row + 2, col + 2);
        
        /* Places current cell pattern into tmp with dead edges */
        for(i = 1; i < row + 2; i++)
            for(j = 1; j < col + 2; j++)
                tmp[i][j] = matrix[i - 1][j - 1];
        
        /* Iterates through rows and cols of tmp that contain current matrix gen */
        for(i = 1; i < row + 2; i++)
            for(j = 1; j < col + 2; j++)
            {
                /* Iterates through adjacent neighbors and counts number alive */
                for(y = i - 1; y < i + 2; y++)
                    for(z = j - 1; z < j + 2; z++)
                    {
                        /* '1' means alive */
                        if(tmp[y][z] == '1')
                            count++;
                    }
            }


        break;
    }

}


/**
 * frees matrix
 * @param matrix multidimensional array of integers
 * @param row number of rows
 */
void free_matrix(unsigned char **matrix, int row)
{
    int i;
    
    for(i = 0; i < row; i++)
        free(matrix[i]);
    
    free(matrix);
}