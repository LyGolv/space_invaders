#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

	void initImage(Image* image);
	void loadImage(Image* image, char* pathImage);
	void displayImage(Image* image);
	void displayImageOnXY(Image* image, int x, int y);
	void hideImage(Image* image);
	void deleteImage(Image* image);
	Image* loadMultipleImage(char* pathImage, int* nbImg);

#endif