/*****************************************************************/
/*   Fila de prioridade / min-heap | PROG2 | MIEEC | 2012/13     */
/*****************************************************************/

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAIZ (0)
#define PAI(x) ((x-1)/2)
#define FILHO_ESQ(x) ((x*2)+1)
#define FILHO_DIR(x) ((x*2)+2)

int menor_que(elemento * e1, elemento * e2);

heap* heap_cria()
{
	heap * h = (heap *) malloc(sizeof(heap));

	if (!h)
		return NULL;

	h->tamanho_atual = 0;
	h->elementos = (elemento **) calloc(0, sizeof(elemento *));


	if (!h->elementos) {
		free(h);
		return NULL;
	}

	return h;
}


void heap_apaga(heap *h)
{
	int i;
	if(!h)
		return;
	/* apaga todos os elementos e respetivas strings */
	for(i=0; i<h->tamanho_atual; i++)
	{
		free(h->elementos[i]->valor);
		free(h->elementos[i]);
		h->elementos[i]=NULL;
	}

	free(h->elementos);
	free(h);
}


elemento * elemento_cria(int prioridade, int decada, int idade_intervalos, int estado_civil, int genero, int populacao)
{
	elemento * elem = (elemento *) malloc(sizeof(elemento));

	if (!elem)
		return NULL;

	// temos de fazer uma reservar memoria para a estrutura de dados *dado= (dados*) malloc(sizeof(dados));

	elem->valor->decada = decada;
	elem->valor->idade_intervalos = idade_intervalos;
	elem->valor->estado_civil = estado_civil;
	elem->valor->genero = genero;
	elem->valor->populacao = populacao;

	elem->prioridade = prioridade;

	return elem;
}


int heap_insere(heap * h, int decada, int idade_intervalos, int estado_civil, int genero, int populacao, int prioridade)
{
	elemento * aux, * elem;
	int i;


	elem = elemento_cria(prioridade, decada, idade_intervalos, estado_civil, genero, populacao);
	if (!elem)
		return 0;

	/* coloca elemento no fim da heap */
	i = h->tamanho_atual;
	h->elementos=(elemento**)realloc(h->elementos, sizeof(elemento *) * (h->tamanho_atual + 1));
	h->elementos[i] = elem;
	h->tamanho_atual++;
	/* enquanto elemento for mais prioritario do que o respetivo pai, troca-os */
	while (i != RAIZ && menor_que(h->elementos[i], h->elementos[PAI(i)]))
	{
		aux = h->elementos[PAI(i)];
		h->elementos[PAI(i)] = h->elementos[i];
		h->elementos[i] = aux;
		i = PAI(i);
	}
	return 1;
}


int *heap_remove(heap * h)
{
	int i, filho_maior;
	dados *valor;
	elemento * aux;

	/* se heap estiver vazia, nao remove elemento */
	if (!h || h->tamanho_atual <= 0)
		return NULL;

	valor = h->elementos[0]->valor;

	/* coloca ultimo elemento da heap na raiz */
	h->tamanho_atual--;
	h->elementos[RAIZ] = h->elementos[h->tamanho_atual];
	h->elementos[h->tamanho_atual] = NULL;

	i = RAIZ;

	/* enquanto nao chegar 'a base da heap */
	while(FILHO_ESQ(i) < h->tamanho_atual)
	{
		filho_maior = FILHO_ESQ(i);
		/* verifica se existe filho 'a direita e se este e' mais prioritario do que 'a esquerda */
		if (FILHO_DIR(i) < h->tamanho_atual && menor_que(h->elementos[FILHO_DIR(i)], h->elementos[FILHO_ESQ(i)]))
			filho_maior = FILHO_DIR(i);

		/* enquanto elemento for mais prioritario do que o respetivo pai, troca-os */
		if (menor_que(h->elementos[filho_maior], h->elementos[i]))
		{
			aux = h->elementos[filho_maior];
			h->elementos[filho_maior] = h->elementos[i];
			h->elementos[i] = aux;
			i = filho_maior;
		}
		else
			break;
	}
	return valor;
}



void mostraHeap(heap *h, int indice)
{
	int i, nivel = 0;
	if (indice < h->tamanho_atual)
	{
		i = indice;
		while(i > RAIZ)
		{
			i = PAI(i);
			nivel++;
		}
		mostraHeap(h, FILHO_ESQ(indice));

		for(i = 0; i < 3 * nivel; i++)

		printf("%d, %d, %d, %d, %d\n",h->elementos[indice]->valor->decada, h->elementos[indice]->valor->idade_intervalos,h->elementos[indice]->valor->estado_civil, h->elementos[indice]->valor->genero, h->elementos[indice]->valor->populacao);
		mostraHeap(h, FILHO_DIR(indice));
	}
	if (nivel == 0)
		printf("\n");
}

int menor_que(elemento * e1, elemento * e2)
{
	if (e1 == NULL || e2 == NULL)
	{
		return 0;
	}

	return e1->prioridade < e2->prioridade;
}

