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

#pragma once
#include <string>



namespace ees2d::io
{
	class InputParser{
public:
    explicit InputParser(std::string &inputPath);

		void parse();
		void printAll();

		// Preprocessing variables
		std::string m_meshFormat;
		std::string m_meshFile;
		std::string m_meshType;

		// Simulation variables
		std::string m_spdOption;
		double m_velocity=0;
		double m_Mach = 0;
		double m_aoa = 0;
		double m_Pressure = 0;
		double m_Temp = 0;
		double m_Visc = 0;
		double m_Density = 0;
		double m_Gamma = 0;
		double m_gasConstant = 0;
		double m_SpecificHeat = 0;

		//Solver variables
		std::string m_scheme;
		double m_minResiudal = 0;
		uint32_t m_maxIter = 0;
		uint32_t m_threadsNum = 0;

		// PostProcessing variables
		std::string m_outputFormat;
		std::string m_outputFile;
		std::string m_generateLog;

private:
		std::string m_inputPath;

	};
}