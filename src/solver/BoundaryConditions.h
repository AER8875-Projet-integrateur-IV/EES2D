//
// Created by amin on 2020-10-25.
//
#include "solver/Simulation.h";
#include "solver/Solver.h"
#include "mesh/Mesh.h"

namespace ees2d::solver::BC {

	void farfieldSupersonicInflow(const uint32_t& elemID1, solver::Solver::faceParams& faceParams , const solver::Simulation& sim );
	void farfieldSupersonicOutflow(const uint32_t& elemID1, solver::Solver::faceParams& faceParams , const solver::Simulation& sim );
	void farfieldSubsonicInflow(const uint32_t& elemID1, const uint32_t& faceid, solver::Solver::faceParams& faceParams , const solver::Simulation& sim , const ees2d::mesh::Mesh&);
	void farfieldSubsonicOutflow(const uint32_t& elemID1, const uint32_t& faceid, solver::Solver::faceParams& faceParams , const solver::Simulation& sim ,const ees2d::mesh::Mesh&);
	void wall(const uint32_t& elemID1, solver::Solver::faceParams& faceParams ,const  solver::Simulation& sim );


}