#include <gmock/gmock.h>// Toujours inclu
#include <gtest/gtest.h>// Toujours inclu
#include <io/Parser.h>
#include <vector>

using namespace ees2d::IO;

TEST(Test_IO, parseCOORDS) {
	std::string path = "../../../tests/io/testmesh.su2";
  Su2Parser mymesh(path);
  mymesh.Parse();
  std::vector<double> ExactCoords{0.0, 0.0, 0.5, 0.0, 1.0, 0.0,
                                  0.0, 0.5, 0.5, 0.5, 1.0, 0.5,
                                  0.0, 1.0, .5, 1.0, 1.0, 1.0};

  std::vector<double> ParsedCOORDS = mymesh.get_coords();
  ASSERT_EQ(ExactCoords.size(), ParsedCOORDS.size()) << "Vectors x and y are of unequal length";

  for (size_t i = 0; i < ExactCoords.size(); ++i) {
    EXPECT_EQ(ExactCoords[i], ParsedCOORDS[i]) << "Vectors x and y differ at index " << i;
  }
}
