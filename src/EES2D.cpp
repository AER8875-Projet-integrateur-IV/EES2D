/* ---------------------------------------------------------------------
 *
 * Copyright (C) 2020 - by the EES2D  authors
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
#include "utils/Timer.h"
#include "mesh/Metrics.h"
#include <iostream>

using ees2d::io::Su2Parser;
using ees2d::mesh::Connectivity;
using ees2d::utils::Timer;
using namespace ees2d::mesh::Metrics;
//using namespace ees2d::Utils;


int main() {
	Timer Timeit("software runtime");
	std::cout << "Euler2D Software" << std::endl;

	std::string path = "../../tests/testmesh.su2";
	//std::string path = "/home/amin/Downloads/naca0012_euler_1025x1025x1_O_1B.su2";
	Su2Parser parser(path);
	parser.Parse();

	auto BC = parser.get_boundaryConditions();


	Connectivity connectivity(parser);
	connectivity.solve();


	MetricsData metrics;
	metrics.computeMetrics(connectivity);

  for (auto &elem : metrics.CvolumesArea) {
    //for (auto &value : elem) {
    std::cout << elem;
    // }
    std::cout << "\n";
  }
	std::cout << "cv Centroids :" << "\n";

  for (auto &elem : metrics.CvolumesCentroid) {
    //for (auto &value : elem) {
    std::cout << elem;
    // }
    std::cout << "\n";
  }

  std::cout << "faces Surfaces :" << "\n";

  for (auto &elem : metrics.facesSurface) {
    //for (auto &value : elem) {
    std::cout << elem;
    // }
    std::cout << "\n";
  }
  std::cout << "faces MIdpoints :" << "\n";

  for (auto &elem : metrics.facesMidPoint) {
    //for (auto &value : elem) {
    std::cout << elem;
    // }
    std::cout << "\n";
  }

  std::cout << "faces vector :" << "\n";
  for (auto &elem : metrics.facesVector) {
    //for (auto &value : elem) {
    std::cout << elem;
    // }
    std::cout << "\n";
  }

	return 0;
}
