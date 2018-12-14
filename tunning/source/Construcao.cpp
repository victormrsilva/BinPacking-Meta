#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include "Util.h"
//#include "Estruturas.h"
#include "Construcao.h"


using namespace std;

void inicializa_soma_particao(int *soma_particao, int *s, int m, int *vetor_peso){
    int i;
    for(i=0; i < m; i++){
        soma_particao[s[i]] = soma_particao[s[i]] + vetor_peso[i];
    }

}

//neste caso, o s_anterior passado guarda a solucao antes de ocorrer o moviemnto
void atualiza_soma_particao(int *soma_particao, int *s_anterior, int melhor_i, int melhor_j, int tipo_mov, int *vetor_peso){
    int aux;
    if(tipo_mov==1){ // movimento do tipo mudanca de uma mala para outra particao
        aux = s_anterior[melhor_i]; //aux recebe a particao de i referente a solucao anterior
        soma_particao[aux] = soma_particao[aux] - vetor_peso[melhor_i];//tira o peco de i dessa particao
        soma_particao[melhor_j] = soma_particao[melhor_j] + vetor_peso[melhor_i];//adicona o peco de i na particao escolhida
    }else{
    // mudanca do tipo troca de uma mala de uma particao com outra mala de outra particao
    soma_particao[s_anterior[melhor_i]] = soma_particao[s_anterior[melhor_i]] -
                                          vetor_peso[melhor_i] + vetor_peso[melhor_j];
    soma_particao[s_anterior[melhor_j]] = soma_particao[s_anterior[melhor_j]] - vetor_peso[melhor_j] + vetor_peso[melhor_i];
    }
}

/* Constroi uma solucao de forma parcialmente gulosa, no caso,
   aloca uma mala pertencente a lista RCL do GRASP a particao com menor peso */
void constroi_solucao_parcialmente_gulosa(int *s, int *vetor_peso, int m, int p, float alpha, float *fo){
/* Inicio da Fase de Construcao de uma solucao */
    //printf("\n Construindo nova solucao ...\n");
    list<item_peso> candidatos;
    int *rcl;
    int min_part, escolhido, j, rcl_count=0;
    int soma_particao[p];
    float rcl_limit;

    rcl = cria_vetor(m);
    inicializa_vetor(rcl, m);
    inicializa_vetor(soma_particao, p);

    for(j=0; j < m; j++){
        item_peso aux;
        aux.id_item = j;
        aux.valor = vetor_peso[j];
        candidatos.push_back(aux);
    }
    //for (list<item_peso>::iterator it=candidatos.begin(); it != candidatos.end(); ++it){
    //        printf("teste valor = %d\n",it->valor);
    //        printf("teste id = %d\n", it->id_item);
    //}
    candidatos.sort(compare_sort_from_list);

    for(j=0; j < m; j++){  //para cada i da solucao a ser construida
        rcl_limit = candidatos.front().valor - alpha * (candidatos.front().valor - candidatos.back().valor); // calcula a funcao rcl
        //printf("rcl limite = %f\n",rcl_limit);
        for (list<item_peso>::iterator it=candidatos.begin(); it != candidatos.end(); ++it){
            if (float(it->valor) >= rcl_limit){
                rcl[rcl_count] = it->id_item; //cria lista rcl
                rcl_count++;
            }
        }
        //printf("rcl count = %d\n",rcl_count);
        //imprime_vetor(rcl,m);
        escolhido = int(randomico(0.0, rcl_count));  //escolhe aleatoriamente uma mala de rcl
        //printf("rcl escolhido = %d\n",escolhido);
        if (escolhido == rcl_count)  escolhido = rcl_count-1;
        min_part = min_valor_vetor(soma_particao, p); // a funcao retorna o index da particao de menor soma
        soma_particao[min_part] = soma_particao[min_part] + vetor_peso[rcl[escolhido]];
        s[rcl[escolhido]] = min_part; // adicona a mala escolhida da rcl na particao de menor peso
        //zera variaveis para criacao da proxima lista rcl
        rcl_count = 0;
        for (list<item_peso>::iterator it = candidatos.begin(); it != candidatos.end(); ){
            if (it->id_item == rcl[escolhido]){
                it =  candidatos.erase(it); //remove da lista corrente a mala que foi adiconada a solucao
                break;
            }else   it++;
        }
    //    candidatos.sort(compare_sort_from_list); // ordena novamente a lista sem a mala que foi adiconada a solucao
    }
    *fo = float(soma_particao[max_valor_vetor(soma_particao,p)] - soma_particao[min_valor_vetor(soma_particao,p)]);
}


float melhor_vizinho(int m, int p, int *s, int *soma_particao, int *vetor_peso, float fo, int *melhor_i,
                     int *melhor_j, int *tipo_mov){

    int aux, part_i;
    float fo_melhor_viz = fo;
    //fazendo a vizinhanca do movimento mudanca de uma mala para outra particao
    *tipo_mov = 1;
    for(int i=0; i < m; i++){ // percorre as malas
        part_i = i;
        for(int j=0; j < p; j++){ //percorre as particoes
            if (j!=part_i){
                atualiza_soma_particao(soma_particao,s,i,j,1,vetor_peso);

                float delta = float(soma_particao[max_valor_vetor(soma_particao,p)] -
                              soma_particao[min_valor_vetor(soma_particao,p)]); //delta eh a fo deste vizinho

                //faz o movimento
                aux = s[i];
                s[i]=j;
                if (delta < fo_melhor_viz){
                    *melhor_i = i;
                    *melhor_j = j;
                    *tipo_mov = 1;
                    fo_melhor_viz = delta;
                }
                //desfaz o movimento
                atualiza_soma_particao(soma_particao,s,i,aux,1,vetor_peso);
                s[i]=aux;
            }
        }
    }
    //fazendo a vizinhanca do movimento troca de uma mala de uma particao com outra mala de outra particao
    *tipo_mov = 2;
    for(int i=0 ; i < m - 1 ; i++){
        for(int j=i+1 ; j < m ; j++){
            atualiza_soma_particao(soma_particao,s,i,j,2,vetor_peso);
            float delta = float(soma_particao[max_valor_vetor(soma_particao,p)] -
                                soma_particao[min_valor_vetor(soma_particao,p)]); //delta eh a fo deste vizinho
            //faz o movimento
            aux = s[i];
            s[i]=s[j];
            s[j]=aux;
            if (delta < fo_melhor_viz){
                *melhor_i = i;
                *melhor_j = j;
                *tipo_mov = 2;
                fo_melhor_viz = delta;
            }
            //desfaz o movimento
            atualiza_soma_particao(soma_particao,s,j,i,2,vetor_peso);
            aux = s[j];
            s[j] = s[i];
            s[i] = aux;

        }
    }

    // retornar a fo do melhor vizinho
    return fo_melhor_viz;
}


float descida(int *s, int *vetor_peso, int m, int p){

/* Inicio da Fase de Construcao de uma solucao atraves de heuristica de refinamento*/
    //printf("\n Construindo nova solucao...\n");
    //item_peso list[m];
    int aux, melhor_i, melhor_j, tipo_mov;
    int soma_particao[p];
    float fo_melhor_viz, fo;
    bool melhorou;

    inicializa_vetor(soma_particao, p);
    inicializa_soma_particao(soma_particao, s, m, vetor_peso);
    //imprime_vetor(soma_particao, p);
    //imprime_vetor(s, m);

    fo = fo_melhor_viz = float(soma_particao[max_valor_vetor(soma_particao,p)] -
                               soma_particao[min_valor_vetor(soma_particao,p)]);
    do{
        melhorou = false;
        fo_melhor_viz = melhor_vizinho(m, p, s, soma_particao, vetor_peso, fo, &melhor_i, &melhor_j, &tipo_mov);
        //imprime_vetor(soma_particao, p);
        if (fo_melhor_viz < fo){
            if(tipo_mov==1){ // movimento do tipo mudanca de uma mala para outra particao
                //atualiza soma_particao antes de atualizar s
                atualiza_soma_particao(soma_particao,s,melhor_i,melhor_j,1,vetor_peso);
                s[melhor_i] = melhor_j;
                fo = fo_melhor_viz;
                melhorou = true;
                continue;
            }
            // movimento do tipo troca de uma mala de uma particao com outra mala de outra particao
            atualiza_soma_particao(soma_particao,s,melhor_i,melhor_j,2,vetor_peso);//atualiza soma_particao antes de atualizar s
            aux = s[melhor_i];
            s[melhor_i] = s[melhor_j];
            s[melhor_j] = aux;
            fo = fo_melhor_viz;
            melhorou = true;

        }
    } while (melhorou == true);
    return fo;
}


float grasp(int m, int p, int *s, int *vetor_peso, unsigned GRASPmax, double alpha){

    int *s_star;
    unsigned iter = 0;
    float fo_star = INT_MAX;
    float fo;

    s_star = cria_vetor(m);

    //printf("iter = %d \n", iter);
    //printf("itermax = %d \n", GRASPmax);
    //printf("alpha = %f \n", alpha);
    iter++;
    constroi_solucao_parcialmente_gulosa(s,vetor_peso,m,p,0.0, &fo);
    fo = descida(s, vetor_peso, m, p);

    while (iter < GRASPmax){
        iter++;
        constroi_solucao_parcialmente_gulosa(s,vetor_peso,m,p,alpha, &fo);
        fo = descida(s, vetor_peso, m, p);
        if (fo < fo_star){
            fo_star = fo;
            atualiza_vetor(s_star, s, m);
        }
        if (fo_star == 0.0 or fo_star == 1.0) break; // se o otimo global ja foi alcancado, para
    }
    atualiza_vetor(s,s_star,m);
    libera_vetor(s_star);
    //printf("fo = %f \n", fo_star);
    return fo_star;
}


