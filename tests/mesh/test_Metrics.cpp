/* ---------------------------------------------------------------------
 *
 * Copyright (C) 2020 - by the EES2D authors
 *
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

#include "io/Su2Parser.h"
#include "mesh/Connectivity.h"
#include "mesh/Metrics.h"
#include "utils/Vector2.h"
#include <gtest/gtest.h>// Toujours inclu
#include <memory>


using ees2d::io::Su2Parser;
using ees2d::mesh::Connectivity;
using ees2d::mesh::MetricsData;
using ees2d::utils::Vector2;


TEST(Test_Metrics, computeCvolumesMetrics) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	MetricsData metrics;
	metrics.computeCvolumesMetrics(connectivity);


	// Act
	std::vector<double> exactcvAreas{0.125, 0.125, 0.125, 0.125,
	                                0.125, 0.125, 0.125, 0.125};

	std::vector<Vector2<double>> exactcvCentroids{Vector2<double>(0.5 / 3, 0.5 / 3),
	                                             Vector2<double>(1.0 / 3, 1.0 / 3),
	                                             Vector2<double>(2.0 / 3, 0.5 / 3),
	                                             Vector2<double>(2.5 / 3, 1.0 / 3),
	                                             Vector2<double>(0.5 / 3, 2.0 / 3),
	                                             Vector2<double>(1.0 / 3, 2.5 / 3),
	                                             Vector2<double>(2.0 / 3, 2.0 / 3),
	                                             Vector2<double>(2.5 / 3, 2.5 / 3)};

	std::vector<double> *cvAreas = &metrics.CvolumesArea;
	std::vector<Vector2<double>> *cvCentroids = &metrics.CvolumesCentroid;


	//Assert
	ASSERT_EQ(cvAreas->size(), exactcvAreas.size()) << "arrays cvAreas are of unequal length";

	for (size_t i = 0; i < exactcvAreas.size(); ++i) {
		ASSERT_NEAR(exactcvAreas[i], (*cvAreas)[i],0.000001) << "arrays cvAreas differ at index " << i;
	}

	ASSERT_EQ(cvCentroids->size(), exactcvCentroids.size()) << "arrays cvCentroids are of unequal length";

	for (size_t i = 0; i < exactcvCentroids.size(); ++i) {
		ASSERT_NEAR(exactcvCentroids[i].x, (*cvCentroids)[i].x,0.000001) << "arrays cvCentroids differ at index " << i;
		ASSERT_NEAR(exactcvCentroids[i].y, (*cvCentroids)[i].y,0.000001) << "arrays cvCentroids differ at index " << i;
	}
}

TEST(Test_Metrics, computeFaceMetrics) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	MetricsData metrics;
	metrics.computeFaceMetrics(connectivity);


	// Act
	std::vector<double> exactfaceSurfaces{0.5, 0.5, 0.707107, 0.5,
	                                     0.5, 0.707107,0.5 , 0.5,
	                                     0.5, 0.5, 0.707107, 0.5,
	                                     0.707107f, 0.5, 0.5, 0.5};

	std::vector<Vector2<double>> exactfaceMidpoints{Vector2<double>(0.25, 0.0),
	                                               Vector2<double>(0, 0.25),
	                                               Vector2<double>(0.25, 0.25),
	                                               Vector2<double>(0.5, 0.25),
	                                               Vector2<double>(0.75, 0),
	                                               Vector2<double>(0.75, 0.25),
	                                               Vector2<double>(1.0, 0.25),
	                                               Vector2<double>(0.25, 0.5),
	                                               Vector2<double>(0.0, 0.75),
	                                               Vector2<double>(0.75, 0.5),
	                                               Vector2<double>(0.25, 0.75),
	                                               Vector2<double>(0.5, 0.75),
	                                               Vector2<double>(0.75, 0.75),
	                                               Vector2<double>(1.0, 0.75),
	                                               Vector2<double>(0.25, 1.0),
	                                               Vector2<double>(0.75, 1.0)};

	std::vector<double> *faceSurfaces = &metrics.facesSurface;
	std::vector<Vector2<double>> *faceMidpoints = &metrics.facesMidPoint;


	//Assert
	ASSERT_EQ(faceSurfaces->size(), exactfaceSurfaces.size()) << "arrays faceSurface are of unequal length";

	for (size_t i = 0; i < exactfaceSurfaces.size(); ++i) {
    ASSERT_NEAR(exactfaceSurfaces[i], (*faceSurfaces)[i],0.000001) << "arrays faceSurface differ at index " << i;
	}

	ASSERT_EQ(faceMidpoints->size(), exactfaceMidpoints.size()) << "arrays faceMidpoints are of unequal length";

	for (size_t i = 0; i < exactfaceMidpoints.size(); ++i) {
		ASSERT_NEAR(exactfaceMidpoints[i].x, (*faceMidpoints)[i].x,0.000001) << "arrays faceMidpoints differ at index " << i;
		ASSERT_NEAR(exactfaceMidpoints[i].y, (*faceMidpoints)[i].y,0.000001) << "arrays faceMidpoints differ at index " << i;
	}
}