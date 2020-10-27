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

#include "io/InputParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>


using ees2d::io::InputParser;
using std::ifstream, std::cout, std::endl;
using std::stringstream;


InputParser::InputParser(std::string &inputPath)
    : m_inputPath(inputPath) {

	ifstream fileStream{m_inputPath};

	if (fileStream.is_open()) {
		cout << "Control file found\n"
		     << "--------------------  Parsing control file !"
		        " ------------------------"
		     << endl;
	} else {
		cout << "Control file not found with given path ! '" << m_inputPath
		     << " '" << endl;
	}
}

//-------------------------------------------

void InputParser::parse() {
	ifstream fileStream(m_inputPath);

	std::string line;

	while (std::getline(fileStream, line)) {

		if (line.find("START") != std::string::npos) {
			while (!(line.find("END") != std::string::npos)) {
				std::getline(fileStream, line);

				stringstream ss1(line);

				if (line.find("MESH_FORMAT") != std::string::npos) {
					ss1.seekg(13) >> m_meshFormat;
				}
				else if (line.find("MESH_FILE") != std::string::npos){
					ss1.seekg(11) >> m_meshFile;
				}
				else if (line.find("MESH_TYPE") != std::string::npos){
					ss1.seekg(11) >> m_meshType;
				}
        else if (line.find("SPEED_OPTION") != std::string::npos){
          ss1.seekg(14) >> m_spdOption;
        }
        else if (line.find("VELOCITY") != std::string::npos){
          ss1.seekg(10) >> m_velocity;
        }
        else if (line.find("MACH") != std::string::npos){
          ss1.seekg(6) >> m_Mach;
        }
        else if (line.find("AOA") != std::string::npos){
          ss1.seekg(5) >> m_aoa;
        }
        else if (line.find("AIRFLOW_PRESSURE") != std::string::npos){
          ss1.seekg(18) >> m_Pressure;
        }
        else if (line.find("AIRFLOW_TEMPERATURE") != std::string::npos){
          ss1.seekg(21) >> m_Temp;
        }
        else if (line.find("AIRFLOW_VISCOSITY") != std::string::npos){
          ss1.seekg(19) >> m_Visc;
        }
        else if (line.find("AIRFLOW_DENSITY") != std::string::npos){
          ss1.seekg(17) >> m_Density;
        }
        else if (line.find("GAMMA") != std::string::npos){
          ss1.seekg(7) >> m_Gamma;
        }
        else if (line.find("GAS_CONSTANT") != std::string::npos){
          ss1.seekg(14) >> m_gasConstant;
        }
        else if (line.find("SPECIFIC_HEAT") != std::string::npos){
          ss1.seekg(15) >> m_SpecificHeat;
        }
        else if (line.find("SCHEME") != std::string::npos){
          ss1.seekg(8) >> m_scheme;
        }
        else if (line.find("MIN_RESIDUAL") != std::string::npos){
          ss1.seekg(14) >> m_minResiudal;
        }
        else if (line.find("MAX_ITER") != std::string::npos){
          ss1.seekg(10) >> m_maxIter;
        }
        else if (line.find("OUTPUT_FORMAT") != std::string::npos){
          ss1.seekg(15) >> m_outputFormat;
        }
        else if (line.find("OUTPUT_FILE") != std::string::npos){
          ss1.seekg(13) >> m_outputFile;
        }
        else if (line.find("GENERATE_LOG") != std::string::npos){
          ss1.seekg(14) >> m_generateLog;
        }
			}

			if (m_spdOption == "MACH"){
				m_velocity = m_Mach * std::sqrt(m_Gamma*(m_Pressure/m_Density));
			}
      std::cout << std::setw(40) << "Parsing Input File : " << std::setw(6) << "Done\n";
			break;
		}
	}
}

void ees2d::io::InputParser::printAll() {
  std::cout << " m_meshFormat  "  <<m_meshFormat << "\n"
            << "m_meshFile     "  <<m_meshFile    << "\n"
            << "m_meshType     "  <<m_meshType    << "\n"
            << "m_spdOption    "  <<m_spdOption   << "\n"
            << "m_velocity     "  <<m_velocity    << "\n"
            << "m_Mach         "  <<m_Mach         << "\n"
            << "m_Pressure     "  <<m_Pressure    << "\n"
            << "m_Temp         "  <<m_Temp         << "\n"
            << "m_Visc         "  <<m_Visc         << "\n"
            << "m_Density      "  <<m_Density      << "\n"
            << "m_Gamma        "  <<m_Gamma       << "\n"
            << "m_gasConstant  "  <<m_gasConstant << "\n"
            << "m_SpecificHeat " <<m_SpecificHeat << "\n"
            << "m_scheme       "  <<m_scheme        << "\n"
            << "m_minResiudal  "  <<m_minResiudal   << "\n"
            << "m_threadsNum   "  <<m_threadsNum    << "\n"
            << "m_outputFormat " <<m_outputFormat << "\n"
            << "m_outputFile   "  <<m_outputFile    << "\n"
            << "m_generateLog  "  <<m_generateLog  << "\n";
}
