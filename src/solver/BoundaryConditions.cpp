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


#include "BoundaryConditions.h"
#include <cmath>
using namespace ees2d::solver;
using namespace ees2d::mesh;
void BC::vortexCorrectionVariables(double &uInfStar, double &vInfStar, double &pInfStar, double &rhoInfStar, const uint32_t &faceId, const Simulation &sim, mesh::Mesh &mymesh) {

  double v_norm = std::sqrt(sim.uInf * sim.uInf + sim.vInf * sim.vInf);
	double circulation = 0.5 * v_norm * 1 * sim.CL;
	double d = std::sqrt(std::pow(mymesh.FaceMidPoint(faceId).x, 2) + std::pow(mymesh.FaceMidPoint(faceId).y , 2));
	double theta = std::atan((mymesh.FaceMidPoint(faceId).y - 0) / ((mymesh.FaceMidPoint(faceId).x )));

	uInfStar = sim.uInf +
	           ((circulation * std::sqrt(1 - sim.MachInf * sim.MachInf)) / (2 * 3.14159265358979 * d)) *
	                   (1 / (1 - std::pow(sim.MachInf, 2) * std::pow(std::sin(theta - sim.aoaRad), 2))) *
	                   std::sin(theta);

	vInfStar = sim.vInf -
	           ((circulation * std::sqrt(1 - sim.MachInf * sim.MachInf)) / (2 * 3.14159265358979 * d)) *
	                   (1 / (1 - std::pow(sim.MachInf, 2) * std::pow(std::sin(theta - sim.aoaRad), 2))) *
	                   std::cos(theta);

	double vnorm_sqrd = sim.uInf * sim.uInf + sim.vInf * sim.vInf;
	double vStar_norm_sqrd = uInfStar * uInfStar + vInfStar * vInfStar;

	pInfStar = std::pow(std::pow(sim.pressureInf,(sim.gammaInf-1)/sim.gammaInf) + ((sim.gammaInf-1)/sim.gammaInf)*((sim.rhoInf*(vnorm_sqrd-vStar_norm_sqrd))/(2*std::pow(sim.pressureInf,1/sim.gammaInf))),sim.gammaInf/(sim.gammaInf-1));
	rhoInfStar = sim.rhoInf*std::pow(pInfStar/sim.pressureInf,1/sim.gammaInf);
}
ConvectiveFlux BC::farfieldSupersonicInflow(const uint32_t &faceId,
                                            Solver::faceParams &faceParams,
                                            const Simulation &sim,
                                            mesh::Mesh &mymesh) {
	faceParams.p = sim.pressureInf;
	faceParams.u = sim.uInf;
	faceParams.v = sim.vInf;
	faceParams.rho = sim.rhoInf;
	double E = sim.pressureInf / ((sim.gammaInf - 1) * sim.rhoInf) + ((sim.uInf * sim.uInf + sim.vInf * sim.vInf) / 2);
	double H = E + sim.pressureInf / sim.rhoInf;
	double V = (faceParams.u * mymesh.FaceVector(faceId).x + faceParams.v * mymesh.FaceVector(faceId).y);


	double rhoV = faceParams.rho * V;
	double rho_uV = faceParams.rho * faceParams.u * V + mymesh.FaceVector(faceId).x * faceParams.p;
	double rho_vV = faceParams.rho * faceParams.v * V + mymesh.FaceVector(faceId).y * faceParams.p;
	double rho_HV = H * faceParams.rho * V;

	ConvectiveFlux Fc(rhoV, rho_uV, rho_vV, rho_HV);
	return Fc;
}
// --------------------------------------------------------
ConvectiveFlux BC::farfieldSupersonicOutflow(const uint32_t &elemID1,
                                             const uint32_t &faceId,
                                             Solver::faceParams &faceParams,
                                             const Simulation &sim,
                                             mesh::Mesh &mymesh) {

	faceParams.p = sim.p[elemID1];
	faceParams.u = sim.u[elemID1];
	faceParams.v = sim.v[elemID1];
	faceParams.rho = sim.rho[elemID1];

	double E = faceParams.p / ((sim.gammaInf - 1) * faceParams.rho) + ((faceParams.u * faceParams.u + faceParams.v * faceParams.v) / 2);
	double H = E + faceParams.p / faceParams.rho;

	double V = (faceParams.u * mymesh.FaceVector(faceId).x + faceParams.v * mymesh.FaceVector(faceId).y);
	double rhoV = faceParams.rho * V;
	double rho_uV = faceParams.rho * faceParams.u * V + mymesh.FaceVector(faceId).x * faceParams.p;
	double rho_vV = faceParams.rho * faceParams.v * V + mymesh.FaceVector(faceId).y * faceParams.p;
	double rho_HV = H * faceParams.rho * V;


	ConvectiveFlux Fc(rhoV, rho_uV, rho_vV, rho_HV);
	return Fc;
}
// --------------------------------------------------------
ConvectiveFlux BC::farfieldSubsonicInflow(const uint32_t &elemID1,
                                          const uint32_t &faceId,
                                          Solver::faceParams &faceParams,
                                          const Simulation &sim,
                                          mesh::Mesh &mymesh) {
	// face conservative values (p,rho,u,v) for a farfield Subsonic inflow boundary condition
  double uInfStar;
	double vInfStar;
	double rhoInfStar;
	double pInfStar;
	//vortexCorrectionVariables(uInfStar,vInfStar,pInfStar,rhoInfStar,faceId,sim,mymesh);


	double c_inside = std::sqrt(sim.gammaInf * (sim.p[elemID1] / sim.rho[elemID1]));//speed of sound inside domain
	faceParams.p = (0.5) * (sim.pressureInf + sim.p[elemID1] - sim.rho[elemID1] * c_inside * (mymesh.FaceVector(faceId).x * (sim.uInf - sim.u[elemID1]) + mymesh.FaceVector(faceId).y * (sim.vInf - sim.v[elemID1])));
	faceParams.rho = sim.rhoInf + (faceParams.p - sim.pressureInf) / (c_inside * c_inside);

	faceParams.u = sim.uInf - mymesh.FaceVector(faceId).x * (sim.pressureInf - faceParams.p) / (sim.rho[elemID1] * c_inside);
	faceParams.v = sim.vInf - mymesh.FaceVector(faceId).y * (sim.pressureInf - faceParams.p) / (sim.rho[elemID1] * c_inside);

	double E = faceParams.p / ((sim.gammaInf - 1) * faceParams.rho) + ((faceParams.u * faceParams.u + faceParams.v * faceParams.v) / 2);
	double H = E + faceParams.p / faceParams.rho;

	double V = (faceParams.u * mymesh.FaceVector(faceId).x + faceParams.v * mymesh.FaceVector(faceId).y);
	double rhoV = faceParams.rho * V;
	double rho_uV = faceParams.rho * faceParams.u * V + mymesh.FaceVector(faceId).x * faceParams.p;
	double rho_vV = faceParams.rho * faceParams.v * V + mymesh.FaceVector(faceId).y * faceParams.p;
	double rho_HV = H * faceParams.rho * V;

	ConvectiveFlux Fc(rhoV, rho_uV, rho_vV, rho_HV);
	return Fc;
}

// --------------------------------------------------------

ConvectiveFlux BC::farfieldSubsonicOutflow(const uint32_t &elemID1,
                                           const uint32_t &faceId,
                                           Solver::faceParams &faceParams,
                                           const Simulation &sim,
                                           mesh::Mesh &mymesh) {

  double uInfStar;
  double vInfStar;
  double rhoInfStar;
  double pInfStar;
  //vortexCorrectionVariables(uInfStar,vInfStar,pInfStar,rhoInfStar,faceId,sim,mymesh);


	double c_inside = std::sqrt(sim.gammaInf * (sim.p[elemID1] / sim.rho[elemID1]));

	faceParams.p = sim.pressureInf;
	faceParams.rho = sim.rho[elemID1] + (faceParams.p - sim.p[elemID1]) / (c_inside * c_inside);
	faceParams.u = sim.u[elemID1] + mymesh.FaceVector(faceId).x * (sim.p[elemID1] - faceParams.p) / (sim.rho[elemID1] * c_inside);
	faceParams.v = sim.v[elemID1] + mymesh.FaceVector(faceId).y * (sim.p[elemID1] - faceParams.p) / (sim.rho[elemID1] * c_inside);

	double E = faceParams.p / ((sim.gammaInf - 1) * faceParams.rho) + ((faceParams.u * faceParams.u + faceParams.v * faceParams.v) / 2);
	double H = E + faceParams.p / faceParams.rho;

	double V = (faceParams.u * mymesh.FaceVector(faceId).x + faceParams.v * mymesh.FaceVector(faceId).y);
	double rhoV = faceParams.rho * V;
	double rho_uV = faceParams.rho * faceParams.u * V + mymesh.FaceVector(faceId).x * faceParams.p;
	double rho_vV = faceParams.rho * faceParams.v * V + mymesh.FaceVector(faceId).y * faceParams.p;
	double rho_HV = H * faceParams.rho * V;

	ConvectiveFlux Fc(rhoV, rho_uV, rho_vV, rho_HV);
	return Fc;
}

// --------------------------------------------------------

ConvectiveFlux BC::wall(const uint32_t &elemID1,
                        const uint32_t &faceId,
                        Solver::faceParams &faceParams,
                        const ees2d::solver::Simulation &sim,
                        mesh::Mesh &mymesh) {

	faceParams.p = sim.p[elemID1];
	faceParams.u = sim.u[elemID1] - (sim.u[elemID1] * mymesh.FaceVector(faceId).x + sim.v[elemID1] * mymesh.FaceVector(faceId).y) * mymesh.FaceVector(faceId).x;
	faceParams.v = sim.v[elemID1] - (sim.u[elemID1] * mymesh.FaceVector(faceId).x + sim.v[elemID1] * mymesh.FaceVector(faceId).y) * mymesh.FaceVector(faceId).y;
	faceParams.rho = sim.rho[elemID1];

	double rhoV = 0;
	double rho_uV = mymesh.FaceVector(faceId).x * faceParams.p;
	double rho_vV = mymesh.FaceVector(faceId).y * faceParams.p;
	double rho_HV = 0;

	ConvectiveFlux Fc(rhoV, rho_uV, rho_vV, rho_HV);
	return Fc;
}
