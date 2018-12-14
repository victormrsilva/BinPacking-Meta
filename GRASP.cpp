#include "GRASP.h"

#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;

GRASP::GRASP(const Instance &inst, double a, int maxIter, double pBL) : inst_(inst), sol(inst_), bl(inst){
    alpha = a;
    maxIterations = maxIter;
    pctBL = pBL;
}

Solution GRASP::greedyRandomizedConstruction(){
    // criação da lista gulosa. Cada elemento recebe peso / quantos itens em conflito
    // isso significa que quanto mais pesado o item e menos conflitos, melhor ele será para ser colocado
    Solution s(inst_);
    vector<pair<int,double>> greedyList;
    for (int i = 0; i < inst_.n(); i++){
        double weight = inst_.w(i);
        double conflicts = inst_.qtdItensConflito(i);
        double valueGreedy =  (conflicts == 0 ? weight+1 : weight / conflicts) ;
        // cout << i+1 << " " << weight << " " << conflicts << " " << valueGreedy << endl;
        greedyList.emplace_back(make_pair(i,valueGreedy));
    }

    std::sort(greedyList.begin(), greedyList.end(), [](const pair<int,double> &a, const pair<int,double> &b) { return (a.second < b.second);}); 

    // cout << endl;
    //inst_.writeInstance("teste");
    while (!greedyList.empty()){
        double c_min = greedyList[0].second;
        double c_max = greedyList[greedyList.size() - 1].second;
        double threshold = c_max - alpha * (c_max-c_min);

        int itensThreshold = std::count_if(greedyList.begin(), greedyList.end(), [threshold](const pair<int,double> &a){return a.second >= threshold ;});

        int itemPos = (greedyList.size() - 1)-(std::rand() % itensThreshold);
        int chosenItem = greedyList[itemPos].first;
        // for (unsigned i = 0; i < greedyList.size(); i++){
        //     cout << "<" << greedyList[i].first << "," << greedyList[i].second << "> ";
        // }
        // cout << endl << "thresold: " <<    threshold << " acima: " << itensThreshold << " escolhido: " << chosenItem <<  " itemPos: " << itemPos << endl;
        s.insertFirstFit(chosenItem);
        //cout << "obj: " << sol.obj() << " " ;
        // s.printSolution();
        greedyList.erase(greedyList.begin()+itemPos);

    }

    return s;
}

Solution GRASP::execute(){
    const clock_t begin = clock();
    Solution s = greedyRandomizedConstruction();
    // for (int i = 0; i < inst_.n(); i++){
    //     cout << i+1 << "(" << s.itemContainer(i) << ") ";
    // }
    // cout << endl;
    // getchar();
    // cout << "greedy solution: " << endl;
    // s.printSolution();
    // bl.optSetBin(s,pctBL);
    bl.optFreeSpace(s, pctBL);
    clock_t end = clock();
    cout << (end - begin) / double(CLOCKS_PER_SEC) << ";" << s.obj() << endl;
    // cout << "optFreeSpace:" << endl;
    // s.printSolution();
    // getchar();
    int melhorIter = 0;
    int iter = 0;
    // cout << iter << " " << melhorIter << endl;
    while ((iter-melhorIter) < maxIterations){
        iter++;
        // cout << "s_aux: ";
        Solution s_aux = greedyRandomizedConstruction();
        // s_aux.printSolution();
        // cout << "s_aux local search: ";
        // bl.optSetBin(s_aux,pctBL);
        bl.optFreeSpace(s_aux, pctBL);
        
        // s_aux.printSolution();
        if (s_aux.obj() < s.obj()){
            s = s_aux;
            melhorIter = iter;
            end = clock();
            cout << (end - begin) / double(CLOCKS_PER_SEC) << ";" << s.obj() << endl;
        }
        // cout << "s: " << endl;
        // s.printSolution();
        // cout << iter << " " << melhorIter << endl;
        // getchar();
        end = clock();
        if (((end - begin) / double(CLOCKS_PER_SEC)) > 300) return s;
        clock_t end = clock();
        //cout << (end - begin) / double(CLOCKS_PER_SEC) << ";" << s.obj() << " " << (iter-melhorIter) << " " << maxIterations<< endl;
    }
    return s;
}