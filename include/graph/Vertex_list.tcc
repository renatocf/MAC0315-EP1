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
#include <iostream>
#include <memory>

// Libraries
#include "Tags.tcc"
#include "Vertex.tcc"
#include "Exception.tcc"

namespace graph 
{
    template<
        typename Vertex = graph::Vertex<>, 
        typename Alloc  = std::allocator<Vertex>
    >class Vertex_list
    {
        private:
            using id_type       = typename Vertex::id_type;
            mutable std::unordered_map<id_type,Vertex> vl {};
            mutable id_type _max_id {};
        
        public:
            using vertex_type   = Vertex;
            using property_type = typename Vertex::property_type;
            
            // Iterators
            using iterator = typename
            std::unordered_map<id_type,Vertex>::iterator;
            using const_iterator = typename
            std::unordered_map<id_type,Vertex>::const_iterator;
            
            Vertex_list(std::initializer_list<Vertex> vertices)
            {
                std::cout << "List constructor" << std::endl;
                for(const Vertex& v : vertices)
                    if(vl.find(v.id) == vl.end())
                    {
                        vl.emplace(v.id,Vertex{v});
                        if(v.id > _max_id) _max_id = v.id;
                    }
                    else throw graph::id_repeated{};
            }
            
            Vertex_list(size_t n, const Vertex& prototype = Vertex{{}})
            {
                std::cout << "Range constructor" << std::endl;
                id_type id {};
                for(size_t m = 0; m < n; ++id, ++m)
                {
                    std::cout << "construct with id " << id << std::endl;
                    vl.emplace(id,Vertex{id,prototype});
                    if(id > _max_id) _max_id = id;
                }
            }
            
            unsigned long size()   const { return vl.size(); }
            const id_type max_id() const { return _max_id; }
            
            Vertex& 
            operator[](const id_type idx) { return vl.at(idx); }
            
            const Vertex&
            operator[](const id_type idx) const { return vl.at(idx); }
            
            friend std::ostream& 
            operator<<(std::ostream& os, const Vertex_list& list)
            {
                for(auto it = list.vl.begin(); it != list.vl.end(); ++it) 
                    os << it->second << endl;
                return os;
            }
            
            // Iterators
            iterator begin() { return vl.begin(); }
            iterator end()   { return vl.end();   }
            
            const_iterator begin() const { return vl.begin(); }
            const_iterator end()   const { return vl.end();   }
    };
}

#endif
