#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define L 100 // lado da rede
#define N (L*L) // numero de sitios
#define TEQU 1000 // tempo de transiente
#define TMAX 5000 // tempo de equilibrio

void construir_matriz(int **vizinhanca);
void iniciar_sitios(int *sitios);
void tentar_inverter(int *sitios, int **vizinhanca, float T);
void fazer_medidas(int *sitios, int **vizinhanca, float *u, float *m);

int main(){
	
	int seed = time(NULL);
	srand(seed);

	FILE *file;
	char filename[30];
	sprintf(filename, "./L%d.dat", L);
	file = fopen(filename, "w");

	fprintf(file, "Modelo de Ising\n");
	fprintf(file, "VTH 08/2023\n\n");
	fprintf(file, "REDE DE LADO %d SEED %d\n", L, seed);
	
	int n, i, t;
	float m, u, T;

	int *sitios = (int*)malloc(N * sizeof(int));

	int **vizinhanca = (int**)malloc(N * sizeof(int*)); 
	for(n = 0; n < N; n++)
		vizinhanca[n] = (int*)malloc(4 * sizeof(int));

	iniciar_sitios(sitios);
	construir_matriz(vizinhanca);

	for(i = 200; i < 301; i++){
		
		T = (float) i/100;

		printf("Temperatura %.2f\n", T);
		fprintf(file, "\nTEMPERATURA %.2f\n", T);

		for(t = 1; t < TEQU; t++)
			for(n = 0; n < N; n++) tentar_inverter(sitios, vizinhanca, T);

		t = 0;

		fazer_medidas(sitios, vizinhanca, &u, &m);
		fprintf(file, "%d %.3f %.3f\n", t, m, u);

		for(t = 1; t < TMAX; t++){
			for(n = 0; n < N; n++) tentar_inverter(sitios, vizinhanca, T);
			if(t % 100 == 0){
				fazer_medidas(sitios, vizinhanca, &u, &m);
				fprintf(file, "%d %.3f %.3f\n", t, m, u);
			}
		}
	}

	fclose(file);

	return 0;
}

void iniciar_sitios(int *sitios){

	int n;

	for(n = 0; n < N; n++) sitios[n] = 1;

}

void tentar_inverter(int *sitios, int **vizinhanca, float T){

	float r;
	int v, n;
	int vizinho, delta = 0;
	
	n = rand() % N;

	for(v = 0; v < 4; v++){
		vizinho = vizinhanca[n][v];
		delta += sitios[vizinho];
	}
	
	delta *= 2*sitios[n];

	if(delta <= 0)
		sitios[n] *= -1;
	else{
		r = ((double)rand()/(double)(RAND_MAX)) * 1.;
		if (r < exp(-delta/T)) 
			sitios[n] *= -1;
	}

}

void fazer_medidas(int *sitios, int **vizinhanca, float *u, float *m){

	int n, v, vizinho;

	(*u) = (*m) = 0;
	
	for(n = 0; n < N; n++){

		(*m) += sitios[n];

		for(v = 0; v < 2; v++){
			vizinho = vizinhanca[n][v];
			(*u) -= sitios[n]*sitios[vizinho];
		}

	}
	
	(*m) /= N;
	(*u) /= N;

}

void construir_matriz(int **vizinhanca){

	int n, v;

	for(n = 0; n < N; n++){
		// direita
		if((n % L) == (L - 1)) // ultima coluna
			vizinhanca[n][0] = n + 1 - L;
		else 
			vizinhanca[n][0] = n + 1;
		// abaixo
		if(n >= (N - L)) // ultima linha
			vizinhanca[n][1] = (n % L); 
		else 
			vizinhanca[n][1] = n + L;
		// esquerda
		if((n % L) == 0) // primeira coluna
			vizinhanca[n][2] = n - 1 + L; 
		else 
			vizinhanca[n][2] = n - 1;
		// acima
		if(n < L) // primeira linha
			vizinhanca[n][3] = n - L + N; 
		else 
			vizinhanca[n][3] = n - L;
	}

}

