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
#include <algorithm>
#include <cmath>


using ees2d::solver::Simulation;

Simulation::Simulation(ees2d::mesh::Mesh & mesh, ees2d::io::InputParser simParameters) {

	u.reserve(mesh.N_elems);
	v.reserve(mesh.N_elems);
  rho.reserve(mesh.N_elems);
  p.reserve(mesh.N_elems);
  H.reserve(mesh.N_elems);
  E.reserve(mesh.N_elems);

	std::fill(u.begin(),u.end(),simParameters.m_velocity*std::cos((M_PI/180)*simParameters.m_aoa));
	std::fill(v.begin(),v.end(),simParameters.m_velocity*std::sin((M_PI/180)*simParameters.m_aoa));
  std::fill(rho.begin(),rho.end(),simParameters.m_Density);
	std::fill(p.begin(),p.end(),simParameters.m_Pressure)
	std:fill


}