#include "SA.h"

#include <iostream>
#include <algorithm>
#include <random>

using std::cout;
using std::endl;


SA::SA(const Instance &instance,double a, double b, double g, double SAmaximum, double pBL) : inst_(instance), sol(instance), bl(instance){
    alpha = a;
    beta = b;
    gama = g;
    SAmax = SAmaximum;
    pctBL = pBL;
}

double SA::defineInitialTemperature(){
    double temp = 100; // initial temperature
    bool isOver = false;
    int accept = 0;
    double x = 0;
    while (!isOver){
        accept = 0;
        for (int i = 1; i <= SAmax; i++){
            int item = randomNumber(0, inst_.n());
            int container = randomNumber(0,(int)sol.qtdBins());
            std::pair<int,double> new_sol = sol.checkRandomNeighbor(item,container);
            double delta = new_sol.second - sol.obj();
            if (delta < 0){
                accept++;
            } else {
                x = randomNumber(0,1);
                if ( x < exp(-delta/temp)){
                    accept++;
                }
            }
        }

        if (accept >= gama * SAmax){
            isOver = true;
        }
        else {
            temp = beta * temp;
        }
    }
    return temp;
}


Solution SA::execute(){
    const clock_t begin = clock();
    generateRandomSolution();
    clock_t end = clock();
    cout << (end - begin) / double(CLOCKS_PER_SEC) << ";" << sol.obj() << endl;
    // for (int i = 0; i < inst_.n(); i++){
    //     cout << i+1 << "(" << sol.itemContainer(i)+1 << ") ";
    // }
    // sol.writeSolution();
    // getchar();
    double x;
    int iterT = 0;
    Solution s_aux = sol;

    double finalTemperature = 0.001;

    double temp = defineInitialTemperature();
    // cout << "Temperatura incial: " << temp << endl;
    //getchar();
    //cout << "temperatura: " << temp << "temp_final: " << finalTemperature << endl;
    while (temp > finalTemperature){
        for (int i = 1; i <= SAmax; i++){
            //cout << i << endl;
            //cout << "s_aux: " << endl;
            //s_aux.printSolution();
            Solution s_aux2 = s_aux;
            int item = randomNumber(0, inst_.n());
            int container = randomNumber(0,sol.qtdBins());
            // std::pair<int,double> new_sol = s_aux2.checkRandomNeighbor(item,container);
            // cout << item+1 << " " << new_sol.first << endl;
            // s_aux2.changeItemContainer(item,new_sol.first);
            // s_aux2.printSolution();
            bl.optFreeSpace(s_aux2,pctBL);
            // s_aux2.printSolution();
            // getchar();
            double delta = s_aux2.obj() - s_aux.obj();
            // cout << new_sol.second << " " << s_aux.obj() << " " << delta << endl;
            // getchar();
            //cout << s_aux.obj() << " " << new_sol.second << " " << sol.obj() << endl;
            if (delta < 0){ // best solution
                //cout << "troca item: " << item << " container: " << new_sol.first << endl;
                s_aux = s_aux2;
                if (s_aux.obj() < sol.obj()){
                    sol = s_aux;
                    end = clock();
                    cout << (end - begin) / double(CLOCKS_PER_SEC) << ";" << sol.obj() << endl;
                    // cout << "trocou solucao" << endl;
                    // cout << "s_aux" << endl;
                    // s_aux.printSolution();
                    // cout << "s" << endl;
                    // sol.printSolution();
                    // getchar();
                }
            } else {
                x = randomNumber(0,1);
                // cout << x << " " <<  exp(-delta/temp) << endl;
                if ( x < exp(-delta/temp)){
                    // cout << " passa por aqui" << endl;
                    //cout << "x troca item: " << item << " container: " << new_sol.first << endl;
                    s_aux = s_aux2;
                    //s_aux.moveRandomNeighbor(item,new_sol.first);
                }
            }
            // cout << "temp: " << temp << " iteracao: " <<  i << " fo_otimo:" << sol.obj() << " limite_teracoes: " << SAmax << endl;
            end = clock();
            if (((end - begin) / double(CLOCKS_PER_SEC)) > 300) return sol;
        }
        temp = alpha * temp;
        // sol.writeSolution();
        // cout << "temperatura: " << temp << " temp_final: " << finalTemperature << " fo: " << sol.obj() << endl;
        //getchar();
    }

    //cout << "Temperatura atual: " << temp << " fo_atual = " << sol.obj() << endl;
    return sol;
}


void SA::generateRandomSolution(){
    std::vector<int> v(inst_.n());
    std::iota(v.begin(), v.end(), 0);
    
    // random visit item order
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
    // getchar();
    for (int i = 0; i < inst_.n(); i++){
        sol.insertFirstFit(v[i]);
    }
}

double SA::randomNumber(int min, int max){
    if (min == max) return min;
    return ((double) (rand()%10000/10000.0)*(max-min) + min);
}

