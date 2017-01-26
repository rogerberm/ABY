/**
 * PriPic Server: Secure Multiparty Database Querying using ABY framework
 */

#include "../../../abycore/circuit/circuit.h"
#include "../../../abycore/aby/abyparty.h"
#include <cassert>

uint32_t* get_db_size();

int32_t serve_argmin();

share* build_argmin_euclidean_dist_circuit(share*** S, share** C, uint32_t n, uint32_t d, share** Ssqr, share* Csqr,
		Circuit* distcirc, BooleanCircuit* mincirc, vector<Sharing*>& sharings, e_sharing minsharing);

void query_db() ;
