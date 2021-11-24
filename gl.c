/**
 * @file gl.c
 * @brief Calls to SDL to setup bitmap to draw graphics of Game of Life
 * @details Originally contained calls to sdl_test for help with setup of SDL.
 * Currently contains calls to SDL to setup bitmap to draw graphics of Game
 * of Life. Also, contains exit from program, main function, and initialization
 * for life.c functions.
 * @author Benjamin Wilson
 * @date Fall 2021
 * @todo Extra Credit for multiple patterns
 * @bug oh, I wouldn't doubt it, but it seems fine to me. A little wierd at 0,0
 * @remarks Works for both 1.06 and 1.05 file format for extra credit.
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
	
	/**
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
	FILE *fp_106 = NULL; /* file pattern for 106.lif */
	FILE *fp_105 = NULL; /* file pattern for 105.lif */
	int x_o_106 = 30; /* Initial x-cord of pattern for 106.lif */
	int y_o_106 = 30; /* Initial y-cord of pattern for 106.lif */
	int x_o_105 = 0; /* Initial x-cord of pattern for 105.lif based on center of window */
	int y_o_105 = 0; /* Initial y-cord of pattern for 105.lif based on center of window */
	char edge = 't'; /* Edge behavior of simulation */
	int cmp; /* Used for command line arguments/comparisons */
	char *find = NULL;
	int call_106 = 0;
	int call_105 = 0;
	char cord[LEN]; /* Used for holding temp cord values in command line arguments */
	int argtmp = -1; /* Integer storage of optarg */
		int i, j; /* Iteration variables */
        
    /* set up SDL -- works with SDL2 */
	/**
	 * Note to self: don't put this in the while loop, it opens individual
	 * windows, learned the hard way
	 */

	/* Usage information */
	if(argc == 1) 
	{
		/* Prints out usage */
		printf("Usage: life -w width -h height -r red -g green -b blue -s size -f filename_106 -o coordinates_106 -Q filename_105 -q filename_105 -e edge\n");
	}

	/* Cycles through command line options */
	while((c = getopt(argc, argv, ":w:h:r:g:b:s:f:o:Q:q:e:H")) != -1)
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
			}
			break;
		
		/* Sprite size */
		case 's':
			/* Error checks for decimals */
			for(i = 0; optarg[i] != '\0'; i++)
				if(optarg[i] == '.')
					cmp = 1;
			
			/* Creates tmp int or exits argument after error check */
			if(cmp == 0)
				argtmp = atoi(optarg);
			else
			{
				printf("%s: argument to option -s has failed. Sprite size must be 2, 4, 8, or 16. Only integers.\n", argv[0]);
				break;
			}

			/* Error checks and sets sprite size if valid */
			if(argtmp == 2 || argtmp == 4 || argtmp == 8 || argtmp == 16)
			{
				sprite_size = argtmp;
			} else 
			{
				printf("%s: argument to option -s has failed. Sprite size must be 2, 4, 8, or 16.\n", argv[0]);
			}
			break;
		
		/* Filename or path for 1.06 files */
		case 'f':
			/* Documents that -f was argued */
			call_106 = 1;

			/* Error checks for proper file format */
			find = strstr(optarg, "105");

			/* Opens the 1.06 life file */
			if(find == NULL && fp_105 == NULL)
				fp_106 = fopen(optarg, "r");

			/* Defaults if improper file format */
			if(fp_106 == NULL)
				printf("%s: argument to option '-f' failed. Filename must contain file 1.06 format and no argument to '-Q' can be made\n", argv[0]);

			break;

		/* Initial x,y coordinates of 1.06 initial pattern */
		case 'o':
			/* Sets tmp int and first assigns x_o_106 value */
			argtmp = atoi(optarg);
				x_o_106 = argtmp;

			/* Finds y cord using comma and error checks */
			for(i = 0; *(optarg + i) != '\0'; i++)
			{
				if(*(optarg + i) == ',')
				{
					i++;
					if(*(optarg + i) == '\0')
					{
						printf("%s: argument to option '-o' failed. Coordinate must contain no spaces and be in format (x,y).\n", argv[0]);
						break;
					}

					/* Places y-cord into cord[] */
					for(j = 0; *(optarg + i) != '\0'; j++)
					{
						cord[j] = *(optarg + i);
						i++;
					}
					
					cord[j] = '\0';
					
					/* Creates tmp int from cord and assigns y_o_106 value */
					argtmp = atoi(cord);
						y_o_106 = argtmp;

					/* Breaks out of for loop */
					break;
				}
			}	
			break;
		
		/* Filename or path of a 1.05 file */
		case 'Q':
			/* Checks that 1.06 was not called or if -f failed while -Q was also called */
			if(call_106 != 1 || fp_106 == NULL)
				call_105 = 1;

			/* Tests for correct file format */
			find = strstr(optarg, "106");

			/* Opens the 1.05 life file if correct file format*/
			if(find == NULL && fp_106 == NULL)
				fp_105 = fopen(optarg, "r");

			/* Defaults if improper file format */
			if(fp_105 == NULL)
				printf("%s: argument to option '-Q' failed. Filename must contain file 1.05 format and '-f' must not be argued or must have failed.\n", argv[0]);

			break;
		
		/* Initial x,y coordinates of 1.05 initial pattern */
		case 'q':
			/* Sets tmp int and first assigns x_o_105 value */
			argtmp = atoi(optarg);
				x_o_105 = argtmp;

			/* Finds y cord using comma and error checks */
			for(i = 0; *(optarg + i) != '\0'; i++)
			{
				if(*(optarg + i) == ',')
				{
					i++;
					if(*(optarg + i) == '\0')
					{
						printf("%s: argument to option '-q' failed. Coordinate must contain no spaces and be in format (x,y).\n", argv[0]);
						break;
					}

					/* Places y-cord into cord[] */
					for(j = 0; *(optarg + i) != '\0'; j++)
					{
						cord[j] = *(optarg + i);
						i++;
					}
					
					cord[j] = '\0';
					
					/* Creates tmp int from cord and assigns y_o_106 value */
					argtmp = atoi(cord);
						y_o_105 = argtmp;

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
			}
			break;
		
		/* Help option for figuring out argument and command format */
		case 'H':
			printf("Usage: life -w width -h height -r red -g green -b blue -s size -f filename_106 -o coordinates_106 -Q filename_105 -q filename_105 -e edge\n");
			printf("w: Screen Width of simulation in pixels. Must be larger than 0. Default of 1280p\n");
			printf("h: Screen height of simlation in pixels. Must be larger than 0. Default of 720p\n");
			printf("r: RGB value (0-255) of red coloring of cells. Default of 140\n");
			printf("g: RGB value (0-255) of green coloring of cells. Default of 145\n");
			printf("b: RGB value (0-255) of blue coloring of cells. Default of 0\n");
			printf("s: Size of the sprite. Valid values are 2, 4, 8, or 16 only. Must be an integer. Default of 8p\n");
			printf("f: Filename and path of initial life pattern in file format 1.06. Default ./patterns/glider_106.lif\n");
			printf("o: Initial (x,y) cooradinates of the initial pattern for 1.06 files on the screen in pixels without spaces and with wrapping. Default of 30,30 with respect to the top left of the window\n");
			printf("Q: Filename and path of initial life pattern in file format 1.05. Default ./patterns/glider_105.lif\n");
			printf("q: Initial (x,y) cooradinates of the initial pattern for 1.05 files on the screen in pixels without spaces and with wrapping. Default of 0,0 with respect to the center of the window\n");
			printf("e: Edge of life simulation. Valid options are \"hedge\", \"torus\", or \"klein\". Default of torus\n");
			printf("NOTE: Invalid arguments are noted in the terminal and the program is run with the default value instead\n");
			printf("NOTE 2: Default of filename is based on provided path to created patterns folder, ./patterns/filename.\n");
			printf("The filename must be provided via a relative file path if the .lif file is not in the same directory as gl.c\n");
			printf("Note 3: Leaving an argument blank, then attempting a second argument results in a default value\n");
			printf("Note 4: If both '-f' and '-Q' are argued, -f will be prioritized. '-Q' will only be run if '-f' is not called or if '-f' fails and '-Q' was argued as well\n");
			exit(EXIT_SUCCESS);
			break;
		
		/* No argument passed */
		case ':':
			printf("%s: option '-%c' requires an argument.\n", argv[0], optopt);
			break;
		
		/* Default case */
		case '?':
		default:
			printf("Illegal option '-%c' ignored.\n", optopt);
			printf("Usage: life -w width -h height -r red -g green -b blue -s size -f filename_106 -o coordinates_106 -Q filename_105 -q filename_105 -e edge\n");
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

	/* Matrix pointer for frees */
	unsigned char **track = gen_A;

	/* Genertation counter */
	int gen = 0;

	/* Default file pattern in case no command is passed or 106 fails after being argued */
	if((fp_106 == NULL && fp_105 == NULL && find == NULL && call_105 == 0 && call_106 == 0) || (call_106 == 1 && fp_106 == NULL))
	{
		/* Opens default pattern file */
		fp_106 = fopen("./patterns/glider_106.lif", "r");
		
		/* Initial pattern reading from text files */
		gen_A = pattern_106(fp_106, cell_w, cell_h, x_o_106, y_o_106);
		
		/* Closes text file */
		fclose(fp_106);
	
	/* Opens file pattern argued if -f is called in command line */
	} else if(call_106 == 1 && fp_106 != NULL)
	{
		/* Initial pattern reading from text files */
		gen_A = pattern_106(fp_106, cell_w, cell_h, x_o_106, y_o_106);
		
		/* Closes text file */
		fclose(fp_106);
	
	/* Default file pattern in case -Q command is passed but fails for some reason */
	} else if(fp_105 == NULL && call_105 == 1)
	{
		/* Opens default pattern file */
		fp_105 = fopen("./patterns/glider_105.lif", "r");

		/* Initial pattern reading from text files */
		gen_A = pattern_105(fp_105, cell_w, cell_h, x_o_105, y_o_105);
		
		/* Closes text file */
		fclose(fp_105);
	
	/* Opens file pattern argued if -Q is called in command line */
	} else if(call_105 == 1 && fp_105 != NULL)
	{
		/* Initial pattern reading from text files */
		gen_A = pattern_105(fp_105, cell_w, cell_h, x_o_105, y_o_105);
		
		/* Closes text file */
		fclose(fp_105);
	}


	/* Frees memory allocated before pattern was called and sets new tracker */
	free_matrix(track, cell_w);
	track = gen_B;

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
			/* Delays game rendering */
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

				/* Frees old B allocation and tracks A */
				free_matrix(track, cell_w);
				track = gen_A;

				/* Iterate gen number */
				gen++;
			}

		/* Renders gen_B if current gen number is NOT divisible by 2 */
		} else if(gen % 2 != 0)
		{
			/* Delays game rendering */
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

				/* Frees old A allocation and tracks B */
				free_matrix(track, cell_w);
				track = gen_B;

				/* Iterate gen number */
				gen++;
			}
		}


		/* change the  modulus value to slow the rendering of sdl test */
		/**
		 * if (SDL_GetTicks() % 1 == 0)
		 * sdl_test(&sdl_info, m, n);
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
