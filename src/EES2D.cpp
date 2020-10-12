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
#include "mesh/Mesh.h"
#include "utils/Timer.h"
#include <iostream>
#include <vector>

using ees2d::io::Su2Parser;
using ees2d::mesh::Mesh;
using ees2d::utils::Timer;
//using namespace ees2d::Utils;


int main() {
	Timer Timeit("software runtime");
	std::cout << "Euler2D Software" << std::endl;
	std::string path = "../../tests/testmesh.su2";

  Su2Parser parser(path);
	parser.Parse();


  Mesh mesh(parser);
	int a = mesh.connecPointSurrElement(3,3);
  mesh.solveElemSurrPoint();
	std::unique_ptr<uint32_t[]> esup2 = mesh.get_esup2();
  std::unique_ptr<uint32_t[]> esup1 = mesh.get_esup1();
	const uint32_t& esup2_size = mesh.get_esup2_size();
  const uint32_t& esup1_size = mesh.get_esup1_size();

	for(uint32_t i=0; i < esup1_size;i++){
		std::cout << esup1[i] << "\n";
	}




}
