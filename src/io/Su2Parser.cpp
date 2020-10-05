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

#include <stdlib.h>

#include <iostream>
#include <sstream>

#include <utils/Timer.h>
#include "Parser.h"

using namespace ees2d::IO;
using namespace ees2d::Utils;
using std::cout, std::endl, std::cerr;

Su2Parser::Su2Parser(const std::string &path) : Parser::Parser(path) {

	std::ifstream m_fileIO{m_path};
	if (m_fileIO.is_open()) {
		cout << "Su2 Mesh file found\n"
		     << "SU2 Parser Initialized !" << endl;
		m_proceed = true;
	} else {
		cerr << "Unable to open mesh file !" << endl;
		exit(EXIT_FAILURE);
	}
}


Su2Parser::~Su2Parser() { cout << "SU2 Parser Destroyed !" << endl; }

void Su2Parser::parseCONNEC(std::ifstream &m_fileIO) {
	std::string line;
	m_fileIO.seekg(m_fileIO.beg);

	while (std::getline(m_fileIO, line)) {

		if (line.find("NELEM") != std::string::npos) {

			std::stringstream ss(line);//Temporary stringstream to parse int from string
			ss.seekg(6) >> m_Nelems;   //Begin at Pos num 6 to extract int : NELEM = 2
			cout << "Number of elements : " << m_Nelems << "\n";
		}
	}
}


void Su2Parser::parseNDim(std::ifstream &m_fileIO) {
	std::string line;
	while (std::getline(m_fileIO, line)) {

		if (line.find("NDIME") != std::string::npos) {

			std::stringstream ss(line);//Temporary stringstream to parse int from string
			ss.seekg(6) >> m_Ndim;     //Begin at Pos num 6 to extract int : NDIME= 2
			cout << "Number of dimensions : " << m_Ndim << "\n";
			break;
		}
	}
}


void Su2Parser::parseCOORDS(std::ifstream &m_fileIO) {
	std::string line;
  //Timer timeit("parseCoords");
	m_fileIO.seekg(m_fileIO.beg);
	while (std::getline(m_fileIO, line)) {
		if (line.find("NPOIN") != std::string::npos) {

			std::stringstream ss1(line);//Temporary stringstream to parse int from string
			ss1.seekg(6) >> m_Ngrids;   //Begin at Pos num 6 to extract int : NPOIN = 2
			cout << "Number of points : " << m_Ngrids << "\n";
			m_COORDS.reserve(m_Ngrids * 2);
			ss1.flush();
			double x, y;
			for (size_t i = 0; i < m_Ngrids; ++i) {

				std::getline(m_fileIO, line);
				std::stringstream ss1(line);
				ss1 >> x >> y;
				m_COORDS.push_back(x);
				m_COORDS.push_back(y);
				ss1.flush();
			}
			break;
		}
	}
}


void Su2Parser::parseNPSUE(std::ifstream &m_fileIO) {}


void Su2Parser::Parse() {

	if (m_proceed) {
		std::ifstream m_fileIO(m_path);
		parseNDim(m_fileIO);
		parseCOORDS(m_fileIO);
		parseCONNEC(m_fileIO);
		parseNPSUE(m_fileIO);
	}
}
