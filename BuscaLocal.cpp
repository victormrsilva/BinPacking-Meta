#include "BuscaLocal.h"
#include <algorithm>
#include <string>

using std::cout;
using std::endl;

BuscaLocal::BuscaLocal(const Instance &inst) : inst_(inst) {
    
}

void BuscaLocal::neighborFirstFit(Solution& sol){
    std::vector<int> v(inst_.n());
    std::iota(v.begin(), v.end(), 0);
    
    // random visit item order
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);

    bool new_obj = false;

    /*
        capacity: 7 occupied: 6 free: 1 total itens: 2. Itens: 1 3
        capacity: 7 occupied: 7 free: 0 total itens: 3. Itens: 7 4 2
        capacity: 7 occupied: 7 free: 0 total itens: 2. Itens: 6 5
    */

    for (unsigned i = 0; i < v.size(); i++){
        // cout << "checking item: " << v[i] << endl;
        std::pair<int,double> new_sol = sol.objChangeContainer(v[i]);
        // cout << " fo: " << new_sol.second << " prev container: " << sol.itemContainer(v[i]) << " new_cont: " << new_sol.first << endl;
        if (sol.obj() > new_sol.second){ // a bin will be empty
            // cout << "changing item " << i << " from container " << sol.itemContainer(v[i]) << "(" << sol.obj() << ") to " << new_sol.first << "("<<new_sol.second << ")" << endl;
            sol.changeItemContainer(i,new_sol.first);
            // sol.printSolution();
            // getchar();
            new_obj = true;
        }
        if (new_obj) {
            break;
        }
    }
    
}

void BuscaLocal::firstFitDescend(Solution &sol){
    bool melhorou = false;
    Solution sol_aux = sol;
    // cout << "solução corrente: " << endl;
    // sol_aux.printSolution();
    // cout << "melhor solução: " << endl;
    // sol.printSolution();
    do{
        melhorou = false;
        neighborFirstFit(sol_aux);
        if (sol_aux.obj() < sol.obj()){
            sol = sol_aux;
            // cout << "melhor solução: " << endl;
            // sol.printSolution();
            melhorou = true;
        }

    } while (melhorou);
}

void BuscaLocal::optSetBin(Solution &sol, double pctBL){
    int binsSorted = ceil(pctBL*sol.qtdBins());
    int timeout = 30; // time limit for mip to run is 30 sec

    LinearProgram* mip = lp_create();
    lp_set_print_messages(mip,0);
    // cout << "Timeout do modelo: "<< timeout <<" Quantidade de bins: " << binsSorted << endl;
    lp_set_max_seconds( mip, timeout );
    std::vector<int> sorteio = std::vector<int>(sol.qtdBins());
    std::iota( sorteio.begin(), sorteio.end(), 0 );

    random_shuffle (sorteio.begin(), sorteio.end());

    // cout << "Bins escolhidos: "<< endl;
    // for (int j = 0; j < binsSorted; j++){
    //     cout << "Bin " << sorteio[j]+1 << " Itens: " << sol.getContainer(sorteio[j]).toString() << endl;
    // }

    std::vector<int> itens = std::vector<int>();
    for (int j = 0; j < binsSorted; j++){
        for (int item : sol.getItensContainer(sorteio[j])){
            itens.push_back(item);
        }
    }

    // cout << "Itens a serem redimensionados: "<< endl;
    // for (unsigned int j = 0; j < itens.size(); j++){
    //     cout << itens[j]+1 << " ";
    // }
    // cout << endl;


    std::vector< std::string > names;
    std::vector< double > lb;
    std::vector< double > ub;
    std::vector< double > obj;
    std::vector< char > integer;
    std::vector< std::vector< int > > x = std::vector< std::vector< int > >(itens.size(),std::vector< int >(sol.qtdBins()));
    std::vector<int> y = std::vector<int>(sol.qtdBins());

    // variaveis
    for (int j = 0; j < binsSorted; j++){
        y[j] = names.size();
        names.push_back("y("+std::to_string(sorteio[j]+1)+")");
        lb.push_back( 0.0 );
        ub.push_back( 1 );
        obj.push_back( 1 );
        integer.push_back( 1 );
        for (unsigned int i = 0; i < itens.size(); i++){
            x[i][j] = names.size();
            //cout << "x("+to_string(itens[i])+","+to_string(sorteio[j])+")" << " ";
            names.push_back("x("+std::to_string(itens[i]+1)+","+std::to_string(sorteio[j]+1)+")");
            lb.push_back( 0.0 );
            ub.push_back( 1 );
            obj.push_back( 0 );
            integer.push_back( 1 );
        }
        //std::cin.get();
    }

    lp_add_cols(mip, obj, lb, ub, integer, names);
    //std::cin.get();
    #ifdef DEBUG
        cout << "Variáveis criadas "<< endl;
    #endif

    // restricao capacidade
    for (int j = 0; j < binsSorted; j++){
        std::vector< int > idx;
        std::vector< double > coef;
        for (unsigned int i = 0; i < itens.size(); i++){
            idx.push_back(x[i][j]);
            coef.push_back(inst_.w(itens[i]));
            
        }
        idx.push_back(y[j]);
        coef.push_back(-1 * inst_.c());
        
        lp_add_row( mip, idx, coef, "capacidade(j="+std::to_string(sorteio[j]+1)+")", 'L', 0 );
    }
    #ifdef DEBUG
        cout << "Restrição capacidade ok "<< endl;
    #endif

    // restricao alocacao unica
    for (unsigned int i = 0; i < itens.size(); i++){
        std::vector< int > idx;
        std::vector< double > coef;
        for (int j = 0; j < binsSorted; j++){
            idx.push_back(x[i][j]);
            coef.push_back(1);
        }
        lp_add_row( mip, idx, coef, "unico(i="+std::to_string(itens[i]+1)+")", 'E', 1 );
    }

    #ifdef DEBUG
        cout << "Restrição alocação ok "<< endl;
    #endif

    // restricao conflito
    for (unsigned int i = 0; i < itens.size(); i++){
        for (unsigned int j = 0; j < itens.size(); j++){
            if (inst_.conflito(itens[i],itens[j])){
                for (int k = 0; k < binsSorted; k++){
                    std::vector< int > idx;
                    std::vector< double > coef;
                    idx.push_back(x[i][k]);
                    coef.push_back(1);
                    idx.push_back(x[j][k]);
                    coef.push_back(1);
                    lp_add_row( mip, idx, coef, "conflito(i="+std::to_string(itens[i]+1)+",j="+std::to_string(itens[j]+1)+",k="+std::to_string(sorteio[k]+1)+")", 'L', 1 );
                }
            }
        }
    }
    #ifdef DEBUG
        cout << "Restrição conflito ok "<< endl;
    #endif

    lp_write_lp( mip, "teste.lp" );
    int status = lp_optimize( mip );
    if (status == 0 || status == 3){
    
        double* x_opt = lp_x(mip);


        // cout << "Valores obtidos: " << endl;
        // for (unsigned int i = 0; i < names.size(); i++){
        //     if (x_opt[i] == 1)
        //         cout << names[i] << " = " << x_opt[i] << endl;
        // }


        for (int j = 0; j < binsSorted; j++){

            // cout << "Limpando bin  " << sorteio[j]+1;

            sol.getContainer(sorteio[j]).clear();

            // cout << " itens:  " << sol.getContainer(sorteio[j]).toString() << endl;

            if (x_opt[y[j]] == 1){ // se o bin ainda continua na solução, inserir os itens dele
                for (unsigned int i = 0; i < itens.size(); i++){
                    if (x_opt[x[i][j]] == 1){
                        //cout << "variável  " << names[x[i][j]] << " no bin " << sorteio[j] << endl;
                        sol.getContainer(sorteio[j]).incluiritem(itens[i]);
                    }
                }
            }
            
            // cout << "bin: " << sorteio[j]+1 <<" Conteúdo:  " << sol.getContainer(sorteio[j]).toString() << endl;
            
        }
    }
    // cout << "Eliminating empty bins." << endl;
    sol.eliminateEmptyBins();
    // cout << "solution: " << endl;
    // sol.printSolution();
    
    //lp_write_sol(mip, "jssp_flow.sol");
    //std::cin.get();
    
    lp_free(&mip);
}

void BuscaLocal::optFreeSpace(Solution &sol, double pctBL){
    // cout << pctBL << " " << sol.qtdBins() << " " << pctBL*sol.qtdBins() << endl;
    int binsSorted = ceil(pctBL*sol.qtdBins());
    int timeout = 30; // time limit for mip to run is 30 sec

    LinearProgram* mip = lp_create();
    lp_set_print_messages(mip,0);
    // cout << "Timeout do modelo: "<< timeout <<" Quantidade de bins: " << binsSorted << endl;
    lp_set_max_seconds( mip, timeout );
    std::vector<int> sorteio = std::vector<int>(sol.qtdBins());
    std::iota( sorteio.begin(), sorteio.end(), 0 );

    random_shuffle (sorteio.begin(), sorteio.end());

    // cout << "Bins escolhidos: "<< endl;
    // for (int j = 0; j < binsSorted; j++){
    //     cout << "Bin " << sorteio[j]+1 << " Itens: " << sol.getContainer(sorteio[j]).toString() << endl;
    // }

    std::vector<int> itens = std::vector<int>();
    for (int j = 0; j < binsSorted; j++){
        for (int item : sol.getItensContainer(sorteio[j])){
            itens.push_back(item);
        }
    }

    // cout << "Itens a serem redimensionados: "<< endl;
    // for (unsigned int j = 0; j < itens.size(); j++){
    //     cout << itens[j]+1 << " ";
    // }
    // cout << endl;


    std::vector< std::string > names;
    std::vector< double > lb;
    std::vector< double > ub;
    std::vector< double > obj;
    std::vector< char > integer;
    std::vector< std::vector< int > > x = std::vector< std::vector< int > >(itens.size(),std::vector< int >(sol.qtdBins()));
    std::vector<int> y = std::vector<int>(sol.qtdBins());

    // variaveis
    for (int j = 0; j < binsSorted; j++){
        y[j] = names.size();
        names.push_back("y("+std::to_string(sorteio[j]+1)+")");
        lb.push_back( 0.0 );
        ub.push_back( 1 );
        obj.push_back( 0 );
        integer.push_back( 1 );
        for (unsigned int i = 0; i < itens.size(); i++){
            x[i][j] = names.size();
            //cout << "x("+to_string(itens[i])+","+to_string(sorteio[j])+")" << " ";
            names.push_back("x("+std::to_string(itens[i]+1)+","+std::to_string(sorteio[j]+1)+")");
            lb.push_back( 0.0 );
            ub.push_back( 1 );
            obj.push_back( 0 );
            integer.push_back( 1 );
        }
        //std::cin.get();
    }

    int c = names.size();
    names.push_back("c");
    lb.push_back( 0.0 );
    ub.push_back( inst_.c() );
    obj.push_back( 1 );
    integer.push_back( 1 );


    lp_add_cols(mip, obj, lb, ub, integer, names);
    //std::cin.get();
    #ifdef DEBUG
        cout << "Variáveis criadas "<< endl;
    #endif

    {
        std::vector< int > idx;
        std::vector< double > coef;
        for (unsigned int i = 0; i < itens.size(); i++){
            idx.push_back(x[i][binsSorted-1]);
            coef.push_back(-inst_.w(itens[i]));
            
        }
        idx.push_back(c);
        coef.push_back(1);
        lp_add_row( mip, idx, coef, "no_space", 'E', 0 );
    }
    
    

    // restricao capacidade
    for (int j = 0; j < binsSorted; j++){
        std::vector< int > idx;
        std::vector< double > coef;
        for (unsigned int i = 0; i < itens.size(); i++){
            idx.push_back(x[i][j]);
            coef.push_back(inst_.w(itens[i]));
            
        }
        idx.push_back(y[j]);
        coef.push_back(-1 * inst_.c());
        
        lp_add_row( mip, idx, coef, "capacidade(j="+std::to_string(sorteio[j]+1)+")", 'L', 0 );
    }
    #ifdef DEBUG
        cout << "Restrição capacidade ok "<< endl;
    #endif

    // restricao alocacao unica
    for (unsigned int i = 0; i < itens.size(); i++){
        std::vector< int > idx;
        std::vector< double > coef;
        for (int j = 0; j < binsSorted; j++){
            idx.push_back(x[i][j]);
            coef.push_back(1);
        }
        lp_add_row( mip, idx, coef, "unico(i="+std::to_string(itens[i]+1)+")", 'E', 1 );
    }

    #ifdef DEBUG
        cout << "Restrição alocação ok "<< endl;
    #endif

    // restricao conflito
    for (unsigned int i = 0; i < itens.size(); i++){
        for (unsigned int j = 0; j < itens.size(); j++){
            if (inst_.conflito(itens[i],itens[j])){
                for (int k = 0; k < binsSorted; k++){
                    std::vector< int > idx;
                    std::vector< double > coef;
                    idx.push_back(x[i][k]);
                    coef.push_back(1);
                    idx.push_back(x[j][k]);
                    coef.push_back(1);
                    lp_add_row( mip, idx, coef, "conflito(i="+std::to_string(itens[i]+1)+",j="+std::to_string(itens[j]+1)+",k="+std::to_string(sorteio[k]+1)+")", 'L', 1 );
                }
            }
        }
    }
    #ifdef DEBUG
        cout << "Restrição conflito ok "<< endl;
    #endif

    lp_write_lp( mip, "teste.lp" );
    int status = lp_optimize( mip );
    if (status == 0 || status == 3){
    
        double* x_opt = lp_x(mip);


        // cout << "Valores obtidos: " << endl;
        // for (unsigned int i = 0; i < names.size(); i++){
        //     if (x_opt[i] == 1)
        //         cout << names[i] << " = " << x_opt[i] << endl;
        // }


        for (int j = 0; j < binsSorted; j++){

            // cout << "Limpando bin  " << sorteio[j]+1;

            sol.getContainer(sorteio[j]).clear();

            // cout << " itens:  " << sol.getContainer(sorteio[j]).toString() << endl;

            if (x_opt[y[j]] == 1){ // se o bin ainda continua na solução, inserir os itens dele
                for (unsigned int i = 0; i < itens.size(); i++){
                    if (x_opt[x[i][j]] == 1){
                        //cout << "variável  " << names[x[i][j]] << " no bin " << sorteio[j] << endl;
                        sol.getContainer(sorteio[j]).incluiritem(itens[i]);
                    }
                }
            }
            
            // cout << "bin: " << sorteio[j]+1 <<" Conteúdo:  " << sol.getContainer(sorteio[j]).toString() << endl;
            
        }
    }
    // cout << "Eliminating empty bins." << endl;
    sol.eliminateEmptyBins();
    // cout << "solution: " << endl;
    // sol.printSolution();
    
    //lp_write_sol(mip, "jssp_flow.sol");
    //std::cin.get();
    
    lp_free(&mip);
}