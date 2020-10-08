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
#include "AbstractParser.h"


namespace ees2d::IO {

	class Su2Parser : public AbstractParser {

	public:
		explicit Su2Parser(const std::string &path);
		~Su2Parser() override;

		// All methods defined in AbstractParser.h
		void parseDimensionInfo(std::ifstream &) override;
		void parseGridsInfo(std::ifstream &) override;
		void parseElementsInfo(std::ifstream &) override;
		void parseBoundaryConditionsInfo(std::ifstream &) override;
		void Parse() override;

	private:
		// Unordered_map to define VTK_cells with following structure -> {vtk_cell_id : number_of_points}
		std::unordered_map<uint8_t, uint8_t> m_Vtk_Cell = {
		        {3, 2},
		        {5, 3},
		        {9, 4},
		};

	};

}// namespace ees2d::IO