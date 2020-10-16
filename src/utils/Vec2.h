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

#include <cmath>
#include <iostream>

namespace ees2d::utils {
	template<class T>
	class Vec2 {
		// Class to deal with 2D vectors
		// Should be declared and defined in header file because its templated
public:
		T x, y;

		Vec2() : x(0), y(0) {}
		Vec2(T x, T y) : x(x), y(y) {}

		//Copy constructor
		Vec2(const Vec2 &v) : x(v.x), y(v.y) {}

		// Copy assignment operator
		Vec2 &operator=(const Vec2 &v) {
			x = v.x;
			y = v.y;
			return *this;
		}


		Vec2 operator+(Vec2 &v) {
			return Vec2(x + v.x, y + v.y);
		}

		Vec2 operator+(Vec2 &&v) {
			return Vec2(x + v.x, y + v.y);
		}


		Vec2 operator-(Vec2 &v) {
			return Vec2(x - v.x, y - v.y);
		}

		static float dot(Vec2 v1, Vec2 v2) {
			return v1.x * v2.x + v1.y * v2.y;
		}
		static float cross(Vec2 v1, Vec2 v2) {
			return (v1.x * v2.y) - (v1.y * v2.x);
		}

		Vec2 operator+(float s) {
			return Vec2(x + s, y + s);
		}
		Vec2 operator-(float s) {
			return Vec2(x - s, y - s);
		}
		Vec2 operator*(float s) {
			return Vec2(x * s, y * s);
		}
		Vec2 operator/(float s) {
			return Vec2(x / s, y / s);
		}

		void set(T x, T y) {
			this->x = x;
			this->y = y;
		}

		void rotate(float deg) {
			float theta = deg / 180.0 * M_PI;
			float c = cos(theta);
			float s = sin(theta);
			float tx = x * c - y * s;
			float ty = x * s + y * c;
			x = tx;
			y = ty;
		}


		float length() const {
			return std::sqrt(x * x + y * y);
		}
		friend std::ostream& operator<<(std::ostream &os,  Vec2<float> const& v);
	};

	std::ostream& operator<<(std::ostream &os,  Vec2<float> const& v) {
		os << "{ " << v.x << " , " << v.y << " }";
		return os;
	}
	// Print overloading


}// Namespace ees2d::utils