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

#pragma once

#include "solver/ConservativeVariables.h"
#include "solver/ConvectiveFlux.h"
#include "solver/Simulation.h"
#include "solver/TimeIntegration.h"

namespace ees2d::solver {

	class Solver {
public:
		Solver(ees2d::solver::Simulation &, ees2d::mesh::Mesh &);

		struct faceParams {
			double rho = 0;
			double p = 0;
			double u = 0;
			double v = 0;
		};

		struct ResidualRMS {
			ResidualRMS(double init_rho,double init_rhoU,double init_rhoV,double init_rhoH)
			    :rho(init_rho),rhoU(init_rhoU),rhoV(init_rhoV),rhoH(init_rhoH) {};
			double rho;
			double rhoU;
			double rhoV;
			double rhoH;
		};

		void run();
		void computeResidual(uint32_t& iteration, uint32_t& numThreads, const std::vector<double>& faceChunks);
		ConvectiveFlux computeBCFlux(const uint32_t &, const uint32_t &, Solver::faceParams &, const uint32_t &);
		void updateResidual(std::shared_ptr<ConvectiveFlux[]> localFc);
		void updateSpectralRadii(const uint32_t &Elem1ID, const uint32_t &Elem2ID, Solver::faceParams &faceP, const uint32_t &iface);
		void updateLocalTimeSteps(double &courantNumber);
		void RK5(uint32_t& iteration,const double &coeff, double courantNumber, const std::vector<ConservativeVariables> &W0,uint32_t &numThreads, const std::vector<double> &faceChunks);
		void eulerExplicit(double courantNumber);
		void updateVariables();


		// ------------------------
		double findMaxRhoResidual();
		double findMaxRhoUResidual();
		double findMaxRhoVResidual();
		double findMaxRhoHResidual();

		//
		//---------------------------------
		// compute residual with Root mean squared
		void findRms(ResidualRMS &Rms);


private:
		ees2d::solver::Simulation &m_sim;
		ees2d::mesh::Mesh &m_mesh;


	};

}// namespace ees2d::solver