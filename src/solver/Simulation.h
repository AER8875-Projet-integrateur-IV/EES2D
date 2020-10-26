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

#include "io/InputParser.h"
#include "mesh/Mesh.h"
#include "solver/ConvectiveFlux.h"
#include "solver/ConservativeVariables.h"
#include "solver/Residual.h"
#pragma once


namespace ees2d::solver {


	struct Simulation {
		Simulation(ees2d::mesh::Mesh &, ees2d::io::InputParser &);



		// Vectors
		std::vector<double> u;
		std::vector<double> v;
		std::vector<double> rho;
		std::vector<double> p;
		std::vector<double> H;
		std::vector<double> E;
		std::vector<double> dt;
		std::vector<Residual> residuals;
		std::vector<double> Mach;
		std::vector<double> temp;
		std::vector<ConvectiveFlux> convectiveFluxes;
		std::vector<ConservativeVariables> conservativeVariables;

		double uInf;
		double vInf;
		double pressureInf;
		double rhoInf;
		double MachInf;
		double soundSpeedInf;
		double gasConstantInf;
		double gammaInf;
		double tempInf;
	};

}// namespace ees2d::solver