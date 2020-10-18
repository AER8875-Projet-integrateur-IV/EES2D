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
#include "mesh/Metrics.h"


namespace ees2d::mesh {

	struct Mesh {

		Mesh(Connectivity &connectivity, Metrics::MetricsData &metrics)
		    : m_connectivity(connectivity), m_metrics(metrics) {}


		inline const uint32_t &ElemToNode(const uint32_t &ElemId, const uint32_t &LocalNodeID) const {
			return m_connectivity.get_parser().get_CONNEC()[m_connectivity.get_parser().get_ElemIndex()[ElemId] + LocalNodeID];
		}
		//---------------------------------------------------
		inline const uint32_t &NodeToElem(const uint32_t &NodeId, const uint32_t &LocalElemId) const {
			return m_connectivity.get_esup1()[m_connectivity.get_esup2()[NodeId] + LocalElemId];
		}
		//---------------------------------------------------
		inline const uint32_t &NodeToNode(const uint32_t &NodeId, const uint32_t &LocalNodeId) const {
			return (*m_connectivity.get_psup1())[m_connectivity.get_psup2()[NodeId] + LocalNodeId];
		}
		//---------------------------------------------------
		inline const uint32_t &ElemToElem(const uint32_t &ElemId, const uint32_t &LocalElemId) const {
			return (*m_connectivity.get_elemToElem())[ElemId][LocalElemId];
		}
		//---------------------------------------------------
		inline const uint32_t &FaceToNode(const uint32_t &FaceId, const uint32_t &LocalNodeId) const {
			return (*m_connectivity.get_FaceToNode())[FaceId][LocalNodeId];
		}
		//---------------------------------------------------
		inline const uint32_t &ElemToFace(const uint32_t &ElemId, const uint32_t &LocalFaceId) const {
			return (*m_connectivity.get_ElemToFace())[ElemId][LocalFaceId];
		}
		//---------------------------------------------------
		inline const uint32_t &FaceToElem(const uint32_t &FaceId, const uint32_t &LocalElemId) const {
			return (*m_connectivity.get_FaceToElem())[FaceId][LocalElemId];
		}
		//---------------------------------------------------
		inline const float &FaceSurface(const uint32_t &FaceId) const {
			return m_metrics.facesSurface[FaceId];
		}
		//---------------------------------------------------
		inline const float &CvolumeArea(const uint32_t &ElemId) const {
			return m_metrics.CvolumesArea[ElemId];
		}
		//---------------------------------------------------
		inline const ees2d::utils::Vector2<float> &FaceMidPoint(const uint32_t &FaceId) const {
			return m_metrics.facesMidPoint[FaceId];
		}
		//---------------------------------------------------
		inline const ees2d::utils::Vector2<float> &FaceVector(const uint32_t &FaceId) const {
			return m_metrics.facesVector[FaceId];
		}
		//---------------------------------------------------
		inline const ees2d::utils::Vector2<float> &CvolumeCentroid(const uint32_t &ElemId) const {
			return m_metrics.CvolumesCentroid[ElemId];
		}

private:
		ees2d::mesh::Metrics::MetricsData &m_metrics;
		ees2d::mesh::Connectivity &m_connectivity;
	};

}// namespace ees2d::mesh
