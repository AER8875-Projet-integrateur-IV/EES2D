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

void PostProcess::solveCoefficients() {
	/*
	 * Compute and write Cp files specified pressure file path
	 */

  // Initialize variables for summation
	double cx =0;
	double cy =0;
	double sumSurface = 0;
  double u_sqrd = m_sim.uInf * m_sim.uInf + m_sim.vInf * m_sim.vInf;
	double minimumYcoord = 0;
	double maximumYcoord = 0;

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

			double Mach = m_sim.Mach[Elem1ID];

			double Cp = (p - pinf) / (0.5 * 1 * u_sqrd);


			// COmpressibility correction (between M 0 and 0.7)
			if (Mach < 0.7) {
				Cp = Cp / sqrt(1 - Mach * Mach);
			}

			Cps.push_back(Cp);

			// Computations for lift and drag forces
			outwardNormal(Elem1ID, iface);

			if(m_mesh.FaceMidPoint(iface).y > maximumYcoord){
				maximumYcoord = m_mesh.FaceMidPoint(iface).y;
			} else if(m_mesh.FaceMidPoint(iface).y < minimumYcoord){
        minimumYcoord = m_mesh.FaceMidPoint(iface).y;
			}

      cx += p*m_mesh.FaceSurface(iface)*m_mesh.FaceVector(iface).y;
			cy += p*m_mesh.FaceSurface(iface)*m_mesh.FaceVector(iface).x;
			//sumSurface += m_mesh.FaceSurface(iface);

			fileStream << m_mesh.FaceMidPoint(iface).x
			           << std::setw(18)
			           << Cp
			           << "\n";
		}
	}
	sumSurface = maximumYcoord - minimumYcoord;
	double Cl = cx / (maximumYcoord - minimumYcoord);
	double Cd = cy / (maximumYcoord - minimumYcoord);

	std::cout << "Cl " << Cl << " | Cd : " << Cd << std::endl;
	fileStream.close();
}



// -------------------------------------------
void PostProcess::outwardNormal(const uint32_t &Elem1ID, const uint32_t &iface) {
	/*
	 * Orient the normal outward of the airfoil surface
	 */
	std::vector<double> midFaceToElem1{m_mesh.CvolumeCentroid(Elem1ID).x - m_mesh.FaceMidPoint(iface).x,
	                                   m_mesh.CvolumeCentroid(Elem1ID).y - m_mesh.FaceMidPoint(iface).y};
	double midFaceToElemNorm = std::sqrt(midFaceToElem1[0] * midFaceToElem1[0] + midFaceToElem1[1] * midFaceToElem1[1]);
	double DotProduct = midFaceToElem1[0] * m_mesh.FaceVector(iface).x + midFaceToElem1[1] * m_mesh.FaceVector(iface).y;
	double angle = std::acos(DotProduct / (midFaceToElemNorm * 1)) * (180 / M_PI);
	if (angle <  80) {
		m_mesh.FaceVector(iface).x *= -1;
		m_mesh.FaceVector(iface).y *= -1;
	}
}
