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
#include <gtest/gtest.h>// Toujours inclu
#include <memory>

using ees2d::io::Su2Parser;
using ees2d::mesh::Connectivity;


TEST(Test_Connectivity, connecNodeSurrElement) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);


	// Act
	std::vector<uint32_t> exactCONNEC{0, 1, 3,
	                                  1, 4, 3,
	                                  1, 2, 4,
	                                  2, 5, 4,
	                                  3, 4, 6,
	                                  4, 7, 6,
	                                  4, 5, 7,
	                                  5, 8, 7};

	const uint32_t a = connectivity.connecNodeSurrElement(0, 0);
	const uint32_t b = connectivity.connecNodeSurrElement(1, 1);
	const uint32_t c = connectivity.connecNodeSurrElement(0, 2);
	const uint32_t d = connectivity.connecNodeSurrElement(2, 4);
	const uint32_t e = connectivity.connecNodeSurrElement(2, 7);

	//Assert

	ASSERT_EQ(0, a);
	ASSERT_EQ(4, b);
	ASSERT_EQ(1, c);
	ASSERT_EQ(6, d);
	ASSERT_EQ(7, e);
}

TEST(Test_Connectivity, solveElemSurrNode) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	// Act
	std::vector<uint32_t> exactesup2{0, 1, 4, 6, 9, 15, 18, 20, 23, 24};
	std::vector<uint32_t> exactesup1{0, 0, 1, 2, 2, 3, 0, 1, 4, 1, 2, 3, 4, 5, 6, 3, 6, 7, 4, 5, 5, 6, 7, 7};

	std::shared_ptr<uint32_t[]> esup2 = connectivity.get_esup2();
	std::shared_ptr<uint32_t[]> esup1 = connectivity.get_esup1();
	const uint32_t &size_esup2 = connectivity.get_esup2_size();
	const uint32_t &size_esup1 = connectivity.get_esup1_size();

	ASSERT_EQ(size_esup2, exactesup2.size()) << "arrays esup2 are of unequal length";
	ASSERT_EQ(size_esup1, exactesup1.size()) << "arrays esup1 are of unequal length";

	for (size_t i = 0; i < exactesup2.size(); ++i) {
		EXPECT_EQ(exactesup2[i], esup2[i]) << "arrays esup2 differ at index " << i;
	}

	for (size_t i = 0; i < exactesup1.size(); ++i) {
		EXPECT_EQ(exactesup1[i], esup1[i]) << "arrays esup1 differ at index " << i;
	}
}

TEST(Test_Connectivity, solveNodeSurrNode) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	// Act
	std::vector<uint32_t> exactpsup2{0, 2, 6, 9, 13, 19, 23, 26, 30, 32};

	std::vector<uint32_t> exactpsup1{1, 3, 0, 3, 4, 2, 1,
	                                 4, 5, 0, 1, 4, 6, 1,
	                                 3, 2, 5, 6, 7, 2, 4,
	                                 7, 8, 3, 4, 7, 4, 6, 5, 8, 5, 7};

	auto psup2 = connectivity.get_psup2();
	const std::vector<uint32_t> *psup1;
	psup1 = connectivity.get_psup1();
	auto psup2_size = connectivity.get_psup2_size();

	ASSERT_EQ(psup2_size, exactpsup2.size()) << "arrays psup2 are of unequal length";
	ASSERT_EQ(psup1->size(), exactpsup1.size()) << "arrays psup1 are of unequal length";

	for (size_t i = 0; i < exactpsup2.size(); ++i) {
		EXPECT_EQ(exactpsup2[i], psup2[i]) << "arrays psup2 differ at index " << i;
	}

	for (size_t i = 0; i < exactpsup1.size(); ++i) {
		EXPECT_EQ(exactpsup1[i], (*psup1)[i]) << "arrays psup1 differ at index " << i;
	}
}

TEST(Test_Connectivity, solveElemSurrElem) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	// Act
	std::vector<std::vector<uint32_t>> exactesuel{{1,uint32_t(-3),uint32_t(-3)}, {2, 4, 0}, {3, 1,uint32_t(-3)},
	                                              {6, 2,uint32_t(-3)}, {1, 5,uint32_t(-3)}, {6, 4,uint32_t(-3)},
	                                              {3, 7, 5}, {6,uint32_t(-3),uint32_t(-3)}};

	auto esuel = connectivity.get_elemToElem();


	ASSERT_EQ(esuel->size(), exactesuel.size()) << "arrays esuel are of unequal length";

	for (size_t i = 0; i < exactesuel.size(); ++i) {
		EXPECT_EQ(exactesuel[i], (*esuel)[i]) << "arrays psup1 differ at index " << i;
	}
}

TEST(Test_Connectivity, solveNodeSurrFace) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	// Act
	std::vector<std::vector<uint32_t>> exactNodeSurrFace{{0, 1}, {0, 3}, {1, 3},
	                                                     {1, 4}, {1, 2}, {2, 4},
	                                                     {2, 5}, {3, 4}, {3, 6},
	                                                     {4, 5}, {4, 6}, {4, 7},
	                                                     {5, 7}, {5, 8}, {6, 7}, {7, 8}};

	auto NodeSurrFace = connectivity.get_FaceToNode();


	ASSERT_EQ(NodeSurrFace->size(), exactNodeSurrFace.size()) << "arrays esuel are of unequal length";

	for (size_t i = 0; i < exactNodeSurrFace.size(); ++i) {
		EXPECT_EQ(exactNodeSurrFace[i], (*NodeSurrFace)[i]) << "arrays psup1 differ at index " << i;
	}
}

TEST(Test_Connectivity, solveFaceSurrElem) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	// Act
	std::vector<std::vector<uint32_t>> ExactFaceSurrElem{{0, 2, 1},
	                                                     {3, 7, 2},
	                                                     {4, 5, 3},
	                                                     {6, 9, 5},
	                                                     {7, 10, 8},
	                                                     {11, 14, 10},
	                                                     {9, 12, 11},
	                                                     {13, 15, 12}};

	auto FaceSurrElem = connectivity.get_ElemToFace();


	ASSERT_EQ(FaceSurrElem->size(), ExactFaceSurrElem.size()) << "arrays Element to face are of unequal length";

	for (size_t i = 0; i < ExactFaceSurrElem.size(); ++i) {
		EXPECT_EQ(ExactFaceSurrElem[i], (*FaceSurrElem)[i]) << "arrays Element to face differ at index " << i;
	}
}

TEST(Test_Connectivity, solveElemSurrFace) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

	Su2Parser parser(path);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	// Act
	std::vector<std::vector<uint32_t>> ExactElemSurrFace{{0,uint32_t(-3)}, {0,uint32_t(-3)}, {0, 1}, {1, 2},
	                                                {2,uint32_t(-3)}, {2, 3}, {3,uint32_t(-3)}, {1, 4},
	                                                {4,uint32_t(-3)}, {3, 6}, {4, 5}, {5, 6},
	                                                {6, 7}, {7,uint32_t(-3)}, {5,uint32_t(-3)}, {7,uint32_t(-3)}};

	auto ElemSurrFace = connectivity.get_FaceToElem();


	ASSERT_EQ(ElemSurrFace->size(), ExactElemSurrFace.size()) << "arrays Element to face are of unequal length";

	for (size_t i = 0; i < ExactElemSurrFace.size(); ++i) {
		EXPECT_EQ(ExactElemSurrFace[i], (*ElemSurrFace)[i]) << "arrays Element to face differ at index " << i;
	}
}
