#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define L 5 // lado da rede
#define N (L*L) // numero de sitios

void construir_matriz(int **vizinhanca);
void gerar_configuracoes(int *sitios);
void fazer_medidas(int *sitios, int **vizinhanca, float *u, float *m);

int main(){

	FILE *file;
	char filename[50];
	int seed = time(NULL);

	// para nao gerar amostras com mesma seed
	bool ok = 0;
	while(!ok){
		sprintf(filename, "./samples/results_L%d_%d.dat", L, seed);
		file = fopen(filename, "r");
		if(file != NULL){
			seed++;
			fclose(file);
		}else ok = 1;
	}

	srand(seed);

	file = fopen(filename, "w");
	fprintf(file, "Modelo de Ising com Temperatura Infinita\n");
	fprintf(file, "VTH 08/2023\n\n");
	fprintf(file, "REDE DE LADO %d SEED %d\n\n", L, seed);

	int n, a; // variaveis auxiliares
	float u = 0, m = 0; // variaveis medidas

	int *sitios = (int*)malloc(N * sizeof(int));

	int **vizinhanca = (int**)malloc(N * sizeof(int*)); 
	for (n = 0; n < N; n++)
		vizinhanca[n] = (int*)malloc(4 * sizeof(int));

	construir_matriz(vizinhanca);
	gerar_configuracoes(sitios);
	fazer_medidas(sitios, vizinhanca, &u, &m);

	fprintf(file, "  M/N\t  U/N\n");
	fprintf(file, "%.3f\t%.3f\n", m, u);

	fclose(file);

	return 0;
}

void gerar_configuracoes(int *sitios){

	int n;

	for(n = 0; n < N; n++){

		switch(rand() % 2){
		case 0:
			sitios[n] = 1;
			break;
		case 1:
			sitios[n] = -1;
			break;	
		}

	}

}

void fazer_medidas(int *sitios, int **vizinhanca, float *u, float *m){

	int n, v, vizinho;
	
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

