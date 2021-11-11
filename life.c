/**
 * @file life.c
 * @brief Game of Life functions
 * @details Contains initializations, calculation of living and dead cells,
 * matrices, etc.
 * @author Benjamin Wilson
 * @date Fall 2021
 * @todo Idk even know what s happening here yet.
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
    unsigned char **matrix = NULL;

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

    /* If it does not fail, then each cell is equal to . (dead) */
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
            matrix[i][j] =  0 ;
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
                printf("%c ", matrix[i][j]);
            else
                printf("%c\n", matrix[i][j]);
        }
    
    printf("\n");
}

/**
 * Calculates next generation of cells using hedge edge
 * @param matrix The current generation of cells that is used to calculate
 * the next generation by Conway s Game of Life.
 * @param row The number of rows in matrix
 * @param col The number of columns in matrix
 * @return The values/pattern of the next generation stored in tmp_mat.
 */
unsigned char **next_gen_hedge(unsigned char **matrix, int row, int col)
{
    /* Iterations and neighbor status tracker */
    int i, j, y, z, count;
    
    /* Initializes tmp matrix */
    unsigned char **tmp = NULL;
    unsigned char **tmp_mat = NULL;


    /* Mallocs space for tmp arrays */
    tmp = init_matrix(row + 2, col + 2);
    tmp_mat = init_matrix(row, col);
    
    /* Places current cell pattern into tmp with dead edges */
    for(i = 1; i < row + 1; i++)
    {
        for(j = 1; j < col + 1; j++)
            tmp[i][j] = matrix[i - 1][j - 1];
    }

    /* Places current cell pattern into tmp_mat */
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
            tmp_mat[i][j] = matrix[i][j];
    }

    /* Used for debugging */
    //print_matrix(tmp_mat, row, col);
    
    /* Iterates through rows and cols of tmp that contain current matrix gen */
    for(i = 1; i < row + 1; i++)
    {
        for(j = 1; j < col + 1; j++)
        {
            /* Resets neighbor count for each cell */
            count = 0;

            /* Iterates through adjacent neighbors and counts number alive */
            for(y = i - 1; y < i + 2; y++)
            {
                for(z = j - 1; z < j + 2; z++)
                {
                    /* Conditions increase live neighbor count */
                    if(tmp[y][z] !=  0 )
                        count++;
                    /* Removes current cell in question from count if alive */
                    if(y == i && z == j && tmp[y][z] !=  0 )
                        count--;
                }
            }

            /* GOL rule 4 */
            if(tmp[i][j] ==  0  && count == 3)
                tmp_mat[i - 1][j - 1] =  1 ;
            
            /*GOL rule 1-3 */
            else if(tmp[i][j] !=  0  && (count < 2 || count > 3))
                tmp_mat[i - 1][j - 1] =  0 ;
        }
    }

    /* Printed for testing of life patterns */
    //print_matrix(tmp_mat, row, col);
    //print_matrix(tmp, row + 2, col + 2);

    /* Frees tmp matrix with hedge dead edges */
    free_matrix(tmp, row + 2);


    return tmp_mat;

    /* Frees tmp matrix */
    free_matrix(tmp_mat, row);
}

/**
 * Calculates next generation of cells using torus edges
 * @param matrix The current generation of cells that is used to calculate
 * the next generation by Conway s Game of Life.
 * @param row The number of rows in matrix
 * @param col The number of columns in matrix
 * @return The values/pattern of the next generation stored in tmp_mat.
 */
unsigned char **next_gen_torus(unsigned char **matrix, int row, int col)
{
    /* Iterations and neighbor status tracker */
    int i, j, y, z, count;
    
    /* Initializes tmp matrix */
    unsigned char **tmp = NULL;
    unsigned char **tmp_mat = NULL;


    /* Mallocs space for tmp arrays */
    tmp = init_matrix(row + 2, col + 2);
    tmp_mat = init_matrix(row, col);
    
    /* Places current cell pattern into tmp with empty dead edges */
    for(i = 1; i < row + 1; i++)
    {
        for(j = 1; j < col + 1; j++)
            tmp[i][j] = matrix[i - 1][j - 1];
    }

    /* Places bottom of cell pattern into top of tmp */
    i = 0;
    for(j = 1; j < col + 1; j++)
        tmp[i][j] = matrix[row - 1][j - 1];
    
    /* Places right column of cell pattern into left column of tmp */
    j = 0;
    for(i = 1; i < row + 1; i++)
        tmp[i][j] = matrix[i - 1][col - 1];

    /* Places top of cell pattern into bottom of tmp */
    i = row + 1;
    for(j = 1; j < col + 1; j++)
        tmp[i][j] = matrix[0][j - 1];
    
    /* Places left column of cell pattern into right column of tmp */
    j = col + 1;
    for(i = 1; i < row + 1; i++)
        tmp[i][j] = matrix[i - 1][0];

    /* Places current cell pattern into tmp_mat */
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
            tmp_mat[i][j] = matrix[i][j];
    }

    /* Used for debugging */
    //print_matrix(tmp_mat, row, col);
    
    /* Iterates through rows and cols of tmp that contain current matrix gen */
    for(i = 1; i < row + 1; i++)
    {
        for(j = 1; j < col + 1; j++)
        {
            /* Resets neighbor count for each cell */
            count = 0;

            /* Iterates through adjacent neighbors and counts number alive */
            for(y = i - 1; y < i + 2; y++)
            {
                for(z = j - 1; z < j + 2; z++)
                {
                    /* Conditions increase live neighbor count */
                    if(tmp[y][z] !=  0 )
                        count++;
                    /* Removes current cell in question from count if alive */
                    if(y == i && z == j && tmp[y][z] !=  0 )
                        count--;
                }
            }

            /* GOL rule 4 */
            if(tmp[i][j] ==  0  && count == 3)
                tmp_mat[i - 1][j - 1] =  1 ;
            
            /*GOL rule 1-3 */
            else if(tmp[i][j] !=  0  && (count < 2 || count > 3))
                tmp_mat[i - 1][j - 1] =  0 ;
        }
    }

    /* Printed for testing of life patterns */
    //print_matrix(tmp_mat, row, col);
    //print_matrix(tmp, row + 2, col + 2);

    /* Frees tmp matrix with hedge dead edges */
    free_matrix(tmp, row + 2);


    return tmp_mat;

    /* Frees tmp matrix */
    free_matrix(tmp_mat, row);
}

/** Calculates next generation based on klein bottle edge
 * @param matrix The current generation of cells that is used to calculate
 * the next generation by Conway s Game of Life.
 * @param row The number of rows in matrix
 * @param col The number of columns in matrix
 * @return The values/pattern of the next generation stored in tmp_mat.
 */
unsigned char **next_gen_klein(unsigned char **matrix, int row, int col)
{
    /* Iterations and neighbor status tracker */
    int i, j, y, z, count;
    
    /* Initializes tmp matrix */
    unsigned char **tmp = NULL;
    unsigned char **tmp_mat = NULL;


    /* Mallocs space for tmp arrays */
    tmp = init_matrix(row + 2, col + 2);
    tmp_mat = init_matrix(row, col);
    
    /* Places current cell pattern into tmp with empty dead edges */
    for(i = 1; i < row + 1; i++)
    {
        for(j = 1; j < col + 1; j++)
            tmp[i][j] = matrix[i - 1][j - 1];
    }

    /* Places bottom of cell pattern into top of tmp */
    i = 0;
    for(j = 1; j < col + 1; j++)
        tmp[i][j] = matrix[row - 1][j - 1];

    /* Places top of cell pattern into bottom of tmp */
    i = row + 1;
    for(j = 1; j < col + 1; j++)
        tmp[i][j] = matrix[0][j - 1];

    /* Places lower right of cell pattern into top left column of tmp */
    j = 0;
    y = row - 1;
    z = col - 1;
    for(i = 1; i < (row + 1) / 2; i++)
        tmp[i][j] = matrix[y--][z];
    
    /* Places upper right of cell pattern into top left column of tmp */
    /* j, y, and z are currently at correct indexes of matrix array */
    for(i = (row + 1) / 2; i < row + 1; i++)
        tmp[i][j] = matrix[y--][z];
    
    /* Places lower left of cell pattern into top right column of tmp */
    j = col + 1;
    y = row - 1;
    z = 0;
    for(i = 1; i < (row + 1) / 2; i++)
        tmp[i][j] = matrix[y--][z];
    
    /* Places upper left of cell pattern into top right column of tmp */
    /* j, y, and z are currently at correct indexes of matrix array */
    for(i = (row + 1) / 2; i < row + 1; i++)
        tmp[i][j] = matrix[y--][z];

    /* Places current cell pattern into tmp_mat */
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
            tmp_mat[i][j] = matrix[i][j];
    }

    /* Used for debugging */
    //print_matrix(tmp_mat, row, col);
    
    /* Iterates through rows and cols of tmp that contain current matrix gen */
    for(i = 1; i < row + 1; i++)
    {
        for(j = 1; j < col + 1; j++)
        {
            /* Resets neighbor count for each cell */
            count = 0;

            /* Iterates through adjacent neighbors and counts number alive */
            for(y = i - 1; y < i + 2; y++)
            {
                for(z = j - 1; z < j + 2; z++)
                {
                    /* Conditions increase live neighbor count */
                    if(tmp[y][z] !=  0 )
                        count++;
                    /* Removes current cell in question from count if alive */
                    if(y == i && z == j && tmp[y][z] !=  0 )
                        count--;
                }
            }

            /* GOL rule 4 */
            if(tmp[i][j] ==  0  && count == 3)
                tmp_mat[i - 1][j - 1] =  1 ;
            
            /*GOL rule 1-3 */
            else if(tmp[i][j] !=  0  && (count < 2 || count > 3))
                tmp_mat[i - 1][j - 1] =  0 ;
        }
    }

    /* Printed for testing of life patterns */
    //print_matrix(tmp_mat, row, col);
    //print_matrix(tmp, row + 2, col + 2);

    /* Frees tmp matrix with hedge dead edges */
    free_matrix(tmp, row + 2);


    return tmp_mat;

    /* Frees tmp matrix */
    free_matrix(tmp_mat, row);
}


unsigned char **pattern(FILE *fp, int row, int col, int x_o, int y_o)
{
    unsigned char **tmp = init_matrix(row, col);

    char in[LEN];
    char cord[LEN];

    /* Line Number being read */
    int count = 0;

    /* Cords for pattern input and iterations */
    int i, j, x, y;

    while(fgets(in, LEN, fp))
    {

        if(count != 0)
        {
            /* Assins x-cord to x */
            for(i = 0; in[i] != ' '; i++)
                cord[i] = in[i];
            /* Separates x cord for atoi and goes to y-cord */
            cord[i++] = ' ';
            x = atoi(cord);

            /* Assigns y cord to y */
            j = 0;
            for(i = i; in[i] != '\n'; i++)
                cord[j++] = in[i];
            /* Separates new cord from old numbers of reused char array */
            cord[j] = ' ';
            y = atoi(cord);

            x += x_o;
            y += y_o;

            /* Checks that cords can't go out of range */
            if(x < (-1) * (col - 1))
                x += (col - 1);
            else if(x > col - 1)
                x -= (col - 1);
            
            if(y < (-1) * (row - 1))
                y += (row - 1);
            else if(y > row - 1)
                y -= (row - 1);            

            /* Places live cell into tmp based on x, y cords */
            /* Prints for debugging */
            //printf("%d, %d\n", x, y);
            if(x >= 0 && y >= 0)
                tmp[y][x] = 1;
            else if(x >= 0 && y < 0)
                tmp[row + y][x] = 1;
            else if(x < 0 && y >= 0)
                tmp[y][col + x] = 1;
            else if(x < 0 && y < 0)
                tmp[row + y][col + x] = 1;
        }

        count++;
    }

    printf("Count: %d\n", count);

    /* Used for debugging */
    print_matrix(tmp, row, col);

    return tmp;

    free_matrix(tmp, row);
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

    matrix = NULL;
}
