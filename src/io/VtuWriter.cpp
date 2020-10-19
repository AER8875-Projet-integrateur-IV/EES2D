#include "io/VtuWriter.h"
#include <fstream>
#include <iostream>
#include <tuple>
using ees2d::io::VtuWriter;
using ees2d::mesh::Connectivity;
using std::ofstream, std::cout;

VtuWriter::VtuWriter(std::string &vtuFileName, Connectivity &connectivity)
    : m_vtuFileName(vtuFileName), m_connectivity(connectivity) {}


void VtuWriter::writeMesh() {

	std::unordered_map<uint32_t, uint32_t> m_Vtk_Cell = {
	        {2, 3},
	        {3, 5},
	        {4, 9},
	};

	uint32_t returnline = 0;
	ofstream fileStream(m_vtuFileName);

	fileStream << "<VTKFile type=\"UnstructuredGrid\">"
	           << "\n"
	           << "<UnstructuredGrid>"
	           << "\n"
	           << "<Piece NumberOfPoints=\"" << m_connectivity.get_parser().get_Ngrids() << "\" NumberOfCells=\" " << m_connectivity.get_parser().get_Nelems() << "\">"
	           << "\n"
	           << "<Points>"
	           << "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">"
	           << "\n";
	for (auto &XY : m_connectivity.get_parser().get_coords()) {
		returnline += 1;
		fileStream << std::get<0>(XY) << " " << std::get<1>(XY) << " "
		           << "0.0"
		           << "\t";
		if (returnline % 3 == 0) {
			fileStream << "\n";
		}
	}
	returnline = 0;

	fileStream << "</DataArray>"
	           << "\n"
	           << "</Points>"
	           << "\n"
	           << "<Cells>"
	           << "\n"
	           << " <DataArray type=\"UInt32\" Name=\"connectivity\" format=\"ascii\">"
	           << "\n";

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
	fileStream.close();

	cout << "Vtu file generated (at " << m_vtuFileName << " )" << std::endl;
}
ees2d::io::VtuWriter::~VtuWriter() {
	ofstream fileStream;
	fileStream.open(m_vtuFileName, std::ios::app);
	fileStream << "</DataArray>"
	           << "\n"
	           << "</Cells>"
	           << "\n"
	           << "</Piece>"
	           << "\n"
	           << "</UnstructuredGrid>"
	           << "\n"
	           << "</VTKFile>";
}
