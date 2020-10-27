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
#include "solver/ConvectiveFlux.h"

namespace ees2d::solver {

	class Residual {

		// Convective flux vector
public:
		Residual(){};
		Residual(double rhoV_residual, double rho_uV_residual, double rho_vV_residual, double rho_HV_residual)
		    : m_rhoV_residual(rhoV_residual), m_rho_uV_residual(rho_uV_residual), m_rho_vV_residual(rho_vV_residual), m_rho_HV_residual(rho_HV_residual) {}


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
		inline Residual operator+=(ConvectiveFlux &&v) {
			return Residual(m_rhoV_residual + v.m_rhoV,
			                m_rho_uV_residual + v.m_rho_uV,
			                m_rho_vV_residual + v.m_rho_vV,
			                m_rho_HV_residual + v.m_rho_HV);
		}

		inline Residual operator-=(ConvectiveFlux &&v) {
			return Residual(m_rhoV_residual - v.m_rhoV,
			                m_rho_uV_residual - v.m_rho_uV,
			                m_rho_vV_residual - v.m_rho_vV,
			                m_rho_HV_residual - v.m_rho_HV);
		}

		inline Residual operator*(double s) {
			return Residual(m_rhoV_residual * s, m_rho_uV_residual * s, m_rho_vV_residual * s, m_rho_HV_residual * s);
		}

		inline void reset() {
			this->m_rhoV_residual = 0;
			this->m_rho_uV_residual = 0;
			this->m_rho_vV_residual = 0;
			this->m_rho_HV_residual = 0;
		}


		double m_rhoV_residual;
		double m_rho_uV_residual;
		double m_rho_vV_residual;
		double m_rho_HV_residual;
	};

}// namespace ees2d::solver