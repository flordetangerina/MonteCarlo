#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define L 20 // lado da rede
#define N (L*L) // numero de sitios
#define S 189 // sitio inicial
#define T 500 // numero de passos

void matriz_vizinhanca(int **vizinhanca);
void passo(int **vizinhanca, int *s, float *x, float *y);

int main(){

	srand(time(NULL));

	int n, t;
	int s = S;
	float x = 0, y = 0;
	
	FILE *file = fopen("trajetoria_b.dat", "w");

	int **vizinhanca = (int**)malloc(N * sizeof(int*)); 
	for (n = 0; n < N; n++)
		vizinhanca[n] = (int*)malloc(4 * sizeof(int));

	// construcao da matriz de vizinhanca
	matriz_vizinhanca(vizinhanca);

	// impressao do estado inicial
	fprintf(file, "%d\t%f\t%f\t%d\n", 0, x, y, s);

	// dinamica 
	for (t = 1; t < T; t++){

		passo(vizinhanca, &s, &x, &y);

		fprintf(file, "%d\t%f\t%f\t%d\n", t, x, y, s);	
	}

	fclose(file);

	return 0;
}

void matriz_vizinhanca(int **vizinhanca){

	int n;

	for (n = 0; n < N; n++){
		// direita
		if ((n % L) == (L - 1)) // ultima coluna
			vizinhanca[n][0] = n + 1 - L;
		else 
			vizinhanca[n][0] = n + 1;
		// abaixo
		if (n >= (N - L)) // ultima linha
			vizinhanca[n][1] = (n % L); 
		else 
			vizinhanca[n][1] = n + L;
		// esquerda
		if ((n % L) == 0) // primeira coluna
			vizinhanca[n][2] = n - 1 + L; 
		else 
			vizinhanca[n][2] = n - 1;
		// acima
		if (n < L) // primeira linha
			vizinhanca[n][3] = n - L + N; 
		else 
			vizinhanca[n][3] = n - L;
	}
}

void passo(int **vizinhanca, int *s, float *x, float *y){

	switch(rand() % 4){
	case 0:
		(*x)++;
		*s = vizinhanca[*s][0];
		break;
	case 1:
		(*y)--;
		*s = vizinhanca[*s][1];
		break;
	case 2:
		(*x)--;
		*s = vizinhanca[*s][2];
		break;
	case 3:
		(*y)++;
		*s = vizinhanca[*s][3];
		break;
	}

}
