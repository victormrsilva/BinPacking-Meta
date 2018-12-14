/* for those who want to use lp in a C++ code, 
 * this header adds some overloaded functions
 * to make it easy to call lp from C++ code */

extern "C"
{
#include "lp.h"
}

#include <vector>
#include <string>

/** @brief adds news column (variables)
 *
 *  adds new columns to lp, specifying objective function, bounds, integrality and names
 *
 *  @param lp the (integer) linear program
 *  @param obj objective function coefficients
 *  @param lb lower bounds - if NULL is specified then it is assumed that all variables have lb=0.0
 *  @param ub upper bounds - if NULL is specified then it is assumed that all variables have ub=infinity
 *  @param integer - vector of boolean values indicating if each variable is integer, if NULL all variables
 *     are assumed to be integral
 *  @param names variable names
 */

void lp_add_cols( LinearProgram *lp, 
        std::vector< double > obj,
        std::vector< double > lb,
        std::vector< double > ub,
        std::vector< char > integer,
        std::vector< std::string > names );

void lp_add_row( LinearProgram *lp, std::vector< int > idx, std::vector< double > coef, const std::string name, char sense, const double rhs );