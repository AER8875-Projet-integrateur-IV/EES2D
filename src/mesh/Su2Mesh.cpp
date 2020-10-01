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

#include "Mesh.h"
#include <iostream>
#include <sstream>

using namespace ees2d::Mesh;
using std::cout, std::endl;

Su2Mesh::Su2Mesh(const std::string &path) : AbstractMesh::AbstractMesh(path) {

	std::ifstream m_fileIO{m_path};
	if (m_fileIO.is_open()) {
		cout << "Mesh file found\n"
		     << "SU2 Mesh Initialized !" << endl;
		m_proceed = true;
	} else {
		cout << "Unable to open file !" << endl;
	}
}

Su2Mesh::~Su2Mesh() { cout << "SU2 Mesh Destroyed !" << endl; }

void Su2Mesh::parseCOORDS(std::ifstream &m_fileIO) {}

void Su2Mesh::parseCONNEC(std::ifstream &m_fileIO) {}

void Su2Mesh::parseNPSUE(std::ifstream &m_fileIO) {}

void Su2Mesh::parseFileInfo(std::ifstream &m_fileIO) {
	std::string line;


	while (std::getline(m_fileIO, line)) {

		if (line.find("NDIME") != std::string::npos) {
			std::stringstream ss(line);//Temporar
			ss.seekg(6) >> m_Ndim;     //Begin at Pos num 6 to extract int : NDIME= 2
			cout << m_Ndim << endl;
			break;
		}
	}
}

void Su2Mesh::Parse() {

	if (m_proceed) {
		std::cout << m_path << std::endl;
		std::ifstream m_fileIO(m_path);
		parseFileInfo(m_fileIO);
		parseCOORDS(m_fileIO);
		parseCONNEC(m_fileIO);
		parseNPSUE(m_fileIO);
	}
}
