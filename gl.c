/**
 * @file gl.c
 * @brief Calls to SDL to setup bitmap to draw graphics of Game of Life
 * @details Originally contained calls to sdl_test for help with setup of SDL.
 * Currently contains calls to SDL to setop bitmap to draw graphics of Game
 * of Life. Also, contains exit from program, main function, and initialization
 * for life.c functions.
 * @author Benjamin Wilson
 * @date Fall 2021
 * @todo Life initialization, free() calls for arrays, and function calls.
 * @bug none
 */

#include <stdlib.h>
#include "SDL2/SDL.h" 
#include "sdl.h"
#include "life.h"

int main(int argc, char *argv[])
{
	int width = 640;
	int height = 480;
	int sprite_size = 2; /* either 2, 4, 8, or 16 */
	/*
	int m = -66;
	int n = -10;
	*/
        /* colors are RGB model valid values [0, 255] */
	unsigned char red = 140;
	unsigned char green = 145;
	unsigned char blue = 250;
        struct sdl_info_t sdl_info; /* this is needed to graphically display the game */
        
    /* set up SDL -- works with SDL2 */
	/*
	 * Note to self: don't put this in the while loop, it opens individual
	 * windows, learned the hard way
	 */
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);

	/* your life initialization code here */
	/* Matrix width and height */
	int cell_w = width / sprite_size;
	int cell_h = height / sprite_size;

	/* Initialization of 2 generations of cells */
	unsigned char **gen_A = init_matrix(cell_w, cell_h);
	unsigned char **gen_B = init_matrix(cell_w, cell_h);
	
	/* Genertation counter */
	int gen = 0;

	/* Print matrix in terminal for seeing what they are */
	/**
	 * print_matrix(gen_A, cell_w, cell_h);
	 * printf("\n");
	 * print_matrix(gen_B, cell_w, cell_h);
	 */

    /* Main loop: loop forever. */
	while (1)
	{
		/* your game of life code goes here  */
		/* Renders gen_A if current gen number is divisble by 2 */
		if(gen % 2 == 0)
		{
			/* Render game of life current gen */
			sdl_render_life(&sdl_info, gen_A);

			/* Determine next generation of cells and store in gen_B using hedge */
			gen_B = next_gen(gen_A, cell_w, cell_h, 'h');

			/* Iterate gen number */
			gen++;

		/* Renders gen_B if current gen number is NOT divisible by 2 */
		} else if(gen % 2 != 0)
		{
			/* Render game of life current gen */
			sdl_render_life(&sdl_info, gen_B);

			/* Determine next generation of cells and store in gen_A using hedge */
			gen_A = next_gen(gen_B, cell_w, cell_h, 'h');

			/* Iterate gen number */
			gen++;
		}


		/* change the  modulus value to slow the rendering */
		/*
		if (SDL_GetTicks() % 1 == 0)
			sdl_test(&sdl_info, m, n);
		*/

                 /* Poll for events, and handle the ones we care about. 
                  * You can click the X button to close the window
                  */
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
                        /* If escape is pressed, return (and thus, quit) */
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					free_matrix(gen_A, cell_w);
					free_matrix(gen_B, cell_w);
					return 0;
				}
				break;
			case SDL_QUIT:
				free_matrix(gen_A, cell_w);
				free_matrix(gen_B, cell_w);
				return(0);
			}
		}
	}

	free_matrix(gen_A, cell_w);
	free_matrix(gen_B, cell_w);
	return 0;
}
