#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "Util.h"



#define MAX(x,y) ((x)<(y) ? (y) : (x))


/* cria memoria para um vetor de tam posicoes */
int *cria_vetor(int tam)
{
  int *vetor;

  vetor = (int *) malloc(tam*sizeof(int));
  if (!vetor){
  	printf("Falta memoria para alocar o vetor de ponteiros");
    exit(1);
  }
  return vetor;
}

/* funcao usada como parametro do qsort */
int compare (const void * a, const void * b)
{

  item_peso *item_pesoA = (item_peso *)a;
  item_peso *item_pesoB = (item_peso *)b;
  if (item_pesoA->valor == item_pesoB->valor) return 0;
  return (item_pesoA->valor > item_pesoB->valor) ? -1 : 1;

}

bool compare_sort_from_list(const item_peso& first, const item_peso& second)
{
    if (first.valor > second.valor)
        return true;
    else
        return false;
}

int max_valor_vetor(int *vetor, int p){

     int max = vetor[0];
     int pos = 0;

     for(int i = 1; i<p; i++)
     {
          if(vetor[i] > max){
            max = vetor[i];
            pos = i;
          }
     }
     return pos;
}

int min_valor_vetor(int *vetor, int p){

     int min = vetor[0];
     int pos = 0;

     for(int i = 1; i<p; i++)
     {
          if(vetor[i] < min){
            min = vetor[i];
            pos = i;
          }
     }
     return pos;
}


double zero_maquina(){

    double TFinal = 1.0;
    do{
        TFinal = TFinal / 2.0;

    }while((1.0 + TFinal) > 1.0);
    return TFinal;
}



/* libera memoria de um vetor */
void libera_vetor(int *vetor)
{
  free(vetor);
}

/* imprime a solucao */
void imprime_vetor(int *s, int n)
{
    for (int j=0; j < n; j++) printf("s[%2d]=%d \n",j,s[j]);
}

/* atualiza a melhor solucao */
void atualiza_vetor(int *s_star, int *s, int n)
{
   for (int j=0; j < n; j++) s_star[j] = s[j];
}

float soma_vetor(int *s, int n){
    float soma = 0.0;
    for(int i = 0; i < n; i++)
        soma = soma + float(s[i]);
    return soma;
}


/* Gera numero aleatorio entre min e max */
float randomico(float min, float max)
{
  if (min == max) return min;
  return ((float) (rand()%10000/10000.0)*(max-min) + min);
}


void inicializa_vetor(int *vetor, int tam)
{
    for (int j=0; j<tam; j++) vetor[j] = 0;
}

void embaralha_vetor(int *vetor, int n)
{
  int aux, j1, j2;

  for (int i=0; i < n; i++){
    j1 = rand()%n;
    j2 = rand()%n;
    while (j1 == j2) j2 = rand()%n;
    aux = vetor[j1];
    vetor[j1] = vetor[j2];
    vetor[j2] = aux;
  }
}

/* Calcula o desvio-padrão das fos da população */
float calcula_desvio_padrao(float *fo_pop, int n)
{
  float media, desvio, somatorio;

  somatorio = 0;
  for (int i = 0; i < n; i++)
    somatorio += fo_pop[i];
  media = somatorio / n;
  somatorio = 0;
  for (int i = 0; i < n; i++)
    somatorio += (fo_pop[i] - media) * (fo_pop[i] - media);
  desvio = sqrt(somatorio / (n - 1));

  desvio = desvio / media;
  return desvio;
}
