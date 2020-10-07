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

#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace ees2d::IO {

	class AbstractParser {
		// Abstract class serving as an interface to other parser classes

public:
		explicit AbstractParser(const std::string &path) : m_path(path){};
		virtual ~AbstractParser(){};

		// Pure virtual function to extract number of dimensions
		virtual void parseNDim(std::ifstream &) = 0;
		virtual void parseCOORDS(std::ifstream &) = 0;
		virtual void parseCONNEC(std::ifstream &) = 0;
		virtual void parseNPSUE(std::ifstream &) = 0;
		virtual void Parse() = 0;
		std::vector<double>& get_coords(){return m_COORDS;}

protected:
	// class attributes
		bool m_proceed = false;
		std::string m_path;
		int m_Ndim{0};
		int m_Ngrids{0};
		int m_Nelems{0};

		std::vector<double> m_COORDS; //m_COORDS = [X1,Y1,X2,Y2,X3,Y3...]
		std::vector<int> m_CONNEC;
		std::vector<int> m_NPSUE;
	};
}// namespace ees2d::IO