/**
 \file 		min-euclidean-dist.cpp
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
 \brief		Minimum Euclidean distance Test class implementation.
 */

//Utility libs
#include "../../abycore/util/crypto/crypto.h"
#include "../../abycore/util/parse_options.h"
//ABY Party class
#include "../../abycore/aby/abyparty.h"

#include "common/argmin_python_client.h"

int32_t read_test_options(int32_t* argcp, char*** argvp, e_role* role, uint32_t* bitlen, uint32_t* nvals, uint32_t* dim, uint32_t* secparam, string* address, uint16_t* port, int32_t* test_op, ePreCompPhase* pre_comp_value) {

	uint32_t int_role = 0, int_port = 0, int_precomp = 0;
	bool useffc = false;

	parsing_ctx options[] = { { (void*) &int_role, T_NUM, "r", "Role: 0/1", true, false }, { (void*) nvals, T_NUM, "n", "Server's database size", true, false }, { (void*) dim, T_NUM, "d", "Dimension of input elements", true, false }, {
			(void*) bitlen, T_NUM, "b", "Bit-length, default 32", false, false }, { (void*) secparam, T_NUM, "s", "Symmetric Security Bits, default: 128", false, false }, {
			(void*) address, T_STR, "a", "IP-address, default: localhost", false, false }, { (void*) &int_port, T_NUM, "p", "Port, default: 7766", false, false }, {
			(void*) test_op, T_NUM, "t", "Single test (leave out for all operations), default: off", false, false }, 
{ (void*) &int_precomp, T_NUM, "c", "PrecompPhase: 0/1/2/3", false, false }  };

	if (!parse_options(argcp, argvp, options, sizeof(options) / sizeof(parsing_ctx))) {
		print_usage(*argvp[0], options, sizeof(options) / sizeof(parsing_ctx));
		cout << "Exiting" << endl;
		exit(0);
	}

	assert(int_role < 2);
	assert(int_precomp < 4);
	*role = (e_role) int_role;
	*pre_comp_value = (ePreCompPhase) int_precomp;

	if (int_port != 0) {
		assert(int_port < 1 << (sizeof(uint16_t) * 8));
		*port = (uint16_t) int_port;
	}

	//delete options;

	return 1;
}

int main(int argc, char** argv) {
	/*e_role role = SERVER;
	//uint32_t bitlen = 32, nvals = 500, secparam = 128, nthreads = 1, dim = 6;
	uint16_t port = 7766;
	string address = "127.0.0.1";
	int32_t test_op = -1;
	e_mt_gen_alg mt_alg = MT_OT;
	ePreCompPhase precomp_phase_value;

	read_test_options(&argc, &argv, &role, &bitlen, &nvals, &dim, &secparam, &address, &port, &test_op, &precomp_phase_value);

	seclvl seclvl = get_sec_lvl(secparam);
    */

    uint32_t* db_size = get_db_size();
    uint32_t N = db_size[0];
    uint32_t dims = db_size[1];
    printf("db size is %d x %d\n", db_size[0], db_size[1]);
    uint32_t *query = (uint32_t*) malloc(sizeof(uint32_t) * dims);

    uint32_t output;
    output = argmin(query);
    printf("The output was %d\n", output);
	//test_argmin_eucliden_dist_circuit(role, (char*) address.c_str(), seclvl, nvals, dim, nthreads, mt_alg, S_ARITH, S_BOOL, precomp_phase_value);

	return 0;
}

