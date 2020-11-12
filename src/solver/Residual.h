/*
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
#include "solver/ConservativeVariables.h"
#include "solver/ConvectiveFlux.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>


namespace ees2d::solver {
	class Residual {

		// Residual Vector
public:
		Residual()
		    : m_rhoV_residual(0), m_rho_uV_residual(0), m_rho_vV_residual(0), m_rho_HV_residual(0){};
		Residual(double rhoV_residual, double rho_uV_residual, double rho_vV_residual, double rho_HV_residual)
		    : m_rhoV_residual(rhoV_residual), m_rho_uV_residual(rho_uV_residual), m_rho_vV_residual(rho_vV_residual), m_rho_HV_residual(rho_HV_residual){};


		inline Residual operator+(ConvectiveFlux &v) {
			return Residual(m_rhoV_residual + v.m_rhoV,
			                m_rho_uV_residual + v.m_rho_uV,
			                m_rho_vV_residual + v.m_rho_vV,
			                m_rho_HV_residual + v.m_rho_HV);
		}
		inline Residual operator-(ConvectiveFlux &v) {
			return Residual(m_rhoV_residual - v.m_rhoV,
			                m_rho_uV_residual - v.m_rho_uV,
			                m_rho_vV_residual - v.m_rho_vV,
			                m_rho_HV_residual - v.m_rho_HV);
		}
		inline Residual &operator+=(ConvectiveFlux &&v) {
			this->m_rhoV_residual += v.m_rhoV;
			this->m_rho_uV_residual += v.m_rho_uV;
			this->m_rho_vV_residual += v.m_rho_vV;
			this->m_rho_HV_residual += v.m_rho_HV;
			return *this;
		}

		inline Residual &operator+=(Residual v) {
			this->m_rhoV_residual += v.m_rhoV_residual;
			this->m_rho_uV_residual += v.m_rho_uV_residual;
			this->m_rho_vV_residual += v.m_rho_vV_residual;
			this->m_rho_HV_residual += v.m_rho_HV_residual;
			return *this;
		}


		inline Residual &operator-=(ConvectiveFlux &&v) {
			this->m_rhoV_residual -= v.m_rhoV;
			this->m_rho_uV_residual -= v.m_rho_uV;
			this->m_rho_vV_residual -= v.m_rho_vV;
			this->m_rho_HV_residual -= v.m_rho_HV;
			return *this;
		}

		inline Residual operator*(double &d) {
			return Residual(m_rhoV_residual * d,
			                m_rho_uV_residual * d,
			                m_rho_vV_residual * d,
			                m_rho_HV_residual * d);
		}

		inline void reset() {
			this->m_rhoV_residual = 0;
			this->m_rho_uV_residual = 0;
			this->m_rho_vV_residual = 0;
			this->m_rho_HV_residual = 0;
		}

		inline double findMax() {
			double member_variables[]{std::abs(m_rhoV_residual), std::abs(m_rho_uV_residual), std::abs(m_rho_vV_residual), std::abs(m_rho_HV_residual)};
			for (auto &res : member_variables) {
				if (std::isnan(res)) {
					std::cerr << "Error : nan residual found !" << std::endl;
					std::exit(EXIT_FAILURE);

				}
			}
			return *std::max_element(member_variables, member_variables + 4);
		}


		double m_rhoV_residual;
		double m_rho_uV_residual;
		double m_rho_vV_residual;
		double m_rho_HV_residual;
	};

}// namespace ees2d::solver