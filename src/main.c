#include <stdio.h>
#include <time.h>

#include "../inc/globals.h"
#include "../inc/gui.h"

int main()
{
	// Renomme le terminal
	setcsltitle("Lio-Invaders");

	//efface le terminal
	clear();	

	// Desactive le curseur clignotant
	disableCursor();

	// passe la main pour la gestion du menu
	menuManagement();
	
	// rendre le curseur visible dans la console du user
	enableCursor();

	//efface le terminal
	clear();

	return 0;
}