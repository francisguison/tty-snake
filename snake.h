/*		snake.h	- Snake main header
 *
 * 		Copyright © 2008 Duquesnoy Martin <xorg62@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#ifndef __SNAKE_H__

#include <stdlib.h>
#include <ncurses.h>			 		
#include <time.h>		
#include <unistd.h> 					
#include <limits.h>						
#include <getopt.h>					

#define MAX_X 25
#define MAX_Y 80

#define BORDER 1
#define INFO 2
#define SNAKE 3
#define FOOD 4
#define RANDW 5

#define TRUE 1
#define FALSE 0



#define color COLOR_PAIR
#define white COLOR_WHITE
#define black COLOR_BLACK
#define red COLOR_RED
#define blue COLOR_BLUE
#define cyan COLOR_CYAN
#define green COLOR_GREEN
#define magenta COLOR_MAGENTA
#define yellow COLOR_YELLOW


#define help_print() printf("snake Usage : snake [option] [option] <arg> \n\n\
  -x <num>	  	X position of snake\n\
  -y <num>		Y position of snake\n\
  -r, --random		Make a random level\n\
  -t, --tailinc		Snake incrementation food value\n\
  -w, --randwall	Make a wall when the snake eat\n\
  -s, --speed		Speed (milisecond of equart)\n\
  -b, --bell		For enable the sound system\n\
  -v, --version		Print version\n\
  -i, --info		Print info\n\
  -h, --help		Print help (this page)\n\n\
also , you can configure Snake in the config.h\n\
<num> must to be an integer (for x < 24 and y < 78) \n\
WARNING ! the random level function is experimental\n");\
exit(EXIT_SUCCESS);


void start(void);
void draw_frame(void);
void snake_func(void);
void lose_screen(void);
void lose(void);
void snake_food(void);
void snake_win(int enable);
void rand_wall(int enable);
void random_level(int enable);
int nrand(int min,int max);

typedef struct {
	int x;
	int y;
} COORD;

#endif
