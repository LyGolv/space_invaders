#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../inc/globals.h"
#include "../inc/gameplay.h"
#include "../inc/vessel.h"
#include "../inc/enemy.h"
#include "../inc/image.h"

int aleatoire_pondere( int items[], int ponderations[], int taille)
{
    int sumPonderations = 0;
    int index = 0;
 
    for (int i = 0; i < taille; i++)
        sumPonderations += ponderations[i];

    int tirage = rand() % sumPonderations;
 
    for(int sum = 0; sum <= tirage; sum += ponderations[index], index++ );
 
    return items[index-1];
}

void loadVesselEnemy(Vessel* v, int num)
{
	switch(num)
	{
		case 0 :
			loadVessel(v, "../images/AKAv1");
			break;
		case 1 :
			loadVessel(v, "../images/AKAv2");
			break;
		case 2 :
			loadVessel(v, "../images/AKAv3");
			break;
		case 3 :
			loadVessel(v, "../images/AKAv4");
			break;
		case 4 :
			loadVessel(v, "../images/GMAa1");
			break;
		case 5 :
			loadVessel(v, "../images/GMAa2");
			break;
		case 6 :
			loadVessel(v, "../images/GMAa3");
			break;
		case 7 :
			loadVessel(v, "../images/GMAa4");
			break;
	}
}

void giveEnemyPoint(Enemy* enemy, int num)
{
	switch(num)
	{
		case 0 :
			enemy->point = 25;
			enemy->blind = 1;
			break;
		case 1 :
			enemy->point = 50;
			enemy->blind = 2;
			break;
		case 2 :
			enemy->point = 75;
			enemy->blind = 3;
			break;
		case 3 :
			enemy->point = 100;
			enemy->blind = 4;
			break;
		case 4 :
			enemy->point = 50;
			enemy->blind = 2;
			break;
		case 5 :
			enemy->point = 100;
			enemy->blind = 4;
			break;
		case 6 :
			enemy->point = 150;
			enemy->blind = 6;
			break;
		case 7 :
			enemy->point = 200;
			enemy->blind = 8;
			break;
	}
}

ListEnemy* initFirstEnemy(int item[], int ponderations[], int taille)
{
    ListEnemy *list = malloc(sizeof(*list));
    Enemy *tmp = malloc(sizeof(*tmp));
    if (list == NULL || tmp == NULL)
        exit(EXIT_FAILURE);

    int i = aleatoire_pondere(item, ponderations, taille);
    loadVesselEnemy(&tmp->v, i);
    giveEnemyPoint(tmp, i);

    tmp->next = NULL;
    list->first = tmp;
    return list;
}

void addNewEnemy(ListEnemy *list, int item[], int ponderations[], int taille)
{
    /* Création du nouvel élément */
    Enemy* new = (Enemy*)malloc(sizeof(Enemy));
    if (list == NULL || new == NULL)
        exit(EXIT_FAILURE);
    int i = aleatoire_pondere(item, ponderations, taille);
    loadVesselEnemy(&new->v, i);
    giveEnemyPoint(new, i);
    /* Insertion de l'élément au début de la liste */
    new->next = list->first;
    list->first = new;
}

ListEnemy* makeEnemy(ListEnemy *list, int item[], int ponderations[], int taille)
{
	if(list == NULL || list->first == NULL)
		return initFirstEnemy(item, ponderations, taille);
	else
		addNewEnemy(list, item, ponderations, taille);
	return list;
}


void orderEnemys(ListEnemy *list, int enemyByLine)
{
	Enemy* p = list->first;
	int i = 0, decY = 0, decX = 0, max = 0, test = 0;
	while(p != NULL)
	{	
		max = (max<p->v.img.height)? p->v.img.height : max;		
		initVessel(&p->v, decX, 1+_MENUGAMEHEIGHT- decY);		
		decX += p->v.img.width + 2;		
		++i;
		p->direction = 0;
		if(i==1)
			p->bord = 1;
		else
			p->bord = 0;
		if(test)
		{
			p->direction = 1;
			test = 0;
		}
		if(i==enemyByLine)
		{
			decX = 0;
			decY+=max;
			// passe à 1 puisque l'enemy est au bord
			p->bord = 1;
			p->direction = 2; 
			if(p->next != NULL)
			{
				p->next->bord = 1;
				test = 1;
			}
			i=0;
		}		
		p = p->next;
	}
}

void initCallEnemy(Game* game, int nbEnemy, int enemyByLine, int enemySpeedMove, int nbEnemySolo, int nbBalls, int timeToShoot_ms)
{
	game->lvl.nbEnemy = nbEnemy;
	game->lvl.enemyByLine = enemyByLine;
	game->lvl.enemySpeedMove = enemySpeedMove;
	game->lvl.nbEnemySolo = nbEnemySolo;
	game->player.nbBalls = nbBalls;
	game->lvl.timeToShoot_ms = timeToShoot_ms;
}

void initEnemyWithOption(Game* game, int nbEnemy, int enemyByLine, int enemySpeedMove, int nbEnemySolo, int nbBalls, int timeToShoot_ms)
{
	srand(time(NULL));
	game->lvl.timeToQuit = clock();
	game->lvl.timeToQuit_ms = 1500;
	game->lvl.proj = NULL;
	game->lvl.timeToShoot = clock();
	game->lvl.enemy = NULL;
	game->lvl.enemySolo = malloc(sizeof(*game->lvl.enemySolo));
	game->lvl.enemySolo->first = NULL;
	game->lvl.enemyAttack = 0;
	switch(game->mode)
	{
		case 0:
		{
			int item[2] = { 0, 1}, ponderations[2] = { 80, 20};
			initCallEnemy(game, nbEnemy, enemyByLine, enemySpeedMove, nbEnemySolo, nbBalls, timeToShoot_ms);
			for(int i=0; i<game->lvl.nbEnemy; ++i)
				game->lvl.enemy = makeEnemy(game->lvl.enemy, item, ponderations, 2);	
			orderEnemys(game->lvl.enemy, game->lvl.enemyByLine);	
			game->lvl.direction = 1;
		}
		break;
		case 1:
		{
			int item[4] = { 0, 1, 2, 3}, ponderations[4] = { 10, 10, 30, 90};
			initCallEnemy(game, nbEnemy, enemyByLine, enemySpeedMove, nbEnemySolo, nbBalls, timeToShoot_ms);
			for(int i=0; i<game->lvl.nbEnemy; ++i)
				game->lvl.enemy = makeEnemy(game->lvl.enemy, item, ponderations, 4);	// le dernier paramètre est la taille du tableau de ponderation
			orderEnemys(game->lvl.enemy, game->lvl.enemyByLine);	
			game->lvl.direction = 1;
		}
		break;
		case 2:
		{
			int item[4] = { 0, 1, 2, 3}, ponderations[4] = { game->lvl.percentEnemy1, game->lvl.percentEnemy2, game->lvl.percentEnemy3, game->lvl.percentEnemy4};
			initCallEnemy(game, nbEnemy, enemyByLine, enemySpeedMove, nbEnemySolo, nbBalls, timeToShoot_ms);
			for(int i=0; i<game->lvl.nbEnemy; ++i)
				game->lvl.enemy = makeEnemy(game->lvl.enemy, item, ponderations, 4);	
			orderEnemys(game->lvl.enemy, game->lvl.enemyByLine);	
			game->lvl.direction = 1;
		}
		break;
		case 3:
		{
			int item[4] = { 4, 5, 6, 7}, ponderations[4] = { 30, 40, 50, 60};
			initCallEnemy(game, nbEnemy, enemyByLine, enemySpeedMove, nbEnemySolo, nbBalls, timeToShoot_ms);
			for(int i=0; i<game->lvl.nbEnemy; ++i)
				game->lvl.enemy = makeEnemy(game->lvl.enemy, item, ponderations, 4);	
			orderEnemys(game->lvl.enemy, game->lvl.enemyByLine);	
			game->lvl.direction = 1;
		}
		break;
	}
}

void initEnemy(Game* game)
{
	game->lvl.percentEnemy1 = 80;
	game->lvl.percentEnemy2 = 50;
	game->lvl.percentEnemy3 = 20;
	game->lvl.percentEnemy4 = 0;
	switch(game->mode)
	{
		case 0:
			initEnemyWithOption(game, 32, 8, 70, 3, 4, 1500);
			break;
		case 1:
			initEnemyWithOption(game, 36, 9, 50, 4, 2, 1000);
			break;
		case 2:
			initEnemyWithOption(game, 32, 8, 70, 3, 4, 1500);
			break;
		case 3:
			initEnemyWithOption(game, 20, 5, 50, 4, 1, 1000);
			break;
		default :
			break;
	}
}

ListEnemy* takeOutEnemy(ListEnemy* list, Enemy* en)
{
	if(list == NULL)
		return NULL;
	if(list->first == en)
	{
		list->first = list->first->next;
		if(list->first != NULL)
			list->first->bord = 1;
		return list;
	}
	Enemy* tmp = list->first;
	while(tmp != NULL)
	{
		if(tmp->next == en)
		{
			tmp->next = en->next;
			// les 02 sont maintenant sur les bords
			tmp->bord=1;
			if(en->next != NULL)
				en->next->bord = 1;
			break;
		}
		tmp = tmp->next;
	}
	return list;
}

ListEnemy* newEnemySolo(ListEnemy* list, Enemy* new)
{
	if(list==NULL || list->first == NULL)
	{
	    new->next = NULL;
	    list->first = new;
	    return list;
	}
    /* Insertion de l'élément au début de la liste */
    new->next = list->first;
    list->first = new;
    return list;
}

int nbEnemyAtEdge(ListEnemy* list)
{
	Enemy* tmp = list->first;
	int nbEnemyAtEdge = 0;
	while(tmp != NULL)
	{
		if(tmp->bord)
			++nbEnemyAtEdge;
		tmp = tmp->next;
	}
	return nbEnemyAtEdge;
}

int nbEnemySoloReach(Game* game)
{
	int nb = 1;
	// determine si le nombre enemis en solo est celui defini
	if(game->lvl.enemySolo != NULL)
	{
		Enemy* tmp = game->lvl.enemySolo->first;	
		if(game->lvl.enemySolo->first != NULL)			
		{
			while(tmp != NULL) 
			{
				++nb;
				tmp = tmp->next;
			}
		}
	}
	if(nb > game->lvl.nbEnemySolo)
		return 1;
	return 0;
}

void throwProjectileOnPlayer(Game* game)
{
	int numEn = aleatoire(game->lvl.enemyByLine, 0);
	Enemy* tmp = game->lvl.enemy->first;
	while(tmp != NULL && numEn > 0)
	{
		--numEn;
		if(tmp->next != NULL)
			tmp = tmp->next;		
	}
	if(tmp != NULL)
	{
		int x = (tmp->v.img.x+(tmp->v.img.x+tmp->v.img.width))/2, y = tmp->v.img.y + tmp->v.img.height;
		if(game->lvl.proj == NULL)
			game->lvl.proj = initFirstProj(x, y);
		else
			addNewProj(game->lvl.proj, x, y);
	}
}

void manageTakeOutEnemySolo(Game* game)
{
	if(!nbEnemySoloReach(game))
	{
		int nb = nbEnemyAtEdge(game->lvl.enemy);
		if(nb == 0)
			return;
		int number = aleatoire(nb, 0);
		Enemy *tmp1 = game->lvl.enemy->first, *tmp2=game->lvl.enemy->first;
		while(tmp1 != NULL && number)
		{	
			if(tmp1->next == NULL)
				break;				
			if(tmp1->bord)
				--number;	
			tmp2 = tmp1;
			tmp1 = tmp1->next;						
		}
		if(tmp2 != NULL && tmp2->bord)
		{
			game->lvl.enemy = takeOutEnemy(game->lvl.enemy, tmp2);
			game->lvl.enemySolo = newEnemySolo(game->lvl.enemySolo, tmp2);
			tmp2->timeToMove = clock();
			tmp2->timeToMove_ms = aleatoire(game->lvl.enemySpeedMove*2 + 50, game->lvl.enemySpeedMove*2);
			tmp2->timeToShoot = clock();
			switch(game->mode)
			{
				case 0:
					tmp2->timeToShoot_ms = aleatoire(3000, 1500);
					break;
				case 1:
					tmp2->timeToShoot_ms = aleatoire(1500, 500);
					break;
				case 2:
					tmp2->timeToShoot_ms = aleatoire(game->lvl.timeToShoot_ms, game->lvl.timeToShoot_ms-200);
					break;
				case 3:
					tmp2->timeToShoot_ms = aleatoire(1500, 500);
					break;
			}			
			tmp2->time_ms = clock();
			initVessel(&tmp2->v, tmp2->v.img.x, tmp2->v.img.y);
		}
	}	
}

void displayEnemyVessel(ListEnemy* list)
{
	if(list == NULL || list->first == NULL)
		return;

	Enemy* tmp = list->first;
	while(tmp != NULL)
	{
		if(tmp->v.img.y + tmp->v.img.height > _MENUGAMEHEIGHT + 2)
			displayImageOnXY(&tmp->v.img, tmp->v.img.x, tmp->v.img.y);
		tmp = tmp->next;
	}
}

void manageVesselEnemys(Game* game)
{	
	if(game->lvl.enemy == NULL)
		return;
	int i = 0;
	projTouchEnemy(game, game->lvl.enemy);
	if(game->lvl.enemyAttack &&  (clock() - game->lvl.timeToQuit) > game->lvl.timeToQuit_ms*TIME_REG)
	{		
		manageTakeOutEnemySolo(game);
		game->lvl.timeToQuit = clock();
	}

	if((clock() - game->lvl.timeToShoot) > game->lvl.timeToShoot_ms*TIME_REG)
	{
		throwProjectileOnPlayer(game);
		game->lvl.timeToShoot = clock();
	}
	Enemy* p = game->lvl.enemy->first;
	int test = 0;
	while(p != NULL)
	{
		if(p->v.img.x - 4 < 0)
		{
			test=1;
			game->lvl.direction = 1;
			break;
		}
		else if(p->v.img.x + p->v.img.width + 4 > game->w_cols)
		{
			test=1;
			game->lvl.direction = 0;
			break;
		}
		p = p->next;
	}
	p = game->lvl.enemy->first;
	while(p != NULL)
	{
		if(p->v.img.y + p->v.img.height > game->w_rows-1)
		{
			Enemy* eSupp = p;
			game->stabilityBar -= (eSupp->blind*2);
			if(game->stabilityBar < 0)
				game->stabilityBar = 0;
			p = p->next;
			destroyEnemy(game->lvl.enemy, eSupp);
			game->stabilityBar_change = 1;
			break;
		}
		if(test)
		{
			if(p->v.img.y + p->v.img.height > _MENUGAMEHEIGHT + 2)
				moveVessel(&p->v, p->v.img.x, p->v.img.y+1);
			else
				p->v.img.y+=1;
		}
		if(game->lvl.direction)
		{
			if(p->v.img.y + p->v.img.height > _MENUGAMEHEIGHT + 2)
				moveVessel(&p->v, p->v.img.x+1, p->v.img.y);
			else
				p->v.img.x+=1;
		}
		else
		{
			if(p->v.img.y + p->v.img.height > _MENUGAMEHEIGHT + 2)
				moveVessel(&p->v, p->v.img.x-1, p->v.img.y);
			else
				p->v.img.x-=1;
		}	
		
		// remet à 0 tous les vaisseaux de première ligne
		if(i < game->lvl.enemyByLine)
			p->direction = 0;

		++i;
		p = p->next;
	}
}

void manageVesselEnemysSolo(Game* game)
{
	if(game->lvl.enemySolo == NULL || game->lvl.enemySolo->first == NULL)
		return;
	projTouchEnemy(game, game->lvl.enemySolo);
	Enemy* p = game->lvl.enemySolo->first;
	while(p != NULL)
	{
		projTouchPlayer(game, p->v.proj);
		if(clock() - p->timeToShoot > p->timeToShoot_ms*TIME_REG)
		{
			throwProjectile(&p->v);	
			p->timeToShoot = clock();
		}
		if(clock() - p->time_ms > _SPEEDMOVEPROJ*TIME_REG)
		{
			moveProjectiles(p->v.proj, game->w_cols, game->w_rows, 1);
			p->time_ms = clock();
		}
		if(p->v.img.y + p->v.img.height > game->w_rows-1)
		{
			Enemy* eSupp = p;
			game->stabilityBar -= (eSupp->blind*2);
			if(game->stabilityBar < 0)
				game->stabilityBar = 0;
			p = p->next;
			destroyEnemy(game->lvl.enemySolo, eSupp);
			game->stabilityBar_change = 1;
			continue;
		}
		if(clock() - p->timeToMove > p->timeToMove_ms*TIME_REG)
		{
			if(p->v.img.x - 4 < 0)
				p->direction = 2;
			else if(p->v.img.x + p->v.img.width + 4 > game->w_cols)
				p->direction = 1;
			if(p->v.img.y + p->v.img.height > _MENUGAMEHEIGHT + 2)
			{
				if(p->direction == 2)
					moveVessel(&p->v, p->v.img.x+5, p->v.img.y+1);
				else if(p->direction == 1)
					moveVessel(&p->v, p->v.img.x-5, p->v.img.y+1);
				else
					moveVessel(&p->v, p->v.img.x, p->v.img.y+1);
			}
			else
			{
				if(p->direction == 2)
					{ p->v.img.x+=5; p->v.img.y+=1; }
				else if(p->direction == 1)
					{ p->v.img.x-=5; p->v.img.y+=1; }
				else
					p->v.img.y+=1;
			}
			p->timeToMove = clock();
		}
		p = p->next;
	}
}

void destroyEnemy(ListEnemy *list, Enemy* eSupp)
{
	if(list == NULL)
		return;
	Enemy* p = list->first;
	if(list->first == eSupp)
		list->first = eSupp->next;
	else
	{
		Enemy* p1 = p;
		while(p != eSupp)
		{
			p1 = p;
			p = p->next;
		}
		p=p->next;
		p1->next = p;
	}
	move_to(eSupp->v.img.x, eSupp->v.img.y);
	clearVessel(&eSupp->v);
	deleteVessel(&eSupp->v);
	free(eSupp);
}

void deleteEnemy(Enemy* enemy)
{
	Enemy* p=enemy;
	while(p != NULL)
	{
		Enemy* p1=p->next;
		deleteVessel(&p->v);
		free(p);
		p=p1;
	}
}