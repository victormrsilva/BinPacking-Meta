//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "Util.h"
#include "Arquivos.h"
#include "Construcao.h"
#include <iostream>
#include <iomanip>
#include <vector>
//---------------------------------------------------------------------------

using namespace std;

int main(int argc, char* argv[])
{
    int m,                    // numero de malas
        p;                    // numero de particoes
    int *vetor_pesos,         // vetor de tamanho m com pesos das malas
        *s;                   // vetor solucao corrente
    float tempo = 0.0,      // tempo de processamento
            fo = 0.0;        // funcao objetivo corrente

    vector<string> arguments(argv + 1, argv + argc);

	string inputFileName = arguments[0];

	unsigned GRASPmax = 100;
	double alphaGRASP = 0.20;

	string::size_type sz;

	//
	for(unsigned int i=1; i<arguments.size(); i+=2)
	{
        if(arguments[i]== "--MAX_GRASP")
            GRASPmax = stoi(arguments[i+1], &sz);
        else if(arguments[i]== "--ALPHA_GRASP")
            alphaGRASP = stod(arguments[i+1], &sz);
    }

    //if(argc != 6){
//		fprintf(stderr, "Usage: [Name of input file]\t --MAX_GRASP [Number of grasp iterations]\t --ALPHA_GRASP [Alpha value]\n");
//		return (1);
//	}

    le_arquivo(strdup(inputFileName.c_str()), &tempo, &m, &p, &vetor_pesos);
    //printf("Saidas: tempo = %f, mala = %d, particao = %d\n",tempo,m, p);
    s = cria_vetor(m);
    inicializa_vetor(s, m);
    fo = grasp(m, p, s, vetor_pesos, GRASPmax, alphaGRASP);

    cout<<fo;
    //printf("%f\n", fo);


  return 0;
}


