/**********************************************************************/
/* Copyright 2014 RCF                                                 */
/*                                                                    */
/* Licensed under the Apache License, Version 2.0 (the "License");    */
/* you may not use this file except in compliance with the License.   */
/* You may obtain a copy of the License at                            */
/*                                                                    */
/*     http://www.apache.org/licenses/LICENSE-2.0                     */
/*                                                                    */
/* Unless required by applicable law or agreed to in writing,         */
/* software distributed under the License is distributed on an        */
/* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,       */
/* either express or implied.                                         */
/* See the License for the specific language governing permissions    */
/* and limitations under the License.                                 */
/**********************************************************************/

#ifndef TCC_GRAPH_VERTEXLIST_DEFINED
#define TCC_GRAPH_VERTEXLIST_DEFINED

// Default libraries
#include <vector>
#include <initializer_list>

// Libraries
#include "Vertex.tcc"
#include "Properties.tcc"

namespace graph 
{
    template<
        typename Properties = no_property
    >class VertexList
    {
        private:
            using Vertex = graph::Vertex<Properties>;
            mutable std::vector<Vertex> vl = {};
        
        public:
            VertexList(std::initializer_list<Properties> properties)
            {
                for(const Properties& p : properties)
                    vl.push_back({ vl.size()+1, p });
            }
            
            unsigned long size() { return vl.size(); }
            
            Vertex& 
            operator[](std::size_t idx) { return vl[idx]; }
            
            const Vertex& 
            operator[](std::size_t idx) const { return vl[idx]; }
    };
}

#endif
