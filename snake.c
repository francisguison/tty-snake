/*		snake.c	- TTY-Snake main function
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

/* ******** */
/* VARIABLE */
/* ******** */

unsigned long tail;					                      
unsigned int score;	
char filename[80];                    
int bg = COLOR_BLACK;
const int lose_option[DEFEAT_POSSIBILITY] = {1,2,4};

option_t option = {
	O_SPEED,						/* option.speed    */
	O_INCT,							/* option.inctail  */
	O_TLENGTH,						/* option.tlength  */
	O_RANDL,						/* option.randl    */	
	O_RANDW,						/* option.randw    */	
	O_PMAT,							/* option.printmat */	
	O_BELL 							/* option.bell     */
};

option_t def_option = {600,	1, 5, FALSE, FALSE,TRUE, FALSE};

coord_t framesize = {25,80};		/* size of the frame			  */
coord_t food;				        /* location X & Y of snake's food */
coord_t direction = {1,0};			/* direction (with key)           */
coord_t snake[USHRT_MAX];			/* snake coordinated              */
coord_t coordinated = {3,3};		/* final ion of snake      		  */
coord_t randwall;					/* random wall fonction           */

int mat[500][500];	
FILE* score_file  = NULL;
int top[SHRT_MAX] = {0};

/* **************** */
/* STARTING FUCTION */
/* **************** */

void 
	start(void) {
	initscr ();		                       
	noecho (); 
	start_color ();
	refresh();
	bg = (use_default_colors() == OK) ? -1 : black;
	keypad (stdscr, TRUE);				                         
	curs_set(0);
	raw();           
	init_pair(BORDER, FGBORDER_COLOR, BORDER_COLOR);
	init_pair(INFO,   FGINFO_COLOR,   INFO_COLOR);
	init_pair(SNAKE,  FGSNAKE_COLOR,  SNAKE_COLOR);
	init_pair(FOOD,   FGFOOD_COLOR,   FOOD_COLOR);
	init_pair(RANDW,  FGRANDW_COLOR,  RANDW_COLOR);
}

/* *************************** */
/* CHECK CONSOLE SIZE FUNCTION */
/* *************************** */

void
check_termsize(void) {
	if(getmaxx(stdscr) < framesize.y
    || getmaxy(stdscr) < framesize.x) {
		mvaddstr(getmaxy(stdscr) / 2,
				 getmaxx(stdscr) / 2 - 9,
				 "TERMINAL TOO SMALL");
		getch();
		endwin();
		exit(EXIT_FAILURE);
	}
}

/* *********************** */
/* RANDOM NUMBERS FUNCTION */
/* *********************** */

int 
nrand(int min,int max) {
	int nb;
		nb = (rand() % (max - min + 1)) + min;
	return nb;
}

/* ********************** */
/* FRAME DRAWING FUNCTION */
/* ********************** */

void
draw_frame(void) {
	int i;
	attron(color(BORDER));
	for(i=0;i<framesize.x;i++){
		mvaddstr (i,0," ");
		mvaddstr (i,(framesize.y-1)," ");
		mat[i][0]=4;
		mat[i][framesize.y-1]=4;
	}
	for(i=0;i<framesize.y;i++) {
		mvaddstr (0,i," ");
		mvaddstr ((framesize.x-1),i," ");
		mat[0][i]=4;
		mat[framesize.x-1][i]=4;
	}
	attroff(color(BORDER));

	attron(color(INFO));
	mvaddstr(0,1,"Score : 0");
	attroff(color(INFO));
}

/* ******************* */
/* SNAKE MAIN FUNCTION */
/*  THE SNAKE'S MOTOR  */
/* ******************* */

void 
snake_func(void) {
	int key;	
	coordinated.x += direction.x;    			                                     	
	coordinated.y += direction.y; 
	++tail;										                                
	snake[tail].x = coordinated.x;		
	snake[tail].y = coordinated.y;		

	snake_win(option.bell);
	
	lose();	
	
	move(coordinated.x,coordinated.y);			
	attron(color(SNAKE));
	printw(" ");	
	attroff(color(SNAKE));
	mat[coordinated.x][coordinated.y]=1;
	halfdelay(1);	
	
	if(tail>option.tlength) {
		mvaddstr(snake[tail-option.tlength].x, snake[tail-option.tlength].y, " ");
		mat[snake[tail-option.tlength].x][snake[tail-option.tlength].y] = 0;
	}

	key=getch();

	switch(key) {
		case KEY_UP:
			if(!direction.x) {
				direction.x = -1;
				direction.y = 0;
			}
			break;
		case KEY_DOWN:					                        
			if(!direction.x) {
				direction.x = 1;
				direction.y = 0;
			}
			break;
		case KEY_LEFT:					                          
			if(!direction.y) {
				direction.y = -1;
				direction.x = 0;
			}
			break;
		case KEY_RIGHT:					                         
			if(!direction.y) {
				direction.y = 1;
				direction.x = 0;
			}
			break;
		case 'p':
		case 'P':
		case ' ' :
			while(getchar() != 'p');
			break;
		case 'q':
		case 'Q':
			lose();
			break;
	refresh();
	}
}

/* ********************* */
/* RANDOM LEVEL FUNCTION */
/* /!\ EXPERIMENTAL /!\  */
/* ********************* */

void 
random_level(int enable) {
	if(enable) {
		attron(color(BORDER));
		int rand1,rand2,rand_choose,
			a,b,c,d,e,f,g,h,
			c_ch,g_ch,
			count=0,_count=0;
		
		rand1 = nrand(1,framesize.x - 15);
		rand2 = nrand(1,framesize.x - 15);
		rand_choose = nrand (1,2);
		d = nrand(0,8);
		
		if(rand_choose) { 
			c_ch = 1 ;
			g_ch = framesize.x - 2;
		} else {
			c_ch = 4;
			g_ch = framesize.x - 2;
		}

		while(count != d) {
			a = nrand(1,rand1 );
			b = nrand(2,framesize.y - 4);
			for(c=c_ch;c<a;c++){
				mvaddstr(c,b," ");
				mat[c][b]=4;
			}
			++count;
		}

		h = nrand(0,8);
		while(_count != h) {
			e = nrand(1,rand2 );
			f = nrand(2,framesize.y - 4);
			for(g=g_ch;g>e;g--){
				mvaddstr(g,f," ");
				mat[g][f]=4;
			}
		++_count;
		}
	attroff(color(BORDER));	
	}
}
		
/* ******************* */
/* SNAKE FOOD FUNCTION */
/* ******************* */

void 
snake_food(void) {
	int pass=0;
	while(pass != 1) {	
		food.x = nrand(3,framesize.x - 3);
		food.y = nrand(3,framesize.y - 3);
		if(!mat[food.x][food.y]) {
			pass = 1;
		}
	}	
	mat[food.x][food.y] = 3;
	attron(color(FOOD));
	mvaddstr(food.x,food.y,"*");
	attroff(color(FOOD));
}

/* ******************** */
/* RANDOM WALL FUNCTION */
/* ******************** */

void 
randw(int enable) {
	if (enable) {
		int pass=0;
		while(pass != 1) {
			randwall.x = nrand(3,framesize.x - 3);
			randwall.y = nrand(3,framesize.y - 3);
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

/* ****************** */
/* CHECK WIN FUNCTION */
/* ****************** */

void
snake_win(int enable) {
	init_pair(2,black,white);
	char bell;

	bell = enable ? '\a' : 0;

	if(mat[coordinated.x][coordinated.y] == 3) {
		printf("%c",bell); 			
		++score;
		option.tlength += option.inctail;
		move(0,9);
		attron(color(2));
		printw("%d",score);
		attroff(color(2));
		randw(option.randw);
		snake_food();
	}
}

/* ********************* */
/* PRINT MATRIX FUNCTION */
/* ********************* */

void 
printmat(int enable){
	if(enable) {
		int i,j;
		int fg,bgc;
		for(i=0;i<framesize.x;i++){
			for(j=0;j<framesize.y;j++){
				switch(mat[i][j]){
					case 0: fg = 37; bgc = 40; break;
					case 1: fg = 30; bgc = 42; break;
					case 2: fg = 30; bgc = 41; break;
					case 3: fg = 30; bgc = 44; break;
					case 4: fg = 30; bgc = 47; break;
				}
			colors(bgc,fg);
			printf("%d",mat[i][j]);
			colors(0,0);
			}
			putchar('\n');
		}
	}
}

/* *************** */
/* TOP 10 FUNCTION */
/* *************** */

void 
print_topten(void) {
	int a,b,c,nm_l=0,mem;
	FILE* score_file  = NULL;
	score_file = fopen(filename,"r+");

	while((c = fgetc(score_file)) != EOF) {
		if( c == '\n' ){
			++nm_l;
  	 	}
	}
	
	int tab_sc[nm_l];
		
	for(a=0 ; a < nm_l ;++a){
		tab_sc[a] = 0;
	}
	rewind(score_file);

	for(a=0;a<nm_l;a++){
  	  	fscanf(score_file,"%d",&top[a]);
		tab_sc[a] = top[a];
  	}
	
	while(b) {
  	    b = 0 ;
	    for(a=0;a<nm_l-1;a++){
			if(tab_sc[a] < tab_sc[a+1]){
				
				mem = tab_sc[a];
				tab_sc[a] = tab_sc[a+1];
				tab_sc[a+1] = mem;
					
				b = 1;    
			}
		}
	}

	printf("Top 10 :\n");
	for(a=0;a<10;++a){
		printf("%d %s : %d\n",a+1,USER,tab_sc[a]);
	}

	fclose(score_file);
}

/* ***************** */
/* PRINT LOSE SCREEN */
/* ***************** */

void 
lose_screen(void) {
	sleep(1);
	clear();
	endwin();
	printmat(option.printmat);
	printf("Your score : %d\n",score);
	score_file = fopen(filename,"a");
	if(score){
		fprintf(score_file,"%d \n",score);	
	}
	fclose(score_file);
	exit(EXIT_SUCCESS);
}

/* ******************* */
/* CHECK LOSE FUNCTION */
/* ******************* */

void 
lose(void) {
	int i;
	for(i = 0; i < DEFEAT_POSSIBILITY; ++i) {
		if(mat[coordinated.x][coordinated.y] == lose_option[i]) {
			lose_screen();
		}	
	}
}

/* ************ */
/* MAIN FUCTION */
/* ************ */

int 
main(int argc,char **argv) {
	
	int c;
	srand(time(NULL));
	sprintf(filename,"%s/.ttysnakescore",getenv("HOME"));
	
	static struct option long_options[] = {
		{"help",    0, NULL, 'h'},
		{"version", 0, NULL, 'v'},
		{"info",    0, NULL, 'i'},
		{"random",  0, NULL, 'r'},
		{"bell",    0, NULL, 'b'},
		{"randwall",0, NULL, 'w'},
		{"tailinc", 0, NULL, 't'},	
		{"length",  0, NULL, 'l'},
		{"speed",   0, NULL, 's'},
		{"pmat",    0, NULL, 'm'},
		{"topten",  0, NULL, 'c'},
		{"default", 0, NULL, 'd'},
		{"all-term",0, NULL, 'a'},
		{NULL,      0, NULL, 0},
	};

	 while ((c = getopt_long(argc,argv,":vdimbwt:rhcx:y:s:l:X:Y:a",
					 long_options,NULL)) != -1) {

		switch(c) {
			case 'v':
				printf("TTY-Snake 0.2alpha1 " __DATE__  " Compiled at " __TIME__ " by %s\n",getenv("USER"));
				exit(EXIT_SUCCESS);
				break;
			case 'i':
				printf("TTY-Snake , By Duquesnoy Martin (xorg62)\n");
				exit(EXIT_SUCCESS);
				break;
			default:
			case 'h':
				help_print();
				break;
			case 'w':
				option.randw = true;
				break;
			case 'm':
				option.printmat = true;
				break;
			case 't':
				option.inctail = atoi(optarg);
				break;
			case 's':
				if(atoi(optarg) < 0) {
					help_print();
				}
				option.speed = atoi(optarg);
				break;
			case 'c':
				print_topten();
				exit(EXIT_SUCCESS);
				break;
			case 'r':
				option.randl = true;
				break;
			case 'd':
			    option = def_option;
				break;
			case 'l':
				if(atoi(optarg) < 1) {
					help_print();
				} 
				option.tlength = atoi(optarg);
				break;
			case 'b':
				option.bell = true;
				break;
			case 'x':
				if(atoi(optarg) > framesize.x-1
						|| atoi(optarg) < 1) {
					help_print(); 
				}
				coordinated.x = atoi(optarg);
				break;
			case 'y':
				if(atoi(optarg) > framesize.y-1
				|| atoi(optarg) < 1 ) {
					help_print();
				}
				coordinated.y = atoi(optarg);
				break;
			case 'X':
				start();
				if(atoi(optarg) < getmaxy(stdscr) &&
							atoi(optarg) > 9) {
					framesize.x = atoi(optarg);
				} else {
					help_print();
				}
				break;
			case'Y':
				if(atoi(optarg) < getmaxx(stdscr) &&
						atoi(optarg) > 9) {
					framesize.y = atoi(optarg);
				} else {
					help_print();
				}
				break;
			case 'a':
				start();
				framesize.y = getmaxx(stdscr);
				framesize.x = getmaxy(stdscr);
				break;
		}
	 }

	start();
	check_termsize();
	snake_food(); 			                      
	draw_frame(); 			                    
 	random_level(option.randl); 	                                                         

	while(1) {
		snake_func();
		usleep(option.speed);
	}
}
