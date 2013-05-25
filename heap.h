/*****************************************************************/
/*   Fila de prioridade / min-heap | PROG2 | MIEEC | 2012/13     */
/*****************************************************************/

#ifndef HEAP_H
#define HEAP_H

typedef struct {
unsigned int decada;
unsigned int idade_intervalos;
int estado_civil;
int genero;
unsigned int populacao;

} dados;

typedef struct {
int prioridade;
dados *valor;
} elemento;

typedef struct {
/* numero de elementos no vetor */
int tamanho_atual;

/* vetor de apontadores para elementos */
elemento ** elementos;
} heap;

/* cria nova heap */
heap* heap_cria();

/* apaga heap */
void heap_apaga(heap *h);

/* insere elemento na heap com uma determinada prioridade */
int heap_insere(heap *h, int decada, int idade_intervalos, int estado_civil, int genero, int populacao, int prioridade);

/* remove elemento na raiz da heap */
int *heap_remove(heap *h);

/* imprime o estado actual da heap, a partir do indice indicado */
void mostraHeap(heap *h, int indice);

#endif /* HEAP_H */

