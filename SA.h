#ifndef SA_H
#define SA_H

#include "Solution.h"
#include "Instance.h"
#include "BuscaLocal.h"

class SA{
    public:
        SA(const Instance &inst,double alpha, double beta, double gama, double SAmax, double pBL);
        Solution execute();
    private:
        double alpha,beta,gama,SAmax,initialTemperature,finalTemperature;
        Solution sol;
        double defineInitialTemperature();
        const Instance &inst_;
        void generateRandomSolution();
        double randomNumber(int min, int max);
        BuscaLocal bl;
        double pctBL;
};  

#endif