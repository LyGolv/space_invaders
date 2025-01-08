#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../inc/vessel.h"
#include "../inc/projectile.h"
#include "../inc/image.h"


void loadVessel(Vessel* v, char str[])
{
	loadImage(&v->img, str);
}

void changeCoord(Vessel* v, int x, int y)
{
	v->img.x = x;
	v->img.y = y;
}

void initVessel(Vessel* v, int x, int y)
{
	changeCoord(v, x, y);
	v->nbProj =0;
	v->proj = NULL;
}

void displayVessel(Vessel* v)
{
	displayImage(&v->img);
}

void clearVessel(Vessel* v)
{
	hideImage(&v->img);
}

void moveVessel(Vessel* v, int x, int y)
{
	move_to(v->img.x, v->img.y);
	clearVessel(v);
	changeCoord(v, x, y);
	move_to(v->img.x, v->img.y);
	displayVessel(v);
}

ListProj* initFirstProj(int x, int y)
{
    ListProj *list = malloc(sizeof(*list));
    Projectile *tmp = malloc(sizeof(*tmp));
    if (list == NULL || tmp == NULL)
        exit(EXIT_FAILURE);
    initProjectile(tmp, x, y, '|');
    tmp->next = NULL;
    list->first = tmp;

    return list;
}

void addNewProj(ListProj *list, int x, int y)
{
    /* Création du nouvel élément */
    Projectile* new = malloc(sizeof(*new));
    if (list == NULL || new == NULL)
        exit(EXIT_FAILURE);
    initProjectile(new, x, y, '|');

    /* Insertion de l'élément au début de la liste */
    new->next = list->first;
    list->first = new;
}

void throwProjectile(Vessel* v)
{
	int x = (v->img.x+(v->img.x+v->img.width))/2, y = v->img.y;
	if(v->proj == NULL)
		v->proj = initFirstProj(x, y);
	else
		addNewProj(v->proj, x, y);
	++v->nbProj;
}

void moveProjectiles(ListProj *list, int winWidth, int winHeight, int vitesse)
{
	if(list == NULL)
		return;
	Projectile* p = list->first;
	while(p != NULL)
	{
		if(projScreenTouch(p, winWidth, winHeight))
		{
			Projectile* pSupp = p;
			p = p->next;
			destroyProjectile(list, pSupp);
			continue;
		}
		moveProjectile(p, p->x, p->y+vitesse);
		p = p->next;
	}
}

void destroyProjectile(ListProj *list, Projectile* pSupp)
{
	if(list == NULL)
		return;
	Projectile* p = list->first;
	if(list->first == pSupp)
		list->first = pSupp->next;
	else
	{
		Projectile* p1 = p;
		while(p != pSupp)
		{
			p1 = p;
			p = p->next;
		}
		p=p->next;
		p1->next = p;
	}
	move_to(pSupp->x, pSupp->y);
	hideProjectile();
	free(pSupp);
}

void deleteProjectiles(Projectile* pjtl)
{
	Projectile* p=pjtl;
	while(p != NULL)
	{
		move_to(p->x, p->y);
		hideProjectile();
		Projectile* p1=p->next;
		free(p);
		p=p1;
	}
}

void deleteVessel(Vessel* v)
{
	deleteImage(&v->img);
	if(v->proj != NULL)
	{
		deleteProjectiles(v->proj->first);
		free(v->proj);
	}
	v->proj = NULL;
}