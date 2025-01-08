/*
constantes.h
------------

par lygolv, pour le projet de fin de Semestre

Rôle: Ce fichier contient toutes les constantes les constantes necessaires au bon fonctionnement du programme

Date de creation: 05/02/2020 01h49

Dernière modification: 03/05/2020
*/

#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

	#include <time.h>
	#include <sys/types.h>
	#include <sys/wait.h>

	// ================== DEFINES ======================

	// enleve le curseur du terminal
	#define disableCursor() printf("\033[?25l")
	#define enableCursor() printf("\033[?25h")

	// <<<<<<<<<<<<<<<<<<<<<<<<<< Define for Input >>>>>>>>>>>>>>>>>>>>>>>>>>>

	// Pour gerer les différentes touches à utiliser
	#define _NBKEYS 10

	// Touches
	#define _UP 0
	#define _DOWN 1
	#define _LEFT 2
	#define _RIGHT 3
	#define _SPACE 4
	#define _ENTER 5
	#define _P 6
	#define _H 7

	// <<<<<<<<<<<<<<<<<<<<<<<<<< Define for Projectile >>>>>>>>>>>>>>>>>>>>>>>>>>>

	#define _SPEEDMOVEPROJ 30

	// <<<<<<<<<<<<<<<<<<<<<<<<<< Define for Vessel >>>>>>>>>>>>>>>>>>>>>>>>>>>

	#define _PATH_VESSEL "../images/vaisseau"

	#define AKAv1 0
	#define AKAv2 1
	#define AKAv3 2
	#define AKAv4 3

	#define GMAa1 0
	#define GMAa2 1
	#define GMAa3 2
	#define GMAa4 3

	// <<<<<<<<<<<<<<<<<<<<<<<<<< Define for Menu >>>>>>>>>>>>>>>>>>>>>>>>>>>

	#define _MENUGAMEHEIGHT 3
	#define TIMETOWAIT_MS 500
	#define printToBold(str) printf("\033[1m%s\033[0m", str);

	// <<<<<<<<<<<<<<<<<<<<<<<<<< OTHERS DEFINE >>>>>>>>>>>>>>>>>>>>>>>>>>>

	// Nombre d'étoiles
	#define NB_STAR 200

	// la fonction clock() varie selon les machines
	#define TIME_REG ((CLOCKS_PER_SEC==1000000)?1000:1)
	
	#define clear() printf("\033c")
	// Efface l'écran et replace le curseur en haut à gauche
	#define clr() printf("\033[H\033[2J")
	// effacer le terminal "\033[J" (partie visible)
	// efface complet du terminal "\033c" ou "\033[1;1H\033[2J"
	
	// se place au coordonnées (x,y)
	#define move_to(x, y) printf("\033[%d;%dH", y, x);

	// Change le titre de la console
	#define setcsltitle(title) printf("\033]0;%s\007", title)	
	

	// Paramètre pour setattrib et setattribforbuf
	#define _BOLD 1
	#define _UNDERLINE 4
	#define _FLASH 5
	#define _SURLINE 7

	// Sélection de couleurs
	#define couleur(param) printf("\033[%sm",param)
	/*   param devant être un const char *, vide (identique à "0") ou formé
	     d'une où plusieurs valeurs séparées par des ; parmi
	         0  réinitialisation         1  haute intensité (des caractères)
	         5  clignotement             7  video inversé
	         30, 31, 32, 33, 34, 35, 36, 37 couleur des caractères
	         40, 41, 42, 43, 44, 45, 46, 47 couleur du fond
	            les couleurs, suivant la logique RGB, étant respectivement
	               noir, rouge, vert, jaune, bleu, magenta, cyan et blanc */

	// Fonction unique propre à aucun fihier
	void window_NbColsAndRows(int* w_col, int* w_row);
	int aleatoire(int max, int min);

	// ================== STRUCTURES ======================

	/* ----------> Gestion des inputs */
	typedef struct
	{	
		int key[_NBKEYS];
		int quit;		
	} Input;

	typedef struct 
	{
		int x, y;
		char img;
		char etat;		
	} Star;

	typedef struct projectile Projectile;
	struct projectile
	{
		int x;
		int y;
		char img;
		Projectile* next;
	};

	typedef struct 
	{
		Projectile* first;
	} ListProj;

	typedef struct
	{
		char* content;
		long t_content;
		int width, height;
		int x, y;
	} Image;

	typedef struct
	{
		Image img;
		ListProj* proj;
		int nbProj;
	} Vessel;

	typedef struct 
	{
		Vessel v;
		int score;
		int nbVie;
		char imgVie[7];
		int waitingTimeToShoot;
		int nbBalls;
	} Player;

	typedef struct enemy Enemy;
	struct enemy 
	{
		Vessel v;
		int point;
		int blind;
		Enemy* next;
		clock_t timeToMove;
		int timeToMove_ms;
		clock_t timeAfterShoot;
		int timeToShoot_ms;
		clock_t timeToShoot;
		int time_ms;
		/*
			* 0 pour le bas 
			* 1 pour Gauche
			* 2 pour droit
		*/
		int direction; // determine comment va se deplacer l'ennemis lorsqu'il quitte le groupe
		int bord; // verifie si un ennemi est situé au bord
	};

	typedef struct 
	{
		Enemy* first;
	} ListEnemy;

	typedef struct 
	{		
		ListEnemy* enemy;
		ListEnemy* enemySolo;
		int scoreMax;
		int nbEnemy;
		int enemyByLine;
		int enemySpeedMove;
		int direction;
		int nbEnemySolo;
		int enemyAttack;
		int timeToQuit_ms;
		clock_t timeToQuit;
		int timeToShoot_ms;
		clock_t timeToShoot;
		ListProj* proj;
		char bestPlayerName[21];
		// pour le niveau progressif
		int percentEnemy1;
		int percentEnemy2;
		int percentEnemy3;
		int percentEnemy4;
	} Level;

	typedef struct 
	{
		Image title;
		Image border;
		Image* mode;
		Image* choice;
		Image pause;
		int nbMode;
		int nbChoice;
	} Menu;

	typedef struct
	{
		Player player;
		Level lvl;
		int over;
		int mode;
		int nbLvlPass;
		Input in;
		int w_cols, w_rows;
		int displayMenu;
		Menu menu;
		int stabilityBar; // la stabilité du vaisseaux l'espace
		int stabilityBar_change;
		pid_t parent;
		pid_t child;
		int status;
		Star star[NB_STAR];	
		clock_t timeToWait;
		int timeToMoveStars_ms;
		clock_t timeToMoveStars;
		int beginAttack;	
	} Game;	

#endif