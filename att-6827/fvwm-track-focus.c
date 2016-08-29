/*
 * fvwm-track-focus.c
 * Peter McAlpine, 2002
 * (hopefully) can be PipeRead from fvwm to raise windows with GotoPage
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXX 5
#define MAXY 5
#define MAX_STR_LEN 64

int errno;

int main(int argc, char *argv[]) {
     FILE *fdat = NULL;
     char idara[MAXX][MAXY][MAX_STR_LEN];
     int j, k;
     int x, y;

     if (argc == 1) {
	  fprintf(stderr, "fvwm-track-focus - by Peter McAlpine\n");
	  fprintf(stderr, "Usage:\n");
	  fprintf(stderr, "      fvwm-track-focus FILE init\n");
	  fprintf(stderr, "      fvwm-track-focus FILE x y\n");
	  fprintf(stderr, "      fvwm-track-focus FILE x y WINDOWID\n");
	  return 1;
     }

     /* handle init c.l. opt */
     if (strcmp(argv[2], "init") == 0) {
	  if ((fdat = fopen(argv[1], "w+")) == NULL) {
	       fprintf(stderr, "rats, could not open file\n");
	       return errno;
	  }
	  j = 0;
	  while (j < MAXX) {
	       k = 0;
	       while (k < MAXY) {
		    fprintf(fdat, "NULL\n");
		    k++;
	       }
	       j++;
	  }
	  fclose(fdat);

	  return 0;
     }

     /* init ara */
     fdat = fopen(argv[1], "r+");
     if (fdat == NULL) {
	  fprintf(stderr, "rats, couldn't open file: %s\n", argv[1]);
	  return errno;
     }
     x = atoi(argv[2]);
     y = atoi(argv[3]);

     /* read ara from dat file */
     j = 0;
     while (j < MAXX) {
	  k = 0;
	  while (k < MAXY) {
	       fscanf(fdat, "%s\n", idara[j][k]);
	       k++;
	  }
	  j++;
     }

     if (argc == 4) {	  // read id request
	  if (strcmp(idara[x][y], "NULL") == 0) {
	       printf("Next [CurrentPage !iconic] Focus\n");
	  } else {
	       printf("WindowId %s Focus NoWarp\n", idara[x][y]);
	  }
     } else {	  // write id request
	  strcpy(idara[x][y], argv[4]);
	  fclose(fdat);
	  fdat = fopen(argv[1], "w+");
	  j = 0;
	  while (j < MAXX) {
	       k = 0;
	       while (k < MAXY) {
		    fprintf(fdat, "%s\n", idara[j][k]);
		    k++;
	       }
	       j++;
	  }
     }

     fclose(fdat);

     return 0;
}

