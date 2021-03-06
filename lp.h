#ifndef LP_HEADER
#define LP_HEADER

#define LP_ME_DEFAULT     0
#define LP_ME_OPTIMALITY  1
#define LP_ME_FEASIBILITY 2

/* Optimization direction */
#define LP_MIN 0
#define LP_MAX 1

/* Optimization result: */
#define LP_OPTIMAL       0
#define LP_INFEASIBLE    1
#define LP_UNBOUNDED     2
#define LP_FEASIBLE      3
#define LP_INTINFEASIBLE 4
#define LP_NO_SOL_FOUND  5
#define LP_ERROR         6

/* types of cut generators */
#define LP_CUT_TYPES   8
#define LPC_GOMORY     0
#define LPC_REDUCE     1
#define LPC_MIR        2
#define LPC_TWO_MIR    3
#define LPC_L_AND_P    4
#define LPC_ZERO_HALF  5
#define LPC_KNAPSACK   6
#define LPC_FLOW       7

/* constraint types, used when querying model */
#define CONS_PARTITIONING  0
#define CONS_PACKING       1
#define CONS_COVERING      2
#define CONS_CARDINALITY   3
#define CONS_KNAPSACK      4
#define CONS_INV_KNAPSACK  5
#define CONS_FLOW_BIN      6
#define CONS_FLOW_INT      7
#define CONS_FLOW_MX       8
#define CONS_VBOUND        9
#define CONS_OTHER        10
#define CONS_NUMBER       11 /* number of types */

/* LP Callback step */
#define LPCB_CUTS 0
#define LPCB_HEUR 1

typedef struct _LinearProgram LinearProgram;
typedef LinearProgram * LinearProgramPtr;

/* Model input & output */
void lp_read( LinearProgram *lp, const char *fileName );
void lp_write_lp( LinearProgram *lp, const char *fileName );
void lp_write_sol( LinearProgram *lp, const char *fileName );
void lp_load_mip_start(LinearProgram *lp, int count, const char **colNames, const double *colValues);
/* uses indices */
void lp_load_mip_starti( LinearProgram *lp, int count, const int *colIndexes, const double *colValues );
int lp_read_mip_start( LinearProgram *lp, const char *fileName );
void lp_save_mip_start( LinearProgram *lp, const char *fileName );
/* tries to discover the source of infeasibility of a MIPStart */
void lp_mipstart_debug( LinearProgram *lp );

/* for debugging purposes: fixes mipstart variables
 * one by one and optimizes (if initial solution is invalid at 
 * some point an infeasible LP will appear) */
void lp_fix_mipstart( LinearProgram *lp );


/* Model creation, modification and destruction */
LinearProgram *lp_create();
LinearProgram *lp_clone( LinearProgram *lp );
void lp_add_row( LinearProgram *lp, const int nz, int *indexes, double *coefs, const char *name, char sense, const double rhs );
void lp_add_rows( LinearProgram *lp, int nRows, int *starts, int *idx, double *coef, char *sense, double *rhs, const char **names );
void lp_remove_row( LinearProgram *lp, int idxRow );
void lp_remove_rows( LinearProgram *lp, int nRows, int *rows );

/** @brief adds new columns
 *
 *  adds new columns to lp, specifying objective function, bounds, integrality and names
 *
 *  @param count number of columns
 *  @param obj objective function coefficients
 *  @param lb lower bounds - if NULL is specified then it is assumed that all variables have lb=0.0
 *  @param ub upper bounds - if NULL is specified then it is assumed that all variables have ub=infinity
 *  @param integer - vector of boolean values indicating if each variable is integer, if NULL all variables
 *     are assumed to be integral
 *  @param names variable names
 */
void lp_add_col( LinearProgram *lp, double obj, double lb, double ub, char integer, char *name, int nz, int *rowIdx, double *rowCoef );
void lp_add_cols( LinearProgram *lp, const int count, double *obj, double *lb, double *ub, char *integer, char **name );
void lp_add_cols_same_bound( LinearProgram *lp, const int count, double *obj, double lb, double ub, char *integer, char **name );
void lp_add_bin_cols( LinearProgram *lp, const int count, double *obj, char **name );
void lp_free( LinearProgramPtr *lp );
void lp_set_direction( LinearProgram *lp, const char direction );
int lp_get_direction( LinearProgram *lp );
void lp_set_obj( LinearProgram *lp, double *obj );
void lp_chg_obj(LinearProgram *lp, int count, int idx[], double obj[] );
void lp_set_rhs( LinearProgram *lp, int row, double rhs );
void lp_set_col_bounds( LinearProgram *lp, int col, const double lb, const double ub );
void lp_fix_col( LinearProgram *lp, int col, double val );
void lp_set_integer( LinearProgram *lp, int nCols, int cols[] );
LinearProgram *lp_pre_process( LinearProgram *lp );
// sets cutoff for MIP optimization, optionally also adds constraint */
void lp_add_cutoff( LinearProgram *lp, double cutoff, char addConstraint );
// higher values indicate that these fractional variables will be branched first
void lp_set_branching_priorities( LinearProgram *lp, int *priorities );
// 1: always chose up first, -1: always chose down first 0: automatic
void lp_set_branching_direction( LinearProgram *lp, int direction );

/* frees some static memory, must be called at the end of the program */
void lp_close_env();

/* Model optimization, results query
   and solution methods parameters */
void lp_set_mip_emphasis( LinearProgram *lp, const int mipEmphasis );
int lp_get_mip_emphasis( LinearProgram *lp );
int lp_optimize( LinearProgram *lp );
char *lp_status_str( int status, char *statusStr );
double lp_solution_time( LinearProgram *lp );
/* if it is a mip, optimizes as a continuous problem */
int lp_optimize_as_continuous( LinearProgram *lp );

/* add cuts over LP relaxation
 *   maxRoundsCuts[] is a vector of integers
 *   0...LP_CUT_TYPES-1 where for each cut one
 *   must indicate the maximum number of rounds
 *   where this cut is separated */
int lp_strengthen_with_cuts( LinearProgram *lp, const int maxRoundsCuts[] );

/* add some cut manually
 * or when using the callback */
void lp_add_cut( LinearProgram *lp, int nz, int *cutIdx, double *cutCoef, const char *name, char sense, double rhs );

/* primal and dual solution */
double lp_obj_value(LinearProgram *lp); /* obj value of best solution found */
double lp_best_bound(LinearProgram *lp); /* best valid bound for the optimal solution obj value found */
/* soluton */
double *lp_x( LinearProgram *lp );
/* dual varibles, price for rows - only available when solving continous models */
double *lp_row_price( LinearProgram *lp );
/* slack for rows, i.e. active (tight) rows have slack = 0 */
double *lp_row_slack( LinearProgram *lp );
/* reduced cost for columns - only available when solving continous models */
double *lp_reduced_cost( const LinearProgram *lp );
/* multiple solutions (if available) */
int lp_num_saved_sols( LinearProgram *lp );
double lp_saved_sol_obj( LinearProgram *lp, int isol );
double *lp_saved_sol_x( LinearProgram *lp, int isol );

/* command line options */
void lp_parse_options( LinearProgram *lp, int argc, const char **argv );
void lp_help_options( );

/* parameters - input/output */
void lp_set_sol_out_file_name( LinearProgram *lp, const char *sfn );
void lp_set_sol_in_file_name( LinearProgram *lp, const char *sfn );
/* parameters - heuristics */
void lp_set_heur_proximity( LinearProgram *lp, char onOff );
void lp_set_heur_fp_passes( LinearProgram *lp, int passes );
/* parameters - cuts */
void lp_set_cuts( LinearProgram *lp, char onOff );
/* parameters - input/output */
void lp_set_print_messages( LinearProgram *lp, char onOff );
/* parameters - limits */
void lp_set_max_seconds( LinearProgram *lp, int _max );
void lp_set_max_solutions( LinearProgram *lp, int _max );
void lp_set_max_nodes( LinearProgram *lp, int _max );
void lp_set_max_saved_sols( LinearProgram *lp, int _max );
void lp_set_abs_mip_gap( LinearProgram *lp, const double _value );
void lp_set_rel_mip_gap( LinearProgram *lp, const double _value );
/* parameters - parallel */
void lp_set_parallel( LinearProgram *lp, char onOff );


/* Model query */
char lp_is_mip( LinearProgram *lp );
char lp_is_integer( const LinearProgram *lp, const int j );
char lp_is_binary( const LinearProgram *lp, const int j );
void lp_cols_by_type( const LinearProgram *lp, int *binaries, int *integers, int *continuous );
int lp_cols( const LinearProgram *lp );
int lp_rows(const  LinearProgram *lp );
int lp_nz( const LinearProgram *lp );
int lp_row( const LinearProgram *lp, int row, int *idx, double *coef );
int lp_col( const LinearProgram *lp, int col, int *idx, double *coef );
double lp_rhs( const LinearProgram *lp, int row );
char lp_sense( const LinearProgram *lp, int row );
char *lp_row_name( const LinearProgram *lp, int row, char *dest );
char *lp_col_name( const LinearProgram *lp, int col, char *dest );
double lp_col_lb( const LinearProgram *lp, int col );
double lp_col_ub( const LinearProgram *lp, int col );
// returns the index of a variable or -1 if name not found
int lp_col_index( LinearProgram *lp, const char *name );
// returns the index of a constraint or -1 if name not found
int lp_row_index( LinearProgram *lp, const char *name );
const double *lp_obj_coef( LinearProgram *lp );
int lp_row_type( LinearProgram *lp, const int row );
void lp_rows_by_type( LinearProgram *lp, int rtype[] );
int *lp_original_colummns( LinearProgram *lp );

/* callback function prototype */
typedef int (*lp_cb)( LinearProgram *lp, int whereFrom, const int *origCols, LinearProgram *origLP, void *data );
/* enter callback info */
void lp_set_callback( LinearProgram *lp, lp_cb callback, void *data );

// global flag indicating if variable/row names will be stored, can save some memory when off
void lp_set_store_names( char store );

int lp_num_binary_cols( const LinearProgram *lp );

char* lp_col_types( const LinearProgram *lp);
void lp_write_mps( LinearProgram *lp, const char *fileName );

int lp_get_constr_by_name(LinearProgram *lp, const char *name);
//char** lp_string_vchar(const std::vector< std::string >& strv);

void fill_x(LinearProgram *lp, double *x);
void fill_rc(LinearProgram *lp, double *rc);

#endif
