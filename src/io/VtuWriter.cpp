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

#include "io/VtuWriter.h"
#include <iostream>
#include <tuple>

using ees2d::io::VtuWriter;
using ees2d::mesh::Connectivity;
using ees2d::mesh::Mesh;
using ees2d::solver::Simulation;
using std::ofstream, std::cout;

VtuWriter::VtuWriter(std::string &vtuFileName, Connectivity &connectivity, Mesh &mesh, Simulation& sim)
    : m_vtuFileName(vtuFileName), m_connectivity(connectivity), m_mesh(mesh), m_sim(sim) {}


void VtuWriter::writeMesh() {


	ofstream fileStream(m_vtuFileName);
	beginFile(fileStream);
	writePoints(fileStream);
	writeCells(fileStream);
	endFile(fileStream);
	fileStream.close();

	cout << "Vtu mesh file generated (at " << m_vtuFileName << " )" << std::endl;
}

//----------------------------------------------------------------
void VtuWriter::writeSolution() {
	ofstream fileStream(m_vtuFileName);
	beginFile(fileStream);
	writePoints(fileStream);
	writeCells(fileStream);
	writeCellsData(fileStream);
	endFile(fileStream);
	fileStream.close();

	cout << "Vtu solution file generated (at " << m_vtuFileName << " )" << std::endl;
}

//----------------------------------------------------------------

void VtuWriter::beginFile(ofstream &fileStream) {

	fileStream << "<VTKFile type=\"UnstructuredGrid\">"
	           << "\n"
	           << "<UnstructuredGrid>"
	           << "\n"
	           << "<Piece NumberOfPoints=\"" << m_connectivity.get_parser().get_Ngrids() << "\" NumberOfCells=\" " << m_connectivity.get_parser().get_Nelems() << "\">"
	           << "\n";
}

//---------------------------------------------------------------
void VtuWriter::writePoints(ofstream &fileStream) {

	fileStream << "<Points>"
	           << "<DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">"
	           << "\n";
	uint32_t returnline = 0;
	for (auto &XY : m_connectivity.get_parser().get_coords()) {

		fileStream << std::get<0>(XY) << " " << std::get<1>(XY) << " "
		           << "0.0"
		           << "\t";
		returnline += 1;
		if (returnline % 3 == 0) {
			fileStream << "\n";
		}
	}
	fileStream << "</DataArray>"
	           << "\n"
	           << "</Points>"
	           << "\n";
}
//---------------------------------------------------------------
void VtuWriter::writeCells(ofstream &fileStream) {

	std::unordered_map<uint32_t, uint32_t> m_Vtk_Cell = {
	        {2, 3},
	        {3, 5},
	        {4, 9},
	};


	fileStream << "<Cells>"
	           << "\n"
	           << " <DataArray type=\"UInt32\" Name=\"connectivity\" format=\"ascii\">"
	           << "\n";
	uint32_t returnline = 0;


	for (auto &value : m_connectivity.get_parser().get_CONNEC()) {
		fileStream << value << " ";
		returnline += 1;
		if (returnline % 9 == 0) {
			fileStream << "\n";
		}
	}


	fileStream << "</DataArray>"
	           << "\n"
	           << "<DataArray type=\"UInt32\" Name=\"offsets\" format=\"ascii\">"
	           << "\n";

	returnline = 0;

	uint32_t offset = 0;
	for (auto &value : m_connectivity.get_parser().get_NPSUE()) {
		offset += value;
		fileStream << offset << " ";
		returnline += 1;
		if (returnline % 10 == 0) {
			fileStream << "\n";
		}
	}

	returnline = 0;

	fileStream << "</DataArray>"
	           << "\n"
	           << "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">"
	           << "\n";

	for (auto &value : m_connectivity.get_parser().get_NPSUE()) {
		fileStream << m_Vtk_Cell[value] << " ";
		returnline += 1;
		if (returnline % 10 == 0) {
			fileStream << "\n";
		}
	}

	fileStream << "</DataArray>"
	           << "\n"
	           << "</Cells>"
	           << "\n";
}
//---------------------------------------------------------------

void VtuWriter::writePointsData(ofstream &) {
}

//---------------------------------------------------------------
void VtuWriter::writeCellsData(ofstream &fileStream) {
	fileStream << "<CellData Scalars=\"Pression\" Vectors=\"velocity\" >"
	           << "\n"
	           << "<DataArray type=\"Float64\" Name=\"Pression\" format=\"ascii\" >"
	           << "\n";

	uint32_t returnline = 0;
	for (uint32_t i = 0; i < m_connectivity.get_parser().get_Nelems(); i++) {

		fileStream << m_sim.p[i] << " ";
		returnline += 1;
		if (returnline % 10 == 0) {
			fileStream << "\n";
		}
	}

	fileStream << "</DataArray>"
	           << "\n"
			   << "<DataArray type=\"Float64\" Name=\"Density\" format=\"ascii\" >"
	           << "\n";

	for(uint32_t i=0; i < m_sim.rho.size();i++){
		fileStream << (m_sim.rho[i])  << "\n";
	}

	fileStream << "</DataArray>"
	           << "\n"
			   << "<DataArray type=\"Float64\" Name=\"Mach\" format=\"ascii\" >"
	           << "\n";

	for(uint32_t i=0; i < m_sim.Mach.size();i++){
		fileStream << (m_sim.Mach[i])  << "\n";
	}

	 // Ecriture des vitesses
	fileStream << "</DataArray>"
	           << "\n"
			   << "<DataArray type=\"Float64\" Name=\"velocity\" format=\"ascii\" NumberOfComponents=\"3\" >"
            << "\n";
	for(uint32_t i=0; i < m_sim.u.size();i++){
		fileStream << (m_sim.u[i]) << " " << m_sim.v[i] << " " << "0.0" << "\n";
	}

  fileStream << "</DataArray>"
             << "\n"
             << "</CellData>"
             << "\n";

}

//---------------------------------------------------------------
void ees2d::io::VtuWriter::endFile(ofstream &fileStream) {
	fileStream << "</Piece>"
	           << "\n"
	           << "</UnstructuredGrid>"
	           << "\n"
	           << "</VTKFile>";
}