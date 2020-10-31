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


#include "mesh/Metrics.h"

#include <cmath>
#include <iomanip>

using namespace ees2d::mesh;
using ees2d::mesh::Connectivity;
using ees2d::utils::Vector2;


//-----------------------------------------------------

void MetricsData::compute(const Connectivity &ConnectivityObject) {
  std::cout << " -------------- Computing Geometrical Quantities!"
               " --------------"
            << std::endl;

	MetricsData::computeCvolumesMetrics(ConnectivityObject);

	MetricsData::computeFaceMetrics(ConnectivityObject);

}


//----------------------------------------------------

void MetricsData::computeCvolumesMetrics(const Connectivity &ConnectivityObject) {

	CvolumesCentroid.reserve(ConnectivityObject.get_parser().get_Nelems());
	CvolumesArea.reserve(ConnectivityObject.get_parser().get_Nelems());

	double Area = 0.0f;
	Vector2<double> Centroid_Vec2;

	// FOr quand elements
	double Area123;
	double Area134;
	Vector2<double> Centroid123_Vec2;
	Vector2<double> Centroid134_Vec2;
	std::vector<Vector2<double>> elem_nodes_temp = {};

	for (uint32_t ielem = 0; ielem < ConnectivityObject.get_parser().get_Nelems(); ielem++) {

		elem_nodes_temp.clear();

		for (uint32_t ilocalNode = 0; ilocalNode < ConnectivityObject.get_parser().get_NPSUE()[ielem]; ilocalNode++) {


			auto &[x, y] = ConnectivityObject.get_parser().get_coords()[ConnectivityObject.connecNodeSurrElement(ilocalNode, ielem)];
			elem_nodes_temp.emplace_back(x, y);
		}

		// If triangle
		if (elem_nodes_temp.size() == 3) {

			Centroid_Vec2 = (elem_nodes_temp[0] + elem_nodes_temp[1] + elem_nodes_temp[2]) * (1.0f / 3.0f);
			CvolumesCentroid.push_back(Centroid_Vec2);

			Area = 0.5 * std::abs((elem_nodes_temp[0].x - elem_nodes_temp[1].x) * (elem_nodes_temp[0].y + elem_nodes_temp[1].y) +
			                       (elem_nodes_temp[1].x - elem_nodes_temp[2].x) * (elem_nodes_temp[1].y + elem_nodes_temp[2].y) +
			                       (elem_nodes_temp[2].x - elem_nodes_temp[0].x) * (elem_nodes_temp[2].y + elem_nodes_temp[0].y));

			CvolumesArea.push_back(Area);

		} else if (elem_nodes_temp.size() == 4) {

			Area = 0.5 * std::abs((elem_nodes_temp[0].x - elem_nodes_temp[2].x) * (elem_nodes_temp[1].y - elem_nodes_temp[3].y)
			                       + (elem_nodes_temp[3].x - elem_nodes_temp[1].x) * (elem_nodes_temp[0].y - elem_nodes_temp[2].y));
			

			CvolumesArea.push_back(Area);


			Centroid123_Vec2 = (elem_nodes_temp[0] + elem_nodes_temp[1] + elem_nodes_temp[2]) * (1.0 / 3.0);
			Centroid134_Vec2 = (elem_nodes_temp[0] + elem_nodes_temp[2] + elem_nodes_temp[3]) * (1.0 / 3.0);

			//Area123 = 0.5 * std::abs((elem_nodes_temp[0].x - elem_nodes_temp[2].x) * (elem_nodes_temp[0].y + elem_nodes_temp[2].y)
//			                          + (elem_nodes_temp[2].x - elem_nodes_temp[1].x) * (elem_nodes_temp[2].y + elem_nodes_temp[1].y)
//			                          + (elem_nodes_temp[1].x - elem_nodes_temp[0].x) * (elem_nodes_temp[1].y + elem_nodes_temp[0].y));

			//Area134 = 0.5 * std::abs((elem_nodes_temp[0].x - elem_nodes_temp[3].x) * (elem_nodes_temp[0].y + elem_nodes_temp[3].y)
//			                          + (elem_nodes_temp[3].x - elem_nodes_temp[2].x) * (elem_nodes_temp[3].y + elem_nodes_temp[2].y)
//			                          + (elem_nodes_temp[2].x - elem_nodes_temp[0].x) * (elem_nodes_temp[2].y + elem_nodes_temp[0].y));

			Centroid_Vec2 = ((Centroid123_Vec2 ) + (Centroid134_Vec2 ) )/2;


			CvolumesCentroid.push_back(Centroid_Vec2);
		}
	}
  std::cout << std::setw(40) << "Control Volumes Area : " << std::setw(6) << "Done\n";
  std::cout << std::setw(40) << "Control Volumes Centroid : " << std::setw(6) << "Done\n";


}

//----------------------------------------------
void MetricsData::computeFaceMetrics(const Connectivity &ConnectivityObject) {

	const uint32_t nfaces = ConnectivityObject.get_FaceToElem()->size();

	facesSurface.reserve(nfaces);
	facesMidPoint.reserve(nfaces);
	facesVector.reserve(nfaces);

	uint32_t Node1ID;
  uint32_t Node2ID;
	double length=0;


	for (uint32_t iface=0;iface<nfaces;iface++){

		Node1ID = (*ConnectivityObject.get_FaceToNode())[iface][0];
    Node2ID = (*ConnectivityObject.get_FaceToNode())[iface][1];

		auto [x1,y1] = ConnectivityObject.get_parser().get_coords()[Node1ID];
    auto [x2,y2] = ConnectivityObject.get_parser().get_coords()[Node2ID];
    length = std::sqrt(std::pow(x2-x1,2.0)+std::pow(y2-y1,2.0));
		facesSurface.push_back(length);
		facesMidPoint.emplace_back(Vector2<double>((x2+x1)/2,(y2+y1)/2));
		facesVector.emplace_back(Vector2<double>((y2-y1)/length,(x1-x2)/length));

	}

  std::cout << std::setw(40) << "Faces surface : " << std::setw(6) << "Done\n";
  std::cout << std::setw(40) << "Faces mid-points : " << std::setw(6) << "Done\n";
  std::cout << std::setw(40) << "Faces orientation : " << std::setw(6) << "Done\n";

}
