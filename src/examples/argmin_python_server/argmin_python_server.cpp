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

#include "common/argmin_python_server.h"

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

    printf("Waiting for connections...\n");
    int run = 1;
    while(run) {
        run = serve_argmin();
    }
	//test_argmin_eucliden_dist_circuit(role, (char*) address.c_str(), seclvl, nvals, dim, nthreads, mt_alg, S_ARITH, S_BOOL, precomp_phase_value);

	return 0;
}
