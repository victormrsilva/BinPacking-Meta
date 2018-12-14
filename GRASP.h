#ifndef GRASP_H
#define GRASP_H

#include "Instance.h"
#include "Container.h"
#include "Solution.h"
#include "BuscaLocal.h"

#include <vector>
#include <string>

class GRASP{
    private:
        const Instance &inst_;
        double alpha;
        Solution greedyRandomizedConstruction();
        int maxIterations;
        Solution sol;
        BuscaLocal bl;
        double pctBL;

    public:
        GRASP(const Instance &inst, double a, int maxIter, double pBL);
        Solution execute();
};

#endif