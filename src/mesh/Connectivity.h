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

typedef std::shared_ptr<uint32_t[]> sharedPtrArray;


namespace ees2d::mesh {

	class Connectivity {

public:
		Connectivity(Su2Parser &parser);

		const uint32_t &connecPointSurrElement(const uint32_t &pointPos, const uint32_t &elementID);
		void solveElemSurrPoint();
		void solvePointSurrPoint();
		void solveElemSurrElem();
		void solve();

		// getters for arrays and vectors
		inline const sharedPtrArray get_esup2() { return m_esup2; }
		inline const sharedPtrArray get_esup1() { return m_esup1; }
		inline const sharedPtrArray get_psup2() { return m_psup2; }
		inline const std::vector<uint32_t> *get_psup1() { return &m_psup1; }
		//inline const std::shared_ptr<std::unique_ptr<uint32_t[]>[]> get_esuel() {return m_esuel;}
		inline const std::vector<std::vector<uint32_t>> *get_esuel() { return &m_esuel; }

		//getters for values
		inline const uint32_t &get_esup2_size() { return m_esup2_size; }
		inline const uint32_t &get_esup1_size() { return m_esup1_size; }
		inline const uint32_t &get_psup2_size() { return m_psup2_size; }
		inline const uint32_t &get_esuel_size() { return m_esuel_size; }


private:
		Su2Parser &m_parser;

		sharedPtrArray m_esup2 = nullptr;
		sharedPtrArray m_esup1 = nullptr;
		sharedPtrArray m_psup2 = nullptr;
		sharedPtrArray m_lpoin = nullptr;
		std::vector<std::vector<uint32_t>> m_esuel;
		std::vector<std::vector<uint32_t>> m_lpofa;

		//size is unknown for psup1, should be dynamically allocated
		std::vector<uint32_t> m_psup1;

		uint32_t m_esup2_size{0};
		uint32_t m_esup1_size{0};
		uint32_t m_psup2_size{0};
		uint32_t m_lpoin_size{0};
		uint32_t m_esuel_size{0};
	};


}// namespace ees2d::mesh