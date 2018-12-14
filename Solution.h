#ifndef Solution_H
#define Solution_H

#include "Instance.h"
#include "Container.h"

#include <vector>
#include <list>
#include <string>

class Solution{
    private:
        const Instance &inst_;
        std::list<Container> solution;
        double bins;
        double objective;
        std::vector<int> itensContainer;
        void calculateObj();

    public:
        Solution(const Instance &inst);
        void insertFirstFit(int item);
        void insertBestFit(int item);
        void insertBestFirstFit(int item);
        double obj();
        int itemContainer(int item) { return itensContainer[item];};
        // try to change container of item. Return pair container,new_obj
        std::pair<int,double> objChangeContainer(int item);
        void printSolution();
        double qtdBins(){ return bins;}
        bool changeItemContainer(int item, int newContainer);
        std::vector<int> getItensContainer(int container);;
        Container& getContainer(int container);
        void eliminateEmptyBins();

        Solution& operator=(const Solution &other);
        std::pair<int,double> checkRandomNeighbor(int item, int newContainer); // <newContainer,obj>
        void moveRandomNeighbor(int item, int newContainer);

        void writeSolution();
};

#endif