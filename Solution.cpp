#include "Solution.h"

#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;

Solution::Solution(const Instance &inst) : inst_(inst), itensContainer(inst.n(),-1){
    bins = 0;
}

double Solution::obj(){
    return objective;
}

void Solution::calculateObj(){
    
    if (bins < 1e-6){
        objective =  0.0;
    }
    double soma = 0;
    for (Container i : solution){
        if (i.livre() != 0)
            soma = soma + 0.1;
    }
    soma = soma / bins; // it's worse to have a bin with free space than one without
    objective = bins + soma;
}

void Solution::insertFirstFit(int item){
    if (bins < 1e-6){ // no bin createad
        Container c(inst_);
        itensContainer[item] = 0;
        c.incluiritem(item);
        bins++;
        solution.emplace_back(c);
        calculateObj();
        return;
    }
    // try to insert item always on last bin
    int position = bins-1;
    // if I can't insert item, create another bin and insert;
    if (!solution.back().incluiritem(item)){
        Container c(inst_);
        c.incluiritem(item);
        solution.emplace_back(c);
        itensContainer[item] = bins;
        bins++;
    } else {
        itensContainer[item] = position;
    }
    calculateObj();
}

void Solution::insertBestFirstFit(int item){
    if (bins < 1e-6){ // no bins created
        Container c(inst_);
        itensContainer[item] = 0;
        c.incluiritem(item);
        solution.emplace_back(c);
        bins++;
        calculateObj();
        return;
    }
    // search in all bins. If can't insert, create another
    bool insert = false;
    for (unsigned i = 0; i < solution.size(); i++){
        std::list<Container>::iterator it = std::next(solution.begin(),i);
        
        if ((*it).incluiritem(item)){
            insert = true;
            itensContainer[item] = i;
            calculateObj();
            break;
        }
    }
    if (!insert){
        Container c(inst_);
        c.incluiritem(item);
        solution.emplace_back(c);
        itensContainer[item] = bins;
        bins++;
        calculateObj();
    }
}

void Solution::printSolution(){
    cout << obj() << " " << inst_.n() << endl;
    for (Container i : solution){
        cout << i.toString() << endl;
    }
}

std::pair<int,double> Solution::objChangeContainer(int item){
    // for (int i = 0; i < inst_.n(); i++){
    //     cout << i << "(" << itensContainer[i] << ") ";
    // }
    // cout << endl;
    int orig_container = itemContainer(item);
    //cout << item << "("<<orig_container<<")"<<endl;
    double new_obj = bins;
    int best_bin = orig_container;
    for (unsigned i = 0; i < solution.size(); i++){
        if (i == orig_container){
            continue;
        }
        std::list<Container>::iterator itNext = next(solution.begin(),i);
        std::list<Container>::iterator itOriginal = next(solution.begin(),orig_container);
        if ((*itNext).podeColocar(item)){
            // change obj_function only if a bin is going to be empty
            //cout << "item: " << item << " orig_cont: " << orig_container << " new_cont: " << i << endl;
            if ((*itOriginal).size() == 1){
                new_obj = bins - 1;
                best_bin = i;
            } else {
                best_bin = i;
            }
            double freeOldContainer = (*itOriginal).livre() + inst_.w(item);
            double freeNewContainer = (*itNext).livre() - inst_.w(item);
            double soma = 0;
            if ((*itOriginal).livre() == 0){ // the removal of this item will make it not full
                soma = soma + 0.1;
            }
            for (unsigned j = 0; j < solution.size(); j++){
                if (j == i || j == orig_container){ 
                    continue;
                } else {
                    soma = soma + 0.1;
                }
            }
            soma = soma / bins; // it's worse to have a bin with free space than one without
            new_obj = new_obj + soma;
        }
    }

    return std::make_pair(best_bin,new_obj);
}

void Solution::writeSolution(){
    cout << bins <<" "<< inst_.n() << endl;
    int i = 0;
    for (Container c : solution){
        i++;
        cout << c.size();
        for (int j = 0; j < c.size(); j++){
            cout << " " << c.item(j)+1;
        }
        cout << endl;
    }
}

bool Solution::changeItemContainer(int item,int newContainer){
    int container = itemContainer(item);
    if (container == newContainer) return false;
    std::list<Container>::iterator itOriginal = next(solution.begin(),container);
    if (newContainer == -1){
        if ((*itOriginal).size() > 1){
            Container c(inst_);
            if (!c.incluiritem(item) ) return false;
            solution.emplace_back(c);
            itensContainer[item] = bins;
            (*itOriginal).removerItem(item);
            bins++;
        }
    } else {
        std::list<Container>::iterator it = next(solution.begin(),newContainer);
        if (!(*it).incluiritem(item) ) return false;
        cout << item+1 << " " << newContainer << "( " << (*it).toString() <<") " << container << " (" << (*itOriginal).toString() << ")" << endl;
        itensContainer[item] = newContainer;
        (*itOriginal).removerItem(item);
    }
    eliminateEmptyBins();
    // for (int i = 0; i < inst_.n(); i++){
    //     cout << i+1 << "(" << itemContainer(i) << ") ";
    // }
    return true;
}

Solution& Solution::operator=(const Solution &other){
    if (this != &other){
        solution.clear();
        solution = other.solution;
        bins = other.bins;
        objective = other.objective;
    }
    return *this;
}

void Solution::eliminateEmptyBins(){
    bool erased = false;
    for (int i = 0; i < solution.size(); i++){
        std::list<Container>::iterator it = next(solution.begin(),i);

        if ((*it).itens().size() == 0){
            // cout << "bin vazio " << i << endl;
            solution.erase(it);
            erased = true;
            bins--;
        }
    }
    if (erased){ // only do this if erased a container from solution
        for (int i = 0; i < solution.size(); i++){
            std::list<Container>::iterator it = next(solution.begin(),i);
            for (int item : (*it).itens()){
                itensContainer[item] = i;
            }
        }
        // printSolution();
        // for (int i = 0; i < inst_.n(); i++){
        //     cout << i+1 << "(" << itemContainer(i) << ") ";
        // }
        // getchar();
    }

    calculateObj();
}

Container& Solution::getContainer(int container){
    std::list<Container>::iterator it = next(solution.begin(),container);
    return (*it);
}

std::vector<int> Solution::getItensContainer(int container) { 
    std::list<Container>::iterator it = next(solution.begin(),container);
    return (*it).itens();
}

std::pair<int,double> Solution::checkRandomNeighbor(int item, int newContainer){
    // cout << "inicio: " << endl;
    // printSolution();
    
    int orig_container = itemContainer(item);
    std::list<Container>::iterator itNew = next(solution.begin(),newContainer);
    std::list<Container>::iterator itOriginal = next(solution.begin(),orig_container);
    // cout << "checa: " << item << " " << newContainer << endl;
    // cout << "---" << endl;
    // cout << (*itNew).toString();
    // cout << "---" << endl;
    double new_obj = 0;
    if ((*itNew).podeColocar(item)){
        double soma = 0;
        if ((*itOriginal).livre() == 0){
            soma += 0.1;
        }
        for (std::list<Container>::iterator i = solution.begin(); i != solution.end(); i++ ){
            if (i == itOriginal || i == itNew){
                continue;
            }
            soma += 0.1;
        }
        soma = soma / bins;
        new_obj = soma + bins;
    } else {
        newContainer = -1;
        new_obj = solution.size()+1; // need to create another container;
    }
    // cout << "item: " << item << " newContainer: " << newContainer << " newObj: " << new_obj << endl;
    return std::make_pair(newContainer,new_obj);
}; // <newContainer,obj>

void Solution::moveRandomNeighbor(int item, int newContainer){
    cout << "movimentacao" << endl;
    int orig_container = itemContainer(item);
    if (newContainer == orig_container) return;
    for (int i = 0; i < inst_.n(); i++){
                cout << i+1 << "(" << itemContainer(i) << ") ";
    }
    std::list<Container>::iterator itOriginal = next(solution.begin(),orig_container);
    cout <<  "item " << item+1 << " container original " << orig_container << " container destino: " << newContainer << endl;
    cout << "itens container original: " << (*itOriginal).toString() << endl;
    if (newContainer == -1 ){
        if ((*itOriginal).size() > 1){
            for (int i = 0; i < inst_.n(); i++){
                cout << i+1 << "(" << itemContainer(i) << ") ";
            }
            Container c(inst_);
            if (!c.incluiritem(item) ) return;
            solution.emplace_back(c);
            itensContainer[item] = bins;
            (*itOriginal).removerItem(item);
            bins++;
            
        }
    } else {
        std::list<Container>::iterator itNew = next(solution.begin(),newContainer);
        if (!(*itNew).incluiritem(item)) return;
        itensContainer[item] = newContainer;
        (*itOriginal).removerItem(item);
    }
    eliminateEmptyBins();
    // calculateObj();
}