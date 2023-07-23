#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define L 20 // lado da rede
#define N (L*L) // numero de sitios
#define S 189 // sitio inicial
#define T 1000 // numero maximo de passos
#define W 1000000 // numero de amostras

void inicio_sitios(bool *sitios, int s);
void matriz_vizinhanca(int **vizinhanca);
void passo(int **vizinhanca, bool *sitios, int *s, float *x, float *y, bool *done);

int main(){

	srand(time(NULL));

	int n, t, w; // variaveis auxiliares

	int s;
	float x, y;
	bool done;

	FILE *file1 = fopen("trajetoria_c.dat", "w");
	FILE *file2 = fopen("msd_c.dat", "w");

	bool *sitios = (bool*)malloc(N * sizeof(bool));

	int **vizinhanca = (int**)malloc(N * sizeof(int*)); 
	for(n = 0; n < N; n++)
		vizinhanca[n] = (int*)malloc(4 * sizeof(int));

	float *MSD = (float*)malloc(T * sizeof(float));
	for(t = 0; t < T; t++)
		MSD[t] = 0;

	float *NMSD = (float*)malloc(T * sizeof(float));
	for(t = 0; t < T; t++)
		NMSD[t] = 0;

	// construcao da matriz de vizinhanca
	matriz_vizinhanca(vizinhanca);

	// dinamica 
	for(w = 0; w < W; w++){

		s = S;
		x = y = 0;

		fprintf(file1, "Amostra %d\n%d\t%f\t%f\t%d\n", w, 0, x, y, s);

		inicio_sitios(sitios, s);

		t = 1;
		done = 0;

		while(t < T && !done){
			
			passo(vizinhanca, sitios, &s, &x, &y, &done);

			MSD[t] += (pow(x, 2) + pow(y, 2));
			NMSD[t]++;
		
			fprintf(file1, "%d\t%f\t%f\t%d\n", t, x, y, s);

			t++;
		}
	}

	for(t = 0; t < T; t++)
		if(NMSD[t] == 0)
			fprintf(file2, "%d\t%f\t%f\n", t, 0., 0.);
		else
			fprintf(file2, "%d\t%f\t%f\n", t, MSD[t]/NMSD[t], NMSD[t]);

	fclose(file1);
	fclose(file2);

	return 0;
}

void inicio_sitios(bool *sitios, int s){

	int n;

	for (n = 0; n < N; n++){
		if (n == s)
			sitios[n] = 1;
		else
			sitios[n] = 0;
	}

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

void passo(int **vizinhanca, bool *sitios, int *s, float *x, float *y, bool *done){

	int n, v, o = 0;
	int xaux = 0, yaux = 0; 

	switch(rand() % 4){
	case 0:
		xaux++;
		n = vizinhanca[*s][0];
		break;
	case 1:
		yaux--;
		n = vizinhanca[*s][1];
		break;
	case 2:
		xaux--;
		n = vizinhanca[*s][2];
		break;
	case 3:
		yaux++;
		n = vizinhanca[*s][3];
		break;
	};

	if(!sitios[n]){ // se já não foi visitado
		*x += xaux;
		*y += yaux;
		*s = n;
		sitios[n] = 1;
	} // caso contrário faz nada

	for(v = 0; v < 4; v++) o += sitios[vizinhanca[*s][v]];
	if(o == 4) *done = 1; // se caminhante está encurralado
}
