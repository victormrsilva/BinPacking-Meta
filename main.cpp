#include <iostream>
#include <algorithm>


#include "Instance.h"
#include "GRASP.h"
#include "SA.h"
#include "Solution.h"

#include "Decoder.h"
#include "Solver.h"
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"

using std::cerr;
using std::cout;
using std::string;
using std::endl;

#include <cstdio>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

class Timeout;
static Timeout * global_timeout_instance = 0;

class Timeout {
public:
	int m_timeout;
	jmp_buf env;

	Timeout(int timeout) : m_timeout(timeout) {
		if (global_timeout_instance) {
			throw "Timeout already in use";
		}
		global_timeout_instance = this;
	}

	~Timeout() {
		stop();
		global_timeout_instance = 0;
	}

	static void alarm_handler(int signum) {
		longjmp(global_timeout_instance->env, 1);
	}

	void start() {
		Timeout * ptr = this;
		if (setjmp(env) != 0) {
			// Don't do anything except throw here, since the state
			// is... funky...
			printf("Alarm fired: %p\n", ptr);
			throw global_timeout_instance;
		}
		signal(SIGALRM, alarm_handler);
		alarm(m_timeout);
		printf("Alarm set: %p\n", ptr);
	}

	void stop() {
		alarm(0);
	}
};

int main(int argc, char** argv){

    if (argc < 2){
        cerr << "binpack arquivoInstancia" << endl;
        return 0;
    }
    std::srand(time(NULL));
	// cout << "Instance file: " << instanceFile << endl;
    std::vector<string> arguments(argv + 1, argv + argc);
    const string instanceFile = string(arguments[0]);
    Instance instance(instanceFile);

    std::string::size_type sz;
    // double alpha;
    int algoritmo = stoi(arguments[1], &sz);
    double pBL;
    if (algoritmo == 1){ // GRASP
        
        double alpha;
        int algoritmo = stoi(arguments[1], &sz);
        int maxIter;
        for(unsigned int i=2; i<arguments.size(); i+=2)
        {
            if(arguments[i]== "--MAX_ITER")
                maxIter = stoi(arguments[i+1], &sz);
            else if(arguments[i]== "--ALPHA")
                alpha = stod(arguments[i+1], &sz);
            else if(arguments[i]== "--PCT_BL")
                pBL = stod(arguments[i+1], &sz);
        }
        try {
            Timeout timeout(300); 
            timeout.start();
            GRASP g(instance, alpha, maxIter,pBL);        
            Solution s = g.execute();
            cout << s.obj() << endl;
        } catch (Timeout * t) {
            cout << "timeout" << endl;
        }
    } else if (algoritmo == 2){ // SA
        double alpha;
        double beta;
        double gama;
        double saMax;
        
        for(unsigned int i=2; i<arguments.size(); i+=2)
        {

            if(arguments[i]== "--SAMAX")
                saMax = stod(arguments[i+1], &sz);
            else if(arguments[i]== "--ALPHA")
                alpha = stod(arguments[i+1], &sz);
            else if(arguments[i]== "--BETA")
                beta = stod(arguments[i+1], &sz);
            else if(arguments[i]== "--GAMA")
                gama = stod(arguments[i+1], &sz);
            else if(arguments[i]== "--PCT_BL")
                pBL = stod(arguments[i+1], &sz);
        }
        try {
            Timeout timeout(300); 
            timeout.start();
            SA sa(instance, alpha, beta, gama, saMax, pBL);
            Solution s = sa.execute();
            cout << s.obj() << endl;
        } catch (Timeout * t) {
            cout << "timeout" << endl;
        }
    } else if (algoritmo == 3) { // BRKGA
        try {
            const clock_t begin = clock();
            clock_t end = clock();
            Timeout timeout(300); 
            timeout.start();
            Decoder decoder(instance); // initialize Decoder

            const long unsigned rngSeed = time(NULL);	// seed to the random number generator
            MTRand rng(rngSeed);					// initialize the random number generator
            // cout << "inicializou rng" << endl;

            const unsigned n = (unsigned) instance.n();		// size of chromosomes
            const unsigned p = 256;		// size of population
            const double pe = 0.10;		// fraction of population to be the elite-set
            const double pm = 0.10;		// fraction of population to be replaced by mutants
            const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
            const unsigned K = 3;		// number of independent populations
            const unsigned MAXT = 2;	// number of threads for parallel decoding

            // initialize the BRKGA-based heuristic
            // cout << "inicializando brkga" << endl;
            BRKGA< Decoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);
            // cout << "inicializou brkga" << endl;

                // BRKGA inner loop (evolution) configuration: Exchange top individuals
            const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
            const unsigned X_NUMBER = 2;	// exchange top 2 best
            const unsigned MAX_GENS = 1000;	// run for 1000 gens

            unsigned relevantGeneration = 1;	// last relevant generation: best updated or reset called
            const unsigned RESET_AFTER = 200;
            std::vector< double > bestChromosome;
            double bestFitness = std::numeric_limits< double >::max();

            // Print info about multi-threading:
            // #ifdef _OPENMP
            //     std::cout << "Running for " << MAX_GENS << " generations using " << MAXT
            //             << " out of " << omp_get_max_threads()
            //             << " available thread units..." << std::endl;
            // #endif
            // #ifndef _OPENMP
            //     std::cout << "Running for " << MAX_GENS
            //             << " generations without multi-threading..." << std::endl;
            // #endif

            // Run the evolution loop:
            unsigned generation = 1;		// current generation
            do {
                algorithm.evolve();	// evolve the population for one generation

                // Bookeeping: has the best solution thus far improved?
                if(algorithm.getBestFitness() < bestFitness) {
                    // Save the best solution to be used after the evolution chain:
                    relevantGeneration = generation;
                    bestFitness = algorithm.getBestFitness();
                    bestChromosome = algorithm.getBestChromosome();
                    
                    // std::cout << "\t" << generation
                    //         << ") Improved best solution thus far: "
                    //         << bestFitness << std::endl;
                    end = clock();
                    cout << (end - begin) / double(CLOCKS_PER_SEC) << ";" << algorithm.getBestFitness() << endl;
                }

                //  Evolution strategy: restart
                if(generation - relevantGeneration > RESET_AFTER) {
                    algorithm.reset();	// restart the algorithm with random keys
                    relevantGeneration = generation;
                    
                    // std::cout << "\t" << generation << ") Reset at generation "
                    //         << generation << std::endl;
                }

                // Evolution strategy: exchange top individuals among the populations
                if(generation % X_INTVL == 0 && relevantGeneration != generation) {
                    algorithm.exchangeElite(X_NUMBER);
                    
                    // std::cout << "\t" << generation
                    //         << ") Exchanged top individuals." << std::endl;
                }

                // Next generation?
                ++generation;
                end = clock();
                if (((end - begin) / double(CLOCKS_PER_SEC)) > 300) break;
            } while (generation < MAX_GENS);

            // print the fitness of the top 10 individuals of each population:
            // std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
            // const unsigned bound = std::min(p, unsigned(10));	// makes sure we have 10 individuals
            // for(unsigned i = 0; i < K; ++i) {
            //     std::cout << "Population #" << i << ":" << std::endl;
            //     for(unsigned j = 0; j < bound; ++j) {
            //         std::cout << "\t" << j << ") "
            //                 << algorithm.getPopulation(i).getFitness(j) << std::endl;
            //     }
            // }

            // rebuild the best solution:
            // Solver bestSolution(instance, bestChromosome);

            // // print its distance:
            // std::cout << "Best solution found has objective value = " << bestSolution.getBins() << std::endl;

            // std::vector< Container > best = bestSolution.getSolution();

            // for (unsigned i = 0; i < best.size(); i++){
            //     cout << best[i].toString() << endl;
            // }

            // cout << "================" << endl;
            // cout << best.size() << endl;
            // for (unsigned i = 0; i < best.size(); i++){
            //     cout << best[i].size();
            //     for (unsigned j = 0; j < best[i].itens().size(); j++){
            //         cout << " " << best[i].item(j)+1;
            //     }
            //     cout << endl;
            // }
        } catch (Timeout * t) {
            cout << "timeout" << endl;
        }
    }

	const clock_t begin = clock();

    // cout << "Quantidade de itens: " << instance.n() << endl;
    // cout << "Peso do container: " << instance.c() << endl;
    // int saMax = 5;//30*instance.n();
    //saMax = saMax * instance.n();
    // SA s(instance, 0.998, (double)2, 0.95, (double)saMax);
    
    // s.execute();
    // cout << "SA" << endl;
    // getchar();
	// GRASP g(instance, alpha, maxIter,pBL);
	

	const clock_t end = clock();
    
	// cout << "BRKGA run finished in " << (end - begin) / double(CLOCKS_PER_SEC) << " s." << endl;



    return 0;
} 