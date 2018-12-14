#include "Instance.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include<algorithm>

using namespace std;

#define str(a) to_string(a)

Instance::Instance( const std::string &fileName){
    ifstream ifs;
    ifs.open( fileName.c_str() );

    instance_name = fileName.c_str();

    string line;

    //getline( ifs, line );
    ifs >> n_ >> c_;

    pesos = vector< int >( n_, 0 );
    conflitos = vector< vector< bool > >( n_, vector<bool>( n_, 0 ) );

    //cout << "itens " << n() << " capacidade do container " << c() <<  endl;
        
    
    getline( ifs, line);
    for (int i=0; i < n_; i++){
        getline( ifs, line);
        
        vector<string> confs = explode(line, ' ' );
        pesos[i] = stoi(confs[1]);
        for (unsigned int j = 2; j < confs.size(); j++){
            int caixa = stoi(confs[j]);
            conflitos[i][caixa - 1] = true;
            conflitos[caixa - 1][i] = true;
        }
    }

    ifs.close();
    writeInstance("instance");
}


const vector<string> Instance::explode(const string& s, const char& c){
    std::vector<std::string> tokens;
    string word;

    istringstream iss(s, istringstream::in);

    while( iss >> word ){
        tokens.push_back(word);
    }
    return tokens;
}

void Instance::writeInstance( const string fname ) const {
    ofstream out( fname );

    out << "Itens: " << n() << " Capacidade " << c() << endl << endl;
    
    for ( int i=0 ; i < n() ; i++ )
    {
        out << "Item " << i+1 << " peso " << w(i) << endl;
        out << "Conflitos" << endl;
        for (int j = 0; j < n(); j++){
            if (conflito(i,j)){
                out << j+1 << " ";
            }
        }
        out << endl << endl;
    }
    out << endl;

    out.close();
}

int Instance::qtdItensConflito(int i) const{
    int qtd = count_if(conflitos[i].begin(), conflitos[i].end(), [] (bool i) { return i;} );
    return qtd;
}
