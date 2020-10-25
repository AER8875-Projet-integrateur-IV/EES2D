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


#include "solver/Simulation.h"
#include "solver/Solver.h"
#include "mesh/Mesh.h"

namespace ees2d::solver::BC {

	void farfieldSupersonicInflow(const uint32_t& elemID1, solver::Solver::faceParams& faceParams , const solver::Simulation& sim );
	void farfieldSupersonicOutflow(const uint32_t& elemID1, solver::Solver::faceParams& faceParams , const solver::Simulation& sim );
	void farfieldSubsonicInflow(const uint32_t& elemID1, const uint32_t& faceid, solver::Solver::faceParams& faceParams , const solver::Simulation& sim , const ees2d::mesh::Mesh&);
	void farfieldSubsonicOutflow(const uint32_t& elemID1, const uint32_t& faceid, solver::Solver::faceParams& faceParams , const solver::Simulation& sim ,const ees2d::mesh::Mesh&);
	void wall(const uint32_t& elemID1, solver::Solver::faceParams& faceParams ,const  solver::Simulation& sim );


}