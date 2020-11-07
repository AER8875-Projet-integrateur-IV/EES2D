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

#include "postProcess.h"
#include <iomanip>

using namespace ees2d::post;
using namespace ees2d::solver;
using namespace ees2d::mesh;

PostProcess::PostProcess(Mesh &mesh, solver::Simulation &sim)
    : m_mesh(mesh), m_sim(sim){};

void PostProcess::writeCP() {
	// Initialize file
	std::ofstream fileStream("../../../Documents/pressure.dat");
	fileStream << "--------------------------"
	           << " PRESSURE ---------------------------\n";

	// Writing CP
	for (uint32_t iface = 0; iface < m_mesh.N_faces; iface++) {
		uint32_t Elem1ID = m_mesh.FaceToElem(iface, 0);
		uint32_t Elem2ID = m_mesh.FaceToElem(iface, 1);
		if (Elem2ID == uint32_t(-1)) {

			double pinf = 1.0;
			double p = m_sim.p[Elem1ID];
			double u_sqrd =  m_sim.uInf*m_sim.uInf + m_sim.vInf*m_sim.vInf;
			double Mach = m_sim.Mach[Elem1ID];

			double Cp = (p-pinf)/(0.5*1*u_sqrd);

			// COmpressibility correction (between M 0 and 0.7)
			if(Mach < 0.7){
			Cp = Cp/sqrt(1-Mach*Mach);
      }

			fileStream << m_mesh.FaceMidPoint(iface).x
			           << std::setw(18)
			           << Cp
			           << "\n";
		}
	}
	fileStream.close();
}
