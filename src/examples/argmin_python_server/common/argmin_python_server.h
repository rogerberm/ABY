/**
 \file 		min-euclidean-dist-circuit.h
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

#ifndef __MIN_EUCL_DIST_H_
#define __MIN_EUCL_DIST_H_

#include "../../../abycore/circuit/circuit.h"
#include "../../../abycore/aby/abyparty.h"
#include <cassert>

uint32_t* get_db_size();

int32_t serve_argmin();

share* build_argmin_euclidean_dist_circuit(share*** S, share** C, uint32_t n, uint32_t d, share** Ssqr, share* Csqr,
		Circuit* distcirc, BooleanCircuit* mincirc, vector<Sharing*>& sharings, e_sharing minsharing);


#endif /* __MIN_EUCL_DIST_H_ */
