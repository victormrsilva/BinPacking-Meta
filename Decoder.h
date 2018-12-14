#ifndef _DECODER_H
#define _DECODER_H

#include "Instance.h"

#include <list>
#include <vector>
#include <algorithm>

class Decoder{

public:
	Decoder(const Instance &inst);	// Constructor
	~Decoder();	// Destructor

	// Decode a chromosome, returning its fitness as a double-precision floating point:
	double decode(const std::vector< double >& chromosome) const;

private:
    const Instance &instance;

};

#endif