#include "Estruturas.h"
#define MAX(x,y) ((x)<(y) ? (y) : (x))



/* cria memoria para um vetor de tam posicoes */
int *cria_vetor(int tam);

/* funcao usada como parametro do qsort */
int compare (const void * a, const void * b);

bool compare_sort_from_list(const item_peso& first, const item_peso& second);

int max_valor_vetor(int *vetor, int p);

int min_valor_vetor(int *vetor, int p);

double zero_maquina();

/* libera memoria de um vetor */
void libera_vetor(int *vetor);

/* imprime a solucao */
void imprime_vetor(int *s, int n);

void atualiza_vetor(int *s_star, int *s, int n);

float soma_vetor(int *s, int n);

/* Gera numero aleatorio entre min e max */
float randomico(float min, float max);

void inicializa_vetor(int *vetor, int tam);

void embaralha_vetor(int *vetor, int n);

/* Calcula o desvio-padrão das fos da população */
float calcula_desvio_padrao(float *fo_pop, int n);



