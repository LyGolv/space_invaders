#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include "../inc/input.h"

// récupère les commandes claviers saisi et les retournent
char key_pressed()
{
	struct termios oldterm, newterm;
	int oldfd; 
	char c, result = 0;
	tcgetattr (STDIN_FILENO, &oldterm);
	newterm = oldterm; 
	newterm.c_lflag &= ~(ICANON | ECHO);
	tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
	oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
	c = getchar();
	tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
	fcntl (STDIN_FILENO, F_SETFL, oldfd);
	if (c != EOF) { 
		ungetc(c, stdin); 
		result = getchar(); 
	}
	return result;
}

void updateInputs(Input* in)
{
	switch(key_pressed())
	{
		case 'l' :
		case 'L' :
			in->quit = 1;
			break;
		case 'z' :
		case 'Z': // Monter
			in->key[_UP] = 1;
			break;
		case 's' :
		case 'S' : // Descendre
			in->key[_DOWN] = 1;
			break;
		case 'q' :
		case 'Q' : // Gauche
			in->key[_LEFT] = 1;
			break;
		case 'd' :
		case 'D' : // Droite
			in->key[_RIGHT] = 1;
			break;
		case 'p' :
		case 'P' : // Mettre pause
			in->key[_P] = 1;
			break;
		case 'h' :
		case 'H' : // Affiche l'aide
			in->key[_H] = 1;
			break;
		case '\040': // Tirer
			in->key[_SPACE] = 1;
			break;
		case 10: // Droite
			in->key[_ENTER] = 1;
			break;
		default:
			break;
	}
}

void initInputs(Input* in)
{
	memset(in, 0, sizeof(Input)); //initialise tout les champs de la variable in à 0
}