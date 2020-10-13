/* ---------------------------------------------------------------------
 *
 * Copyright (C) 2020 - by the EES2D  authors
 *
 * This file is part of EES2D.
 *
 *   EES2D is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   EES2D is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with EES2D.  If not, see <https://www.gnu.org/licenses/>.
 *
 * ---------------------------------------------------------------------
 *
 * Authors: Amin Ouled-Mohamed & Ali Omais, Polytechnique Montreal, 2020-
 */

#include "io/Su2Parser.h"
#include "mesh/Connectivity.h"
#include "utils/Timer.h"
#include <iostream>
#include <vector>

using ees2d::io::Su2Parser;
using ees2d::mesh::Connectivity;
using ees2d::utils::Timer;
//using namespace ees2d::Utils;


int main() {
	Timer Timeit("software runtime");
	std::cout << "Euler2D Software" << std::endl;

	std::string path = "../../tests/testmesh.su2";
	//std::string path = "/home/amin/Downloads/naca0012_euler_1025x1025x1_O_1B.su2";
	Su2Parser parser(path);
	parser.Parse();


	Connectivity connectivity(parser);
	connectivity.solve();

	auto psup2 = connectivity.get_psup2();
	auto psup2_size = connectivity.get_psup2_size();
	auto NPSUE = parser.get_NPSUE();

	auto esuel = connectivity.get_esuel();
	auto esuel_size = connectivity.get_esuel_size();

	for (auto &elem : (*esuel)) {
		for (auto &value : elem) {
			std::cout << value << " ";
		}
		std::cout << "\n";
	}
}
