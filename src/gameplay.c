#include <stdio.h>
#include <stdlib.h>

#include "../inc/game.h"
#include "../inc/vessel.h"
#include "../inc/gui.h"
#include "../inc/player.h"
#include "../inc/enemy.h"
#include "../inc/image.h"
#include "../inc/star.h"

// Pour la gestion des évènements
#include <time.h>
#include <unistd.h>
#include <string.h>

void resetWindowGameplay(Game* game, int* winCol, int* winRow)
{
	clear();
	disableCursor();
	paintScreen(game);
	move_to(0, 0);
	displayGameMenu(game);
	displayStars(game);
	game->player.v.img.x+=(game->w_cols-*winCol);
	game->player.v.img.y+=(game->w_rows-*winRow);
	moveVessel(&game->player.v, game->player.v.img.x, game->player.v.img.y);
	game->stabilityBar_change = 1;
	move_to(0,0);
	*winCol = game->w_cols;
	*winRow = game->w_rows;
}

void resetGameplay(Game* game)
{
	game->player.nbBalls = 0;
	game->lvl.enemyAttack = 0;
	game->lvl.nbEnemy = 0;
	game->lvl.nbEnemySolo = 0;
	game->beginAttack = 1;
	game->timeToWait = clock();
	if(game->lvl.proj != NULL)
	{
		deleteProjectiles(game->lvl.proj->first);
		free(game->lvl.proj);
	}
	if(game->lvl.enemy != NULL)
	{
		deleteEnemy(game->lvl.enemy->first);
		free(game->lvl.enemy);
	}
	if(game->lvl.enemySolo != NULL)
	{
		deleteEnemy(game->lvl.enemySolo->first);
		free(game->lvl.enemySolo);
	}
	// Destruction du vaisseau
	deleteVessel(&game->player.v);
}

void displayStabilityBar(Game* game)
{
	couleur("32");
	int nb = game->w_cols-32, t_bar = (nb > 100)? 100 : (nb < 0)? 0 : nb;
	float coef = (float)t_bar/100, stabOnTbar = game->stabilityBar*coef;	
	int posX = game->w_cols/2 - t_bar/2-16;	
	int stabOnTbar_int = (int) (stabOnTbar* 100);
	if(posX < 0)
		posX = 0;
	move_to(posX, game->w_rows);
	printf("Stability : ");
	printf("[");
	for(int i = 0; i< stabOnTbar_int/100; i++)
	{
		printf("#");
	}
	for(int i = stabOnTbar_int/100+1; i< t_bar; ++i)
	{
		printf(".");
	}
	printf("]");
	float percent = (float)game->stabilityBar/100 * 100;
	printf(" %.0f%% remaining  ", percent);
	couleur("37");
}

void animatePlayer(Game* game)
{
	clock_t time = clock();
	int i=0;
	while(i<5)
	{
		couleur("33");
		moveVessel(&game->player.v, game->player.v.img.x, game->player.v.img.y);
		while(clock() - time < 100*TIME_REG);
		time = clock();
		couleur("34");
		moveVessel(&game->player.v, game->player.v.img.x, game->player.v.img.y);
		while(clock() - time < 100*TIME_REG);
		time = clock();
		couleur("35");
		moveVessel(&game->player.v, game->player.v.img.x, game->player.v.img.y);
		while(clock() - time < 100*TIME_REG);
		time = clock();
		++i;
	}
	couleur("37");

}

int collisionAABB(Image* img1, Image* img2) 
{
	return ((img2->x > img1->x + img1->width) 
			|| (img2->x + img2->width < img1->x) 
			|| (img2->y > img1->y + img1->height) 
			|| (img2->y + img2->height < img1->y))? 0 : 1;
}

void projTouchEnemy(Game* game, ListEnemy* list)
{
	if(game->player.v.proj == NULL)
		return;
	Enemy* ptrEnemy = list->first;
	Projectile* ptrProj = game->player.v.proj->first;

	while(ptrEnemy != NULL)
	{
		while(ptrProj != NULL)
		{
			if((ptrProj->x >= ptrEnemy->v.img.x && ptrProj->x <= ptrEnemy->v.img.x + ptrEnemy->v.img.width)
				&& (ptrProj->y >= ptrEnemy->v.img.y && ptrProj->y <= ptrEnemy->v.img.y + ptrEnemy->v.img.height))
			{
				Enemy* eSupp = ptrEnemy;
				Projectile* pSupp = ptrProj;
				ptrEnemy = ptrEnemy->next;
				--eSupp->blind;
				if(eSupp->blind > 0)
				{
					deleteVessel(&eSupp->v);
					loadVesselEnemy(&eSupp->v, eSupp->blind-1);
				}
				else
				{
					// mise à jour du menu
					game->displayMenu = 1;
					// attribution des points au joueur si vaisseau detruit
					game->player.score += eSupp->point;
					destroyEnemy(list, eSupp);				
				}
				ptrProj = ptrProj->next;
				destroyProjectile(game->player.v.proj, pSupp);
				if(ptrEnemy != NULL)
					continue;
				else
					return;
			}
			ptrProj = ptrProj->next;
		}
		ptrEnemy = ptrEnemy->next;
		ptrProj = game->player.v.proj->first;
	}
}

void projTouchPlayer(Game* game, ListProj* list)
{
	if(list == NULL)
		return;
	Projectile* ptrProj = list->first;
	Vessel v = game->player.v;
	while(ptrProj != NULL)
	{
		if((ptrProj->x >= v.img.x && ptrProj->x <= v.img.x + v.img.width)
			&& (ptrProj->y >= v.img.y && ptrProj->y <= v.img.y + v.img.height))
		{
			Projectile* pSupp = ptrProj;
			// reduction de la vie
			--game->player.nbVie;
			ptrProj = ptrProj->next;
			destroyProjectile(list, pSupp);
			animatePlayer(game);
			// mise à jour du menu
			displayGameMenu(game);
			continue;
		}
		ptrProj = ptrProj->next;
	}
}

void EnemyTouchPlayer(Game* game, ListEnemy* list)
{
	if(game->player.v.proj == NULL || list ==NULL)
		return;
	Enemy* ptrEnemy = list->first;
	Vessel v = game->player.v;

	while(ptrEnemy != NULL)
	{	
		if(collisionAABB(&ptrEnemy->v.img, &v.img))
		{
			Enemy* eSupp = ptrEnemy;
			ptrEnemy = ptrEnemy->next;
			--game->player.nbVie;
			// mise à jour du menu
			displayGameMenu(game);
			// attribution des points au joueur
			game->player.score += eSupp->point;
			animatePlayer(game);
			destroyEnemy(list, eSupp);
			if(ptrEnemy != NULL)
				continue;
			else
				return;
		}
		ptrEnemy = ptrEnemy->next;
	}
}

void reCallEnemy(Game* game)
{
	game->beginAttack = 1;
	game->timeToWait = clock();
	if(game->lvl.proj != NULL)
	{
		deleteProjectiles(game->lvl.proj->first);
		free(game->lvl.proj);
	}
	if(game->lvl.enemy != NULL)
	{
		deleteEnemy(game->lvl.enemy->first);
		free(game->lvl.enemy);
	}
	if(game->lvl.enemySolo != NULL)
	{
		deleteEnemy(game->lvl.enemySolo->first);
		free(game->lvl.enemySolo);
	}
	switch(game->mode)
	{
		case 0:
			initEnemyWithOption(game, game->lvl.nbEnemy+game->lvl.enemyByLine, game->lvl.enemyByLine, 
				game->lvl.enemySpeedMove, game->lvl.nbEnemySolo+1, game->player.nbBalls, game->lvl.timeToShoot_ms);
			break;
		case 1:
			initEnemyWithOption(game, game->lvl.nbEnemy+game->lvl.enemyByLine, game->lvl.enemyByLine, 
				game->lvl.enemySpeedMove, game->lvl.nbEnemySolo+1, game->player.nbBalls, game->lvl.timeToShoot_ms);
			break;
		case 2:
			game->lvl.percentEnemy1 += 1;
			game->lvl.percentEnemy2 += 5;
			game->lvl.percentEnemy3 += 10;
			game->lvl.percentEnemy4 += 15;
			if( game->player.nbBalls > 1 && (game->lvl.percentEnemy4%2))
				game->player.nbBalls--;
			initEnemyWithOption(game, game->lvl.nbEnemy+game->lvl.enemyByLine, game->lvl.enemyByLine, 
				game->lvl.enemySpeedMove-5, game->lvl.nbEnemySolo+1, game->player.nbBalls, game->lvl.timeToShoot_ms-100);
			break;
		case 3:
			initEnemyWithOption(game, game->lvl.nbEnemy+game->lvl.enemyByLine, game->lvl.enemyByLine, 
				game->lvl.enemySpeedMove-5, game->lvl.nbEnemySolo+1, game->player.nbBalls, game->lvl.timeToShoot_ms-100);
			break;
		default :
			break;
	}
}

void loadHighScore(Game* game)
{
	FILE* file = fopen("../others/bestscore", "r+");
	if(file == NULL)
	{
		printf("Impossible de trouver le fichier %s", "../others/bestscore");
		exit(EXIT_FAILURE);
	}

	int num = game->mode+1, score;
	char name[21];
	while(num)
	{
		if(fscanf(file, "%s %d", name, &score) != 2)
		{
			printf("Erreur dans le fichier %s", "../others/bestscore");
			exit(EXIT_FAILURE);
		}
		--num;
	}
	game->lvl.scoreMax = score;
	strcpy(game->lvl.bestPlayerName, name);
	fclose(file);
}

void saveBestScore(Game* game)
{
	FILE* file = fopen("../others/bestscore", "r+");
	FILE* newfile = fopen("../others/scoreMax", "w");
	if(file == NULL || newfile == NULL)
	{
		printf("Impossible de trouver le fichier %s", "../others/bestscore");
		exit(EXIT_FAILURE);
	}
	int num = game->mode, i=0;
	char name[70];
	while(fgets(name, 70, file) != NULL)
	{
		if(i == num)
			fprintf(newfile, "%s %d\n", game->lvl.bestPlayerName, game->lvl.scoreMax);
		else
			fprintf(newfile, "%s", name);
		++i;
	}
	fclose(file);
	fclose(newfile);
	remove("../others/bestscore");
	rename("../others/scoreMax", "../others/bestscore");
}

void viderBuffer()
{
    int c;   
    do { 
        c = getchar(); 
    } while (c != '\n' && c != EOF); 
}

void cleanTypicalBuffer()
{
	char c = 'd';
    while((c == '\n' || c=='z' || c=='q' || c=='s' || c=='d') && c!=EOF)
    {
        c = getchar();
    }
}

void drawBox(Game* game)
{
	couleur("47");
	for(int i=game->w_cols/2-20; i<=game->w_cols/2+20; ++i)
	{
		for(int j=game->w_rows/2+5; j<=game->w_rows/2+15; ++j)
		{
			move_to(i, j);
			printf(" ");
		}
	}
	couleur("30");
	
	for(int j=game->w_rows/2+5; j<=game->w_rows/2+15; ++j)
	{
		if(j != game->w_rows/2+5 || j != game->w_rows/2+15);
		else
			continue;
		for(int i=game->w_cols/2-20; i<=game->w_cols/2+20; ++i)
		{
			move_to(i, j);
			if(j == game->w_rows/2+5 || j == game->w_rows/2+15)
			{
				if(i == game->w_cols/2-20 && j == game->w_rows/2+5)
					printf("╔");
				else if(i == game->w_cols/2+20 && j == game->w_rows/2+5)
					printf("╗");
				else if(i == game->w_cols/2-20 && j == game->w_rows/2+15)
					printf("╚");
				else if(i == game->w_cols/2+20 && j == game->w_rows/2+15)
					printf("╝");
				else
					printf("═");
			}
			else if(i == game->w_cols/2-20 || i == game->w_cols/2+20)
				printf("║");
			else
				continue;
		}
	}

	int posY = game->w_rows/2+5 + 1;
	if(game->player.score > game->lvl.scoreMax)
	{
		move_to(game->w_cols/2-20 + 5, posY);
		printf("Vous êtes le nouveau KING!!!");
		couleur("47");
		couleur("30");
		move_to(0, 0);
		viderBuffer();
		printf(" ");
		move_to(game->w_cols/2-20 + 5, posY + 2);
		printf("Votre NOM :  ");
		move_to(game->w_cols/2-20 + 5+13, posY + 2);
		enableCursor();
		couleur("40");
		couleur("37");
		cleanTypicalBuffer();
		if(scanf("%21s", game->lvl.bestPlayerName) != 1)
			exit(EXIT_FAILURE);
		disableCursor();
		game->lvl.scoreMax = game->player.score;
		posY+=2;
		couleur("47");
		couleur("30");
	}
	else
	{
		++posY;
		move_to(game->w_cols/2-20 + 5, posY);
		printf("Votre partie est terminé !!!");
	}	
	move_to(game->w_cols/2-20 + 5, posY+2);
	printf("Meilleur Score : %d" , game->lvl.scoreMax);
	move_to(game->w_cols/2-20 + 5, posY+4);
	printf("Votre score : %d" , game->player.score);
	move_to(game->w_cols/2-20 + 5, posY+6);
	printf("The KING : ");
	printToBold(game->lvl.bestPlayerName);

}

void gameplay(Game* game)
{
	int winCol=0, winRow=0;
	clock_t time_ms = clock(), timeForEnemy_ms = clock();
	clock_t waitNextWave = clock();
	int testWave = 0;

	// Récupère la taille du terminal
	window_NbColsAndRows(&game->w_cols, &game->w_rows);
	// servira pour la taille de la console avant le redimensionnnement
	winCol = game->w_cols;
	winRow = game->w_rows;

	// recupère le vaisseau du joueur
	loadVessel(&game->player.v, _PATH_VESSEL);
	// initialise la position du vaisseau (à placer après "loadVessel()")
	initVessel(&game->player.v, game->w_cols/2 - game->player.v.img.width/2, game->w_rows - game->player.v.img.height);	
	
	// Premier affichage du vaisseau pour le rendre visible à l'ecran
	moveVessel(&game->player.v, game->player.v.img.x, game->player.v.img.y);
	move_to(0,0);

	// initialisation des ennemies
	initEnemy(game);

	loadHighScore(game);

	game->stabilityBar = 100;
	game->stabilityBar_change = 1;

	game->player.score = 0;
	game->player.nbVie = 3;
	game->displayMenu=1;

	displayGameMenu(game);

	initStars(game);
	displayStars(game);

	game->timeToWait = clock();
	game->beginAttack=1;

	// Boucle de jeu
	while(!game->in.quit)
	{
		window_NbColsAndRows(&game->w_cols, &game->w_rows);

		//mettre à jour les entrées
		updateInputs(&game->in);

		if(game->in.quit)
		{
			displayImageOnXY(&game->menu.choice[2], game->w_cols/2 - game->menu.choice[2].width/2, game->w_rows/2-game->menu.choice[2].height);
			int choice = 0, test = 1;
			while(!game->in.key[_ENTER])
			{
				updateInputs(&game->in);
				if(game->in.key[_ENTER])
				{
					game->in.quit = choice;
					game->in.key[_ENTER] = 0;
					couleur("37");
					couleur("40");
					for(int i =0; i<=2; ++i)
					{
						// je ne sais pas pourquoi mais l'affichage se fesait de y+1 à height au lieu de y
						game->menu.choice[i].y--;
						hideImage(&game->menu.choice[i]);
					}
					break;
				}
				if(game->in.key[_LEFT])
				{
					test = 1;
					choice = 0;
					game->in.key[_LEFT] = 0;
				}
				else if(game->in.key[_RIGHT])
				{
					test = 1;
					choice = 1;
					game->in.key[_RIGHT] = 0;
				}
				if(test)
				{
					for(int i =0; i<2; ++i)
					{
						if(choice == i)
							couleur("37");
						displayImageOnXY(&game->menu.choice[i], (game->menu.choice[2].x+game->menu.choice[2].width/2-8) + (i?-1:1)*(game->menu.choice[i].width-5), game->menu.choice[2].y + game->menu.choice[2].height+7);
						if(choice == i)
							couleur("36");
					}	
					test = 0;	
				}		
			}
		}

		if(game->in.key[_P])
		{
			couleur("37");
			displayImageOnXY(&game->menu.border, game->w_cols/2 - game->menu.border.width/2, game->w_rows/2-game->menu.border.height/2);			
			displayImageOnXY(&game->menu.pause, (game->menu.border.x+game->menu.border.width/2) - game->menu.pause.width/2, game->menu.border.y+5);				
			move_to(game->menu.border.x + 20, game->menu.pause.y + game->menu.pause.height + 3);
			printf("Meilleur Score : %d" , game->lvl.scoreMax);			
			move_to(game->menu.border.x + 20, game->menu.pause.y + game->menu.pause.height + 5);
			printf("Votre score : %d" , game->player.score);	
			couleur("36");
			int x = game->menu.border.x+game->menu.border.width/2-14, y =game->menu.border.y + game->menu.border.height -6;
			move_to(x, y);
			couleur("47");
			couleur("35");
			printf("Press the \"ENTER\" key !!!");
			while(!game->in.key[_ENTER])
				updateInputs(&game->in);			
			game->in.key[_P] = 0;
			game->in.key[_ENTER] = 0;
			couleur("37");
			couleur("40");
			hideImage(&game->menu.border);
			hideImage(&game->menu.pause);	
			move_to(x, y);
			printf("                                    ");	
			initInputs(&game->in);
		}

		// menu
		if(game->displayMenu)
		{
			displayGameMenu(game);
			game->displayMenu = 0;
		}

		if((clock() - game->timeToWait > (game->lvl.enemySpeedMove*TIME_REG)) && game->beginAttack)
		{
			game->beginAttack = 0;
			game->lvl.enemyAttack = 1;
			game->lvl.timeToQuit = clock();
		}

		// si redimensionnement
		if(winCol-game->w_cols!=0 || winRow-game->w_rows!=0)
			resetWindowGameplay(game, &winCol, &winRow);

		// deplacer les étoiles
		moveStars(game);

		// deplacer les énnemies
		if(clock() - timeForEnemy_ms > game->lvl.enemySpeedMove*TIME_REG)
		{
			// Gère les vaisseaux ennemies
			EnemyTouchPlayer(game, game->lvl.enemy);
			manageVesselEnemys(game);
			timeForEnemy_ms = clock();
		}
		else
			displayEnemyVessel(game->lvl.enemy);

		EnemyTouchPlayer(game, game->lvl.enemySolo);
		manageVesselEnemysSolo(game);
		displayEnemyVessel(game->lvl.enemySolo);

		// Gère le vaisseau du joueur
		manageVesselPlayer(game);

		// deplacer les missiles
		if(clock() - time_ms > _SPEEDMOVEPROJ*TIME_REG)
		{
			couleur("35");
			moveProjectiles(game->player.v.proj, game->w_cols, game->w_rows, -1);
			projTouchPlayer(game, game->lvl.proj);
			couleur("36");
			moveProjectiles(game->lvl.proj, game->w_cols, game->w_rows, 1);
			couleur("37");
			time_ms = clock();
		}

		if(game->stabilityBar_change)
		{
			displayStabilityBar(game);
			game->stabilityBar_change = 0;
		}
		if(!testWave)
			waitNextWave = clock();
		if((game->lvl.enemy == NULL || game->lvl.enemy->first == NULL) && (game->lvl.enemySolo == NULL || game->lvl.enemySolo->first == NULL))
		{
			testWave = 1;
			if(clock() - waitNextWave > 3000*TIME_REG)
			{
				testWave = 0;
				waitNextWave = clock();
				reCallEnemy(game);
			}
		}	

		if(game->stabilityBar == 0 || game->player.nbVie <= 0)
		{
			displayGameMenu(game);
			game->over = 1;
			Image img;
			if(game->player.score > game->lvl.scoreMax)
			{
				loadImage(&img, "../images/newhighscore");
				couleur("35");
				displayImageOnXY(&img, game->w_cols/2 - img.width/2, game->w_rows/2 - img.height/2-5);				
				drawBox(game);
				viderBuffer();
				saveBestScore(game);
			}
			else
			{
				loadImage(&img, "../images/gameover");
				couleur("32");			
				displayImageOnXY(&img, game->w_cols/2 - img.width/2, game->w_rows/2 - img.height/2-5);	
				drawBox(game);
			}
			couleur("37");
			initInputs(&game->in);
			move_to(game->w_cols/2 - 15, game->w_rows/2 +18);
			couleur("47");
			couleur("35");
			printf("Press the \"ENTER\" key !!!");
			move_to(0, 0);
			while(!game->in.key[_ENTER])
				updateInputs(&game->in);
			game->in.quit = 1;
			deleteImage(&img);
		}

		// remet le curseur de la souris au coin superieur gauche
		move_to(game->w_cols,game->w_rows/2);
	}

	initInputs(&game->in);
	resetGameplay(game);
}