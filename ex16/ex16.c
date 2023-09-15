#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define I 1 // caso inicial
#define T 2.27 // temperatura
#define L 100 // lado da rede
#define N (L*L) // numero de sitios
#define TEQU 100 // tempo de transiente
#define TMAX 1000 // tempo de equilibrio

void construir_matriz(int **vizinhanca);
void iniciar_sitios(int *sitios);
void tentar_inverter(int *sitios, int **vizinhanca);
void fazer_medidas(int estado, int espacial, int *sitios, int *sitios0, int **vizinhanca, float *correlacao, float *u, float *m, float *m0, float *ct);

int main(){
	
	FILE *file;
	char filename[50];
	int seed = time(NULL);

	// para nao gerar amostras com mesma seed
	bool ok = 0;
	while(!ok){
		sprintf(filename, "./samples/L%d/T%.2f/results_%d.dat", L, T, seed);
		file = fopen(filename, "r");
		if(file != NULL){
			seed++;
			fclose(file);
		}else ok = 1;
	}

	srand(seed);

	file = fopen(filename, "w");
	fprintf(file, "Modelo de Ising\n");
	fprintf(file, "VTH 08/2023\n\n");
	fprintf(file, "REDE DE LADO %d SEED %d\n", L, seed);
	fprintf(file, "TEMPERATURA %.2f\n\n", T);
	fprintf(file, "t M/N U/N C(t)\n");
	
	int n, t, ncr = 0; // variaveis auxiliares
	float u, m, m0, ct; // variaveis medidas

	int *sitios = (int*)malloc(N * sizeof(int));
	int *sitios0 = (int*)malloc(N * sizeof(int));

	int **vizinhanca = (int**)malloc(N * sizeof(int*)); 
	for(n = 0; n < N; n++) vizinhanca[n] = (int*)malloc(4 * sizeof(int));

	float *correlacao = (float*)malloc(L/2 * sizeof(float));
	for(n = 0; n < L/2; n++) correlacao[n] = 0;

	iniciar_sitios(sitios);
	construir_matriz(vizinhanca);
	
	// transiente
	for(t = 1; t < TEQU; t++){
		for(n = 0; n < N; n++) 
			tentar_inverter(sitios, vizinhanca);
	}
	
	fazer_medidas(0, 0, sitios, sitios0, vizinhanca, correlacao, &u, &m, &m0, &ct);
	fprintf(file, "%d %.3f %.3f %.3f\n", 0, m, u, ct);

	// equilibrio
	for(t = 1; t < TMAX; t++){

		for(n = 0; n < N; n++) 
			tentar_inverter(sitios, vizinhanca);

		if((t % 100) == 0){ 
			fazer_medidas(1, 1, sitios, sitios0, vizinhanca, correlacao, &u, &m, &m0, &ct);
			ncr++;
		}else 
			fazer_medidas(1, 0, sitios, sitios0, vizinhanca, correlacao, &u, &m, &m0, &ct);

		fprintf(file, "%d %.3f %.3f %.3f\n", t, m, u, ct);
	}

	fprintf(file, "\nr C(r)\n");

	for(n = 0; n < L/2; n++)
		fprintf(file, "%d %.3f\n", n + 1, correlacao[n]/ncr);

	fclose(file);

	return 0;
}

void iniciar_sitios(int *sitios){

	int n;

	for(n = 0; n < N; n++){

		switch(rand() % 2){
		case 0:
			sitios[n] = 1;
			break;
		case 1:
			if(I == 0) sitios[n] = -1;
			else if(I == 1) sitios[n] = 1;
			break;	
		}

	}

}

void tentar_inverter(int *sitios, int **vizinhanca){

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

void fazer_medidas(int estado, int espacial, int *sitios, int *sitios0, int **vizinhanca, float *correlacao, float *u, float *m, float *m0, float *ct){

	float c;
	int n, v, vizinho, vizdir, vizbai;

	(*m) = (*u) = 0;
	
	for(n = 0; n < N; n++){

		(*m) += sitios[n];

		for(v = 0; v < 2; v++){
			vizinho = vizinhanca[n][v];
			(*u) -= sitios[n]*sitios[vizinho];
		}

	}
	
	(*m) /= N;
	(*u) /= N;
	
	// correlacao temporal
	switch(estado){
	case 0:
		for(n = 0; n < N; n++) sitios0[n] = sitios[n];
		(*m0) = (*m);
		break;
	case 1:
		(*ct) = 0;
		for(n = 0; n < N; n++) (*ct) += sitios[n]*sitios0[n];
		(*ct) /= N;
		(*ct) -= (*m)*(*m0);
		break;
	}

	// correlacao espacial
	if(espacial){

		for(n = 0; n < N; n++){
			vizdir = vizinhanca[n][0];
			vizbai = vizinhanca[n][1];
			for(v = 0; v < L/2; v++){
				c = sitios[n]*(sitios[vizdir] + sitios[vizbai]);
				vizdir = vizinhanca[vizdir][0];
				vizbai = vizinhanca[vizbai][1];
				correlacao[v] += c;
			}
		}

		for(v = 0; v < L/2; v++){
			correlacao[v] /= (2*N);
			//correlacao[v] -= (*m)*(*m);
		}
	}

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

