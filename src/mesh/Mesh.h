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
#include "io/Su2Parser.h"
#include <iostream>
using ees2d::io::Su2Parser;


namespace ees2d::mesh {

	class Mesh {

public:
		Mesh(std::shared_ptr<Su2Parser> parser);

		const uint32_t &connecPointSurrElement(const uint32_t &pointPos, const uint32_t &elementID);
		void solveElemSurrPoint();

		inline const std::shared_ptr<uint32_t[]> &get_esup2() { return m_esup2; }
		inline const std::shared_ptr<uint32_t[]> &get_esup1() { return m_esup1; }
		inline const uint32_t &get_esup2_size() { return m_esup2_size; }
		inline const uint32_t &get_esup1_size() { return m_esup1_size; }

private:
		std::shared_ptr<Su2Parser> m_parser;
		std::shared_ptr<uint32_t[]> m_esup2;
		std::shared_ptr<uint32_t[]> m_esup1;
		uint32_t m_esup2_size{0};
		uint32_t m_esup1_size{0};
	};


}// namespace ees2d::mesh