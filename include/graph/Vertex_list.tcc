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

#ifndef TCC_GRAPH_VERTEX_LIST_DEFINED
#define TCC_GRAPH_VERTEX_LIST_DEFINED

// Default libraries
#include <unordered_map>
#include <initializer_list>

// Libraries
#include "Vertex.tcc"
#include "Properties.tcc"

namespace graph 
{
    template<
        typename Vertex = graph::Vertex<>
    >class Vertex_list
    {
        private:
            mutable 
            std::unordered_map<typename Vertex::id_type,Vertex> vl;
        
        public:
            class id_repeated{}; // Exception
            
            using id_type       = typename Vertex::id_type;
            using property_type = typename Vertex::property_type;
            
            Vertex_list(std::initializer_list<Vertex> vertices)
            {
                for(const Vertex& v : vertices)
                    if(vl.find(v.id) == vl.end())
                        vl.insert({v.id, v});
                    else throw Vertex_list::id_repeated{};
            }
            
            Vertex_list(const id_type n, const Vertex& prototype = {})
            {
                for(id_type i = 0; i < n; i++)
                    vl.insert({i,prototype});
            }
            
            unsigned long size() { return vl.size(); }
            
            Vertex& 
            operator[](const id_type idx) { return vl[idx]; }
            
            const Vertex& 
            operator[](const id_type idx) const { return vl[idx]; }
            
            friend std::ostream& 
            operator<<(std::ostream& os, const Vertex_list& vl)
            {
                for(auto it = vl.vl.begin(); it != vl.vl.end(); ++it) 
                    os << it->second << endl;
                return os;
            }
    };
}

#endif
