/**
 * @file life.h
 * @brief Header file for life.c
 * @details Contains function prototypes, definitions, and #include
 * statements for life.c, and basically stuff for Conway's Game of Life
 * outside of SDL graphics.
 * @author Benjamin Wilson
 * @date Fall 2021
 * @todo I don't know what I'm even doing yet
 * @bug none
 */

#ifndef LIFE_H_
#define LIFE_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* Matrix functions */
unsigned char **init_matrix(int row, int col);
void print_matrix(unsigned char **matrix, int row, int col);
unsigned char **next_gen(unsigned char **matrix, int row, int col, char edge);
void free_matrix(unsigned char **matrix, int row);

#endif
