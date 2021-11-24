/**
 * @file life.h
 * @brief Header file for life.c
 * @author Benjamin Wilson
 * @date Fall 2021
 */

#ifndef LIFE_H_
#define LIFE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define LEN 256

unsigned char **init_matrix(int row, int col);
void print_matrix(unsigned char **matrix, int row, int col);
unsigned char **next_gen_hedge(unsigned char **matrix, int row, int col);
unsigned char **next_gen_torus(unsigned char **matrix, int row, int col);
unsigned char **next_gen_klein(unsigned char **matrix, int row, int col);
unsigned char **pattern_106(FILE *fp, int row, int col, int x_o, int y_o);
unsigned char **pattern_105(FILE *fp, int row, int col, int x_o, int y_o);
void free_matrix(unsigned char **matrix, int row);

#endif
