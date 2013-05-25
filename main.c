/*
 * main.c
 *
 *  Created on: May 24, 2013
 *      Author: ee03070
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "heap.h"


int main( int argc, char **argv ) {

	FILE *r, *w;
	heap *fem, *mas;
	int cont, ano, cod, m=1, f=2, pop_m, pop_f, v;
	char linha[150];
	if(argc!=4) printf("Número invalido de argumentos!\n");

	/*abrir o ficheiro de leitura e escrita*/
	if ( ( r = fopen ( argv[1], "r" ) ) == NULL )
		printf("O ficheiro de leitura não foi corretamente aberto!");

	//temos de fazer o ficheiro de saida????????????????????????????????????????????????????????????????????
	if ( ( w = fopen ( "output.csv", "w" ) ) == NULL )
		printf("O ficheiro de escrita nao foi aberto corretamente!");

	ano=atoi(argv[2]);
	cod=atoi(argv[3]);
	if ( ( fem = heap_cria () ) == NULL )
		printf("Heap não foi criada corretamente!");
	if ( ( mas = heap_cria () ) == NULL )
		printf("Heap não foi criada corretamente!");
	/*ler até ao fim do ficheiro*/
	for ( cont=0; fgets(linha, 150, r) != NULL; cont++){

		unsigned int decada = atoi (strtok ( linha, "," ));

		if(decada == ano-(ano%10))
		{
			unsigned int idade_intervalos = atoi (strtok ( NULL, "," ));

			int estado_civil = atoi(strtok ( NULL, "," ));
			if(estado_civil==cod)
			{
				int genero = atoi(strtok ( NULL, "," ));
				if(genero==m){

					unsigned int populacao = atoi ( strtok ( NULL, "," ));
					pop_m+=populacao;

					/*adicionar o elemento à heap*/
					v=heap_insere ( mas, decada, idade_intervalos, estado_civil, genero, populacao, populacao);

					if  ( v != 1 )
						puts("Os dados não foram devidamente inseridos na Heap");
				}
				if(genero==f){

					unsigned int populacao = atoi ( strtok ( NULL, "," ));
					pop_f+=populacao;

					/*adicionar o elemento à heap*/
					v=heap_insere ( fem, decada, idade_intervalos, estado_civil, genero, populacao, populacao);

					if  ( v != 1 )
						puts("Os dados não foram devidamente inseridos na Heap");
				}

			}
		}
	}

	if(pop_m>pop_f)
	mostraHeap(mas,0);
		//fprintf(w, "%d, %d, %d, %d, %d (%d)\n",mas->elementos[cont]->valor->decada, mas->elementos[cont]->valor->idade_intervalos, mas->elementos[cont]->valor->estado_civil, mas->elementos[cont]->valor->genero, mas->elementos[cont]->valor->populacao, mas->elementos[cont]->prioridade);
	else
	mostraHeap(fem,0);
		//fprintf(w, "%d, %d, %d, %d, %d (%d)\n",fem->elementos[cont]->valor->decada, fem->elementos[cont]->valor->idade_intervalos, fem->elementos[cont]->valor->estado_civil, fem->elementos[cont]->valor->genero, fem->elementos[cont]->valor->populacao, fem->elementos[cont]->prioridade);


	fclose(r);
	fclose(w);

	return 0;
}
