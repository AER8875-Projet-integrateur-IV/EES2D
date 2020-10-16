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

#pragma once


#include "mesh/Connectivity.h"
#include "utils/Vector2.h"


namespace ees2d::mesh::Metrics
{

	struct MetricsData{

    void computeMetrics(const ees2d::mesh::Connectivity&);           // Calls below methods
    void computeFaceMetrics(const ees2d::mesh::Connectivity&);
    void computeCvolumesMetrics(const ees2d::mesh::Connectivity&);

		std::vector<ees2d::utils::Vector2<float>> facesMidPoint;                  // Holds mid points of faces. Usage ->
		std::vector<ees2d::utils::Vector2<float>> facesNormalVector;              // Hold normal vectors of faces, facesNormalVector[FaceID]= = Vector2{x,y}

		std::vector<float> CvolumesArea;                                        // Holds Areas of control volumes (2D) , CvolumesArea[ElementID] = Area (float)

		std::vector<ees2d::utils::Vector2<float>> CvolumesCentroid;                     // Holds Centroids of control volumes (2D) Usage ->

	};



} // Namespace ees2d::mesh::Metrics