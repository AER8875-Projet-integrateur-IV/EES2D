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

#include "Su2Parser.h"
#include "AbstractParser.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

using ees2d::io::Su2Parser;
using std::cout, std::endl, std::cerr;
using std::swap;


Su2Parser::Su2Parser(const std::string &path) : AbstractParser::AbstractParser(path) {

	std::ifstream m_fileIO{m_path};

	if (m_fileIO.is_open()) {
		cout << "--------------------  Parsing Su2 Mesh file !"
		        " ------------------------"
		     << endl;
		m_proceed = true;
	} else {
		cerr << "Unable to open mesh file !" << endl;
		exit(EXIT_FAILURE);
	}
}


Su2Parser::~Su2Parser() {}


void Su2Parser::Parse() {

	if (m_proceed) {
		std::ifstream m_fileIO(m_path);
		parseDimensionInfo(m_fileIO);
		parseGridsInfo(m_fileIO);
		parseElementsInfo(m_fileIO);
		parseBoundaryConditionsInfo(m_fileIO);
	}
}

void Su2Parser::parseDimensionInfo(std::ifstream &m_fileIO) {
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


void Su2Parser::parseGridsInfo(std::ifstream &m_fileIO) {
	std::string line;

	m_fileIO.seekg(std::ifstream::beg);
	while (std::getline(m_fileIO, line)) {

		if (line.find("NPOIN") != std::string::npos) {
			//Temporary stringstream to parse int from string
			std::stringstream ss1(line);

			//Begin at Pos num 6 to extract int : NPOIN = 2
			ss1.seekg(6) >> m_Ngrids;
			cout << std::setw(40) << "Number of nodes : " << std::setw(6) << m_Ngrids << "\n";
			m_COORDS.reserve(m_Ngrids + 1);
			float x, y;

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


void Su2Parser::parseElementsInfo(std::ifstream &m_fileIO) {
	std::string line;

	// Setting m_fileIO back to the beginning the file
	m_fileIO.seekg(m_fileIO.beg);

	while (std::getline(m_fileIO, line)) {

		if (line.find("NELEM") != std::string::npos) {

			//Temporary stringstream to parse int from string
			std::stringstream ss(line);

			//Begin at Pos num 6 to extract int : NELEM = ....
			ss.seekg(6) >> m_Nelems;
			cout << std::setw(40) << "Number of elements : " << std::setw(6) << m_Nelems << "\n";

			//Size is known for vectors,
			// except m_CONnec, assumed at least 3 points connected to each element
			m_ElemIndex.reserve(m_Nelems + 1);
			m_ElemIndex.push_back(0);
			m_NPSUE.reserve(m_Nelems + 1);
			m_CONNEC.reserve(m_Nelems * 3);

			uint32_t type;
			uint32_t grid_id;
			uint32_t index_counter = 0;

			for (uint32_t i = 0; i < m_Nelems; ++i) {
				std::getline(m_fileIO, line);
				std::stringstream ss(line);
				ss >> type;

				m_NPSUE.push_back(m_Vtk_Cell[type]);
				index_counter += m_Vtk_Cell[type];
				m_ElemIndex.push_back(index_counter);

				for (uint32_t j = 0; j < m_Vtk_Cell[type]; ++j) {
					ss >> grid_id;

					m_CONNEC.push_back(grid_id);
				}
			}
			break;
		}
	}
}


void Su2Parser::parseBoundaryConditionsInfo(std::ifstream &m_fileIO) {
	std::string line;

	// Setting m_fileIO back to the beginning the file
	m_fileIO.seekg(m_fileIO.beg);


	while (std::getline(m_fileIO, line)) {

		//entry condition to parse boundary info
		if (line.find("NMARK") != std::string::npos) {
			std::stringstream ss(line);
			ss.seekg(6) >> m_Nboundaries;
			cout << std::setw(40) << "Number of Boundaries (markers) : " << std::setw(6) << m_Nboundaries << "\n";

			// Temporary variables to hold info of each SU2 mesh line
			std::string boundary_tag;
			uint32_t N_boudaryelements;
			uint32_t element_type;
			uint32_t grid_id;
			uint32_t grid_id2;
			uint32_t boundary_id =0;
			std::vector<uint32_t> temp ={};
      std::vector<uint32_t> temp_first_node_pos ={};

			// Loop though the number of Boundary Conditions (Tags)
			for (size_t i = 0; i < m_Nboundaries; ++i) {
				std::getline(m_fileIO, line);

				//parse Tag name
				if (line.find("MARKER_TAG") != std::string::npos) {
					std::stringstream ss(line);
					ss.seekg(11) >> boundary_tag;
					if(boundary_tag == "airfoil" || boundary_tag=="wall"){
						boundary_id=-1;
					}
					else if(boundary_tag == "slipwall"){
            boundary_id = -2;
					}
					else if(boundary_tag == "farfield"){
            boundary_id = -3;
					}
					else if(boundary_tag == "condition4"){
						boundary_id = -4;
					}
					else {
						std::cerr << "Unknown boundary condition in mesh file : '" << boundary_tag << "' " <<std::endl;
            exit(EXIT_FAILURE);
					}
					std::getline(m_fileIO, line);

					// Parse Number of element in current looped tag
					if (line.find("MARKER_ELEMS") != std::string::npos) {
						std::stringstream ss(line);
						ss.seekg(13) >> N_boudaryelements;


						// Loop through Elements representing current Boundary condition (tag)
						for (size_t j = 0; j < N_boudaryelements; ++j) {
							temp = {};
							std::getline(m_fileIO, line);
							std::stringstream ss(line);
							ss >> element_type >> grid_id >> grid_id2;
							if (grid_id > grid_id2){
								swap(grid_id2,grid_id);
							}
							temp_first_node_pos.push_back(grid_id);
							temp.push_back(grid_id);
							temp.push_back(grid_id2);
							temp.push_back(boundary_id);
							m_boundaryConditions.push_back(temp);
						}
					}
				}
			}
			m_boundaryConditions.push_back(temp_first_node_pos);
			break;
		}
	}
}