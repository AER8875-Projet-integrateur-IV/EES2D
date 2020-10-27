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
#include "solver/Residual.h"
namespace ees2d::solver {

	class ConservativeVariables {

		// Conservative variables vector
public:
		ConservativeVariables()
		    : m_rho(0), m_rho_u(0), m_rho_v(0), m_rho_E(0){};
		ConservativeVariables(double rho, double rho_u, double rho_v, double rho_E)
		    : m_rho(rho), m_rho_u(rho_u), m_rho_v(rho_v), m_rho_E(rho_E) {}


		inline ConservativeVariables &operator+(ConservativeVariables &v) {
			this->m_rho += v.m_rho;
			this->m_rho_u += v.m_rho_u;
			this->m_rho_v += v.m_rho_v;
			this->m_rho_E += v.m_rho_E;
			return *this;
		}

		inline ConservativeVariables &operator-(ConservativeVariables &v) {
			this->m_rho -= v.m_rho;
			this->m_rho_u -= v.m_rho_u;
			this->m_rho_v -= v.m_rho_v;
			this->m_rho_E -= v.m_rho_E;
			return *this;
		}

		inline ConservativeVariables &operator+=(Residual &&v) {
			this->m_rho += v.m_rhoV_residual;
			this->m_rho_u += v.m_rho_uV_residual;
			this->m_rho_v += v.m_rho_vV_residual;
			this->m_rho_E += v.m_rho_HV_residual;
			return *this;
		}

		double m_rho;
		double m_rho_u;
		double m_rho_v;
		double m_rho_E;
	};
}// namespace ees2d::solver