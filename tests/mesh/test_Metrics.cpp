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
using ees2d::mesh::Metrics::MetricsData;
using ees2d::utils::Vector2;


TEST(Test_Metrics, ccomputeCvolumesMetrics) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	MetricsData metrics;
	metrics.computeCvolumesMetrics(connectivity);


	// Act
	std::vector<float> exactcvAreas{0.125, 0.125, 0.125, 0.125,
	                                0.125, 0.125, 0.125, 0.125};

	std::vector<Vector2<float>> exactcvCentroids{Vector2<float>(0.5 / 3, 0.5 / 3),
	                                             Vector2<float>(1.0 / 3, 1.0 / 3),
	                                             Vector2<float>(2.0 / 3, 0.5 / 3),
	                                             Vector2<float>(2.5 / 3, 1.0 / 3),
	                                             Vector2<float>(0.5 / 3, 2.0 / 3),
	                                             Vector2<float>(1.0 / 3, 2.5 / 3),
	                                             Vector2<float>(2.0 / 3, 2.0 / 3),
	                                             Vector2<float>(2.5 / 3, 2.5 / 3)};

	std::vector<float> *cvAreas = &metrics.CvolumesArea;
	std::vector<Vector2<float>> *cvCentroids = &metrics.CvolumesCentroid;


	//Assert
	ASSERT_EQ(cvAreas->size(), exactcvAreas.size()) << "arrays cvAreas are of unequal length";

	for (size_t i = 0; i < exactcvAreas.size(); ++i) {
		EXPECT_EQ(exactcvAreas[i], (*cvAreas)[i]) << "arrays cvAreas differ at index " << i;
	}

	ASSERT_EQ(cvCentroids->size(), exactcvCentroids.size()) << "arrays cvCentroids are of unequal length";

	for (size_t i = 0; i < exactcvCentroids.size(); ++i) {
		ASSERT_FLOAT_EQ(exactcvCentroids[i].x, (*cvCentroids)[i].x) << "arrays cvCentroids differ at index " << i;
		ASSERT_FLOAT_EQ(exactcvCentroids[i].y, (*cvCentroids)[i].y) << "arrays cvCentroids differ at index " << i;
	}
}

TEST(Test_Metrics, ccomputeFaceMetrics) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	MetricsData metrics;
	metrics.computeFaceMetrics(connectivity);


	// Act
	std::vector<float> exactfaceSurfaces{0.5, 0.5, 0.707107f, 0.5,
	                                     0.5, 0.707107f,0.5 , 0.5,
	                                     0.5, 0.5, 0.707107f, 0.5,
	                                     0.707107f, 0.5, 0.5, 0.5};

	std::vector<Vector2<float>> exactfaceMidpoints{Vector2<float>(0.25, 0.0),
	                                               Vector2<float>(0, 0.25),
	                                               Vector2<float>(0.25, 0.25),
	                                               Vector2<float>(0.5, 0.25),
	                                               Vector2<float>(0.75, 0),
	                                               Vector2<float>(0.75, 0.25),
	                                               Vector2<float>(1.0, 0.25),
	                                               Vector2<float>(0.25, 0.5),
	                                               Vector2<float>(0.0, 0.75),
	                                               Vector2<float>(0.75, 0.5),
	                                               Vector2<float>(0.25, 0.75),
	                                               Vector2<float>(0.5, 0.75),
	                                               Vector2<float>(0.75, 0.75),
	                                               Vector2<float>(1.0, 0.75),
	                                               Vector2<float>(0.25, 1.0),
	                                               Vector2<float>(0.75, 1.0)};

	std::vector<float> *faceSurfaces = &metrics.facesSurface;
	std::vector<Vector2<float>> *faceMidpoints = &metrics.facesMidPoint;


	//Assert
	ASSERT_EQ(faceSurfaces->size(), exactfaceSurfaces.size()) << "arrays faceSurface are of unequal length";

	for (size_t i = 0; i < exactfaceSurfaces.size(); ++i) {
    ASSERT_FLOAT_EQ(exactfaceSurfaces[i], (*faceSurfaces)[i]) << "arrays faceSurface differ at index " << i;
	}

	ASSERT_EQ(faceMidpoints->size(), exactfaceMidpoints.size()) << "arrays faceMidpoints are of unequal length";

	for (size_t i = 0; i < exactfaceMidpoints.size(); ++i) {
		ASSERT_FLOAT_EQ(exactfaceMidpoints[i].x, (*faceMidpoints)[i].x) << "arrays faceMidpoints differ at index " << i;
		ASSERT_FLOAT_EQ(exactfaceMidpoints[i].y, (*faceMidpoints)[i].y) << "arrays faceMidpoints differ at index " << i;
	}
}