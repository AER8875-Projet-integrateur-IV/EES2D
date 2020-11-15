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
#include <omp.h>


using namespace ees2d::solver;

Solver::Solver(ees2d::solver::Simulation &sim, ees2d::mesh::Mesh &mesh)
    : m_sim(sim), m_mesh(mesh) {
}

// ---------------------------------------
void Solver::run() {
	// Initialize residual file

	std::ofstream residualStream(m_sim.residualPath);

	residualStream << "--------------------------"
	               << " RESIDUAL ---------------------------\n";
	residualStream << "Mesh file : " << m_sim.meshPath << "\n";
	residualStream << "Mach : " << m_sim.MachInf << "\n";
	residualStream << "AOA : " << m_sim.aoa << "\n";

	residualStream
	        << "rho" << std::setw(18)
	        << "rhoU" << std::setw(15)
	        << "rhoV" << std::setw(15)
	        << "rhoH"
	        << "\n";

	// Initialize iteration number
	uint32_t iteration = 0;

	std::cout << "Solver running ..." << std::endl;

	// RK5 coefficients
	std::vector<double> RK5_coeffs = {0.0533, 0.1263, 0.2375, 0.4414, 1};

	// Initialize Residual

	ResidualRMS rms(1, 1, 1, 1);
	double courant_number = m_sim.cfl;
	uint32_t maxIterations = m_sim.maxIter;

	// Setup parallelization variables

	const int numTotalFaces = m_mesh.N_faces;
	uint32_t numThreads = m_sim.threadNum;
	uint32_t rest = m_mesh.N_faces % numThreads;
	std::vector<double> faceChunks;

	faceChunks.push_back(0);
	for (uint32_t i = 1; i < numThreads + 1; i++) {
		faceChunks.push_back(((numTotalFaces - rest) / numThreads) * i);
	}
	faceChunks.back() += rest;

	// Local Fc for faces (temporary residual vector for parallelization)
	std::shared_ptr<ConvectiveFlux[]> localFc = std::make_unique<ConvectiveFlux[]>(m_mesh.N_faces);

	while (rms.rho > m_sim.minResidual && iteration < maxIterations) {

		computeResidual(iteration, numThreads, faceChunks, localFc);

		//Update delta W of conservative Variables (rho, u ,v, E)
		if (m_sim.timeIntegration == "RK5") {
			const std::vector<ConservativeVariables> W0 = m_sim.conservativeVariables;
			for (auto &coeff : RK5_coeffs) {
				RK5(iteration, coeff, courant_number, W0, numThreads, faceChunks,localFc);
			}
		} else if (m_sim.timeIntegration == "EXPLICIT_EULER") {
			eulerExplicit(courant_number);
		}

		iteration += 1;

		findRms(rms);

		if (iteration % 50 == 0) {
			std::cout << "Iteration :" << iteration << std::endl;
			std::cout << " RMS_rho : " << rms.rho << " | RMS_rho_u : " << rms.rhoU << " | RMS_rho_v : " << rms.rhoV << " | RMS_rho_H : " << rms.rhoH << std::endl;
		}


		residualStream << rms.rho << std::setw(15)
		               << rms.rhoU << std::setw(15)
		               << rms.rhoV << std::setw(15)
		               << rms.rhoH << "\n";
	}
	residualStream.close();
}

// -------------------------------------------------------------
void Solver::computeResidual(uint32_t &iteration, uint32_t &numThreads, const std::vector<double> &faceChunks,std::shared_ptr<ConvectiveFlux[]> localFc) {

	// ID of Elements on both sides of each face
	

	for (auto &residual : m_sim.residuals) {
		residual.reset();
	}


	for (auto &spec : m_sim.spectralRadii) {
		spec = 0;
	}

#pragma omp parallel for num_threads(numThreads) default(none) shared(localFc, faceChunks, iteration, std::cerr)
	for (uint32_t task = 0; task < faceChunks.size() - 1; task++) {


		for (uint32_t iface = faceChunks[task]; iface < faceChunks[task + 1]; iface++) {

			uint32_t Elem1ID = m_mesh.FaceToElem(iface, 0);
			uint32_t Elem2ID = m_mesh.FaceToElem(iface, 1);

			faceParams faceP;
			// Get the elements IDs connected to the face

			if (Elem2ID < Elem1ID) {
				std::swap(Elem2ID, Elem1ID);
			}

			// Elem 1ID is the looped node

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

			if (std::isnan(Fc.m_rhoV)) {
				std::cerr << "Error : nan flux found at iteration " << iteration << " and elem : " << Elem1ID << std::endl;
				std::exit(EXIT_FAILURE);
			}

			// Update spectral radiation for timestep calculation
			updateSpectralRadii(Elem1ID, Elem2ID, faceP, iface);
			//std::cout << faceP.rho << "/" << faceP.u << "/" << faceP.v << "/" << faceP.p << std::endl;

			// Update residual of elements connected to face
			localFc[iface] = Fc;
		}
	}
	updateResidual(localFc);
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
			Fc = BC::farfieldSupersonicInflow(iface, faceP, m_sim, m_mesh);

		} else if (m_sim.Mach[Elem1ID] < 1 && V <= 0) {
			Fc = BC::farfieldSubsonicInflow(Elem1ID, iface, faceP, m_sim, m_mesh);
		}
	}

	return Fc;
}

//-------------------------------------------

void Solver::updateResidual(std::shared_ptr<ConvectiveFlux[]> localFc) {
	// Calculating Residual if BC
	for (uint32_t iface = 0; iface < m_mesh.N_faces; iface++) {
		uint32_t Elem1ID = m_mesh.FaceToElem(iface, 0);
		uint32_t Elem2ID = m_mesh.FaceToElem(iface, 1);

		if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {

			m_sim.residuals[Elem1ID] += (localFc[iface] * m_mesh.FaceSurface(iface));

			// Calculating Residual if internal face
		} else {
			m_sim.residuals[Elem1ID] += (localFc[iface] * m_mesh.FaceSurface(iface));
			m_sim.residuals[Elem2ID] -= (localFc[iface] * m_mesh.FaceSurface(iface));
		}
	}
}


// --------------------------------------
void Solver::updateSpectralRadii(const uint32_t &Elem1ID, const uint32_t &Elem2ID, Solver::faceParams &faceP, const uint32_t &iface) {
	//double c = sqrt(m_sim.gammaInf * (faceP.p / faceP.rho));
	//double elemSpectralRadiiX = 0.5 * (std::abs(faceP.u) + c ) * std::abs(m_mesh.FaceVector(iface).x * m_mesh.FaceSurface(iface));
	//double elemSpectralRadiiY = 0.5 * (std::abs(faceP.v) + c ) *std::abs(m_mesh.FaceVector(iface).y * m_mesh.FaceSurface(iface));
	double elemSpectralRadii = (std::abs((faceP.u * m_mesh.FaceVector(iface).x + faceP.v * m_mesh.FaceVector(iface).y)) +
	                            sqrt(m_sim.gammaInf * (faceP.p / faceP.rho))) *
	                           m_mesh.FaceSurface(iface);


	if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {
		m_sim.spectralRadii[Elem1ID] += elemSpectralRadii;

	} else {

		m_sim.spectralRadii[Elem1ID] += elemSpectralRadii;
		m_sim.spectralRadii[Elem2ID] += elemSpectralRadii;
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
void Solver::RK5(uint32_t &iteration, 
				const double &coeff, double courantNumber,
 				const std::vector<ConservativeVariables> &W0,
  				uint32_t &numThreads, 
  				const std::vector<double> &faceChunks,
				std::shared_ptr<ConvectiveFlux[]> localFc) {
	// Update time
	updateLocalTimeSteps(courantNumber);

	TimeIntegration::RK5(m_sim, m_mesh, coeff, W0);

	Solver::updateVariables();

	if (coeff != 1) {
		computeResidual(iteration, numThreads, faceChunks,localFc);
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
		if (std::isnan(m_sim.rho[elem])) {
			std::cerr << "Error : nan rho variable found at elem : " << elem << std::endl;
			std::exit(EXIT_FAILURE);
		}
		m_sim.u[elem] = m_sim.conservativeVariables[elem].m_rho_u / m_sim.rho[elem];
		m_sim.v[elem] = m_sim.conservativeVariables[elem].m_rho_v / m_sim.rho[elem];
		m_sim.E[elem] = m_sim.conservativeVariables[elem].m_rho_E / m_sim.rho[elem];
		m_sim.p[elem] = (m_sim.gammaInf - 1) * m_sim.rho[elem] * (m_sim.E[elem] - ((m_sim.u[elem] * m_sim.u[elem] + m_sim.v[elem] * m_sim.v[elem]) / 2));

		m_sim.H[elem] = m_sim.E[elem] + (m_sim.p[elem] / m_sim.rho[elem]);
		m_sim.Mach[elem] = std::sqrt(m_sim.u[elem] * m_sim.u[elem] + m_sim.v[elem] * m_sim.v[elem]) / std::sqrt(m_sim.gammaInf * (m_sim.p[elem] / m_sim.rho[elem]));
	}
}

// ---------------------------------------------------
void Solver::findRms(Solver::ResidualRMS &RMS) {
	double sumRhoResidual = 0;
	double sumRhoUResidual = 0;
	double sumRhoVResidual = 0;
	double sumRhoHResidual = 0;


	for (auto &ResidualinElement : m_sim.residuals) {
		sumRhoResidual += (ResidualinElement.m_rhoV_residual * ResidualinElement.m_rhoV_residual);
		sumRhoUResidual += (ResidualinElement.m_rho_uV_residual * ResidualinElement.m_rho_uV_residual);
		sumRhoVResidual += (ResidualinElement.m_rho_vV_residual * ResidualinElement.m_rho_vV_residual);
		sumRhoHResidual += (ResidualinElement.m_rho_HV_residual * ResidualinElement.m_rho_HV_residual);
	}

	RMS.rho = sqrt((1.0 / m_mesh.N_elems) * sumRhoResidual);
	RMS.rhoU = sqrt((1.0 / m_mesh.N_elems) * sumRhoUResidual);
	RMS.rhoV = sqrt((1.0 / m_mesh.N_elems) * sumRhoVResidual);
	RMS.rhoH = sqrt((1.0 / m_mesh.N_elems) * sumRhoHResidual);
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
//double Solver::findMaxRhoResidual() {
//  double maxResidual = 0;
//  for (auto &ResidualinElement : m_sim.residuals) {
//    if (std::isnan(ResidualinElement.m_rhoV_residual)) {
//      std::cerr << "Error : nan rhoV residual found !" << std::endl;
//      std::exit(EXIT_FAILURE);
//    }
//
//    if (ResidualinElement.m_rhoV_residual > maxResidual) {
//
//      maxResidual = ResidualinElement.m_rhoV_residual;
//    }
//  }
//  return maxResidual;
//}
////--------------------------------------------------
//double Solver::findMaxRhoUResidual() {
//  double maxResidual = 0;
//  for (auto &ResidualinElement : m_sim.residuals) {
//    if (std::isnan(ResidualinElement.m_rho_uV_residual)) {
//      std::cerr << "Error : nan rho_uV residual found !" << std::endl;
//      std::exit(EXIT_FAILURE);
//    }
//
//    if (ResidualinElement.m_rho_uV_residual > maxResidual) {
//
//      maxResidual = ResidualinElement.m_rho_uV_residual;
//    }
//  }
//  return maxResidual;
//}
//
//// ------------------------------------------------
//double Solver::findMaxRhoVResidual() {
//  double maxResidual = 0;
//  for (auto &ResidualinElement : m_sim.residuals) {
//    if (std::isnan(ResidualinElement.m_rho_vV_residual)) {
//      std::cerr << "Error : nan rho_vV residual found !" << std::endl;
//      std::exit(EXIT_FAILURE);
//    }
//
//    if (ResidualinElement.m_rho_vV_residual > maxResidual) {
//
//      maxResidual = ResidualinElement.m_rho_vV_residual;
//    }
//  }
//  return maxResidual;
//}
//// --------------------------------------------------
//double Solver::findMaxRhoHResidual() {
//  double maxResidual = 0;
//  for (auto &ResidualinElement : m_sim.residuals) {
//    if (std::isnan(ResidualinElement.m_rho_HV_residual)) {
//      std::cerr << "Error : nan rho_HV residual found !" << std::endl;
//      std::exit(EXIT_FAILURE);
//    }
//
//    if (ResidualinElement.m_rho_HV_residual > maxResidual) {
//
//      maxResidual = ResidualinElement.m_rho_HV_residual;
//    }
//  }
//  return maxResidual;
// }