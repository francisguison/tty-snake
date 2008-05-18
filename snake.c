/*		snake.c	- Snake main function
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

#include "config.h"
#include "snake.h"

/* variables */
    
int act_random =		O_RANDL;
int speed =				O_SPEED;
int inc_tail =		 	O_INCT;
int enable_rand_wall = 	O_RANDW;						
int enable_print_mat =	O_PMAT;
int enable_bell = 		O_BELL;		/* bell option */
int tail_length=		O_TLENGTH;	/* the tail's length */
unsigned int score;					/* score */
int mat[MAX_X][MAX_Y];				/* frame matrice */
unsigned long tail;					/* snake's tail */
 	
COORD food;						/* location X & Y of snake's food */
COORD direction = {1,0};		/* direction (with key) */
COORD snake[USHRT_MAX];			/* snake coordinated */  
COORD coordinated = {3,3};		/* final location of snake */
COORD randwall;					/* for the random wall fonction */

/* start function */

void start(void) {
	initscr ();							/* ncurses init */
 	noecho (); 
 	start_color ();
 	refresh();
 	keypad (stdscr, TRUE);				/* for arrow key */

	/* init color */
	init_pair(BORDER ,FGBORDER_COLOR, BORDER_COLOR);
	init_pair(INFO ,FGINFO_COLOR, INFO_COLOR);
	init_pair(SNAKE ,FGSNAKE_COLOR, SNAKE_COLOR);
	init_pair(FOOD ,FGFOOD_COLOR, FOOD_COLOR);
	init_pair(RANDW ,FGRANDW_COLOR, RANDW_COLOR);
}

/* random number function */

int nrand(int min,int max) {
	int nb;
	nb = (rand() % (max - min + 1)) + min;
	return nb;
}

/* fram drawing function */

void draw_frame(void) {
	int i;
	attron(color(BORDER));
	for(i=0;i<MAX_X;i++){
		mvaddstr (i,0," ");
		mvaddstr (i,(MAX_Y-1)," ");
		mat[i][0]=4;
		mat[i][MAX_Y-1]=4;
	}
	for(i=0;i<MAX_Y;i++) {
		mvaddstr (0,i," ");
		mvaddstr ((MAX_X-1),i," ");
		mat[0][i]=4;
		mat[MAX_X-1][i]=4;
	}
	attroff(color(BORDER));

	attron(color(INFO));
	mvaddstr(0,12,"Score : 0");
	attroff(color(INFO));
}
			

void snake_func(void) {
	int key;	
	coordinated.x += direction.x;    			/* snake direction incrementation */ 	
	coordinated.y += direction.y; 
	++tail;										/* tail incrementation */
	snake[tail].x = coordinated.x;		
	snake[tail].y = coordinated.y;		

	/* check snake_win situation */
	snake_win(enable_bell);
	/* check the losing situation */
	lose();	
	/* draw the snake */
	move(coordinated.x,coordinated.y);			
	attron(color(SNAKE));
	printw(" ");	
	attroff(color(SNAKE));
	/* put the addstr at the left-top */
	mat[coordinated.x][coordinated.y]=1;
	
	attron(color(BORDER));
	mvaddstr(0,0," ");
	attroff(color(BORDER));
	halfdelay(1);	
	/* remove the end of the tail when the lenght is reached */
	
	if(tail>tail_length) {
		mvaddstr(snake[tail-tail_length].x , snake[tail-tail_length].y, " ");
		mat[snake[tail-tail_length].x][snake[tail-tail_length].y] = 0;
	}

	key=getch();

	switch(key) {
		case KEY_UP: 					/* for up key arrow  */
			if(!direction.x) {
				direction.x = -1;
				direction.y = 0;
			}
			break;
		case KEY_DOWN:					/* for down key arrow */
			if(!direction.x) {
				direction.x = 1;
				direction.y = 0;
			}
			break;
		case KEY_LEFT:					/* for left key arrow */
			if(!direction.y) {
				direction.y = -1;
				direction.x = 0;
			}
			break;
		case KEY_RIGHT:					/* for right key arrow */
			if(!direction.y) {
				direction.y = 1;
				direction.x = 0;
			}
			break;
		case 'p':
			while(getchar() != 'p');
			break;
		case 'q':
			lose();
			break;
	refresh();
	}
}

void random_level(int enable) {
	if(enable) {
		attron(color(BORDER));
		int rand1,rand2,rand_choose,
			a,b,c,d,e,f,g,h,
			c_ch,g_ch,
			count=0,counts=0;
		
		rand1 = nrand(1,10);
		rand2 = nrand(1,10);
		rand_choose = nrand (1,2);
		d = nrand(0,8);
		
		if(rand_choose) { 
			c_ch = 1 ;
			g_ch = 21;
		} else {
			c_ch = 4;
			g_ch = 23;
		}

		while(count != d) {
			a = nrand(1,rand1 );
			b = nrand(2,76);
			for(c=c_ch;c<a;c++){
				mvaddstr(c,b," ");
				mat[c][b]=4;
			}
			++count;
		}
		h = nrand(0,8);
		while(counts != h) {
			e = nrand(1,rand2 );
			f = nrand(2,76);
			for(g=g_ch;g>e;g--){
				mvaddstr(g,f," ");
				mat[g][f]=4;
			}
		++counts;
		}
	attroff(color(BORDER));	
	}
}
		

/* food make function */

void snake_food(void) {
	int pass=0;
	while(pass != 1) {	
		food.x = nrand(3,22);
		food.y = nrand(3,77);
		if(!mat[food.x][food.y]) {
			pass = 1;
		}
	}	
	mat[food.x][food.y] = 3;
	attron(color(FOOD));
	mvaddstr(food.x,food.y,"*");
	attroff(color(FOOD));
}

void rand_wall(int enable) {
	if (enable) {
		int pass=0;
		while(pass != 1) {
			randwall.x = nrand(3,22);
			randwall.y = nrand(3,77);
			if(!mat[randwall.x][randwall.y]) {
				pass = 1;
			}
		}
		attron(color(RANDW));
		mvaddstr(randwall.x,randwall.y,"X");
		attroff(color(RANDW));
		mat[randwall.x][randwall.y] = 2;
	}
}
		
/* check function (win) */

void snake_win(int enable) {
	init_pair(2,black,white);
	char bell;
	bell = enable ? '\a' : 0;
	if(mat[coordinated.x][coordinated.y] == 3) {
		printf("%c",bell); 			/* for make a sound when the snake eat */
		++score;
		tail_length += inc_tail;
		move(0,20);
		attron(color(2));
			printw("%d",score);
		attroff(color(2));
		rand_wall(enable_rand_wall);
		snake_food();
	}
}

void print_mat(int enable){
	if(enable) {
		int i,j;
		int fg,bg;
		for(i=0;i<25;i++){
			for(j=0;j<80;j++){
				switch(mat[i][j]){
					case 0:
						fg = 37;
						bg = 40;
						break;
					case 2:
						fg = 30;
						bg = 41;
						break;
					case 1:
						fg = 30;
						bg = 42;
						break;
					case 3:
						fg = 30;
						bg = 44;
						break;
					case 4:
						fg = 30;
						bg = 47;
						break;
				}
			colors(bg,fg);
			printf("%d",mat[i][j]);
			colors(0,0);
			}
			printf("\n");
		}
	}
}


void lose_screen(void) {
	sleep(1);
	clear();
	endwin();
	print_mat(enable_print_mat);
	printf("Your score : %d\n",score);
	
	exit(EXIT_SUCCESS);
}

/* check function (lose) */

void lose(void) {

	if((mat[coordinated.x][coordinated.y] == 1)
	|| (mat[coordinated.x][coordinated.y] == 2)
	|| (mat[coordinated.x][coordinated.y] == 4)) {
		lose_screen();
	}
}

int main(int argc,char **argv) {
	
	int c;
	srand(time(NULL));

	static struct option long_options[] =
    {
        {"help",    0, NULL, 'h'},
        {"version", 0, NULL, 'v'},
		{"info",	0, NULL, 'i'},
		{"random",	0, NULL, 'r'},
		{"bell",	0, NULL, 'b'},
		{"randwall",0, NULL, 'w'},
		{"tailinc",	0, NULL, 't'},	
		{"length",	0, NULL, 'l'},
		{"speed",	0, NULL, 's'},
		{"pmat",	0, NULL, 'm'},
        {NULL,      0, NULL, 0}
    };

	 while ((c = getopt_long(argc,argv,":vimbwt:rhx:y:s:l:",
					 long_options,NULL)) != -1) {

		switch(c) {
			case 'v':
				printf("TTY-Snake beta3\nCompiled at %s ",__DATE__);
				printf("%s\n",__TIME__);
				exit(EXIT_SUCCESS);
				break;
			case 'i':
				printf("Snake , By Duquesnoy Martin\n");
				exit(EXIT_SUCCESS);
				break;
			case 'h':
				help_print();
				break;
			case 'w':
				enable_rand_wall = 1;
				break;
			case 'm':
				enable_print_mat = 1;
				break;
			case 't':
				inc_tail = atoi(optarg);
				break;
			case 's':
				if(atoi(optarg)<0) {
					help_print();
				}
				speed = atoi(optarg);
				break;
			case 'r':
				act_random = 1;
				break;
			case 'l':
				if(atoi(optarg)<1) {
					help_print();
				} 
				tail_length = atoi(optarg);
				break;
			case 'b':
				enable_bell = 1;
				break;
			case 'x':
				if(atoi(optarg) > MAX_X-1
						|| atoi(optarg) < 1) {
					help_print(); 
				}
				coordinated.x = atoi(optarg);
				break;
			case 'y':
				if(atoi(optarg) > MAX_Y-1
					|| atoi(optarg) < 1 ) {
					help_print();
				}
				coordinated.y = atoi(optarg);
				break;
		}
	 }

	start();					/* init ncurses */
	snake_food(); 				/* init the food */
	draw_frame(); 				/* init the frame */
 	random_level(act_random); 	/* init the random level if it's enable */

	while(1) {
		snake_func();
		usleep(speed);
	}
}

