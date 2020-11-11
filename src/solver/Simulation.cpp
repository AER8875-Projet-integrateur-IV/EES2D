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
#include "solver/ConservativeVariables.h"
#include "solver/ConvectiveFlux.h"


using namespace ees2d::solver;

Simulation::Simulation(ees2d::mesh::Mesh &mesh, ees2d::io::InputParser &simParameters) {

	// Output paths
	residualPath = simParameters.m_outputResidual;
	pressurePath = simParameters.m_outputPressure;
	meshPath = simParameters.m_meshFile;

	//Initialize freestream values and simulation parameters
	minResidual = simParameters.m_minResiudal;
  gammaInf = simParameters.m_Gamma;
	gasConstantInf = simParameters.m_gasConstant;
	soundSpeedInf = std::sqrt(simParameters.m_Gamma*(simParameters.m_Pressure/simParameters.m_Density));
	cfl = simParameters.m_cfl;
	maxIter = simParameters.m_maxIter;
	timeIntegration = simParameters.m_timeIntegration;
	MachInf = simParameters.m_velocity/(soundSpeedInf);
	aoa = simParameters.m_aoa;


	tempInf = simParameters.m_Temp;
  Einf = pressureInf/((gammaInf-1)*rhoInf)+((uInf*uInf + vInf*vInf)/2);

  uInf = MachInf*sqrt(gammaInf)*std::cos((3.14159265358979 / 180)* simParameters.m_aoa);
  vInf = MachInf*sqrt(gammaInf)*std::sin((3.14159265358979 / 180)* simParameters.m_aoa);
  rhoInf = 1.0;
  pressureInf = 1.0;
	Einf =pressureInf/((gammaInf-1)*rhoInf)+((uInf*uInf + vInf*vInf)/2);



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
  spectralRadii.resize(mesh.N_elems);


	convectiveFluxes.resize(mesh.N_elems);
	conservativeVariables.resize(mesh.N_elems);

  // fill solution vectors with Initial Conditions
	std::fill(u.begin(), u.end(), uInf);
	std::fill(v.begin(), v.end(), vInf);
	std::fill(Mach.begin(),Mach.end(),MachInf);
	std::fill(rho.begin(), rho.end(), rhoInf);
	std::fill(p.begin(), p.end(), pressureInf);
  std::fill(E.begin(), E.end(), Einf);
	std::fill(H.begin(), H.end(), E[0]+(pressureInf/rhoInf));
	std::fill(dt.begin(), dt.end(), 0);
	std::fill(convectiveFluxes.begin(),convectiveFluxes.end(),ConvectiveFlux(0,0,0,0));
	std::fill(conservativeVariables.begin(),conservativeVariables.end(),ConservativeVariables(rhoInf,rhoInf*uInf,rhoInf*vInf,rhoInf*E[0]));

}