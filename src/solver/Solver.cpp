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
    : m_sim(sim), m_mesh(mesh) {
}

// ---------------------------------------
void Solver::run() {
	uint32_t iteration = 0;
	std::cout << "Solver running ..." << std::endl;


	double maxResidual = 100000;
	while (maxResidual > 1e-5) {


		// ID of Elements on both sides of each face
		uint32_t Elem1ID;
		uint32_t Elem2ID;
		for (auto &residual : m_sim.residuals) {
			residual.reset();
		}
		for(auto& spec : m_sim.spectralRadii){
			spec = 0;
		}

		// variables at face MidPoint
		for (uint32_t iface = 0; iface < m_mesh.N_faces; iface++) {

			faceParams faceP;
			// Get the elements IDs connected to the face
			Elem1ID = m_mesh.FaceToElem(iface, 0);
			Elem2ID = m_mesh.FaceToElem(iface, 1);

			// Elem 1ID is the looped node


			std::vector<double> midFaceToElem1{m_mesh.CvolumeCentroid(Elem1ID).x - m_mesh.FaceMidPoint(iface).x,
			                                   m_mesh.CvolumeCentroid(Elem1ID).y - m_mesh.FaceMidPoint(iface).y};
			double midFaceToElemNorm = std::sqrt(midFaceToElem1[0] * midFaceToElem1[0] + midFaceToElem1[1] * midFaceToElem1[1]);
			double DotProduct = midFaceToElem1[0] * m_mesh.FaceVector(iface).x + midFaceToElem1[1] * m_mesh.FaceVector(iface).y;
			double angle = std::acos(DotProduct / (midFaceToElemNorm * 1)) * (180 / M_PI);
			if (angle < 90) {
				m_mesh.FaceVector(iface).x *= -1;
				m_mesh.FaceVector(iface).y *= -1;
			}

			//Initialize Convective Flux
			ConvectiveFlux Fc;

			// if boundary cells connected to the face
			if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {

				Fc = computeBCFlux(Elem1ID, Elem2ID, faceP, iface);

			}

			// if internal face
			else {
				if (Elem2ID < Elem1ID) {
					std::swap(Elem2ID, Elem1ID);
				}

				Fc = scheme::RoeScheme(Elem1ID,
				                       Elem2ID,
				                       iface,
				                       faceP,
				                       m_sim,
				                       m_mesh);
			}

			// Update residual of elements connected to face
			updateResidual(Elem1ID, Elem2ID, Fc, iface);

			// Update spectral radiation for timestep calculation
			updateSpectralRadii(Elem1ID, Elem2ID, faceP, iface);

			//std::cout << faceP.rho << "/" << faceP.u << "/" << faceP.v << "/" << faceP.p << std::endl;
		}

		// Loop through all elements to update timesteps and computes variables
		double courantNumber = 0.8;
		for (uint32_t elem = 0; elem < m_sim.dt.size(); elem++) {

			// Update time
			updateLocalTimeSteps(elem, courantNumber);

			// Update delta W of conservative Variables (rho, u ,v, E)
			TimeIntegration::explicitEuler(elem,m_sim,m_mesh);

			// update conservative and primitve variables (rho, u, v, E , p , M ,H)
			updateVariables(elem);
		}

		maxResidual = findMaxResidual();

		iteration += 1;
		std::cout << "Iteration :" << iteration << std::endl;
		std::cout << "maxResidual :" << maxResidual << std::endl;
	}
}

//----------------------------------------------------------------

ConvectiveFlux Solver::computeBCFlux(const uint32_t &Elem1ID, const uint32_t &Elem2ID, faceParams &faceP, const uint32_t &iface) {

	ConvectiveFlux Fc;

	// Contravariant Velocity
	double V = m_sim.u[Elem1ID] * m_mesh.FaceVector(iface).x + m_sim.v[Elem1ID] * m_mesh.FaceVector(iface).y;

	//Treatement of wall BC
	if (Elem2ID == uint32_t(-1)) {
		Fc = BC::wall(Elem1ID, iface, faceP, m_sim, m_mesh);
	}

	//Treamtement of farfield BC
	else if (Elem2ID == uint32_t(-3)) {
		//Treatement of Outflows
		if (m_sim.Mach[Elem1ID] >= 1 && V > 0) {
			Fc = BC::farfieldSupersonicOutflow(Elem1ID, iface, faceP, m_sim, m_mesh);
		} else if (m_sim.Mach[Elem1ID] < 1 && V > 0) {
			Fc = BC::farfieldSubsonicOutflow(Elem1ID, iface, faceP, m_sim, m_mesh);
		}
		// Treatment of Inflows

		else if (m_sim.Mach[Elem1ID] >= 1 && V <= 0) {
			Fc = BC::farfieldSupersonicInflow(Elem1ID, iface, faceP, m_sim, m_mesh);
		} else if (m_sim.Mach[Elem1ID] < 1 && V <= 0) {
			Fc = BC::farfieldSubsonicInflow(Elem1ID, iface, faceP, m_sim, m_mesh);
		}

	}

	return Fc;
}

//-------------------------------------------

void Solver::updateResidual(const uint32_t &Elem1ID, const uint32_t &Elem2ID, ConvectiveFlux &Fc, const uint32_t &iface) {
	// Calculating Residual if BC
	if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {
		m_sim.residuals[Elem1ID] += (Fc * m_mesh.FaceSurface(iface));
		// Calculating Residual if internal face
	} else {

		m_sim.residuals[Elem1ID] += (Fc * m_mesh.FaceSurface(iface));
		m_sim.residuals[Elem2ID] -= (Fc * m_mesh.FaceSurface(iface));
	}
}
// ----------------------------------------------------


void Solver::updateSpectralRadii(const uint32_t &Elem1ID, const uint32_t &Elem2ID, Solver::faceParams &faceP, const uint32_t &iface) {

  double elemSpectralRadii = (std::abs((faceP.u * m_mesh.FaceVector(iface).x + faceP.v * m_mesh.FaceVector(iface).y)) +
                              sqrt(m_sim.gammaInf * (faceP.p / faceP.rho)) )*
                             m_mesh.FaceSurface(iface);

	if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {
		m_sim.spectralRadii[Elem1ID] += elemSpectralRadii;
	} else {
		m_sim.spectralRadii[Elem1ID] += elemSpectralRadii;
		m_sim.spectralRadii[Elem2ID] += elemSpectralRadii;
	}
}

// ----------------------------------------------------------------


void Solver::updateLocalTimeSteps(const uint32_t &ielem, double &courantNumber) {
	m_sim.dt[ielem] = courantNumber * m_mesh.CvolumeArea(ielem) / (m_sim.spectralRadii[ielem]);
}
// ----------------------------------------------------------------

void Solver::updatedeltaW(const uint32_t &elem) {

	double dtOverArea = (m_sim.dt[elem] * (-1)) / m_mesh.CvolumeArea(elem);
	//Computing delta vector of conservative variables values
	ConservativeVariables deltaW(m_sim.residuals[elem].m_rhoV_residual * dtOverArea,
	                             m_sim.residuals[elem].m_rho_uV_residual * dtOverArea,
	                             m_sim.residuals[elem].m_rho_vV_residual * dtOverArea,
	                             m_sim.residuals[elem].m_rho_HV_residual * dtOverArea);

	// Updating conservative Variables
	m_sim.conservativeVariables[elem].m_rho += deltaW.m_rho;
	m_sim.conservativeVariables[elem].m_rho_u += deltaW.m_rho_u;
	m_sim.conservativeVariables[elem].m_rho_v += deltaW.m_rho_v;
	m_sim.conservativeVariables[elem].m_rho_E += deltaW.m_rho_E;
}

// ----------------------------------------------------


void Solver::updateVariables(const uint32_t &elem) {
	m_sim.rho[elem] = m_sim.conservativeVariables[elem].m_rho;
	m_sim.u[elem] = m_sim.conservativeVariables[elem].m_rho_u / m_sim.rho[elem];
	m_sim.v[elem] = m_sim.conservativeVariables[elem].m_rho_v / m_sim.rho[elem];
	m_sim.E[elem] = m_sim.conservativeVariables[elem].m_rho_E / m_sim.rho[elem];
	m_sim.p[elem] = (m_sim.gammaInf - 1) * m_sim.rho[elem] * (m_sim.E[elem] - ((m_sim.u[elem] * m_sim.u[elem] + m_sim.v[elem] * m_sim.v[elem]) / 2));

	m_sim.H[elem] = m_sim.E[elem] + (m_sim.p[elem] / m_sim.rho[elem]);
	m_sim.Mach[elem] = std::sqrt(m_sim.u[elem] * m_sim.u[elem] + m_sim.v[elem] * m_sim.v[elem]) / std::sqrt(m_sim.gammaInf * (m_sim.p[elem] / m_sim.rho[elem]));
}

// ----------------------------------------------------

double Solver::findMaxResidual() {
	double maxResidual = 0;
	double maxResidualinElement = 0;
	for (auto &ResidualinElement : m_sim.residuals) {

		//			std::cout << ResidualinElement.m_rhoV_residual << "/"
		//			          << ResidualinElement.m_rho_uV_residual << "/"
		//			          << ResidualinElement.m_rho_vV_residual << "/"
		//			          << ResidualinElement.m_rho_HV_residual << std::endl;
		maxResidualinElement = ResidualinElement.findMax();

		if (maxResidualinElement > maxResidual) {

			maxResidual = maxResidualinElement;
		}
	}
	return maxResidual;
}