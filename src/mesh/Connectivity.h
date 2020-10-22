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

typedef std::shared_ptr<uint32_t[]> sharedUintPtrArray;
typedef std::vector<std::vector<uint32_t>> IntVector2D;

namespace ees2d::mesh {

	class Connectivity {

public:
		Connectivity(ees2d::io::Su2Parser &parser);

		const uint32_t &connecNodeSurrElement(const uint32_t &pointPos, const uint32_t &elementID) const ;// Return nodeID given an Element ID and its local node ID (from 0 to 2 for a 3 node element)
		void solve();                                                                              // Call all below methods
		void solveElemSurrNode();                                                                  // Method to populate m_esup2 and m_esup 1 arrays
		void solveNodeSurrNode();                                                                  // Populate m_psup1 and m_psup2 arrays
		void solveElemSurrElem();                                                                  // Populate m_elemToElem vector
		void solveNodeSurrFace();                                                                  // Populate m_faceToNode vector
		void solveFaceSurrElem();                                                                  // Populate m_elemToFace vector
		void solveElemSurrFace();                                                                  // Populate m_faceToElem vector
		void solveElemIDtoBC();                                                                    // Populate m_ElemToBC unordred map


		// getters for arrays and vectors
		inline const sharedUintPtrArray get_esup2() { return m_esup2; }
		inline const sharedUintPtrArray get_esup1() { return m_esup1; }
		inline const sharedUintPtrArray get_psup2() { return m_psup2; }
		inline const std::vector<uint32_t> *get_psup1() { return &m_psup1; }
		//inline const std::shared_ptr<std::unique_ptr<uint32_t[]>[]> get_elemToElem() {return m_elemToElem;}
		inline const std::vector<std::vector<uint32_t>> *get_elemToElem() const { return &m_elemToElem; }
		inline const std::vector<std::vector<uint32_t>> *get_FaceToNode() const { return &m_faceToNode; }
		inline const std::vector<std::vector<uint32_t>> *get_ElemToFace() const { return &m_elemToFace; }
		inline const std::vector<std::vector<uint32_t>> *get_FaceToElem() const { return &m_faceToElem; }
		inline ees2d::io::Su2Parser& get_parser() const {return m_parser;}

		//getters for values
		inline const uint32_t &get_esup2_size() { return m_esup2_size; }
		inline const uint32_t &get_esup1_size() { return m_esup1_size; }
		inline const uint32_t &get_psup2_size() { return m_psup2_size; }
		inline const uint32_t &get_esuel_size() { return m_esuel_size; }


private:
		ees2d::io::Su2Parser &m_parser;

		sharedUintPtrArray m_esup2 = nullptr;                                                         // Array containing Element position in m_esup1 (Linked list)
		sharedUintPtrArray m_esup1 = nullptr;                                                         // Linked list containing ElementIDs surrouding a specific node, used with m_esup2
		std::vector<uint32_t> m_psup1;                                                            // Linked list to find Node to Node connectivity (m_psup1 and m_psup2)
		sharedUintPtrArray m_psup2 = nullptr;
		uint32_t *m_lpoin = nullptr;                                                               // Temporary help array used to solve connecitivity
		IntVector2D m_elemToElem;                                                                  // 2D Uint32 vector contains Element IDs surrounding a specific element. Usage : m_elemToElem[ELEMID][LocalElEMID]
		IntVector2D m_faceToNode;                                                                  // 2D Uint32 vector contains NOde IDs surrounding a specific face. Usage : m_faceToNode[Face][LocalNODEID]
		IntVector2D m_elemToFace;                                                                  // 2D Uint32 vector contains Face IDs surrounding a specific Element. Usage : m_elemToFace[ELEMID][LocalFACEID]
		IntVector2D m_faceToElem;                                                                  // 2D Uint32 vector contains Elements IDs surrounding a specific face. Usage : m_faceToElem[FACE][localELEMID]
		uint32_t *m_inpoe1 = nullptr;                                                              // Temporary help array used to solve connecitivity

		//size is unknown for psup1, should be dynamically allocated


		uint32_t m_esup2_size{0};
		uint32_t m_esup1_size{0};
		uint32_t m_psup2_size{0};
		uint32_t m_lpoin_size{0};
		uint32_t m_esuel_size{0};
	};

}// namespace ees2d::mesh