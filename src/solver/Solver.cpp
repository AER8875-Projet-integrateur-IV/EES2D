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
#include <cstdlib>
#include <iomanip>


using namespace ees2d::solver;

Solver::Solver(ees2d::solver::Simulation &sim, ees2d::mesh::Mesh &mesh)
    : m_sim(sim), m_mesh(mesh) {
}

// ---------------------------------------
void Solver::run() {

	std::ofstream residualStream("../../../Documents/Residual.dat");

	residualStream << "--------------------------"
	               << " RESIDUAL ---------------------------\n";

	residualStream << "rho" << std::setw(18)
	               << "rhoU" << std::setw(15)
	               << "rhoV" << std::setw(15)
	               << "rhoH"
	               << "\n";


	uint32_t iteration = 0;
	std::cout << "Solver running ..." << std::endl;

	std::vector<double> RK5_coeffs = {0.0533, 0.1263, 0.2375, 0.4414, 1};
	double maxResidual = 100000;
	//double courant_number = 1 / m_sim.MachInf;
	double courant_number = 0.8;
	while (iteration < 6000) {


		// ID of Elements on both sides of each face
		uint32_t Elem1ID;
		uint32_t Elem2ID;


		for (auto &residual : m_sim.residuals) {
			residual.reset();
		}


		for (auto &spec : m_sim.spectralRadii) {
			spec = 0;
		}

		// variables at face MidPoint
		for (uint32_t iface = 0; iface < m_mesh.N_faces; iface++) {


			//      if(Elem1ID == 240){
			//        std::cout << "here" <<  std::endl;
			//      }

			faceParams faceP;
			// Get the elements IDs connected to the face
			Elem1ID = m_mesh.FaceToElem(iface, 0);
			Elem2ID = m_mesh.FaceToElem(iface, 1);

			if (Elem2ID < Elem1ID) {
				std::swap(Elem2ID, Elem1ID);
			}

			// Elem 1ID is the looped node
			computeNormalOrientation(Elem1ID, iface);

			//Initialize Convective Flux
			ConvectiveFlux Fc;


			// if boundary cells connected to the face
			if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {

				Fc = computeBCFlux(Elem1ID, Elem2ID, faceP, iface);

			}


			// if internal face
			else {
				Fc = scheme::RoeScheme(Elem1ID,
				                       Elem2ID,
				                       iface,
				                       faceP,
				                       m_sim,
				                       m_mesh);
			}

			if (std::isnan(Fc.m_rhoV) || std::isnan(Fc.m_rho_uV) || std::isnan(Fc.m_rho_vV) || std::isnan(Fc.m_rho_HV)) {
				std::cerr << "Error : nan flux found at iteration " << iteration << " and elem : " << Elem1ID << std::endl;
				std::exit(EXIT_FAILURE);
			}

			// Update residual of elements connected to face
			updateResidual(Elem1ID, Elem2ID, Fc, iface);

			// Update spectral radiation for timestep calculation
			updateSpectralRadii(Elem1ID, Elem2ID, faceP, iface);

			//std::cout << faceP.rho << "/" << faceP.u << "/" << faceP.v << "/" << faceP.p << std::endl;
		}


		// Update delta W of conservative Variables (rho, u ,v, E)

		//				for (auto &coeff : RK5_coeffs) {
		//					RK5(coeff,1.5);
		//				}

		eulerExplicit(courant_number);


		double maxrhoResidual = findMaxRhoResidual();
		double maxUResidual = findMaxRhoUResidual();
		double maxVResidual = findMaxRhoVResidual();
		double maxHResiudal = findMaxRhoHResidual();

		std::vector<double> maxResiduals = {maxrhoResidual, maxUResidual, maxVResidual, maxHResiudal};

		std::sort(maxResiduals.begin(), maxResiduals.end());
		maxResidual = maxResiduals.back();
		//maxResidual = maxrhoResidual;

		iteration += 1;
		if (iteration % 100 == 0) {

			std::cout << "Iteration :" << iteration << std::endl;
			std::cout << " rho : " << maxrhoResidual << " u : " << maxUResidual << " v : " << maxVResidual << " H : " << maxHResiudal << std::endl;
			std::cout << maxResidual << std::endl;
		}


		residualStream << maxrhoResidual << std::setw(15)
		               << maxUResidual << std::setw(15)
		               << maxVResidual << std::setw(15)
		               << maxHResiudal << "\n";
	}
	residualStream.close();
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

// --------------------------------------
void Solver::updateSpectralRadii(const uint32_t &Elem1ID, const uint32_t &Elem2ID, Solver::faceParams &faceP, const uint32_t &iface) {
  //double c = sqrt(m_sim.gammaInf * (faceP.p / faceP.rho));
	//double elemSpectralRadiiX = 0.5 * (std::abs(faceP.u) + c ) * std::abs(m_mesh.FaceVector(iface).x * m_mesh.FaceSurface(iface));
	//double elemSpectralRadiiY = 0.5 * (std::abs(faceP.v) + c ) *std::abs(m_mesh.FaceVector(iface).y * m_mesh.FaceSurface(iface));
  double elemSpectralRadii = (std::abs((faceP.u * m_mesh.FaceVector(iface).x + faceP.v * m_mesh.FaceVector(iface).y)) +
                              sqrt(m_sim.gammaInf * (faceP.p / faceP.rho)) )*
                             m_mesh.FaceSurface(iface);


	if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {

		//m_sim.spectralRadiiX[Elem1ID] += elemSpectralRadiiX;
		//m_sim.spectralRadiiY[Elem1ID] += elemSpectralRadiiY;
		m_sim.spectralRadii[Elem1ID] += elemSpectralRadii;
	} else {
    m_sim.spectralRadii[Elem1ID] += elemSpectralRadii;
    m_sim.spectralRadii[Elem2ID] += elemSpectralRadii;
		//m_sim.spectralRadiiX[Elem1ID] += elemSpectralRadiiX;
		//m_sim.spectralRadiiY[Elem1ID] += elemSpectralRadiiY;
		//m_sim.spectralRadiiX[Elem2ID] += elemSpectralRadiiX;
		//m_sim.spectralRadiiY[Elem2ID] += elemSpectralRadiiY;
	}
}
//----------------------------------------------------------------
void Solver::eulerExplicit(double courantNumber) {
	// Update time
	updateLocalTimeSteps(courantNumber);

	TimeIntegration::explicitEuler(m_sim, m_mesh);
	Solver::updateVariables();
}
// ----------------------------------------------------------------
void Solver::RK5(const double &coeff, double courantNumber) {
	// Update time
	updateLocalTimeSteps(courantNumber);

	TimeIntegration::RK5(m_sim, m_mesh, coeff);
	Solver::updateVariables();
	if (coeff != 1) {
		uint32_t Elem1ID;
		uint32_t Elem2ID;

		for (auto &residual : m_sim.residuals) {
			residual.reset();
		}
		for (auto &spec : m_sim.spectralRadiiX) {
			spec = 0;
		}
		for (auto &spec : m_sim.spectralRadiiY) {
			spec = 0;
		}


		for (uint32_t iface = 0; iface < m_mesh.N_faces; iface++) {

			faceParams faceP;
			// Get the elements IDs connected to the face
			Elem1ID = m_mesh.FaceToElem(iface, 0);
			Elem2ID = m_mesh.FaceToElem(iface, 1);

			// Elem 1ID is the looped node
			computeNormalOrientation(Elem1ID, iface);

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
			updateSpectralRadii(Elem1ID, Elem2ID, faceP, iface);
		}
	}
}
//----------------------------------------------------------------

void Solver::updateLocalTimeSteps(double &courantNumber) {
	for (uint32_t ielem = 0; ielem < m_sim.dt.size(); ielem++) {
    m_sim.dt[ielem] = courantNumber * m_mesh.CvolumeArea(ielem) / (m_sim.spectralRadii[ielem]);

//		m_sim.dt[ielem] =   (courantNumber * m_mesh.CvolumeArea(ielem) /
//            ( (m_sim.spectralRadiiX[ielem]) + m_sim.spectralRadiiY[ielem] ) );
	}
}

// ----------------------------------------------------


void Solver::updateVariables() {
	for (uint32_t elem = 0; elem < m_sim.dt.size(); elem++) {
		m_sim.rho[elem] = m_sim.conservativeVariables[elem].m_rho;

		m_sim.u[elem] = m_sim.conservativeVariables[elem].m_rho_u / m_sim.rho[elem];
		m_sim.v[elem] = m_sim.conservativeVariables[elem].m_rho_v / m_sim.rho[elem];
		m_sim.E[elem] = m_sim.conservativeVariables[elem].m_rho_E / m_sim.rho[elem];
		m_sim.p[elem] = (m_sim.gammaInf - 1) * m_sim.rho[elem] * (m_sim.E[elem] - ((m_sim.u[elem] * m_sim.u[elem] + m_sim.v[elem] * m_sim.v[elem]) / 2));

		m_sim.H[elem] = m_sim.E[elem] + (m_sim.p[elem] / m_sim.rho[elem]);
		m_sim.Mach[elem] = std::sqrt(m_sim.u[elem] * m_sim.u[elem] + m_sim.v[elem] * m_sim.v[elem]) / std::sqrt(m_sim.gammaInf * (m_sim.p[elem] / m_sim.rho[elem]));
	}
}
//----------------------------------------------------
void Solver::computeNormalOrientation(const uint32_t &Elem1ID, const uint32_t &iface) {
	std::vector<double> midFaceToElem1{m_mesh.CvolumeCentroid(Elem1ID).x - m_mesh.FaceMidPoint(iface).x,
	                                   m_mesh.CvolumeCentroid(Elem1ID).y - m_mesh.FaceMidPoint(iface).y};
	double midFaceToElemNorm = std::sqrt(midFaceToElem1[0] * midFaceToElem1[0] + midFaceToElem1[1] * midFaceToElem1[1]);
	double DotProduct = midFaceToElem1[0] * m_mesh.FaceVector(iface).x + midFaceToElem1[1] * m_mesh.FaceVector(iface).y;
	double angle = std::acos(DotProduct / (midFaceToElemNorm * 1)) * (180 / M_PI);
	if (angle < 90) {
		m_mesh.FaceVector(iface).x *= -1;
		m_mesh.FaceVector(iface).y *= -1;
	}
}
// ----------------------------------------------------

//double Solver::findMaxResidual() {
//	double maxResidual = 0;
//	double maxResidualinElement = 0;
//	for (auto &ResidualinElement : m_sim.residuals) {
//
//		maxResidualinElement = ResidualinElement.findMax();
//
//		if (maxResidualinElement > maxResidual) {
//
//			maxResidual = maxResidualinElement;
//		}
//	}
//	return maxResidual;
//}
// -------------------------------------------------
double Solver::findMaxRhoResidual() {
	double maxResidual = 0;
	for (auto &ResidualinElement : m_sim.residuals) {
		if (std::isnan(ResidualinElement.m_rhoV_residual)) {
			std::cerr << "Error : nan rhoV residual found !" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		if (ResidualinElement.m_rhoV_residual > maxResidual) {

			maxResidual = ResidualinElement.m_rhoV_residual;
		}
	}
	return maxResidual;
}
//--------------------------------------------------
double Solver::findMaxRhoUResidual() {
	double maxResidual = 0;
	for (auto &ResidualinElement : m_sim.residuals) {
		if (std::isnan(ResidualinElement.m_rho_uV_residual)) {
			std::cerr << "Error : nan rho_uV residual found !" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		if (ResidualinElement.m_rho_uV_residual > maxResidual) {

			maxResidual = ResidualinElement.m_rho_uV_residual;
		}
	}
	return maxResidual;
}

// ------------------------------------------------
double Solver::findMaxRhoVResidual() {
	double maxResidual = 0;
	for (auto &ResidualinElement : m_sim.residuals) {
		if (std::isnan(ResidualinElement.m_rho_vV_residual)) {
			std::cerr << "Error : nan rho_vV residual found !" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		if (ResidualinElement.m_rho_vV_residual > maxResidual) {

			maxResidual = ResidualinElement.m_rho_vV_residual;
		}
	}
	return maxResidual;
}
// --------------------------------------------------
double Solver::findMaxRhoHResidual() {
	double maxResidual = 0;
	for (auto &ResidualinElement : m_sim.residuals) {
		if (std::isnan(ResidualinElement.m_rho_HV_residual)) {
			std::cerr << "Error : nan rho_HV residual found !" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		if (ResidualinElement.m_rho_HV_residual > maxResidual) {

			maxResidual = ResidualinElement.m_rho_HV_residual;
		}
	}
	return maxResidual;
}