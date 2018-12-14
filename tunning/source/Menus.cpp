#include <stdio.h>
#include "Menus.h"


/* Menu principal */
int menu_principal(void)
{
  int escolha;

  do {
    printf("\n*******************Menu Principal************************* \n");
    printf("ATENCAO: Necessario gerar solucao inicial antes de refinar\n");
    printf("                1. Gere solucao inicial (Gulosa) \n");
    printf("                2. Descida (Best Improvement)\n");
    //printf("                3. Descida randomica \n");
    //printf("                4. Descida com Primeiro de Melhora (First Improvement)\n");
    //printf("                5. Multi-Start \n");
    printf("                3. Simulated Annealing \n");
    //printf("                7. Busca Tabu \n");
    printf("                4. ILS \n");
    printf("                5. GRASP \n");
    printf("                6. VND \n");
    printf("                7. VNS \n");
    //printf("               12. Algoritmos Geneticos \n");
    //printf("               13. Algoritmos Memeticos \n");
    //printf("               14. Colonia de Formigas \n");
    printf("                0. Sair \n");
    printf("                Escolha: ");
    scanf("%d",&escolha);
  } while (escolha < 0 || escolha > 7);
  return escolha;
}


/* Menu Algoritmos Geneticos */
int menu_AG(void)
{
  int escolha;

  do {
    printf("\n****************Menu Algoritmos Geneticos********************** \n");
    printf("                1. Operador OX \n");
    printf("                2. Operador ERX \n");
    printf("                Escolha: ");
    scanf("%d",&escolha);
  } while (escolha < 1 || escolha > 2);
  return escolha;
}