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

#include <iostream>
#include <utility>
#include <mesh/Mesh.h>
using ees2d::mesh::Mesh;

Mesh::Mesh(std::shared_ptr<Su2Parser> parser) : m_parser(std::move(parser)) {
	parser = nullptr;

std::cout << "Mesh initialized !" << std::endl;

}

int Mesh::connecPointElement(const uint32_t &pointPos, const uint32_t &elementID) {
	return m_parser->get_CONNEC()[m_parser->get_ElemIndex()[elementID] + pointPos];
}


