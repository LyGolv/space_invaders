#include <stdio.h>
#include <stdlib.h>

#include "../inc/game.h"
#include "../inc/vessel.h"
#include "../inc/gui.h"
#include "../inc/player.h"
#include "../inc/enemy.h"
#include "../inc/gameplay.h"
#include "../inc/image.h"

// Pour la gestion des évènements
#include <time.h>
#include <unistd.h>

#include "../inc/globals.h"
#include "../inc/gui.h"

void displayGameMenu(Game* game)
{
	int y=0, x=2, txscore_vie=0;
	couleur("46");
	for(int i=0; i<game->w_cols; ++i)
	{
		for(int j=0; j<_MENUGAMEHEIGHT; ++j)
		{
			move_to(i, j);
			printf(" ");
		}
	}
	couleur("39");
	while(y< 4)
	{
		move_to(0, y);
		printf("║");
		move_to(game->w_cols, y);
		printf("║");
		++y;
	}
	move_to(0, _MENUGAMEHEIGHT);
	printf("╚");
	move_to(game->w_cols, _MENUGAMEHEIGHT);
	printf("╝");
	while(x < game->w_cols)
	{
		move_to(x, _MENUGAMEHEIGHT);
		printf("═");
		++x;
	}

	// affichage du score (15 est la taille du mots score + le score maximum (99999) + (" + ") = 3)
	move_to(2, 1);
	printf("Score : ");
	printf("%d", game->player.score);
	move_to(2, 2);
	x=2;
	while(x < 15)
	{
		move_to(x, 2);
		printf("-");
		++x;
	}
	y=0;
	while(y < _MENUGAMEHEIGHT)
	{
		move_to(x, y);
		printf("┊");
		++y;
	}
	++x;

	// affichage du nombre de vie
	move_to(x, 1);
	printf(" Vie :");
	for(int i=0; i < game->player.nbVie; ++i)
		printf("%s  ", game->player.imgVie);
	move_to(x, 2);
	y=x; // juste pour ne pas déclarer une nouvelle variable (y = après la barre séparant le score de la vie)
	while(x-y < 14)
	{
		move_to(x, 2);
		printf("-");
		++x;
	}
	y=0;
	while(y < _MENUGAMEHEIGHT)
	{
		move_to(x, y);
		printf("┊");
		++y;
	}
	// largeur de la chaine score + vie
	txscore_vie = x+1;

	// affichage du meilleur score
	x=game->w_cols - 20;
	move_to(x, 1);
	printf(" High Score : ");
	printf("%d", game->lvl.scoreMax);
	y=0;
	while(y < _MENUGAMEHEIGHT)
	{
		move_to(x-1, y);
		printf("┊");
		++y;
	}
	move_to(x, 2);
	y=x; // juste pour ne pas déclarer une nouvelle variable (y = après la barre séparant le score de la vie)
	while(x-y < 20)
	{
		move_to(x, 2);
		printf("-");
		++x;
	}

	// affichage du mode
	x = (txscore_vie + game->w_cols - 20)/2 -8;
	++x;
	move_to(x, 2);
	printf(" Mode : ");
	switch(game->mode)
	{
		case 0:
			printToBold("Facile");
			break;
		case 1:
			printToBold("Difficile");
			break;
		case 2:
			printToBold("Progressif");
			break;
		case 3:
			printToBold("Gladiateur");
			break;			
	}

	couleur("40");
}

void paintScreen(Game* game)
{
	couleur("40");
	for(int i=0; i<=game->w_cols; ++i)
	{
		for(int j=0; j<=game->w_rows; ++j)
		{
			move_to(i, j);
			printf(" ");
		}
	}
}

void loadTitle(Image* image, char* pathTitle)
{
	loadImage(image, pathTitle);
}

void displayModeSelect(Game* game)
{
	int x,y;
	x= game->menu.border.x+game->menu.border.width-25;
	y= game->menu.border.y+game->menu.border.height+1; 
	move_to(x, y);
	for(int i=x; i<game->w_cols-1;++i)
		printf(" ");
	move_to(x, y);
	couleur("36");
	printf("Mode sélectionné: ");
	couleur("35");
	if(game->mode == 0)
		printf("Facile");
	else if(game->mode == 1)
		printf("Difficile");
	else if(game->mode == 2)
		printf("Progressif");
	else if(game->mode == 3)
		printf("Gladiateur");
}

void displayModeText(Game* game)
{
	couleur("36");
	for(long i = 0; i< game->menu.nbMode; ++i)
	{		
		if(game->mode == i)
			couleur("47");
		displayImageOnXY(game->menu.mode+i, game->menu.border.x+(game->menu.border.width/2) -(game->menu.mode[i].width/2), game->menu.border.y+5*(i+1));
		if(game->mode == i)
			couleur("40");
	}
}

void displayMenuText(Game* game)
{
	couleur("36");
	displayImageOnXY(&game->menu.title, (game->w_cols/2)-(game->menu.title.width/2), 5);
	move_to(game->menu.title.x+game->menu.title.width-20, game->menu.title.y+game->menu.title.height);
	couleur("37");
	printf("by ");
	couleur("35");
	printf("c6xen");
	couleur("37");
	printf(" and ");
	couleur("35");
	printf("Malik");
	couleur("36");
	displayImageOnXY(&game->menu.border, (game->w_cols/2)-(game->menu.border.width/2), game->menu.title.y+game->menu.title.height+4);
	move_to(game->menu.border.x + 23, game->menu.border.y + 1);
	couleur("35");
	printf("Press \"h\" for Help!!!!!");
	couleur("36");
	displayModeText(game);
	couleur("37");
	move_to(2, game->w_rows-1);
	printf("Press \"l\" for Leave Game!!!!!");
	displayModeSelect(game);
}

void resetWindowMenu(Game* game, int* winCol, int* winRow)
{
	clear();
	disableCursor();
	paintScreen(game);
	move_to(0, 0);
	displayMenuText(game);
	move_to(0,0);
	*winCol = game->w_cols;
	*winRow = game->w_rows;
}

void manageWindowsHelp(Game* game)
{
	clear();
	disableCursor();
	paintScreen(game);
	move_to(0, 0);
	FILE* file = fopen("../others/helps", "r+");
	if(file==NULL)
	{
		printf("Fichier d'aide introuvable!!");
		exit(EXIT_FAILURE);
	}
	char ligne[200];
	while(fgets(ligne, 200, file) != NULL)
		printf("%s", ligne);
	while(!game->in.key[_LEFT])
		updateInputs(&game->in);
	initInputs(&game->in);
	resetWindowMenu(game, &game->w_cols, &game->w_rows);
	fclose(file);
}

void menuManagement()
{
	Game game;

	int winCol=0, winRow=0;

	// initialisation des variables du jeu
	initGame(&game);

	// Récupère la taille du terminal
	window_NbColsAndRows(&game.w_cols, &game.w_rows);
	// servira pour la taille de la console avant le redimensionnnement
	winCol = game.w_cols;
	winRow = game.w_rows;	

	paintScreen(&game);

	loadTitle(&game.menu.title, "../images/title");
	loadTitle(&game.menu.border, "../images/border");
	game.menu.mode = loadMultipleImage("../images/mode", &game.menu.nbMode);
	game.menu.choice = loadMultipleImage("../images/choice", &game.menu.nbChoice);
	loadImage(&game.menu.pause, "../images/pause");

	displayMenuText(&game);
	clear();
	resetWindowMenu(&game, &winCol, &winRow);

	// Boucle du menu
	while(!game.in.quit)
	{
		window_NbColsAndRows(&game.w_cols, &game.w_rows);

		//mettre à jour les entrées
		updateInputs(&game.in);

		// si redimensionnement
		if(winCol-game.w_cols!=0 || winRow-game.w_rows!=0)
			resetWindowMenu(&game, &winCol, &winRow);

		if(game.in.key[_ENTER])
		{
			couleur("30");
			paintScreen(&game);
			game.in.key[_ENTER] = 0;
			gameplay(&game);	
			game.in.quit = 0;
			resetWindowMenu(&game, &winCol, &winRow);
		}
		// Fenêtre pour l'affichage de l'aide
		if(game.in.key[_H])
		{
			manageWindowsHelp(&game);
			game.in.key[_H] = 0;
		}
		// gestion des modes
		if(game.in.key[_UP])
		{
			--game.mode;
			if(game.mode<0)
				game.mode=0;
			displayModeText(&game);
			displayModeSelect(&game);
			game.in.key[_UP]=0;
		}
		if(game.in.key[_DOWN])
		{
			++game.mode;
			if(game.mode>3)
				game.mode=3;
			displayModeText(&game);
			displayModeSelect(&game);
			game.in.key[_DOWN]=0;
		}
		move_to(0,game.w_rows);
	}
	if(game.menu.mode != NULL)
	{
		for(long i = 0; i< game.menu.nbMode; ++i)
		{
			deleteImage(&game.menu.mode[i]);
		}
		free(game.menu.mode);
	}
	if(game.menu.choice != NULL)
	{
		for(long i = 0; i< game.menu.nbChoice; ++i)
		{
			deleteImage(&game.menu.choice[i]);
		}
		free(game.menu.choice);
	}

	freeGame(&game);
}