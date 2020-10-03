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

#include "mesh/Mesh.h"
#include <iostream>
//include "utils/Timer.h"

using namespace ees2d::Mesh;
//using namespace ees2d::Utils;


int main() {
	std::cout << "Euler2D Software" << std::endl;
	std::string path = "../../tests/mesh/naca0012_euler_65x65x1_O_1B.su2";
	std::unique_ptr<Su2Mesh> mymesh(std::make_unique<Su2Mesh>(path));
	//Timer timeit(std::string("Su2Mesh class"));
	mymesh->Parse();
}
