//
// Created by lygolv on 08/01/2025.
//

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

// Récupère la taille du terminal
void window_NbColsAndRows(int* w_col, int* w_row)
{
    /*
        The "winsize" structure is defined in `sys/ioctl.h' as follows:
        struct winsize
        {
          unsigned short ws_row;	// rows, in characters
          unsigned short ws_col;	// columns, in characters
          unsigned short ws_xpixel;	// horizontal size, pixels
          unsigned short ws_ypixel;	// vertical size, pixels
        };
    */
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *w_row = w.ws_row;
    *w_col = w.ws_col;
}

int aleatoire(int max, int min)
{
    return (rand() % max) + min;
}