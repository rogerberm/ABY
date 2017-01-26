/**
 \file 		min-euclidean-dist-circuit.cpp
 \author 	michael.zohner@ec-spride.de
 \copyright	ABY - A Framework for Efficient Mixed-protocol Secure Two-party Computation
			Copyright (C) 2015 Engineering Cryptographic Protocols Group, TU Darmstadt
			This program is free software: you can redistribute it and/or modify
			it under the terms of the GNU Affero General Public License as published
			by the Free Software Foundation, either version 3 of the License, or
			(at your option) any later version.
			This program is distributed in the hope that it will be useful,
			but WITHOUT ANY WARRANTY; without even the implied warranty of
			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
			GNU Affero General Public License for more details.
			You should have received a copy of the GNU Affero General Public License
			along with this program. If not, see <http://www.gnu.org/licenses/>.
 \brief		Implementation of Minimum Euclidean Distance Circuit
 */
#include "argmin_python_client.h"

uint32_t* get_db_size() {
    uint32_t* db_size = (uint32_t*) malloc(2 * sizeof(uint32_t));
    db_size[0] = 500;
    db_size[1] = 128;
    return db_size;
}

int32_t argmin(uint32_t* query) {
	uint32_t bitlen = 8, i, j, temp, tempsum, maxbitlen=32, secparam=128, nthreads=1;
	e_mt_gen_alg mt_alg = MT_OT;
    string address = "127.0.0.1";

    seclvl seclvl = get_sec_lvl(secparam);

	ABYParty* party = new ABYParty(CLIENT, (char*) address.c_str(), seclvl, maxbitlen, nthreads, mt_alg);
	vector<Sharing*>& sharings = party->GetSharings();
    uint32_t int_precomp = 0;
	ePreCompPhase precomp_phase_value = (ePreCompPhase) int_precomp;
	sharings[S_BOOL]->SetPreCompPhaseValue(precomp_phase_value);

	crypto* crypt = new crypto(seclvl.symbits, (uint8_t*) const_seed);
	uint64_t verify;

	Circuit *distcirc, *mincirc;
	
	share ***Sshr, **Cshr, **Ssqr, *Csqr, *mindst;

    distcirc = sharings[S_ARITH]->GetCircuitBuildRoutine();
    mincirc = sharings[S_BOOL]->GetCircuitBuildRoutine();
	//set server input
    uint32_t* dbsize = get_db_size();
    uint32_t N=dbsize[0], dim=dbsize[1];
    //free(dbsize);
	Sshr = (share***) malloc(sizeof(share**) * N);
	for (i = 0; i < N; i++) {
		Sshr[i] = (share**) malloc(sizeof(share*) * dim);
		for (j = 0; j < dim; j++) {
			Sshr[i][j] = distcirc->PutINGate((uint32_t) 0, bitlen, SERVER);
		}
	}

	Ssqr = (share**) malloc(sizeof(share*) * N);
	for (i = 0; i < N; i++) {
		Ssqr[i] = mincirc->PutINGate((uint32_t) 0, 2*bitlen+ceil_log2(dim), SERVER);
	}

	//set client input
	Cshr = (share**) malloc(sizeof(share*) * dim);
	tempsum = 0;
	for (j = 0; j < dim; j++) {
		temp = query[j];
		Cshr[j] = distcirc->PutINGate(2*temp, bitlen+1, CLIENT);
		tempsum += (temp * temp);
	}
	Csqr = mincirc->PutINGate(tempsum, 2*bitlen+ceil_log2(dim), CLIENT);

	mindst = build_argmin_euclidean_dist_circuit(Sshr, Cshr, N, dim, Ssqr, Csqr, distcirc, (BooleanCircuit*) mincirc, sharings, S_BOOL);

	mindst = mincirc->PutOUTGate(mindst, ALL);

	party->ExecCircuit();

    int32_t result = mindst->get_clear_value<uint64_t>();
    
	//TODO free
	for(uint32_t i = 0; i < N; i++) {
		free(Sshr[i]);
	}

	free(Sshr);
	free(Ssqr);

	free(Cshr);

    return result;
}

//Build_
share* build_argmin_euclidean_dist_circuit(share*** S, share** C, uint32_t n, uint32_t d, share** Ssqr, share* Csqr,
		Circuit* distcirc, BooleanCircuit* mincirc, vector<Sharing*>& sharings, e_sharing minsharing) {
	share **distance, **indices, *temp, *argmindist;
	uint32_t i, j;

	Circuit *yaocirc = sharings[S_YAO]->GetCircuitBuildRoutine();

	distance = (share**) malloc(sizeof(share*) * n);
	indices = (share**) malloc(sizeof(share*) * n);
	assert(mincirc->GetCircuitType() == C_BOOLEAN);

	for (i=0; i < n; i++) {
		distance[i] = distcirc->PutMULGate(S[i][0], C[0]);
		indices[i] = mincirc->PutINGate(i, 32, CLIENT);
		for (j=1; j < d; j++) {
			temp = distcirc->PutMULGate(S[i][j], C[j]);
			distance[i] = distcirc->PutADDGate(distance[i], temp);
		}
		temp = mincirc->PutADDGate(Ssqr[i], Csqr);
		if(minsharing == S_BOOL) {
			distance[i] = ((BooleanCircuit*)yaocirc)->PutA2YGate(distance[i]);
			distance[i] = mincirc->PutY2BGate(distance[i]);
		}
		else if(minsharing == S_YAO) {
			distance[i] = mincirc->PutA2YGate(distance[i]);
		}
		distance[i] = mincirc->PutSUBGate(temp, distance[i]);
	}
  
	argmindist = mincirc->PutArgminGate(distance, indices, n);
	free(distance);
	return argmindist;
    //return S[0][0];
}
