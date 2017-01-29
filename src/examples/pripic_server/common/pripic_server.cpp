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
#include "pripic_server.h"
#include <sqlite3.h>

#define DEBUG

const string DATABASE_PATH = "/tmp/test.db";
const string GET_SIZE_QUERY = "select count(id) from user;";
const string GET_TABLE_QUERY = "select face_representation from user;";

uint32_t* get_db_size() {
    uint32_t* db_size = (uint32_t*) malloc(2 * sizeof(uint32_t));

    db_size[0] = 0;
    db_size[1] = 128;

    sqlite3 *database;
    sqlite3_stmt *statement;
#ifdef DEBUG
    printf("Opening database %s\n", DATABASE_PATH.c_str());
#endif
    int rc = sqlite3_open(DATABASE_PATH.c_str(), &database);
    if (rc) {
        fprintf(stderr, "Can't open database.\n");
        sqlite3_free(database);
        return NULL;
    }
#ifdef DEBUG
    printf("Opening database %s\n", DATABASE_PATH.c_str());
#endif
    rc = sqlite3_prepare_v2(database, GET_SIZE_QUERY.c_str(), -1, &statement, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Couldn't fetch data\n");
        sqlite3_close(database);
        return NULL;
    }
    rc = sqlite3_step(statement);

    if (rc == SQLITE_ROW) {
        db_size[0] = sqlite3_column_int(statement, 0);
    }

    //db_size[0] = 500;
    return db_size;
}

uint32_t** get_table() {
    uint32_t *db_size = get_db_size();
    uint32_t N = db_size[0], dim = db_size[1];
    free(db_size);

    sqlite3 *database;
    sqlite3_stmt *statement;
    int rc = sqlite3_open(DATABASE_PATH.c_str(), &database);
    if (rc) {
        fprintf(stderr, "Can't open database.\n");
        sqlite3_free(database);
        return NULL;
    }
    rc = sqlite3_prepare_v2(database, GET_TABLE_QUERY.c_str(), -1, &statement, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Couldn't fetch data\n");
        sqlite3_close(database);
        return NULL;
    }

    uint32_t** serverdb = (uint32_t**) malloc(sizeof(uint32_t*) * N);
    rc = sqlite3_step(statement);
    int i=0;
    const unsigned char* select_result;
    char temp=0;
    char buffer[9];
    buffer[8] = 0;

    while (rc == SQLITE_ROW) {
        select_result = sqlite3_column_text(statement, 0);
        printf("%s\n", select_result);
        serverdb[i] = (uint32_t*) malloc(sizeof(uint32_t) * dim);
        for(int j = 0; j < dim; j++) {
            memcpy(buffer, &select_result[8*j], 8*sizeof(char));
            printf("%s\n",buffer);
            sscanf(buffer,"%08lX",&serverdb[i][j]);
            printf("read %d\n", serverdb[i][j]);
        }
        i++;
        rc = sqlite3_step(statement);
    }
    return serverdb;
}

int32_t serve_argmin() {
    //query_db();
	uint32_t bitlen = 8, i, j, temp, tempsum, maxbitlen=32, secparam=128, nthreads=1;
	e_mt_gen_alg mt_alg = MT_OT;
    string address = "127.0.0.1";

    seclvl seclvl = get_sec_lvl(secparam);

	ABYParty* party = new ABYParty(SERVER, (char*) address.c_str(), seclvl, maxbitlen, nthreads, mt_alg);
    printf("* Client connected. ");
    fflush(stdout);
	vector<Sharing*>& sharings = party->GetSharings();
    uint32_t int_precomp = 0;
	ePreCompPhase precomp_phase_value = (ePreCompPhase) int_precomp;
	sharings[S_BOOL]->SetPreCompPhaseValue(precomp_phase_value);

	crypto* crypt = new crypto(seclvl.symbits, (uint8_t*) const_seed);
	uint32_t **serverdb;
	uint64_t verify;

	Circuit *distcirc, *mincirc;
	
	share ***Sshr, **Cshr, **Ssqr, *Csqr, *mindst;

	srand(time(NULL));

    uint32_t* dbsize = get_db_size();
    uint32_t N=dbsize[0], dim=dbsize[1];
    free(dbsize);
	//generate dbsize * dim * bitlen random bits as server db
    serverdb = get_table();
    /*
	serverdb = (uint32_t**) malloc(sizeof(uint32_t*) * N);
	for(i = 0; i < N; i++) {
		serverdb[i] = (uint32_t*) malloc(sizeof(uint32_t) * dim);
		for(j = 0; j < dim; j++) {
			serverdb[i][j] = rand() % ((uint64_t) 1 << bitlen);
		}
	}
    */

	distcirc = sharings[S_ARITH]->GetCircuitBuildRoutine();
	mincirc = sharings[S_BOOL]->GetCircuitBuildRoutine();
    
	//set server input
	Sshr = (share***) malloc(sizeof(share**) * N);
	for (i = 0; i < N; i++) {
		Sshr[i] = (share**) malloc(sizeof(share*) * dim);
		for (j = 0; j < dim; j++) {
			Sshr[i][j] = distcirc->PutINGate(serverdb[i][j], bitlen, SERVER);
		}
	}

	Ssqr = (share**) malloc(sizeof(share*) * N);
	for (i = 0; i < N; i++) {
		tempsum = 0; 
		for (j = 0; j < dim; j++) {
			temp = serverdb[i][j];
			tempsum += (temp * temp);
		}
		Ssqr[i] = mincirc->PutINGate(tempsum, 2*bitlen+ceil_log2(dim), SERVER);
	}

	//set client input
	Cshr = (share**) malloc(sizeof(share*) * dim);
	tempsum = 0;
	for (j = 0; j < dim; j++) {
		//temp = query[j];
		Cshr[j] = distcirc->PutINGate((uint32_t) 0, bitlen+1, CLIENT);
		//tempsum += (temp * temp);
	}
	Csqr = mincirc->PutINGate((uint32_t) 0, 2*bitlen+ceil_log2(dim), CLIENT);

	mindst = build_argmin_euclidean_dist_circuit(Sshr, Cshr, N, dim, Ssqr, Csqr, distcirc, (BooleanCircuit*) mincirc, sharings, S_BOOL);

	mindst = mincirc->PutOUTGate(mindst, ALL);

	party->ExecCircuit();

    int32_t result = mindst->get_clear_value<uint64_t>();
    printf("  Client served.\n");
    
	//TODO free
	for(uint32_t i = 0; i < N; i++) {
		free(serverdb[i]);
		free(Sshr[i]);
	}

	free(serverdb);
	free(Sshr);
	free(Ssqr);

	free(Cshr);

    return result;
}

//Build circuit
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    uint32_t** data;
    
    for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
  }

void query_db() {
    printf("connecting to database\n");
    sqlite3 *database;
    char *zErrMsg;
    string query = "select * from facedata;";
    int rc = sqlite3_open("/tmp/test.db", &database);
    if (rc) {
        fprintf(stderr, "Can't open database.\n");
        sqlite3_close(database);
        return;
    }
    rc = sqlite3_exec(database, query.c_str(), callback, 0, &zErrMsg);
    if (rc!=SQLITE_OK) {
        fprintf(stderr, "SQL error :( %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(database);
    printf("connected to database!\n");
}
