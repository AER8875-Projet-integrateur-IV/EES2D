/* ---------------------------------------------------------------------
 *
 * Copyright (C) 2020 - by the EES2D authors
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

#include <fstream>
#include <memory>
#include <string>

namespace ees2d::Mesh {

	class AbstractMesh {
	public:
		explicit AbstractMesh(const std::string &path);
		virtual ~AbstractMesh();

		virtual void parseFileInfo(std::ifstream &) = 0;
		virtual void parseCOORDS(std::ifstream &) = 0;
		virtual void parseCONNEC(std::ifstream &) = 0;
		virtual void parseNPSUE(std::ifstream &) = 0;
		virtual void Parse() = 0;

	public:
		bool m_proceed = false;
		std::string m_path;
		int m_Ndim{0};
		int m_Ngrids{0};
		int m_Nelems{0};
		std::unique_ptr<double[]> m_COORDS{nullptr};// Careful : Works only on C++17 compatible compiler
		std::unique_ptr<int[]> m_CONNEC{nullptr};
		std::unique_ptr<int[]> m_NPSUE{nullptr};
	};

	class Su2Mesh : public AbstractMesh {
	public:
		explicit Su2Mesh(const std::string &path);
		~Su2Mesh() override;
		void parseFileInfo(std::ifstream &) override;
		void parseCOORDS(std::ifstream &) override;
		void parseCONNEC(std::ifstream &) override;
		void parseNPSUE(std::ifstream &) override;
		void Parse() override;
	};
}// namespace ees2d::Mesh