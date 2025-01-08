#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../inc/star.h"

void initStars(Game* game)
{
	for(int i = 0; i<NB_STAR-2; ++i)
	{
		game->star[i].x=aleatoire(game->w_cols-1, 1);
		game->star[i].y=aleatoire(game->w_rows-2, _MENUGAMEHEIGHT+1);
		int pond = aleatoire(10, 0);
		if(pond == 1)
			game->star[i].img='*';
		else
			game->star[i].img='.';
		game->star[i].etat = aleatoire(2, 0);		
	}
	game->timeToMoveStars_ms = 350;
	game->timeToMoveStars = clock();
	
}

void displayStar(Star* star)
{
	move_to(star->x, star->y);
	printf("%c", star->img);
}

void hideStar(Star* star)
{
	move_to(star->x, star->y);
	printf(" ");
}

void displayStars(Game* game)
{
	for(int i = 0; i<NB_STAR-2; ++i)
	{
		if(game->star[i].etat)
			displayStar(&game->star[i]);
	}
}

void moveStars(Game* game)
{
	if(clock() - game->timeToMoveStars > game->timeToMoveStars_ms*TIME_REG)
	{
		for(int i = 0; i<NB_STAR-2; ++i)
		{
			if(game->star[i].etat)
			{
				int y = game->star[i].y;			
				hideStar(&game->star[i]);								
				if(y < game->w_rows-2)
					++game->star[i].y;
				else
				{
					game->star[i].x = aleatoire(game->w_cols-1, 1);
					game->star[i].y = _MENUGAMEHEIGHT+1;
				}
				game->timeToMoveStars = clock();
				displayStar(&game->star[i]);
			}				
		}		
	}
}