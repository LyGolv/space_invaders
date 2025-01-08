#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../inc/globals.h"
#include "../inc/image.h"

void initImage(Image* image)
{
	image->content = NULL;
	image->t_content = 0;
	image->x = 0;
	image->y = 0;
	image->width = 0;
	image->height = 0;
}

void loadImage(Image* image, char* pathImage)
{
	long i=0;
	char str[3];
	FILE* file = fopen(pathImage, "r+");
	if(file == NULL)
	{
		printf("Impossible d'acceder à : \"%s\"\n", pathImage);
		exit(-1);
	}
	fseek(file, 0, SEEK_END);
	image->t_content = ftell(file);
	rewind(file);
	image->content = malloc(image->t_content* sizeof(char*));
	image->height=0;
	if(fgets(str, 3, file) == NULL)
	{
		printf("\nLecture Impossible du fichier: \"%s\"\n" , pathImage);
		exit(EXIT_FAILURE);
	};
	image->width = atoi(str);
	image->t_content -= ftell(file); // taille - la première ligne
	while(i<image->t_content)
	{	
		image->content[i] = fgetc(file);
		if(image->content[i] == '\n')
			++image->height;
		++i;
	}
	fclose(file);
}

void displayImage(Image* image)
{
	long i=0;
	int y=image->y;
	while(i<image->t_content)
	{
		printf("%c", image->content[i]);
		if(image->content[i] == '\n')
		{
			y++;
			move_to(image->x, y);
		}		
		 ++i;
	}
}

void displayImageOnXY(Image* image, int x, int y)
{
	move_to(x, y);
	image->x=x;
	image->y=y;
	displayImage(image);
}

void hideImage(Image* image)
{
	long i=0;
	int y=image->y;
	while(i<image->t_content)
	{
		printf(" ");
		if(image->content[i] == '\n')
		{
			y++;
			move_to(image->x, y);
		}		
		++i;
	}
}

void deleteImage(Image* image)
{
	if(image->content != NULL)
		free(image->content);	
}

Image* loadMultipleImage(char* pathImage, int* nbImg)
{
	Image* image;
	FILE* file = fopen(pathImage, "r+");
	long t_max, nbImage=1, i=0, k=0, cpt = 0;
	if(file == NULL)
	{
		printf("Impossible d'acceder à : \"%s\"\n", pathImage);
		exit(-1);
	}
	fseek(file, 0, SEEK_END);	
	t_max = ftell(file);
	rewind(file);
	char* str = malloc(t_max* sizeof(char*));
	while(i < t_max && ++i)
	{
		str[i-1] = fgetc(file);
		if(str[i-1] == '#')
			++nbImage;
	}
	fclose(file);
	image = malloc(nbImage * sizeof(*image));
	i=0;
	*nbImg = nbImage;
	++nbImage;
	while(--nbImage)
	{
		cpt=0;
		long taille = 0;
		char num[4];
		while(str[i] != '\n')
		{
			num[cpt] = str[i];
			++i; ++cpt;
		}
		++i;
		num[cpt] = '\0';
		image[k].width = atoi(num);	
		while(str[i] != '#' && ++taille && ++i && i<t_max);
		i+=2;
		image[k].content = malloc(taille*sizeof(char*));
		cpt = 0;
		image[k].t_content = taille-1;
		image[k].height=0;
		while(--taille)
		{
			image[k].content[cpt] = str[i-taille-3];
			if(image[k].content[cpt] == '\n')
				++image[k].height;
			++cpt;
		}
		image[k].content[cpt] = '\0';
		++k;
	}
	free(str);

	return image;
}