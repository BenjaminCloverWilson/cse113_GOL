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
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define LEN 256

/* Matrix functions */
unsigned char **init_matrix(int row, int col);
void print_matrix(unsigned char **matrix, int row, int col);
unsigned char **next_gen_hedge(unsigned char **matrix, int row, int col);
unsigned char **next_gen_torus(unsigned char **matrix, int row, int col);
unsigned char **next_gen_klein(unsigned char **matrix, int row, int col);
unsigned char **pattern(FILE *fp, int row, int col, int x_o, int y_o);
void free_matrix(unsigned char **matrix, int row);

#endif
