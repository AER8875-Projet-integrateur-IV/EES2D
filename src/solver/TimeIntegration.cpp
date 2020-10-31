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
#include "TimeIntegration.h"


using namespace ees2d::solver::TimeIntegration;
using namespace ees2d::solver;
using namespace ees2d::mesh;

void TimeIntegration::explicitEuler(const uint32_t &elem, Simulation &sim, Mesh &mesh) {
	double dtOverArea = (sim.dt[elem] * (-1)) / mesh.CvolumeArea(elem);
	//Computing delta vector of conservative variables values
	ConservativeVariables deltaW(sim.residuals[elem].m_rhoV_residual * dtOverArea,
	                             sim.residuals[elem].m_rho_uV_residual * dtOverArea,
	                             sim.residuals[elem].m_rho_vV_residual * dtOverArea,
	                             sim.residuals[elem].m_rho_HV_residual * dtOverArea);

	// Updating conservative Variables
	sim.conservativeVariables[elem].m_rho += deltaW.m_rho;
	sim.conservativeVariables[elem].m_rho_u += deltaW.m_rho_u;
	sim.conservativeVariables[elem].m_rho_v += deltaW.m_rho_v;
	sim.conservativeVariables[elem].m_rho_E += deltaW.m_rho_E;
}
// -------------------------------------
void TimeIntegration::RK45(const uint32_t &elem, Simulation &sim, Mesh &mesh) {

  ConservativeVariables W0 = sim.conservativeVariables[elem];
	Residual Q(sim.residuals[elem].m_rhoV_residual/W0.m_rho,
	           sim.residuals[elem].m_rho_uV_residual/W0.m_rho_u,
             sim.residuals[elem].m_rho_vV_residual/W0.m_rho_v,
             sim.residuals[elem].m_rho_HV_residual/W0.m_rho_E);


	double StageOneCoeff = 0.0533*(sim.dt[elem]/mesh.CvolumeArea(elem));
  ConservativeVariables W1 = W0 - sim.residuals[elem]*StageOneCoeff;

//	Residual R1 = W1-W0;
//	double StageTwoCoeff = 0.1263*;
//	double StageThreeCoeff =;
//	double StageFourCoeff;
//	double StageFiveCoeff;



}
