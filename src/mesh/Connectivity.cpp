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

#include "mesh/Connectivity.h"
#include <iostream>
using ees2d::mesh::Connectivity;


Connectivity::Connectivity(Su2Parser &parser) : m_parser(parser) {

	std::cout << " ---------------- Constructing connectivity tables !"
	             " -----------------"
	          << std::endl;


	// Initializing element surrounding points arrays
	for (auto &val : m_parser.get_NPSUE())
		m_esup1_size += val;
	m_esup1 = std::make_unique<uint32_t[]>(m_esup1_size);

	for (uint32_t i = 0; i < m_esup1_size; i++) {
		m_esup1[i] = 0;
	}

	m_esup2_size = m_parser.get_Ngrids() + 1;
	m_esup2 = std::make_unique<uint32_t[]>(m_esup2_size);

	for (uint32_t i = 0; i < m_esup2_size; i++) {
		m_esup2[i] = 0;
	}

	// Initializing points surrounding points arrays
	m_psup2_size = m_parser.get_Ngrids() + 1;
	m_lpoin_size = m_psup2_size - 1;
	m_psup2 = std::make_unique<uint32_t[]>(m_psup2_size);
	m_lpoin = std::make_unique<uint32_t[]>(m_psup2_size - 1);
	m_psup1.reserve(m_lpoin_size);//Ngrids == m_lpoin_size


	// Initializing Elements surrounding elements array
	bool all_triangles = true;
	for (const auto &Nb_faces : m_parser.get_NPSUE()) {
		if (Nb_faces != 3) {
			all_triangles = false;
			break;
		}
	}
	m_esuel_size = m_parser.get_Nelems();
	m_esuel.reserve(m_esuel_size);
}


const uint32_t &Connectivity::connecPointSurrElement(const uint32_t &pointPos, const uint32_t &elementID) {
	return m_parser.get_CONNEC()[m_parser.get_ElemIndex()[elementID] + pointPos];
}


void Connectivity::solveElemSurrPoint() {

	const uint32_t &Ngrids = m_parser.get_Ngrids();
	const uint32_t &Nelems = m_parser.get_Nelems();

	uint32_t ipoi1 = 0;
	uint32_t ipoin = 0;
	uint32_t istor = 0;

	// Pass 1 : Count the number of elements connected to each point
	for (uint32_t ielem = 0; ielem < Nelems; ielem++) {
		for (uint32_t inode = 0; inode < m_parser.get_NPSUE()[ielem]; inode++) {
			ipoi1 = connecPointSurrElement(inode, ielem) + 1;
			m_esup2[ipoi1] = m_esup2[ipoi1] + 1;
		}
	}
	// Reshuffling pass 1
	for (uint32_t ipoin = 1; ipoin < Ngrids + 1; ipoin++) {
		m_esup2[ipoin] = m_esup2[ipoin] + m_esup2[ipoin - 1];
	}

	// Calculate sum of NPSUE


	//Pass 2 : Store the elements in m_esup1
	for (uint32_t ielem = 0; ielem < Nelems; ielem++) {
		for (uint32_t inode = 0; inode < m_parser.get_NPSUE()[ielem]; inode++) {
			ipoin = connecPointSurrElement(inode, ielem);
			istor = m_esup2[ipoin] + 1;
			m_esup2[ipoin] = istor;
			m_esup1[istor - 1] = ielem;
		}
	}

	// Reshuffling pass 2
	for (uint32_t ipoin = Ngrids; ipoin > 0; ipoin--) {
		m_esup2[ipoin] = m_esup2[ipoin - 1];
	}
	m_esup2[0] = 0;

	std::cout << "Element to point connectivity : Done\n";
}


void Connectivity::solvePointSurrPoint() {


	for (uint32_t i = 0; i < m_lpoin_size; i++) {
		m_lpoin[i] = 0;
	}

	m_psup2[0] = 0;
	uint32_t istor = 0;
	uint32_t ielem = 0;
	uint32_t jpoin = 0;

	for (uint32_t ipoin = 0; ipoin < m_parser.get_Ngrids(); ipoin++) {

		for (uint32_t iesup = m_esup2[ipoin]; iesup < m_esup2[ipoin + 1]; iesup++) {
			ielem = m_esup1[iesup];

			for (uint32_t inode = 0; inode < m_parser.get_NPSUE()[ielem]; inode++) {
				jpoin = connecPointSurrElement(inode, ielem);
				if ((jpoin != ipoin) && (m_lpoin[jpoin] != ipoin || ipoin == 0)) {
					istor += 1;
					m_psup1.push_back(jpoin);
					m_lpoin[jpoin] = ipoin;
				}
			}
		}
		m_psup2[ipoin + 1] = istor;
	}
}


void Connectivity::solveElemSurrElem() {

	uint32_t nnofa = 2;// Always two nodes per face (2D)
	std::array<uint32_t, 2> lhelp;
	uint32_t ipoin = 0;
	uint32_t jelem = 0;
	uint32_t nnofj = 2;
	uint32_t icoun = 0;
	uint32_t jpoin = 0;

	for (uint32_t i = 0; i < m_lpoin_size; i++) {
		m_lpoin[i] = 0;
	}

	for (uint32_t ielem = 0; ielem < m_parser.get_Nelems(); ielem++) {

		std::vector<uint32_t> temp_surr_elem = {};
		if (m_parser.get_NPSUE()[ielem] == 3) {
			m_lpofa = {{0, 1},
			           {1, 2},
			           {2, 0}};
		} else if (m_parser.get_NPSUE()[ielem] == 4) {
			m_lpofa = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};
		}

		for (uint32_t ifael = 0; ifael < m_parser.get_NPSUE()[ielem]; ifael++) {

			for (uint32_t nnofa = 0; nnofa < 2; nnofa++) {
				lhelp[nnofa] = connecPointSurrElement(m_lpofa[ifael][nnofa], ielem);
				m_lpoin[lhelp[nnofa]] = 1;
			}
			ipoin = lhelp[0];

			for (uint32_t istor = m_esup2[ipoin]; istor < m_esup2[ipoin + 1]; istor++) {
				jelem = m_esup1[istor];

				if (jelem != ielem) {
					for (uint32_t jfael = 0; jfael < m_parser.get_NPSUE()[ielem]; jfael++) {
						if (nnofj == nnofa) {
							icoun = 0;
							for (uint32_t jnofa = 0; jnofa < nnofa; jnofa++) {
								jpoin = connecPointSurrElement(m_lpofa[jfael][jnofa], jelem);
								icoun = icoun + m_lpoin[jpoin];
							}
							if (icoun == nnofa) {
								temp_surr_elem.push_back(jelem);
							}
						}
					}
				}
			}
			for (uint32_t nnofa = 0; nnofa < 2; nnofa++) {
				m_lpoin[lhelp[nnofa]] = 0;
			}
		}
		m_esuel.push_back(temp_surr_elem);
	}
}


void ees2d::mesh::Connectivity::solve() {
	solveElemSurrPoint();
	solvePointSurrPoint();
	solveElemSurrElem();
}
