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
#pragma once
#include "mesh/Mesh.h"
#include "solver/Simulation.h"
#include "solver/Solver.h"
namespace ees2d::post {

	class PostProcess {

public:
		PostProcess(ees2d::mesh::Mesh&, ees2d::solver::Simulation&);
		void solveCoefficients();
		void outwardNormal(const uint32_t &Elem1ID, const uint32_t &iface);

		std::vector<double> Cps;

private:
		ees2d::mesh::Mesh &m_mesh;
		ees2d::solver::Simulation &m_sim;
	};

}// namespace ees2d::post