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
#include <tuple>
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

		// Getters
		inline std::vector<std::tuple<double, double>> &get_coords() { return m_COORDS; }
		inline std::vector<int> &get_ElemIndex() { return m_ElemIndex; }
		inline std::vector<int> &get_NPSUE() { return m_NPSUE; }
		inline std::vector<int> &get_CONNEC() { return m_CONNEC; }


	protected:
		// class attributes
		bool m_proceed = false;
		std::string m_path;
		int m_Ndim{0};
		int m_Ngrids{0};
		int m_Nelems{0};

		//m_COORDS --> Grid Coordinates = [{X1,Y1},{X2,Y2} ...]
		std::vector<std::tuple<double, double>> m_COORDS;

		//m_ElemIds -- > Element id in same order as SU2 file [Element1_ID,Element2_ID...]
		std::vector<int> m_ElemIds;

		//m_ElemIndex --> Element start index in m_CONNEC vector [ELM1_START INDEX, ELM2_START INDEX...]
		std::vector<int> m_ElemIndex;

		//m_NPSUE -- > Number of Points surrounding each element [NBPTS_ELEM1 NBPTS_ELEM2 ...]
		std::vector<int> m_NPSUE;

		//m_CONNEC --> Grid Ids connected to elements [G1,G2,G3,G1,G2,G3....]
		std::vector<int> m_CONNEC;
	};
}// namespace ees2d::IO