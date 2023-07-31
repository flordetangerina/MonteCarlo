#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define L 100 // lado da rede
#define N (L*L) // numero de sitios
#define A 1000 // numero de amostras

void matriz_vizinhanca(int **vizinhanca);
void povoar_sitios(bool *sitios, float P);
void colocar_fogo(bool *fire, bool *sitios);
bool propagar_fogo(bool *sitios, bool *fire, int **vizinhanca);

int main(){

	srand(time(NULL));

	int n, p, a, t, tmed;
	bool done;
	float P;

	char filename[20] = {0};
    sprintf(filename, "tempo_L%d.dat", L);

	FILE *file1 = fopen(filename, "w");

	bool *sitios = (bool*)malloc(N * sizeof(bool));
	bool *fire = (bool*)malloc(N * sizeof(bool));

	int **vizinhanca = (int**)malloc(N * sizeof(int*)); 
	for (n = 0; n < N; n++)
		vizinhanca[n] = (int*)malloc(4 * sizeof(int));

	matriz_vizinhanca(vizinhanca);

	for(p = 30; p < 90; p++){

		tmed = 0;

		P = ((float)p)/100;
		printf("Povoacao %f\n", P);

		for(a = 0; a < A; a++){

			// inicializacao
			povoar_sitios(sitios, P);
			colocar_fogo(fire, sitios);

			t = 1;
			done = 0;

			// dinamica
			while(!done){
				done = propagar_fogo(sitios, fire, vizinhanca);
				t++;
			}

			tmed += t;			
		}

		fprintf(file1, "%f\t%d\n", P, tmed/A);
	}

	fclose(file1);

	return 0;
}

void povoar_sitios(bool *sitios, float P){

	int n;
	float r;

	for (n = 0; n < N; n++){

		r = ((float)rand()/(float)RAND_MAX);

		if (r < P)
			sitios[n] = 1;
		else
			sitios[n] = 0;
	}

}

void colocar_fogo(bool *fire, bool *sitios){

	int n;

	for (n = 0; n < N; n++){
		if (n < L && sitios[n]) 
			fire[n] = 1;
		else
			fire[n] = 0;
	}

}

bool propagar_fogo(bool *sitios, bool *fire, int **vizinhanca){

	int n, v, vizinho;

	bool done = 1;

	for(n = 0; n < N; n++){
		if(fire[n]){
			for(v = 0; v < 4; v++){
				vizinho = vizinhanca[n][v];
				if(!fire[vizinho] && sitios[vizinho]){
					fire[vizinho] = 1;
					done = 0;
				}
			}
		}
	}

	return done;

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

