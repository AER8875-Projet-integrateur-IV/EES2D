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
#include "Solver.h"


using namespace ees2d::solver::TimeIntegration;
using namespace ees2d::solver;
using namespace ees2d::mesh;

void TimeIntegration::explicitEuler(Simulation &sim, Mesh &mesh) {
	for (uint32_t elem = 0; elem < sim.dt.size(); elem++) {
		// Updating conservative Variables
		sim.conservativeVariables[elem].m_rho -= (sim.residuals[elem].m_rhoV_residual) * (sim.dt[elem]) / mesh.CvolumeArea(elem);
		sim.conservativeVariables[elem].m_rho_u -= (sim.residuals[elem].m_rho_uV_residual) * (sim.dt[elem]) / mesh.CvolumeArea(elem);
		sim.conservativeVariables[elem].m_rho_v -= (sim.residuals[elem].m_rho_vV_residual) * (sim.dt[elem]) / mesh.CvolumeArea(elem);
		sim.conservativeVariables[elem].m_rho_E -= (sim.residuals[elem].m_rho_HV_residual) * (sim.dt[elem]) / mesh.CvolumeArea(elem);
	}
}
// -------------------------------------
void TimeIntegration::RK5(Simulation &sim, Mesh &mesh, const double &coeff,const std::vector<ConservativeVariables>& W0) {


	for (uint32_t elem = 0; elem < sim.dt.size(); elem++) {
		double commonCoeff = sim.dt[elem] / mesh.CvolumeArea(elem);

		double stageCoeff = coeff * commonCoeff;
		sim.conservativeVariables[elem].m_rho   = W0[elem].m_rho   - sim.residuals[elem].m_rhoV_residual   * stageCoeff;
		sim.conservativeVariables[elem].m_rho_u = W0[elem].m_rho_u - sim.residuals[elem].m_rho_uV_residual * stageCoeff;
		sim.conservativeVariables[elem].m_rho_v = W0[elem].m_rho_v - sim.residuals[elem].m_rho_vV_residual * stageCoeff;
		sim.conservativeVariables[elem].m_rho_E = W0[elem].m_rho_E - sim.residuals[elem].m_rho_HV_residual * stageCoeff;
	}
}