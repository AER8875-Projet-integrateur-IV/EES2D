/*
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

#include "io/InputParser.h"
#include "mesh/Mesh.h"
#pragma once


namespace ees2d::solver {


struct Simulation {
	Simulation(ees2d::mesh::Mesh& ,ees2d::io::InputParser);

	// Vectors
	std::vector<float> u;
  std::vector<float> v;
  std::vector<float> rho;
  std::vector<float> p;
  std::vector<float> H;
	std::vector<float> E;
  std::vector<float> dt;
	std::vector<float>
};

} // namespace ees2d::solver