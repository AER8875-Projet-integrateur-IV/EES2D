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
#include <gtest/gtest.h>// Toujours inclu
#include <io/Su2Parser.h>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>
using ees2d::io::Su2Parser;

TEST(Test_Parser, parseGridInfo) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";
	Su2Parser mymesh(path);
	mymesh.Parse();

	// Act
	std::vector<std::tuple<double, double>> ExactCoords{
	        {0.0, 0.0},
	        {0.5, 0.0},
	        {1.0, 0.0},
	        {0.0, 0.5},
	        {0.5, 0.5},
	        {1.0, 0.5},
	        {0.0, 1.0},
	        {.5, 1.0},
	        {1.0, 1.0}};


	std::vector<std::tuple<double, double>> ParsedCOORDS = mymesh.get_coords();


	// Assert
	ASSERT_EQ(ExactCoords.size(), ParsedCOORDS.size()) << "Vectors x and y are of unequal length";

	for (size_t i = 0; i < ExactCoords.size(); ++i) {
		EXPECT_EQ(ExactCoords[i], ParsedCOORDS[i]) << "Vectors ExactCoords and ParsedCoords differ at index " << i;
	}
}


TEST(Test_Parser, parseElementsInfo) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";
	Su2Parser mymesh(path);
	mymesh.Parse();

	// Act
	std::vector<uint32_t> ExactElemIndex{0, 3, 6, 9, 12,
	                                     15, 18, 21, 24};

	std::vector<uint32_t> exactNPSUE{3, 3, 3, 3, 3, 3, 3, 3};

	std::vector<uint32_t> exactCONNEC{0, 1, 3,
	                                  1, 4, 3,
	                                  1, 2, 4,
	                                  2, 5, 4,
	                                  3, 4, 6,
	                                  4, 7, 6,
	                                  4, 5, 7,
	                                  5, 8, 7};

	std::vector<uint32_t> ElemIndex = mymesh.get_ElemIndex();
	std::vector<uint32_t> NPSUE = mymesh.get_NPSUE();
	std::vector<uint32_t> CONNEC = mymesh.get_CONNEC();

	std::cout << ElemIndex.size() << std::endl;

	// Assert
	ASSERT_EQ(ExactElemIndex.size(), ElemIndex.size()) << "Vectors OF ElemIndex are of unequal length";
	ASSERT_EQ(exactNPSUE.size(), NPSUE.size()) << "Vectors of  ExactNPSUE are of unequal length";
	ASSERT_EQ(exactCONNEC.size(), CONNEC.size()) << "Vectors of exactCONNEC  are of unequal length";

	for (size_t i = 0; i < ExactElemIndex.size(); ++i) {
		EXPECT_EQ(ExactElemIndex[i], ElemIndex[i]) << "Vectors ExactElementInex and ElemIndex differ at index " << i;
	}

	for (size_t i = 0; i < exactNPSUE.size(); ++i) {
		EXPECT_EQ(exactNPSUE[i], NPSUE[i]) << "Vectors exactNPSUE and NPSUE differ at index " << i;
	}

	for (size_t i = 0; i < exactCONNEC.size(); ++i) {
		EXPECT_EQ(exactCONNEC[i], CONNEC[i]) << "Vectors exactCONNEC and CONNEC differ at index " << i;
	}
}

TEST(Test_Parser, parseBoundaryConditionsInfo) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";
	Su2Parser mymesh(path);
	mymesh.Parse();
	// Act

	std::unordered_map<std::string, std::vector<std::vector<uint32_t>>> exactBoundaryConiditon = {
	        {"lower",
	         {{0, 1}, {1, 2}}},
	        {"right",
	         {{2, 5}, {5, 8}}},
	        {"upper",
	         {{8, 7}, {7, 6}}},
	        {"left",
	         {{6, 3}, {3, 0}}}};

	std::unordered_map<std::string, std::vector<std::vector<uint32_t>>> BoundaryCondition = mymesh.get_boundaryConditions();
	// Assert
	ASSERT_EQ(exactBoundaryConiditon.size(), BoundaryCondition.size()) << "maps OF BoundaryConditions are of unequal length";

	ASSERT_EQ(exactBoundaryConiditon["lower"], BoundaryCondition["lower"]) << "different elements in \"lower\" Marker tag";
	ASSERT_EQ(exactBoundaryConiditon["right"], BoundaryCondition["right"]) << "different elements in \"right\" Marker tag";
	ASSERT_EQ(exactBoundaryConiditon["upper"], BoundaryCondition["upper"]) << "different elements in \"upper\" Marker tag";
	ASSERT_EQ(exactBoundaryConiditon["left"], BoundaryCondition["left"]) << "different elements in \"left\" Marker tag";
}


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}