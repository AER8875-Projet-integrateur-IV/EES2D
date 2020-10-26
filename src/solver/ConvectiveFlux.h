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
#include <ostream>
namespace ees2d::solver {

	class ConvectiveFlux {

		// Convective flux vector
public:
    ConvectiveFlux(){};
		ConvectiveFlux(double rhoV, double rho_uV, double rho_vV, double rho_HV)
		    : m_rhoV(rhoV), m_rho_uV(rho_uV), m_rho_vV(rho_vV), m_rho_HV(rho_HV) {}


		inline ConvectiveFlux operator+(ConvectiveFlux &v) {
			return ConvectiveFlux(m_rhoV + v.m_rhoV,
			                      m_rho_uV + v.m_rho_uV,
			                      m_rho_vV + v.m_rho_vV,
			                      m_rho_HV + v.m_rho_HV);
		}
		inline ConvectiveFlux operator-(ConvectiveFlux &v) {
			return ConvectiveFlux(m_rhoV - v.m_rhoV,
			                      m_rho_uV - v.m_rho_uV,
			                      m_rho_vV - v.m_rho_vV,
			                      m_rho_HV - v.m_rho_HV);
		}

		inline ConvectiveFlux operator*(double s) {
			return ConvectiveFlux(m_rhoV * s, m_rho_uV * s, m_rho_vV * s, m_rho_HV * s);
		}


		double m_rhoV;
		double m_rho_uV;
		double m_rho_vV;
		double m_rho_HV;
	};


}// namespace ees2d::solver