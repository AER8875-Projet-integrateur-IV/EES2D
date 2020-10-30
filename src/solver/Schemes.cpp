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


#include "Schemes.h"
using namespace ees2d::solver;
ConvectiveFlux scheme::RoeScheme(const uint32_t &elemID1,
                                 const uint32_t &elemID2,
                                 const uint32_t &faceid,
                                 Solver::faceParams &faceParams,
                                 const solver::Simulation &sim,
                                 ees2d::mesh::Mesh &mymesh) {

	// Computing Roe averages
	double rhoHat = std::sqrt(sim.rho[elemID1] * sim.rho[elemID2]);
	double uHat = (sim.u[elemID1] * sqrt(sim.rho[elemID1]) + sim.u[elemID2] * sqrt(sim.rho[elemID2])) / (sqrt(sim.rho[elemID1]) + sqrt(sim.rho[elemID2]));
	double vHat = (sim.v[elemID1] * sqrt(sim.rho[elemID1]) + sim.v[elemID2] * sqrt(sim.rho[elemID2])) / (sqrt(sim.rho[elemID1]) + sqrt(sim.rho[elemID2]));
	double HHat = (sim.H[elemID1] * sqrt(sim.rho[elemID1]) + sim.H[elemID2] * sqrt(sim.rho[elemID2])) / (sqrt(sim.rho[elemID1]) + sqrt(sim.rho[elemID2]));
	double qHatSquared = uHat * uHat + vHat * vHat;
	double cHat = sqrt((sim.gammaInf - 1) * (HHat - (qHatSquared) / 2));
	double VHat = uHat * mymesh.FaceVector(faceid).x + uHat * mymesh.FaceVector(faceid).y;
//
//	faceParams.u = uHat;
//	faceParams.v = vHat;
//	faceParams.rho = rhoHat;
	faceParams.u = (sim.u[elemID2] + sim.u[elemID1]) / 2;
	faceParams.v = (sim.v[elemID2] + sim.v[elemID1]) / 2;
	faceParams.rho = (sim.rho[elemID2] + sim.rho[elemID1]) / 2;
	faceParams.p = (sim.p[elemID2] + sim.p[elemID1]) / 2;


	// Computing deltas (jump condition)
	double pDelta = sim.p[elemID2] - sim.p[elemID1];
	double VDelta = (sim.u[elemID2] * mymesh.FaceVector(faceid).x + sim.v[elemID2] * mymesh.FaceVector(faceid).y) -
	                (sim.u[elemID1] * mymesh.FaceVector(faceid).x + sim.v[elemID1] * mymesh.FaceVector(faceid).y);
	double rhoDelta = sim.rho[elemID2] - sim.rho[elemID1];
	double uDelta = sim.u[elemID2] - sim.u[elemID1];
	double vDelta = sim.v[elemID2] - sim.v[elemID1];

	// Hartens entropy correction
	double F1HartensCorrection;
	double F5HartensCorrection;
	double hartensCriterion = (1 /10) * std::sqrt(sim.gammaInf * (sim.p[elemID1] / sim.rho[elemID1]));
	if (std::abs(VHat - cHat) > hartensCriterion) {
		F1HartensCorrection = std::abs(VHat - cHat);
	} else {
		F1HartensCorrection = ((VHat - cHat) * (VHat - cHat) + hartensCriterion * hartensCriterion) / (2 * hartensCriterion);
	}

	if (std::abs(VHat + cHat) > hartensCriterion) {
		F5HartensCorrection = std::abs(VHat + cHat);
	} else {
		F5HartensCorrection = ((VHat + cHat) * (VHat + cHat) + hartensCriterion * hartensCriterion) / (2 * hartensCriterion);
	}

	// Computing Fluxes
	// Delta F1 flux
	double commonDeltaF1Term = F1HartensCorrection * (pDelta - rhoHat * cHat * VDelta) / (2 * cHat * cHat);

	ConvectiveFlux deltaF1(commonDeltaF1Term,
	                       commonDeltaF1Term * (uHat - cHat * mymesh.FaceVector(faceid).x),
	                       commonDeltaF1Term * (vHat - cHat * mymesh.FaceVector(faceid).y),
	                       commonDeltaF1Term * (HHat - cHat * VHat));

	// Delta F234 flux
	double commonDeltaF234Term = std::abs(VHat) * (rhoDelta - (pDelta / (cHat * cHat)));

	ConvectiveFlux deltaF234(commonDeltaF234Term,
	                         commonDeltaF234Term * uHat + std::abs(VHat) * rhoHat * (uDelta - VDelta * mymesh.FaceVector(faceid).x),
	                         commonDeltaF234Term * vHat + std::abs(VHat) * rhoHat * (vDelta - VDelta * mymesh.FaceVector(faceid).y),
	                         commonDeltaF234Term * (qHatSquared / 2) + std::abs(VHat) * rhoHat * (uHat * uDelta + vHat * vDelta - VHat * VDelta));


	// Delta F5 flux
	double commonDeltaF5Term = F5HartensCorrection * (pDelta + rhoHat * cHat * VDelta) / (2 * cHat * cHat);

	ConvectiveFlux deltaF5(commonDeltaF5Term,
	                       commonDeltaF5Term * (uHat + cHat * mymesh.FaceVector(faceid).x),
	                       commonDeltaF5Term * (vHat + cHat * mymesh.FaceVector(faceid).y),
	                       commonDeltaF5Term * (HHat + cHat * VHat));

	// Right side Flux (Element 2)
	double V_FcR = sim.u[elemID2] * mymesh.FaceVector(faceid).x + sim.v[elemID2] * mymesh.FaceVector(faceid).y;

	ConvectiveFlux FcR(sim.rho[elemID2] * V_FcR,
	                   sim.rho[elemID2] * sim.u[elemID2] * V_FcR + mymesh.FaceVector(faceid).x * sim.p[elemID2],
	                   sim.rho[elemID2] * sim.v[elemID2] * V_FcR + mymesh.FaceVector(faceid).y * sim.p[elemID2],
	                   sim.rho[elemID2] * sim.H[elemID2] * V_FcR);


	// Left side flux (Element 1)
	double V_FcL = sim.u[elemID1] * mymesh.FaceVector(faceid).x + sim.v[elemID1] * mymesh.FaceVector(faceid).y;

	ConvectiveFlux FcL(sim.rho[elemID1] * V_FcL,
	                   sim.rho[elemID1] * sim.u[elemID1] * V_FcL + mymesh.FaceVector(faceid).x * sim.p[elemID1],
	                   sim.rho[elemID1] * sim.v[elemID1] * V_FcL + mymesh.FaceVector(faceid).y * sim.p[elemID1],
	                   sim.rho[elemID1] * sim.H[elemID1] * V_FcL);

	ConvectiveFlux Fc;
	Fc = (FcL + FcR - deltaF1 - deltaF234 - deltaF5) * 0.5;

	return Fc;
}


ConvectiveFlux scheme::AveragingScheme(const uint32_t &elemID1,
                                       const uint32_t &elemID2,
                                       const uint32_t &faceid,
                                       Solver::faceParams &faceParams,
                                       const solver::Simulation &sim,
                                       ees2d::mesh::Mesh &mymesh) {

	faceParams.u = (sim.u[elemID2] + sim.u[elemID1]) / 2;
	faceParams.v = (sim.v[elemID2] + sim.v[elemID1]) / 2;
	faceParams.rho = (sim.rho[elemID2] + sim.rho[elemID1]) / 2;
	faceParams.p = (sim.p[elemID2] + sim.p[elemID1]) / 2;
	double H = (sim.H[elemID2] + sim.H[elemID1]) / 2;
  double V = (faceParams.u*mymesh.FaceVector(faceid).x + faceParams.v*mymesh.FaceVector(faceid).y);
	ConvectiveFlux Fc(abs(faceParams.rho*V),
                    abs(faceParams.rho*faceParams.u*V+mymesh.FaceVector(faceid).x*faceParams.p),
                    abs(faceParams.rho*faceParams.v*V+mymesh.FaceVector(faceid).y*faceParams.p),
                    abs(faceParams.rho*H*V));
	return Fc;
}