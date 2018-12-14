void inicializa_soma_particao(int *soma_particao, int *s, int m, int *vetor_peso);

void atualiza_soma_particao(int *soma_particao, int *s_anterior, int melhor_i, int melhor_j, int tipo_mov, int *vetor_peso);

void constroi_solucao_parcialmente_gulosa(int *s, int *vetor_peso, int m, int p, float alpha, float *fo);

float melhor_vizinho(int m, int p, int *s, int *soma_particao, int *vetor_peso, float fo,
                     int *melhor_i, int *melhor_j, int *tipo_mov);

float descida(int *s, int *vetor_peso, int m, int p);

float grasp(int m, int p, int *s, int *vetor_peso, unsigned GRASPmax, double alpha);

