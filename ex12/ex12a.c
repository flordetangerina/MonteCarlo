#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 1000 // numero de amostras
#define T 500 // numero de passos

int main(){

	srand(time(NULL));

	FILE *file1 = fopen("msd_a.dat", "w");
	FILE *file2 = fopen("trajetoria_a.dat", "w");

	float *MSD = (float*)malloc(sizeof(float)*T);

	int n, t;
	float x, y;

	for(n = 0; n < N; n++){

		x = y = 0;

		for(t = 0; t < T; t++){

			switch(rand() % 4){
			case 0:
				x++;
				break;
			case 1:
				y--;
				break;
			case 2:
				x--;
				break;
			case 3:
				y++;
				break;
			}

			MSD[t] += (pow(x, 2) + pow(y, 2));

			if(n == 0)
				fprintf(file2, "%d\t%f\t%f\n", t, x, y);
			
		}
	}

	for(t = 0; t < T; t++)
		fprintf(file1, "%d\t%f\n", t, MSD[t]/N);

	fclose(file1);
	fclose(file2);

	return 0;
}

