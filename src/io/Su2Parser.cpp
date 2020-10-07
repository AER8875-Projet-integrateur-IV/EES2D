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

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "AbstractParser.h"
#include "Su2Parser.h"
#include "utils/Timer.h"

using ees2d::IO::Su2Parser;
using ees2d::Utils::Timer;
using std::cout, std::endl, std::cerr;


Su2Parser::Su2Parser(const std::string &path) : AbstractParser::AbstractParser(path) {

	std::ifstream m_fileIO{m_path};

	if (m_fileIO.is_open()) {
		cout << "Su2 Mesh file found\n"
		     << "SU2 AbstractParser Initialized !" << endl;
		m_proceed = true;
	} else {
		cerr << "Unable to open mesh file !" << endl;
		exit(EXIT_FAILURE);
	}
}


Su2Parser::~Su2Parser() { cout << "SU2 AbstractParser Destroyed !" << endl; }


void Su2Parser::parseCONNEC(std::ifstream &m_fileIO) {
	Timer timeit("PARSECONNEC");
	std::string line;

	// Setting m_fileIO back to the beginning the file
	m_fileIO.seekg(m_fileIO.beg);

	while (std::getline(m_fileIO, line)) {

		if (line.find("NELEM") != std::string::npos) {

			//Temporary stringstream to parse int from string
			std::stringstream ss(line);

			//Begin at Pos num 6 to extract int : NELEM = ....
			ss.seekg(6) >> m_Nelems;
			cout << "Number of elements : " << m_Nelems << "\n";

			//Size is known for vectors, except m_CONnec
			m_ElemIndex.reserve(m_Nelems + 1);
			m_ElemIndex.push_back(0);
			m_NPSUE.reserve(m_Nelems + 1);
			m_CONNEC.reserve(m_Nelems * 2);

			int type, grid_id;
			int index_counter = 0;

			for (size_t i = 0; i < m_Nelems; ++i) {
				std::getline(m_fileIO, line);
				std::stringstream ss1(line);
				ss1 >> type;

				m_NPSUE.push_back(m_Vtk_Cell[type]);
				index_counter += m_Vtk_Cell[type];
				m_ElemIndex.push_back(index_counter);

				for (size_t j = 0; j < m_Vtk_Cell[type]; ++j) {
					ss1 >> grid_id;
					m_CONNEC.push_back(grid_id);
				}
			}
			break;
		}
	}
}


void Su2Parser::parseNDim(std::ifstream &m_fileIO) {
	// Parse mesh dimension from SU2 file

	std::string line;
	while (std::getline(m_fileIO, line)) {

		if (line.find("NDIME") != std::string::npos) {
			//Temporary stringstream to parse int from string
			std::stringstream ss(line);

			//Begin at Pos num 6 to extract int : NDIME= ...
			ss.seekg(6) >> m_Ndim;
			break;
		}
	}

	if (m_Ndim != 2) {
		// Exit software if not a 2D mesh

		cerr << "Not a 2D mesh !" << endl;
		exit(EXIT_FAILURE);
	}
}


void Su2Parser::parseCOORDS(std::ifstream &m_fileIO) {
	std::string line;

	m_fileIO.seekg(std::ifstream::beg);
	while (std::getline(m_fileIO, line)) {

		if (line.find("NPOIN") != std::string::npos) {
			//Temporary stringstream to parse int from string
			std::stringstream ss1(line);

			//Begin at Pos num 6 to extract int : NPOIN = 2
			ss1.seekg(6) >> m_Ngrids;
			cout << "Number of points : " << m_Ngrids << "\n";
			m_COORDS.reserve(m_Ngrids + 1);
			double x, y;

			for (size_t i = 0; i < m_Ngrids; ++i) {
				std::getline(m_fileIO, line);
				std::stringstream ss1(line);
				ss1 >> x >> y;
				m_COORDS.push_back({x, y});
			}

			break;
		}
	}
}


void Su2Parser::parseNPSUE(std::ifstream &m_fileIO) {
}


void Su2Parser::Parse() {

	if (m_proceed) {
		std::ifstream m_fileIO(m_path);
		parseNDim(m_fileIO);
		parseCOORDS(m_fileIO);
		parseCONNEC(m_fileIO);
		parseNPSUE(m_fileIO);
	}
}
