//
// Created by amin on 2020-10-25.
//
#include "BoundaryConditions.h"
#include <cmath>
using namespace ees2d::solver;
using namespace ees2d::mesh;

void BC::farfieldSupersonicInflow(const uint32_t &elemID1, Solver::faceParams &faceParams, const solver::Simulation &sim) {

}
void BC::farfieldSupersonicOutflow(const uint32_t &elemID1, Solver::faceParams &faceParams, const Simulation &sim) {
}
void BC::farfieldSubsonicInflow(const uint32_t &elemID1, const uint32_t & faceId, Solver::faceParams &faceParams, const Simulation &sim, const mesh::Mesh& mymesh) {
	// face conservative values (p,rho,u,v) for a farfield Subsonic inflow boundary condition

	double c_inside = std::sqrt(sim.gammaInf*sim.gasConstantInf*sim.tempInf) ;//speed of sound inside domain
  faceParams.p = (0.5)*(sim.pressureInf+sim.p[elemID1] - sim.rho[elemID1]*c_inside*(mymesh.FaceVector(faceId).x*(sim.uInf-sim.u[elemID1])
	                                                                                          + mymesh.FaceVector(faceId).y*(sim.vInf -sim.v[elemID1])));
	faceParams.rho = sim.rhoInf * (faceParams.p-sim.pressureInf)/(c_inside*c_inside);

	faceParams.u = sim.uInf - mymesh.FaceVector(faceId).x*(sim.pressureInf-faceParams.p)/(sim.rho[elemID1]*c_inside);
  faceParams.v = sim.vInf - mymesh.FaceVector(faceId).y*(sim.pressureInf-faceParams.p)/(sim.rho[elemID1]*c_inside);
}

void BC::farfieldSubsonicOutflow(const uint32_t &elemID1, const uint32_t & faceId, Solver::faceParams &faceParams, const Simulation &sim, const mesh::Mesh& mymesh) {
}
void BC::wall(const uint32_t &elemID1, Solver::faceParams &faceParams, const ees2d::solver::Simulation &sim) {
}
