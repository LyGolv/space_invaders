#include <stdio.h>
#include <stdlib.h>

#include "../inc/globals.h"
#include "../inc/projectile.h"

void initProjectile(Projectile* p, int x, int y, char img)
{
	p->x = x; 
	p->y = y;
	p->img = img;
}

void showProjectile(Projectile* p)
{
	printf("%c", p->img);		
}

void hideProjectile()
{
	printf(" ");
}

void moveProjectile(Projectile* p, int x, int y)
{
	move_to(p->x, p->y);
	hideProjectile(p);
	initProjectile(p, x, y, p->img);
	move_to(p->x, p->y);
	showProjectile(p);
}

int projScreenTouch(Projectile* p, int winWidth, int winHeight)
{
	if(p->y - 4 < _MENUGAMEHEIGHT + 1)
		return 1;
	else if(p->x - 1 < 0)
		return 1;
	else if(p->x > winWidth + 1)
		return 1;
	else if(p->y + 3 > winHeight)
		return 1;
	return 0;
}