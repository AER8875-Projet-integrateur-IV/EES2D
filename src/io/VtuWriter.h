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
#include <iostream>
#include "mesh/Mesh.h"

namespace ees2d::io {
class VtuWriter{
	public:
	VtuWriter(std::string& vtuFileName, ees2d::mesh::Connectivity&);
	~VtuWriter();
	void writeMesh();                                         // Writes only mesh without solution
	void writeSolution();                                     // Writes mesh and solution at every element/node

	ees2d::mesh::Connectivity& m_connectivity;
	std::string m_vtuFileName;
};

}