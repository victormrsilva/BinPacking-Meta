#include "Solver.h"
#include <iostream>

Solver::Solver(const Instance &inst, const std::vector< double >& chromosome) : instance(inst), itens(inst.n()), bins(0){
	// 1) Obtain a permutation out of the chromosome -- this will be the tour:
	for(unsigned i = 0; i < chromosome.size(); ++i) { itens[i] = ValueKeyPair(chromosome[i], i); }

	// Here we sort 'rank', which will produce a permutation of [n] stored in ValueKeyPair::second:
	std::sort(itens.begin(), itens.end());

    // insert each iten in the first container. If the item can't be put in the container, another container will be created and the item will be put in it
    solution.emplace_back(Container(instance));
    
    for(unsigned i = 0; i < itens.size(); i++){
        int chosenBin = -1;
        double freeSpace = 99999;
        for (unsigned j = 0; j < solution.size(); j++){
            if (solution[j].podeColocar(itens[i].second)){
                if (freeSpace > solution[j].livre()){
                    chosenBin = j;
                    freeSpace = solution[j].livre();
                }
            }
        }

        if (chosenBin > -1){
            solution[chosenBin].incluiritem(itens[i].second);
        } else {
            solution.emplace_back(Container(instance));
            int idx = solution.size()-1;
            solution[idx].incluiritem(itens[i].second);
        }
    }
}

Solver::~Solver(){

}