#ifndef BUSCALOCAL_H
#define BUSCALOCAL_H
#include <iostream>

#include "Solution.h"
#include "Instance.h"
#include "lp.hpp"

class BuscaLocal{
    private:
        void neighborFirstFit(Solution &sol);
        Solution& neighborBestFit(Solution &sol);
        const Instance &inst_;
        double pctBL;
    public:
        void firstFitDescend(Solution &sol);
        Solution& bestFit(Solution &sol);
        BuscaLocal(const Instance &inst);
        void optSetBin(Solution &sol, double pctBL);
        void optFreeSpace(Solution &sol, double pctBL);
};

#endif 