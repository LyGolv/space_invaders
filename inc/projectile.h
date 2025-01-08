#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

	#include "../inc/globals.h"

	void initProjectile(Projectile* p, int x, int y, char img);
	void showProjectile(Projectile* p);
	void hideProjectile();
	void moveProjectile(Projectile* p, int x, int y);
	int projScreenTouch(Projectile* p, int winWidth, int winHeight);

#endif