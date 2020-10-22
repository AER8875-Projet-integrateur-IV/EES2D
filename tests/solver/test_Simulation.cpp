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
#include "io/InputParser.h"
#include "io/Su2Parser.h"
#include "mesh/Connectivity.h"
#include "mesh/Metrics.h"
#include "solver/Simulation.h"
#include <gtest/gtest.h>

using ees2d::io::InputParser;
using ees2d::io::Su2Parser;
using ees2d::mesh::Connectivity;
using ees2d::mesh::Mesh;
using ees2d::mesh::MetricsData;
using ees2d::solver::Simulation;


TEST(test_Simulation, SimulationConstructor) {
	// Arrange
	std::cout << "Euler2D Software" << std::endl;

	std::string inputFilePath = "../../../tests/io/testmesh.ees2d";
	InputParser simulationParameters{inputFilePath};
	simulationParameters.parse();
	simulationParameters.printAll();

	Su2Parser parser(simulationParameters.m_meshFile);
	parser.Parse();

	Connectivity connectivity(parser);
	connectivity.solve();

	MetricsData metrics;
	metrics.compute(connectivity);

	Mesh mesh(connectivity, metrics);

	Simulation mysim(mesh, simulationParameters);

	//Act
	std::vector<float> *u = &mysim.u;
	std::vector<float> *v = &mysim.v;
	std::vector<float> *rho = &mysim.rho;
	std::vector<float> *p = &mysim.p;

	std::vector<float> exact_u = {203.983885, 203.983885, 203.983885, 203.983885,
	                              203.983885, 203.983885, 203.983885, 203.983885};
	std::vector<float> exact_v = {8.90613, 8.90613, 8.90613, 8.90613, 8.90613,
	                              8.90613, 8.90613, 8.90613};
	std::vector<float> exact_rho = {1.2886,1.2886,1.2886,1.2886,
                                  1.2886,1.2886,1.2886,1.2886};
	std::vector<float> exact_p = {101325,101325,101325,101325,
                                101325,101325,101325,101325};

	//Assert
  ASSERT_EQ(u->size(), exact_u.size()) << "vectors u are of unequal length";
  ASSERT_EQ(v->size(), exact_v.size()) << "vectors v are of unequal length";
  ASSERT_EQ(rho->size(), exact_rho.size()) << "vectors rho are of unequal length";
  ASSERT_EQ(p->size(), exact_p.size()) << "vectors p are of unequal length";


  for (size_t i = 0; i < exact_u.size(); ++i) {
    EXPECT_FLOAT_EQ(exact_u[i], (*u)[i]) << "vectors u differ at index " << i;
  }

  for (size_t i = 0; i < exact_v.size(); ++i) {
    EXPECT_FLOAT_EQ(exact_v[i], (*v)[i]) << "vectors v differ at index " << i;
  }

  for (size_t i = 0; i < exact_rho.size(); ++i) {
    EXPECT_FLOAT_EQ(exact_rho[i], (*rho)[i]) << "vectors rho differ at index " << i;
  }

  for (size_t i = 0; i < exact_p.size(); ++i) {
    EXPECT_FLOAT_EQ(exact_p[i], (*p)[i]) << "vectors p differ at index " << i;
  }
}