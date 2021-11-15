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
	/* Screen width and height and spite size all in pixels */
	int width = 1280;
	int height = 720;
	int sprite_size = 8; /* either 2, 4, 8, or 16 */
	
	/*
	 * int m = -66;
	 * int n = -10;
	 */
    
	/* colors are RGB model valid values [0, 255] */
	unsigned char red = 140;
	unsigned char green = 145;
	unsigned char blue = 0;
        struct sdl_info_t sdl_info; /* this is needed to graphically display the game */
	
	/* Command line arguments and additional values needed for life.c functions */
	int c; /* Used for command line arguments */
	FILE *fp = NULL; /* file pattern */
	int x_o = 45; /* Initial x-cord of pattern */
	int y_o = 45; /* Initial y-cord of pattern */
	char edge = 't'; /* Edge behavior of simulation */
	int cmp; /* Used for command line arguments/comparisons */
	char cord[LEN]; /* Used for holding temp cord values in command line arguments */
	int argtmp = -1; /* Integer storage of optarg */
		int i, j; /* Iteration variable */
        
    /* set up SDL -- works with SDL2 */
	/*
	 * Note to self: don't put this in the while loop, it opens individual
	 * windows, learned the hard way
	 */

	/* Command line stuff here */
	if(argc == 1) 
	{
		/* Prints out usage */
		printf("Usage: life -w width -h height -r red -g green -b blue -s size -f filename -o coordinates -e edge\n");
		exit(EXIT_FAILURE);
	}

	/* Cycles through command line options */
	while((c = getopt(argc, argv, ":w:h:r:g:b:s:f:o:e:")) != -1)
	{
		switch(c) {
		
		/* Screen width argument */
		case 'w':
			/* Creates tmp int */
			argtmp = atoi(optarg);

			/* Error Checks and sets width if valid */
			if(argtmp > 0)
				width = argtmp;
			else
			{
				printf("%s: argument to option -w failed. Window width must be larger than 0.\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		
		/* Screen height argument */
		case 'h':
			/* Creates tmp int */
			argtmp = atoi(optarg);

			/* Error checks and sets height if valid */
			if(argtmp > 0)
				height = argtmp;
			else
			{
				printf("%s: argument to option -h has failed. Window height must be larger than 0.\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		
		/* Red RGB value */
		case 'r':
			/* Creates tmp int */
			argtmp = atoi(optarg);

			/* Error checks and sets red value if valid */
			if(argtmp > -1 && argtmp < 256)
				red = argtmp;
			else
			{
				printf("%s: argument to option -r has failed. Argument must follow rgb standards (0-255).\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		
		/* Green RGB value */
		case 'g':
			/* Creates tmp int */
			argtmp = atoi(optarg);

			/* Error checks and sets green value if valid */
			if(argtmp > -1 && argtmp < 256)
				green = argtmp;
			else
			{
				printf("%s: argument to option -g has failed. Argument must follow rgb standards (0-255).\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		
		/* Blue RGB value */
		case 'b':
			/* Creates tmp int */
			argtmp = atoi(optarg);

			/* Error checks and sets blue value if valid */
			if(argtmp > -1 && argtmp < 256)
				blue = argtmp;
			else
			{
				printf("%s: argument to option -b has failed. Argument must follow rgb standards (0-255).\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		
		/* Sprite size */
		case 's':
			/* Creates tmp int */
			argtmp = atoi(optarg);

			/* Error checks and sets sprite size if valid */
			if(argtmp == 2 || argtmp == 4 || argtmp == 8 || argtmp == 16)
			{
				sprite_size = argtmp;
			} else 
			{
				printf("%s: argument to option -s has failed. Sprite size must be 2, 4, 8, or 16.\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		
		/* Filename or path */
		case 'f':
			/* Opens the 1.06 life file */
			fp = fopen(optarg, "r");

			/* Exits if improper file format */
			if(fp == NULL)
			{
				printf("%s: argument to option '-f' failed. Filename must contain file 1.06 format.\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		
		/* Initial x,y coordinates of initial pattern */
		case 'o':
			/* Sets tmp int and first assigns x_o value */
			argtmp = atoi(optarg);
				x_o = argtmp;

			/* Finds y cord using comma and error checks */
			for(i = 0; *(optarg + i) != '\0'; i++)
			{
				if(*(optarg + i) == ',')
				{
					i++;
					if(*(optarg + i) == '\0')
					{
						printf("%s: argument to option '-o' failed. Coordinate must contain no spaces and be in format (x,y).\n", argv[0]);
						exit(EXIT_FAILURE);
					}

					/* Places y-cord into cord[] */
					for(j = 0; *(optarg + i) != '\0'; j++)
					{
						cord[j] = *(optarg + i);
						i++;
					}
					
					cord[j] = '\0';
					
					/* Creates tmp int from cord and assigns y_o value */
					argtmp = atoi(cord);
						y_o = argtmp;

					/* Breaks out of for loop */
					break;
				}
			}
			
			break;
		
		/* Edge behavior */
		case 'e':
			/* Assigns e to hedge behavior if argument matches */
			cmp = strncmp(optarg, "hedge", 6);
			if(cmp == 0)
				edge = 'h';
			else
			{
				/* Assigns e to torus behavior if argument matches */
				cmp = strncmp(optarg, "torus", 6);
				if(cmp == 0)
					edge = 't';
				else
				{
					/* Assigns e to klein behavior if argument matches */
					cmp = strncmp(optarg, "klein", 6);
					if(cmp == 0)
						edge = 'k';
					else
						edge = '0';
				}
			}
			
			/* Error checks */
			if(edge != 'h' && edge != 't' && edge != 'k')
			{
				printf("%s: argument to option '-e' failed. Edge must be hedge, torus, or klein.\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		
		/* Help option for figuring out argument and command format */
		case 'H':
			printf("Usage: life [-w width -h height -r red -g green -b blue -s sprite] -f filename -o coordinates -e edge\n");
			printf("w: Screen Width of simulation in pixels. Must be larger than 0. Default of 1280p\n");
			printf("h: Screen height of simlation in pixels. Must be larger than 0. Default of 720p\n");
			printf("r: RGB value (0-255) of red coloring of cells. Default of 140\n");
			printf("g: RGB value (0-255) of green coloring of cells. Default of 145\n");
			printf("b: RGB value (0-255) of blue coloring of cells. Default of 0\n");
			printf("s: Size if the sprite. Valid values are 2, 4, 8, or 16 only. Must be an integer. Default of 8p\n");
			printf("f: Filename and path of initial life pattern in file format 1.06. Default glider_106.lif\n");
			printf("o: Initial (x,y) cooradinates of the initial pattern on the screen in pixels without spaces. Default of 45,45\n");
			printf("e: Edge of life simulation. Valid options are \"hedge\", \"torus\", or \"klein\". Default of torus\n");
			exit(EXIT_SUCCESS);
			break;
		
		/* No argument passed */
		case ':':
			printf("%s: option '-%c' requires an argument.\n", argv[0], optopt);
			exit(EXIT_FAILURE);
		
		/* Default case */
		case '?':
		default:
			printf("Illegal option '-%c' ignored.\n", optopt);
			printf("Usage: life [-w width -h height -r red -g green -b blue -s sprite] -f filename -o coordinates -e edge\n");
			break;
		}
	}

	/* SDL initialization */
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

	/* Default file pattern in case no command is passed */
	if(fp == NULL)
		fp = fopen("./patterns/glider_106.lif", "r");

	/* Initial pattern reading from text files */
	gen_A = pattern(fp, cell_w, cell_h, x_o, y_o);

	/* Closes text file */
	fclose(fp);

	/* Print matrix in terminal for seeing what they are for debugging */
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
			if (SDL_GetTicks() % 50 == 0)
			{
				/* Render game of life current gen */
				sdl_render_life(&sdl_info, gen_A);

				/* Determine next generation of cells and store in gen_B using correct edge behavior */
				if(edge == 'h')
					gen_B = next_gen_hedge(gen_A, cell_w, cell_h);
				else if(edge == 't')
					gen_B = next_gen_torus(gen_A, cell_w, cell_h);
				else if(edge == 'k')
					gen_B = next_gen_klein(gen_A, cell_w, cell_h);

				/* Iterate gen number */
				gen++;
			}

		/* Renders gen_B if current gen number is NOT divisible by 2 */
		} else if(gen % 2 != 0)
		{
			if (SDL_GetTicks() % 50 == 0)
			{
				/* Render game of life current gen */
				sdl_render_life(&sdl_info, gen_B);

				/* Determine next generation of cells and store in gen_A using correct edge behavior */
				if(edge == 'h')
					gen_A = next_gen_hedge(gen_B, cell_w, cell_h);
				else if(edge == 't')
					gen_A = next_gen_torus(gen_B, cell_w, cell_h);
				else if(edge == 'k')
					gen_A = next_gen_klein(gen_B, cell_w, cell_h);

				/* Iterate gen number */
				gen++;
			}
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
					/* Free heap */
					free_matrix(gen_A, cell_w);
					free_matrix(gen_B, cell_w);
					return 0;
				}
				break;
			case SDL_QUIT:
				/* Free heap */
				free_matrix(gen_A, cell_w);
				free_matrix(gen_B, cell_w);
				return(0);
			}
		}
	}

	/* Free heap */
	free_matrix(gen_A, cell_w);
	free_matrix(gen_B, cell_w);
	return 0;
}
