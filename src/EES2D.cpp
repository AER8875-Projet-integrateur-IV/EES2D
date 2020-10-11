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
	std::string path = "../../tests/io/testmesh.su2";

	std::shared_ptr<Su2Parser> parser(std::make_shared<Su2Parser>(path));
	parser->Parse();
  auto coordinates = parser->get_coords();
  auto BoundaryConditions = parser->get_boundaryConditions();

  std::shared_ptr<Mesh<Su2Parser>> mesh(std::make_shared<Mesh<Su2Parser>>(parser));
	int a = mesh->connecPointElement(1, 3);
	std::cout << a << std::endl;

}
