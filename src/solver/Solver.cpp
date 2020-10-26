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
using namespace ees2d::solver;

Solver::Solver(ees2d::solver::Simulation &sim, ees2d::mesh::Mesh &mesh)
    : m_sim(sim), m_mesh(mesh) {}

// ---------------------------------------
void Solver::run() {
	// ID of Elements on both sides of each face
	uint32_t Elem1ID;
	uint32_t Elem2ID;

	// variables at face MidPoint


	for (uint32_t iface = 0; iface < m_mesh.N_faces; iface++) {
		// Get the elements IDs connected to the face
		Elem1ID = m_mesh.FaceToElem(iface, 0);
		Elem2ID = m_mesh.FaceToElem(iface, 1);
		ConvectiveFlux Fc(0, 0, 0, 0);

		// Check if boundary cells connected to the face
		// -----------------------
		if (Elem2ID == uint32_t(-1) || Elem2ID == uint32_t(-3)) {

			//Treatement of wall BC
			if (Elem2ID == uint32_t(-1)) {
				Fc = BC::wall(Elem1ID, iface, faceP, m_sim, m_mesh);
			}
			//Treamtement of farfield BC
			else if (Elem1ID == uint32_t(-3)) {
				//Treatement of Outflows
				if (m_sim.Mach[Elem1ID] > m_sim.MachInf) {
					if (m_sim.Mach[Elem1ID] >= 1) {
						Fc = BC::farfieldSupersonicOutflow(Elem1ID, iface, faceP, m_sim, m_mesh);
					} else if (m_sim.Mach[Elem1ID] < 1) {
						Fc = BC::farfieldSubsonicOutflow(Elem1ID, iface, faceP, m_sim, m_mesh);
					}
				}
				// Treatment of Inflows
				else if (m_sim.Mach[Elem1ID] < m_sim.MachInf) {
					if (m_sim.Mach[Elem1ID] >= 1) {
						Fc = BC::farfieldSupersonicInflow(Elem1ID, iface, faceP, m_sim, m_mesh);
					} else if (m_sim.Mach[Elem1ID] < 1) {
						Fc = BC::farfieldSubsonicInflow(Elem1ID, iface, faceP, m_sim, m_mesh);
					}
				}
			}
		}
		// ---------------------------

		else{
      ConvectiveFlux a(2,2,2,2);
			Fc = a ;
		}

		std::cout << Fc.m_rhoV <<  "/" << Fc.m_rho_uV <<  "/" << Fc.m_rho_vV <<  "/"  << Fc.m_rho_HV <<  "/" << std::endl;
	}
}
