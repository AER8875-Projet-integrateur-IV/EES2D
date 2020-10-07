#include <gmock/gmock.h>// Toujours inclu
#include <gtest/gtest.h>// Toujours inclu
#include <io/AbstractParser.h>
#include <io/Su2Parser.h>
#include <tuple>
#include <vector>
using ees2d::IO::Su2Parser;

TEST(Test_IO, parseCOORDS) {
	// Arrange
	std::string path = "../../../tests/io/testmesh.su2";
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
};


TEST(Test_IO, parseCONNEC) {
	// Arrange
	std::string path = "../../../tests/io/testmesh.su2";
	Su2Parser mymesh(path);
	mymesh.Parse();

	// Act
	std::vector<int> ExactElemIndex{0, 3, 6, 9, 12,
	                                15, 18, 21, 24};

	std::vector<int> exactNPSUE{3, 3, 3, 3, 3, 3, 3, 3};

	std::vector<int> exactCONNEC{0, 1, 3,
	                             1, 4, 3,
	                             1, 2, 4,
	                             2, 5, 4,
	                             3, 4, 6,
	                             4, 7, 6,
	                             4, 5, 7,
	                             5, 8, 7};

	std::vector<int> ElemIndex = mymesh.get_ElemIndex();
	std::vector<int> NPSUE = mymesh.get_NPSUE();
	std::vector<int> CONNEC = mymesh.get_CONNEC();

	// Assert

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
