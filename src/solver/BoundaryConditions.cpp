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

ConvectiveFlux BC::farfieldSupersonicInflow(const uint32_t &elemID1,
                                            const uint32_t & faceId,
                                            Solver::faceParams &faceParams,
                                            const Simulation &sim,
                                             mesh::Mesh& mymesh) {
  faceParams.p = sim.pressureInf;
  faceParams.u = sim.uInf;
  faceParams.v = sim.vInf;
  faceParams.rho = sim.rhoInf;

  double V = (faceParams.u*mymesh.FaceVector(faceId).x + faceParams.v*mymesh.FaceVector(faceId).y);


	double rhoV = faceParams.rho * V ;
	double rho_uV = faceParams.rho * faceParams.u * V + mymesh.FaceVector(faceId).x*faceParams.p;
  double rho_vV = faceParams.rho * faceParams.v * V + mymesh.FaceVector(faceId).y*faceParams.p;
	double rho_HV = sim.H[elemID1]*faceParams.rho*V;

  ConvectiveFlux Fc(rhoV,rho_uV,rho_vV,rho_HV);
	return Fc;

}
// --------------------------------------------------------
ConvectiveFlux BC::farfieldSupersonicOutflow(const uint32_t &elemID1,
                                             const uint32_t & faceId,
                                             Solver::faceParams &faceParams,
                                             const Simulation &sim,
                                              mesh::Mesh& mymesh) {

  faceParams.p = sim.p[elemID1];
  faceParams.u = sim.u[elemID1];
  faceParams.v = sim.v[elemID1];
  faceParams.rho = sim.rho[elemID1];

  double V = (faceParams.u*mymesh.FaceVector(faceId).x + faceParams.v*mymesh.FaceVector(faceId).y);
  double rhoV = faceParams.rho * V ;
  double rho_uV = faceParams.rho * faceParams.u * V + mymesh.FaceVector(faceId).x*faceParams.p;
  double rho_vV = faceParams.rho * faceParams.v * V + mymesh.FaceVector(faceId).y*faceParams.p;
  double rho_HV = sim.H[elemID1]*faceParams.rho*V;


  ConvectiveFlux Fc(rhoV,rho_uV,rho_vV,rho_HV);
  return Fc;

}
// --------------------------------------------------------
ConvectiveFlux BC::farfieldSubsonicInflow(const uint32_t &elemID1,
                                const uint32_t & faceId,
                                Solver::faceParams &faceParams,
                                const Simulation &sim,
                                 mesh::Mesh& mymesh)
{
	// face conservative values (p,rho,u,v) for a farfield Subsonic inflow boundary condition

	double c_inside = std::sqrt(sim.gammaInf*(sim.p[elemID1]/sim.rho[elemID1])) ;//speed of sound inside domain
  faceParams.p = (0.5)*(sim.pressureInf+sim.p[elemID1] - sim.rho[elemID1]*c_inside*(mymesh.FaceVector(faceId).x*(sim.uInf-sim.u[elemID1])
	                                                                                          + mymesh.FaceVector(faceId).y*(sim.vInf -sim.v[elemID1])));
	faceParams.rho = sim.rhoInf + (faceParams.p-sim.pressureInf)/(c_inside*c_inside);

	faceParams.u = sim.uInf - mymesh.FaceVector(faceId).x*(sim.pressureInf-faceParams.p)/(sim.rho[elemID1]*c_inside);
  faceParams.v = sim.vInf - mymesh.FaceVector(faceId).y*(sim.pressureInf-faceParams.p)/(sim.rho[elemID1]*c_inside);

  double V = (faceParams.u*mymesh.FaceVector(faceId).x + faceParams.v*mymesh.FaceVector(faceId).y);
  double rhoV = faceParams.rho * V ;
  double rho_uV = faceParams.rho * faceParams.u * V + mymesh.FaceVector(faceId).x*faceParams.p;
  double rho_vV = faceParams.rho * faceParams.v * V + mymesh.FaceVector(faceId).y*faceParams.p;
  double rho_HV = sim.H[elemID1]*faceParams.rho*V;

  ConvectiveFlux Fc(rhoV,rho_uV,rho_vV,rho_HV);
  return Fc;

}

// --------------------------------------------------------

ConvectiveFlux BC::farfieldSubsonicOutflow(const uint32_t &elemID1,
                                 const uint32_t & faceId,
                                 Solver::faceParams &faceParams,
                                 const Simulation &sim,
                                  mesh::Mesh& mymesh) {

  double c_inside = std::sqrt(sim.gammaInf*(sim.p[elemID1]/sim.rho[elemID1])) ; ;

	faceParams.p = sim.pressureInf;
	faceParams.rho = sim.rho[elemID1] + (faceParams.p - sim.p[elemID1])/(c_inside*c_inside);
	faceParams.u = sim.u[elemID1] + mymesh.FaceVector(faceId).x*(sim.p[elemID1] - faceParams.p)/(sim.rho[elemID1]*c_inside);
	faceParams.v = sim.v[elemID1] + mymesh.FaceVector(faceId).y*(sim.p[elemID1] - faceParams.p)/(sim.rho[elemID1]*c_inside);

  double V = (faceParams.u*mymesh.FaceVector(faceId).x + faceParams.v*mymesh.FaceVector(faceId).y);
  double rhoV = faceParams.rho * V ;
  double rho_uV = faceParams.rho * faceParams.u * V + mymesh.FaceVector(faceId).x*faceParams.p;
  double rho_vV = faceParams.rho * faceParams.v * V + mymesh.FaceVector(faceId).y*faceParams.p;
  double rho_HV = sim.H[elemID1]*faceParams.rho*V;

  ConvectiveFlux Fc(rhoV,rho_uV,rho_vV,rho_HV);
  return Fc;


}

// --------------------------------------------------------

ConvectiveFlux BC::wall(const uint32_t &elemID1,
              const uint32_t & faceId,
              Solver::faceParams &faceParams,
              const ees2d::solver::Simulation &sim,
               mesh::Mesh& mymesh) {

	faceParams.p = sim.p[elemID1];
	faceParams.u = sim.u[elemID1] - (sim.u[elemID1]*mymesh.FaceVector(faceId).x + sim.v[elemID1]*mymesh.FaceVector(faceId).y)*mymesh.FaceVector(faceId).x;
  faceParams.v = sim.v[elemID1] - (sim.u[elemID1]*mymesh.FaceVector(faceId).x + sim.v[elemID1]*mymesh.FaceVector(faceId).y)*mymesh.FaceVector(faceId).y;
	faceParams.rho = sim.rho[elemID1];

  double rhoV = 0;
  double rho_uV = mymesh.FaceVector(faceId).x*faceParams.p;
  double rho_vV = mymesh.FaceVector(faceId).y*faceParams.p;
  double rho_HV = 0;

  ConvectiveFlux Fc(rhoV,rho_uV,rho_vV,rho_HV);
  return Fc;
}
