#ifndef SOLVER_H
#define SOLVER_H

#include <list>
#include <limits>
#include <vector>
#include <algorithm>

#include "Instance.h"
#include "Container.h"


class Solver{
    public:
        Solver(const Instance &inst, const std::vector< double >& chromosome);
        ~Solver();
        double getBins() {return solution.size();}

        std::vector<Container> getSolution(){return solution;}

    private:
        double bins;
        typedef std::pair< double, unsigned > ValueKeyPair;
        std::vector<Container> solution;
        const Instance &instance;
        std::vector<ValueKeyPair> itens;
};

#endif