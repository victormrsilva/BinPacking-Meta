#include "Decoder.h"
#include "Solver.h"

Decoder::Decoder(const Instance &inst) : instance(inst){

};

Decoder::~Decoder() {
}

double Decoder::decode(const std::vector< double >& chromosome) const {
	// 1) Solve the problem (i.e., create a tour out of this chromosome):
	// Avoids race conditions by making sure we have a single TSPSolver for each thread calling
	// ::decode (as long as TSPSolver does not make use of 'static' or other gimmicks):
	Solver solver(instance, chromosome);
	// 2) Extract the fitness (tour distance):
	const double fitness = solver.getBins();

	// 3) Return:
	return fitness;
}