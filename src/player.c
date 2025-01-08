#include <stdio.h>
#include <stdlib.h>

#include "../inc/game.h"
#include "../inc/vessel.h"
#include "../inc/image.h"

int canIThrowProjectile(Game* game)
{
	if(game->player.v.proj == NULL || game->player.v.proj->first == NULL)
		return 1;
	Projectile* ptrProj = game->player.v.proj->first;
	int nb = 1;
	while(ptrProj != NULL)
	{
		++nb;
		ptrProj = ptrProj->next;
	}
	if(nb > game->player.nbBalls)
		return 0;
	else
		return 1;
}

void manageVesselPlayer(Game* game)
{
	int x = game->player.v.img.x, y = game->player.v.img.y;
	int test= 0;
	if(game->in.key[_UP])
	{
		y--;
		if(y<1) y = 1;
		test=1;
		game->in.key[_UP] = 0;
	}
	else if(game->in.key[_DOWN])
	{
		y++;
		if(y>game->w_rows-game->player.v.img.height) y = game->w_rows-game->player.v.img.height;
		test=1;
		game->in.key[_DOWN]=0;
	}
	else if(game->in.key[_LEFT])
	{
		x-=3;
		if(x<2) x = 2; 
		test=1;
		game->in.key[_LEFT]=0;
	}
	else if(game->in.key[_RIGHT])
	{
		x+=3;
		if(x + game->player.v.img.width > game->w_cols-1) x = game->w_cols-game->player.v.img.width-1;
		test=1;
		game->in.key[_RIGHT]=0;
	}

	if(game->in.key[_SPACE])
	{
		if(canIThrowProjectile(game))
			throwProjectile(&game->player.v);
		game->in.key[_SPACE]=0;
	}

	// déplacement du vaisseau	
	if(test)
		moveVessel(&game->player.v, x, y);
	else
		displayImageOnXY(&game->player.v.img, x, y);
}