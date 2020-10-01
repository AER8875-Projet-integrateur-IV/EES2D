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

#include "Mesh.h"
#include <fstream>
#include <iostream>

using namespace ees2d::Mesh;

Su2Mesh::Su2Mesh(const std::string path)
        : AbstractMesh::AbstractMesh(path) { std::cout << "SU2 Mesh initialized !" << std::endl; }

Su2Mesh::~Su2Mesh() {
    std::cout << "SU2 Mesh Destroyed !" << std::endl;
}

void Su2Mesh::parseCOORDS() {
}

void Su2Mesh::parseCONNEC() {
}

void Su2Mesh::parseNPSUE() {
}

void Su2Mesh::parseFileInfo() {
    std::string line;

    std::ifstream fileIO(m_path);
    if (fileIO.is_open()) {
        while (std::getline(fileIO, line)) {
            std::cout << line << "\n"
                      << std::endl;
        }

        fileIO.close();
    } else
        std::cout << "Unable to open file !" << std::endl;
}
