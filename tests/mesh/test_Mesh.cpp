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
 *///

#include <gtest/gtest.h>// Toujours inclu
#include "io/Su2Parser.h"
#include "mesh/Mesh.h"
#include <memory>

using ees2d::io::Su2Parser;
using ees2d::mesh::Mesh;


TEST(Test_Mesh, connecPointSurrElement) {
	// Arrange
	std::string path = "../../../tests/testmesh.su2";

  std::shared_ptr<Su2Parser> parser(std::make_shared<Su2Parser>(path));
	parser->Parse();

	std::unique_ptr<Mesh> mesh (new Mesh(parser));

	// Act
  std::vector<uint32_t> exactCONNEC{0, 1, 3,
                                    1, 4, 3,
                                    1, 2, 4,
                                    2, 5, 4,
                                    3, 4, 6,
                                    4, 7, 6,
                                    4, 5, 7,
                                    5, 8, 7};

	int a = mesh->connecPointSurrElement(0,0);
	int b = mesh->connecPointSurrElement(1,1);
	int c = mesh->connecPointSurrElement(0,2);
	int d = mesh->connecPointSurrElement(2,4);
	int e = mesh->connecPointSurrElement(2,7);

	//Assert

	ASSERT_EQ(0,a );
  ASSERT_EQ(4, b);
  ASSERT_EQ(1, c);
  ASSERT_EQ(6, d);
  ASSERT_EQ(7, e);

}

