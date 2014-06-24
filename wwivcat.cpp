/**************************************************************************\
**  WWIVcat - command line tool to display files with WWIV Color Codes    **
**  Copyright (c) 2014 Eric Manuel Pareja                                 **
**                                                                        **
\**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
		char ch;
		int speed=2400;  // Default speed is 2400 bps :)
		FILE *file;

		switch ( argc ) {
				case 1:
						file=stdin;
						break;
				case 3:
						speed=atoi(argv[2]);
				case 2:
						file=fopen( argv[1], "r");
						if (!file)
							{
							printf("Couldn't open file: %s\n", argv[1]);
							return (1);
							}
						break;
				default:
						printf("Too many arguments. I can't take it anymore!\n");
						return (0);
				}


//        printf("\x1b[?25l"); //turn cursor off
		while ( ch = getc(file) ) {
				if ( ch == EOF ) {
						printf("\x1b[0m"); 
//						printf("\x1b[?25h"); //turn cursor on
						return(0);
				} else {
				if (ch == 3 ) {
						ch = getc(file);
						switch(ch) {
							case '1':
								printf("\x1b[0;1;36m");     // high cyan 
								break;
							case '2':
								printf("\x1b[0;1;33m");     // high yellow 
								break;
							case '3':
								printf("\x1b[0;35m");       // low magenta 
								break;
							case '4':
								printf("\x1b[0;1;44m");     // white on blue 
								break;
							case '5':
								printf("\x1b[0;32m");       // low green 
								break;
							case '6':
								printf("\x1b[0;1;5;31m");   // high blinking red 
								break;
							case '7':
								printf("\x1b[0;1;34m");     // high blue 
								break;
							case '8':
								printf("\x1b[0;34m");       // low blue 
								break;
							case '9':
								printf("\x1b[0;36m");       // low cyan 
								break;
							default:
								printf("\x1b[0m");       // low grey 
						}
				}
				else {
						printf("%c",ch);
				}
						usleep(10000000 / speed);
						fflush(NULL);
				}
		}
}




