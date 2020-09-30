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


#ifndef haha
#define haha
#include <string>
#include <memory>


namespace ees2d {
    
    namespace Mesh {

        class AbstractMesh
        {
            public:
                AbstractMesh(const std::string path);
                virtual ~AbstractMesh();
                
                virtual void parseFileInfo()= 0;
                virtual void parseCOORDS()  = 0;
                virtual void parseCONNEC()  = 0;
                virtual void parseNPSUE()   = 0;
                void Parse();

                // Getters
                unsigned int GetNgrids();
                unsigned int GetNelems();
                std::unique_ptr<double[]> GetCOORDS();
                std::unique_ptr<unsigned int[]> GetCONNEC();
                std::unique_ptr<unsigned int[]> GetNPSUE() ;    

            protected:
                std::string m_path;
                unsigned int m_Ndim;
                unsigned int m_Ngrids;
                unsigned int m_Nelems;
                std::unique_ptr<double[]> m_COORDS;  // Careful : Works only on C++17 compatible compiler
                std::unique_ptr<unsigned int[]> m_CONNEC;
                std::unique_ptr<unsigned int[]> m_NPSUE;
        };


        class Su2Mesh : public AbstractMesh 
        {   
            public: 
                Su2Mesh(const std::string path);
                ~Su2Mesh();
                virtual void parseFileInfo();
                virtual void parseCOORDS()  ;
                virtual void parseCONNEC()  ;
                virtual void parseNPSUE()   ;

        };
    } // namespace Mesh
} // namespace ees2d
#endif