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

#ifndef TCC_GRAPH_ADJACENCY_LIST_DEFINED
#define TCC_GRAPH_ADJACENCY_LIST_DEFINED

// Default libraries
#include <list>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <iostream>

// Libraries
#include "Arc.tcc"
#include "Tags.tcc"
#include "Vertex.tcc"
#include "Arc_list.tcc"
#include "Exception.tcc"
#include "Vertex_list.tcc"

namespace graph 
{
    template<
        typename Vertex_list, typename Arc_list, typename directed
    >struct Adjacency_list_gen {};
    
    template<
        typename Vertex_list, typename Arc_list
    >struct Adjacency_list_gen<Vertex_list,Arc_list,directed>
    {
        using vertex_type     = typename Vertex_list::vertex_type;
        using vertex_property = typename vertex_type::property_type;
        using arc_type        = typename Arc_list::arc_type;
        using arc_property    = typename arc_type::property_type;
        using vertex_id       = typename vertex_type::id_type;
        using arc_pair        = std::pair<vertex_type,arc_property>;
        
        // Adjacency list
        std::vector<std::list<arc_pair>> adj_list {};
        
        Adjacency_list_gen(Vertex_list& vertices, Arc_list& arcs)
            : adj_list{vertices.size()}
        {
            for(arc_type& arc : arcs)
            {
                if(vertices[arc.beg.id] != arc.beg
                || vertices[arc.end.id] != arc.end)
                    throw graph::id_not_found{};
                
                this->adj_list[arc.beg.id].push_back(
                    { arc.end, arc.properties }
                );
            }
        }
    };
    
    template<
        typename Vertex_list, typename Arc_list
    >struct Adjacency_list_gen<Vertex_list,Arc_list,undirected>
    {
        using vertex_type    = typename Vertex_list::vertex_type;
        using arc_type       = typename Arc_list::arc_type;
        using vertex_id_type = typename vertex_type::id_type;
        
        // Adjacency list
        std::vector<std::list<arc_type>> adj_list {};
        
        Adjacency_list_gen(Vertex_list& vertices, Arc_list& arcs)
            : adj_list{vertices.size()}
        {
            for(arc_type& arc : arcs)
            {
                if(vertices[arc.beg.id] != arc.beg
                || vertices[arc.end.id] != arc.end)
                    throw graph::id_not_found{};
                
                this->adj_list[arc.beg.id].push_back(arc);
            }
        }
    };
    
    template<
        typename Vertex_list, typename Arc_list
    >struct Adjacency_list_gen<Vertex_list,Arc_list,graph::bidirectional>
    {
        using vertex_type     = typename Vertex_list::vertex_type;
        using vertex_property = typename vertex_type::property_type;
        using arc_type        = typename Arc_list::arc_type;
        using arc_property    = typename arc_type::property_type;
        using vertex_id       = typename vertex_type::id_type;
        using arc_pair        = std::pair<vertex_type,arc_property>;
        
        // Adjacency list
        std::vector<std::list<arc_pair>> adj_list {};
        
        Adjacency_list_gen(Vertex_list& vertices, Arc_list& arcs)
            : adj_list{vertices.size()}
        {
            for(arc_type& arc : arcs)
            {
                if(vertices[arc.beg.id] != arc.beg
                || vertices[arc.end.id] != arc.end)
                    throw graph::id_not_found{};
                
                this->adj_list[arc.beg.id].push_back(
                    { arc.end, arc.properties }
                );
                this->adj_list[arc.end.id].push_back(
                    { arc.beg, arc.properties }
                );
            }
        }
    };
    
    template<
        typename Directed    = graph::directed,
        typename Vertex_list = graph::Vertex_list<>,
        typename Arc_list    = graph::Arc_list<graph::Arc<
                                typename Vertex_list::vertex_type>>
    >class Adjacency_list
        : public Adjacency_list_gen<Vertex_list,Arc_list,Directed>
    {
        private:
            using Generator = 
            Adjacency_list_gen<Vertex_list,Arc_list,Directed>;
           
        public:
            using vertex_type = typename Generator::vertex_type;
            using arc_type    = typename Generator::arc_type;
        
            Adjacency_list(Vertex_list vertices = {}, Arc_list arcs = {})
                : Generator{vertices,arcs} {}
    };
}

#endif
