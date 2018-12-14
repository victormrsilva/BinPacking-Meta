#ifndef CONTAINER_H
#define CONTAINER_H

#include "Instance.h"
#include <string>
#include <vector>


class Container{
public:

    Container(const Instance &_inst);

    int ocupado() const { return ocupado_; } // number of jobs

    int livre() const { return inst_.c() - ocupado_;} // number of machines

    bool removerItem(int i);

    bool incluiritem(int i);

    int item(int i) const;

    int size() const { return itens_.size(); }

    void clear();

    bool podeColocar(int i);

    std::vector<int> itens() { return itens_; };

    std::string toString();

    Container& operator=(const Container& other){
        if (this != &other) { // self-assignment check expected
            if (other.itens_.size() != itens_.size() && other.ocupado_ != ocupado_) {         // storage cannot be reused
                itens_.clear();              // destroy storage in this
                ocupado_ = other.ocupado_;
                itens_ = other.itens_;             // preserve invariants in case next line throws
            } 
        }
        return *this;
    }

private:
    const Instance &inst_;
    int ocupado_;
    std::vector< int > itens_;

};

#endif

