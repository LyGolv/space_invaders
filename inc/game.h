/*
game.h
-------

par lygolv, pour le jeu space invaders

Rôle: Permet la gestion du jeu en général

Date de creation: 21/10/2021 16h30

Dernière modification: 21/10/2021
*/

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED	

	#include "../inc/input.h"
	#include "../inc/projectile.h"

	void initGame(Game* game); // initialise les variables de ma structure game
	void freeGame(Game* game);
	
#endif