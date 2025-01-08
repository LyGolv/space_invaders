#include <stdio.h>
#include <stdlib.h>

#include "../inc/game.h"
#include "../inc/vessel.h"
#include "../inc/player.h"
#include "../inc/enemy.h"
#include "../inc/image.h"
#include "../inc/globals.h"

#include <string.h>
#include <unistd.h>

void initGame(Game* game)
{
	game->parent = getpid();

    if ((game->child = fork()) < 0)
        exit(EXIT_FAILURE);
    else if (game->child == 0) /* fils */
   	{
        execl("../sh/menuSon.sh" ,"../sh/menuSon.sh", (char *)0);
        exit(0);
   	}

	//initialisation des entrées
	initInputs(&game->in);

	strcpy(game->player.imgVie, "❤️");

	game->menu.mode = NULL;
	game->menu.choice = NULL;
	game->mode = 0;
	game->timeToWait = clock();
	
	initImage(&game->menu.title);
	initImage(&game->menu.border);
}

void freeGame(Game* game)
{
	if ((game->child = fork()) < 0)
        exit(EXIT_FAILURE);
    else if (game->child == 0) /* fils */
   	{
        // on Arrète le son
		execl("./sh/stopSong.sh" + game->child ,"./sh/stopSong.sh", (char *)0);
        exit(0);
   	}
   	wait(NULL);
	deleteImage(&game->menu.title);
	deleteImage(&game->menu.border);
	deleteImage(&game->menu.pause);
}