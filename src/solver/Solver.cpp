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
#include "Solver.h"
#include "BoundaryConditions.h"
#include "solver/Schemes.h"
using namespace ees2d::solver;

Solver::Solver(ees2d::solver::Simulation &sim, ees2d::mesh::Mesh &mesh)
    : m_sim(sim), m_mesh(mesh) {}

// ---------------------------------------
void Solver::run() {
	uint32_t iteration = 0;
	std::cout << "Solver running ..." << std::endl;

	double maxResidual = 1;
	while (iteration < 3) {
		faceParams faceP;

		// ID of Elements on both sides of each face
		uint32_t Elem1ID;
		uint32_t Elem2ID;
		for (auto &residual : m_sim.residuals) {
			residual.reset();
		}

		// variables at face MidPoint


		for (uint32_t iface = 0; iface < m_mesh.N_faces; iface++) {
			// Get the elements IDs connected to the face
			Elem1ID = m_mesh.FaceToElem(iface, 0);
			Elem2ID = m_mesh.FaceToElem(iface, 1);
			ConvectiveFlux Fc;

			// Check if boundary cells connected to the face
			// -----------------------
			if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {

				//Treatement of wall BC
				if (Elem2ID == uint32_t(-1)) {
					Fc = BC::wall(Elem1ID, iface, faceP, m_sim, m_mesh);
				}
				//Treamtement of farfield BC
				else if (Elem2ID == uint32_t(-3)) {
					//Treatement of Outflows
					if (m_sim.Mach[Elem1ID] > m_sim.MachInf) {
						if (m_sim.Mach[Elem1ID] >= 1) {
							Fc = BC::farfieldSupersonicOutflow(Elem1ID, iface, faceP, m_sim, m_mesh);
						} else if (m_sim.Mach[Elem1ID] < 1) {
							Fc = BC::farfieldSubsonicOutflow(Elem1ID, iface, faceP, m_sim, m_mesh);
						}
					}
					// Treatment of Inflows
					else if (m_sim.Mach[Elem1ID] < m_sim.MachInf) {
						if (m_sim.Mach[Elem1ID] >= 1) {
							Fc = BC::farfieldSupersonicInflow(Elem1ID, iface, faceP, m_sim, m_mesh);
						} else if (m_sim.Mach[Elem1ID] < 1) {
							Fc = BC::farfieldSubsonicInflow(Elem1ID, iface, faceP, m_sim, m_mesh);
						}
					} else if (m_sim.Mach[Elem1ID] == m_sim.MachInf) {
						faceP.p = m_sim.pressureInf;
						faceP.rho = m_sim.rhoInf;
						faceP.u = m_sim.uInf;
						faceP.v = m_sim.vInf;
						Fc = ConvectiveFlux(0, 0, 0, 0);
					}
				}


			}
			// ---------------------------

			else {
				Fc = scheme::RoeScheme(Elem1ID,
				                       Elem2ID,
				                       iface,
				                       faceP,
				                       m_sim,
				                       m_mesh);
			}

			//std::cout << Fc.m_rhoV << "/" << Fc.m_rho_uV << "/" << Fc.m_rho_vV << "/" << Fc.m_rho_HV << std::endl;
			double elemSpectralRadii = (std::abs(faceP.u * m_mesh.FaceVector(iface).x + faceP.v * m_mesh.FaceVector(iface).y) +
			                            sqrt(m_sim.gammaInf * (faceP.p / faceP.rho))) *
			                           m_mesh.FaceSurface(iface);



			// Calculating Residual
			if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {
				m_sim.residuals[Elem1ID] += (Fc * m_mesh.FaceSurface(iface));
				m_sim.spectralRadii[Elem1ID] += elemSpectralRadii;
			} else {
				m_sim.residuals[Elem1ID] += (Fc * m_mesh.FaceSurface(iface));
				m_sim.residuals[Elem2ID] -= (Fc * m_mesh.FaceSurface(iface));
				m_sim.spectralRadii[Elem1ID] += elemSpectralRadii;
				m_sim.spectralRadii[Elem2ID] += elemSpectralRadii;
			}
		}
		for(auto& spectral: m_sim.spectralRadii){
			std::cout <<spectral<<std::endl;
		}
		for (auto &res : m_sim.residuals) {
			std::cout << res.m_rhoV_residual << "/" << res.m_rho_uV_residual << "/" << res.m_rho_vV_residual << "/" << res.m_rho_HV_residual << std::endl;
		}


		double dtOverArea;
		for (uint32_t ielem = 0; ielem < m_sim.dt.size(); ielem++) {
			m_sim.dt[ielem] = 0.69 * m_mesh.CvolumeArea(ielem) / (m_sim.spectralRadii[ielem]);//Sigma chosen for second-order scheme with 3 stages
			dtOverArea = (m_sim.dt[ielem] * (-1)) / m_mesh.CvolumeArea(ielem);


			//Computing delta vector of conservative variables values
			ConservativeVariables deltaW(m_sim.residuals[ielem].m_rhoV_residual * dtOverArea,
			                             m_sim.residuals[ielem].m_rho_uV_residual * dtOverArea,
			                             m_sim.residuals[ielem].m_rho_vV_residual * dtOverArea,
			                             m_sim.residuals[ielem].m_rho_HV_residual * dtOverArea);

			// Updating conservative Variables
			m_sim.conservativeVariables[ielem].m_rho += deltaW.m_rho;
			m_sim.conservativeVariables[ielem].m_rho_u += deltaW.m_rho_u;
			m_sim.conservativeVariables[ielem].m_rho_v += deltaW.m_rho_v;
			m_sim.conservativeVariables[ielem].m_rho_E += deltaW.m_rho_E;


			m_sim.rho[ielem] = m_sim.conservativeVariables[ielem].m_rho;
			m_sim.u[ielem] = m_sim.conservativeVariables[ielem].m_rho_u / m_sim.rho[ielem];
			m_sim.v[ielem] = m_sim.conservativeVariables[ielem].m_rho_v / m_sim.rho[ielem];
			m_sim.E[ielem] = m_sim.conservativeVariables[ielem].m_rho_E / m_sim.rho[ielem];
			m_sim.p[ielem] = (m_sim.gammaInf - 1) * m_sim.rho[ielem] * (m_sim.E[ielem] - ((m_sim.u[ielem] * m_sim.u[ielem] + m_sim.v[ielem] * m_sim.v[ielem]) / 2));

			m_sim.H[ielem] = m_sim.E[ielem] + m_sim.p[ielem] / m_sim.rho[ielem];
			m_sim.Mach[ielem] = std::sqrt(m_sim.u[ielem] * m_sim.u[ielem] + m_sim.v[ielem] * m_sim.v[ielem]) / std::sqrt(m_sim.gammaInf * (m_sim.p[ielem] / m_sim.rho[ielem]));
		}
		maxResidual = 0;
		double maxResidualinElement = 0;
		for (auto &ResidualinElement : m_sim.residuals) {
			std::cout << ResidualinElement.m_rhoV_residual << "/"
			          << ResidualinElement.m_rho_uV_residual << "/"
			          << ResidualinElement.m_rho_vV_residual << "/"
			          << ResidualinElement.m_rho_HV_residual << std::endl;

			maxResidualinElement = ResidualinElement.findMax();
			if (maxResidualinElement > maxResidual) {

				maxResidual = maxResidualinElement;
			}
		}

		iteration += 1;
		std::cout << "Iteration :" << iteration << std::endl;
		std::cout << "maxResidual :" << maxResidual << std::endl;

	}
}
