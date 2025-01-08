/*
constantes.h
------------

par Lioguy, pour le jeu space invaders

Rôle: Permet de gérer le Vaisseau du joueur

Date de creation: 08/10/2021 14h16

Dernière modification: 08/10/2021 14h16
*/

#ifndef VESSEL_H_INCLUDED
#define VESSEL_H_INCLUDED

	#include "globals.h"

	void loadVessel(Vessel* v, char str[]);
	void changeCoord(Vessel* v, int x, int y);
	void initVessel(Vessel* v, int x, int y);
	void displayVessel(Vessel* v);
	void clearVessel(Vessel* v);
	void moveVessel(Vessel* v, int x, int y);
	void deleteVessel(Vessel* v);
	ListProj* initFirstProj(int x, int y);
	void addNewProj(ListProj *liste, int x, int y);
	void throwProjectile(Vessel* v);
	void moveProjectiles(ListProj *liste, int winWidth, int winHeight, int vitesse);
	void deleteProjectiles(Projectile* pjtl);
	void destroyProjectile(ListProj *liste, Projectile* pSupp);

#endif