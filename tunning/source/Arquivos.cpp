#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "Util.h"
#include "Arquivos.h"
#include <iostream>
#include <iomanip>

using namespace std;


void le_arquivo(char nomearq[], float *tempo, int *m, int *p, int **vetor_pesos)
{
    FILE *arquivo;
    int mala,
        particao,
        i = 0,
        *teste,
        aux;
    float tempo_processamento;
    char separador;

    //printf("estou na funcao leitura do arquivo.\n");
    arquivo = fopen(nomearq,"r");
    if (!arquivo) {
        printf("O Arquivo %s nao pode ser aberto.\n", nomearq);
        getchar();
        exit(1);
    }


    fscanf(arquivo, "%f\n", &tempo_processamento);
    fscanf(arquivo, "%c\n", &separador);
    fscanf(arquivo, "%d\n", &mala);
    fscanf(arquivo, "%c\n", &separador);
    fscanf(arquivo, "%d\n", &particao);
    fscanf(arquivo, "%c\n", &separador);

    teste = cria_vetor(mala);
    inicializa_vetor(teste, mala);
    //cout << "teste_inicializa 2: " << teste[2] << endl;



    while (!feof(arquivo)){
    fscanf(arquivo, "%d\n", &aux);
    //cout << "aux: " << aux << "i: " << i << endl;
    teste[i++] = aux;
    }
    //imprime_vetor(teste, mala);

    *tempo = tempo_processamento;
    *m = mala;
    *p = particao;
    *vetor_pesos = teste;

    fclose(arquivo);

}
