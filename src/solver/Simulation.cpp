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


using namespace ees2d::solver;

Simulation::Simulation(ees2d::mesh::Mesh &mesh, ees2d::io::InputParser &simParameters) {

	//Initialize freestream values
  gammaInf = simParameters.m_Gamma;
	gasConstantInf = simParameters.m_gasConstant;
	uInf  = simParameters.m_velocity * std::cos((M_PI / 180)* simParameters.m_aoa ) ;
	vInf =  simParameters.m_velocity * std::sin((M_PI / 180) * simParameters.m_aoa) ;
	soundSpeedInf = std::sqrt(simParameters.m_Gamma*simParameters.m_Temp*simParameters.m_gasConstant);
	MachInf = simParameters.m_velocity/(soundSpeedInf);
	rhoInf = simParameters.m_Density;
	pressureInf = simParameters.m_Pressure;
	tempInf = simParameters.m_Temp;

	// Initialize solution vectors
	u.resize(mesh.N_elems);
	v.resize(mesh.N_elems);
	rho.resize(mesh.N_elems);
	p.resize(mesh.N_elems);
	H.resize(mesh.N_elems);
	E.resize(mesh.N_elems);
	dt.resize(mesh.N_elems);
	residuals.resize(mesh.N_elems);
	Mach.resize(mesh.N_elems);
	temp.resize(mesh.N_elems);

  // fill solution vectors with Initial Conditions
	std::fill(u.begin(), u.end(), uInf);
	std::fill(v.begin(), v.end(), vInf);
	std::fill(Mach.begin(),Mach.end(),MachInf);
	std::fill(rho.begin(), rho.end(), rhoInf);
	std::fill(p.begin(), p.end(), pressureInf);
	std::fill(H.begin(), H.end(), 0);
	std::fill(dt.begin(), dt.end(), 0);
	std::fill(E.begin(), E.end(), 0);
	std::fill(residuals.begin(),residuals.end(),0);
	std::fill(temp.begin(),temp.end(),tempInf);

}