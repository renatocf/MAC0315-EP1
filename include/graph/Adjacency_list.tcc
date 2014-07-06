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
#include <tuple>
#include <memory>
#include <vector>
#include <algorithm>
#include <initializer_list>

// Libraries
#include "Arc.tcc"
#include "Tags.tcc"
#include "Vertex.tcc"
#include "Exception.tcc"

namespace graph 
{
    template<
        typename directed, typename Vertex, typename Arc, 
        template<typename T,typename Alloc> class Vertex_list,
        template<typename T,typename Alloc> class Arc_list,
        typename AllocV, typename AllocA
    >class Adjacency_list_gen {};
    
    template<
        typename Vertex, typename Arc,
        template<typename T,typename Alloc> class Vertex_list,
        template<typename T,typename Alloc> class Arc_list,
        typename AllocV, typename AllocA
    >class Adjacency_list_gen
        <directed,Vertex,Arc,Vertex_list,Arc_list,AllocV,AllocA>
    {
        public:
            using vertex_type     = Vertex;
            using vertex_property = typename vertex_type::property_type;
            using vertex_id       = typename vertex_type::id_type;
            using VertexList      = Vertex_list<vertex_type*,AllocV>;
            
            using arc_type        = Arc;
            using arc_property    = typename arc_type::property_type;
            using ArcList         = Arc_list<arc_type*,AllocA>;
            
        private:
            Vertex_list<vertex_type,AllocV>  vertices; // Vertices
            Arc_list<arc_type,AllocA>        arcs;     // Arcs
            
            Vertex_list< // Adjacency list
                std::tuple<VertexList,ArcList>,AllocV
            > adj_list {};
        public:
            
            Adjacency_list_gen(
                Vertex_list<Vertex,AllocV>& vertices, 
                Arc_list<Arc,AllocA>& arcs)
                : vertices{vertices}, adj_list{vertices.size()}
            {
                for(arc_type& arc : arcs)
                {
                    if(vertices[arc.beg().id] != arc.beg()
                    || vertices[arc.end().id] != arc.end())
                        throw graph::id_not_found{};
                    
                    // Create a copy of the arc with internal references
                    this->arcs.emplace_back(
                        this->vertices[arc.beg().id],
                        this->vertices[arc.end().id],
                        arc.properties
                    );
                    
                    // Put a pointer to the arc end vertex
                    std::get<0>(this->adj_list[arc.beg().id]).push_back(
                        &(this->vertices[arc.end().id]));
                    
                    // Put a pointer to the arc
                    std::get<1>(this->adj_list[arc.beg().id]).push_back(
                        &(this->arcs.back()));
                }
            }
            
            using out_arcs_iterator = typename VertexList::iterator;
            
            VertexList& out_arcs_list(const Vertex& v)       
            { return this->adj_list[v.id].first; }
             
            const VertexList& out_arcs_list(const Vertex& v) const 
            { return this->adj_list[v.id].first; }
            
            std::pair<out_arcs_iterator,out_arcs_iterator>
            out_arcs(const Vertex& v) 
            {
                return { std::begin (std::get<0>(this->adj_list[v.id])) ,
                         std::end   (std::get<0>(this->adj_list[v.id])) };
            }
    };
    
    template<
        typename Vertex, typename Arc,
        template<typename T,typename Alloc> class Vertex_list,
        template<typename T,typename Alloc> class Arc_list,
        typename AllocV, typename AllocA
    >class Adjacency_list_gen
        <undirected,Vertex,Arc,Vertex_list,Arc_list,AllocV,AllocA>
    {
        public:
            using vertex_type     = Vertex;
            using vertex_property = typename vertex_type::property_type;
            using vertex_id       = typename vertex_type::id_type;
            using VertexList      = Vertex_list<vertex_type*,AllocV>;
            
            using arc_type        = Arc;
            using arc_property    = typename arc_type::property_type;
            using ArcList         = Arc_list<arc_type*,AllocA>;
            
        private:
            Vertex_list<vertex_type,AllocV>  vertices; // Vertices
            Arc_list<arc_type,AllocA>        arcs;     // Arcs
            
            Vertex_list< // Adjacency list
                std::tuple<VertexList,ArcList>,AllocV
            > adj_list {};
        public:
            
            Adjacency_list_gen(
                Vertex_list<Vertex,AllocV>& vertices, 
                Arc_list<Arc,AllocA>& arcs)
                : vertices{vertices}, adj_list{vertices.size()}
            {
                for(arc_type& arc : arcs)
                {
                    if(vertices[arc.beg().id] != arc.beg()
                    || vertices[arc.end().id] != arc.end())
                        throw graph::id_not_found{};
                    
                    // Create a copy of the arc with internal references
                    this->arcs.emplace_back(
                        this->vertices[arc.beg().id],
                        this->vertices[arc.end().id],
                        arc.properties
                    );
                    
                    // Put a pointer to the arc end vertex
                    std::get<0>(this->adj_list[arc.beg().id]).push_back(
                        &(this->vertices[arc.end().id]));
                    
                    // Put a pointer to the arc
                    std::get<1>(this->adj_list[arc.beg().id]).push_back(
                        &(this->arcs.back()));
                }
            }
            
            using out_arcs_iterator = typename VertexList::iterator;
            using in_arcs_iterator  = typename VertexList::iterator;
            
            VertexList& out_arcs_list(const Vertex& v)       
            { return this->adj_list[v.id].first; }
             
            const VertexList& out_arcs_list(const Vertex& v) const 
            { return this->adj_list[v.id].first; }
            
            std::pair<out_arcs_iterator,out_arcs_iterator>
            out_arcs(const Vertex& v) 
            {
                return { std::begin (std::get<0>(this->adj_list[v.id])) ,
                         std::end   (std::get<0>(this->adj_list[v.id])) };
            }
            
            VertexList& in_arcs_list(const Vertex& v)       
            { return this->adj_list[v.id].first; }
             
            const VertexList& in_arcs_list(const Vertex& v) const 
            { return this->adj_list[v.id].first; }
            
            std::pair<in_arcs_iterator,in_arcs_iterator>
            in_arcs(const Vertex& v) 
            {
                return { std::begin (std::get<0>(this->adj_list[v.id])) ,
                         std::end   (std::get<0>(this->adj_list[v.id])) };
            }
    };
    
    template<
        typename Vertex, typename Arc,
        template<typename T,typename Alloc> class Vertex_list,
        template<typename T,typename Alloc> class Arc_list,
        typename AllocV, typename AllocA
    >class Adjacency_list_gen
        <bidirectional,Vertex,Arc,Vertex_list,Arc_list,AllocV,AllocA>
    {
        public:
            using vertex_type     = Vertex;
            using vertex_property = typename vertex_type::property_type;
            using vertex_id       = typename vertex_type::id_type;
            using VertexList      = Vertex_list<vertex_type*,AllocV>;
            
            using arc_type        = Arc;
            using arc_property    = typename arc_type::property_type;
            using ArcList         = Arc_list<arc_type*,AllocA>;
            
        private:
            Vertex_list<vertex_type,AllocV>  vertices; // Vertices
            Arc_list<arc_type,AllocA>        arcs;     // Arcs
            
            Vertex_list< // Adjacency list
                std::tuple<VertexList,VertexList,ArcList>,AllocV
            > adj_list {};
        public:
            
            Adjacency_list_gen(
                Vertex_list<Vertex,AllocV>& vertices, 
                Arc_list<Arc,AllocA>& arcs)
                : vertices{vertices}, adj_list{vertices.size()}
            {
                for(arc_type& arc : arcs)
                {
                    if(vertices[arc.beg().id] != arc.beg()
                    || vertices[arc.end().id] != arc.end())
                        throw graph::id_not_found{};
                    
                    // Create a copy of the arc with internal references
                    this->arcs.emplace_back(
                        this->vertices[arc.beg().id],
                        this->vertices[arc.end().id],
                        arc.properties
                    );
                    
                    // Put a pointer to the arc end vertex
                    std::get<0>(this->adj_list[arc.beg().id]).push_back(
                        &(this->vertices[arc.end().id]));
                    
                    // Put a pointer to the arc begining vertex
                    std::get<1>(this->adj_list[arc.end().id]).push_back(
                        &(this->vertices[arc.beg().id]));
                    
                    // Put a pointer to the arc
                    this->adj_list[arc.beg().id].second.push_back(
                        &(this->arcs.back()));
                }
            }
            
            using out_arcs_iterator = typename VertexList::iterator;
            using in_arcs_iterator  = typename VertexList::iterator;
            
            VertexList& out_arcs_list(const Vertex& v)       
            { return std::get<0>(this->adj_list[v.id]); }
             
            const VertexList& out_arcs_list(const Vertex& v) const 
            { return std::get<0>(this->adj_list[v.id]); }
            
            std::pair<out_arcs_iterator,out_arcs_iterator>
            out_arcs(const Vertex& v) 
            {
                return { std::begin (std::get<0>(this->adj_list[v.id])) ,
                         std::end   (std::get<0>(this->adj_list[v.id])) };
            }
            
            VertexList& in_arcs_list(const Vertex& v)       
            { return std::get<1>(this->adj_list[v.id]); }
             
            const VertexList& in_arcs_list(const Vertex& v) const 
            { return std::get<1>(this->adj_list[v.id]); }
            
            std::pair<in_arcs_iterator,in_arcs_iterator>
            in_arcs(const Vertex& v)
            {
                return { std::begin (std::get<1>(this->adj_list[v.id])) ,
                         std::end   (std::get<1>(this->adj_list[v.id])) };
            }
    };
    
    template<
        typename Directed = graph::directed,
        typename Vertex   = graph::Vertex<>,
        typename Arc      = graph::Arc<Vertex>,
        template<class T,class Alloc> class Vertex_list = std::vector,
        template<class T,class Alloc> class Arc_list    = std::vector,
        typename AllocV   = std::allocator<Vertex>,
        typename AllocA   = std::allocator<Arc>
    >class Adjacency_list
        : public Adjacency_list_gen
            <Directed,Vertex,Arc,Vertex_list,Arc_list,AllocV,AllocA>
    {
        private:
            using Generator = Adjacency_list_gen
                <Directed,Vertex,Arc,Vertex_list,Arc_list,AllocV,AllocA>;
           
        public:
            using vertex_type      = Vertex;
            using vertex_allocator = AllocV;
            
            using arc_type         = Arc;
            using arc_allocator    = AllocA;
            
            using arc_list    = Arc_list<arc_type,arc_allocator>;
            using vertex_list = Vertex_list<vertex_type,vertex_allocator>;
             
            Adjacency_list(vertex_list vertices = {}, arc_list arcs = {})
                : Generator{vertices,arcs} {}
            
            using out_arcs_iterator =
            typename Generator::out_arcs_iterator;
            
            inline std::pair<out_arcs_iterator,out_arcs_iterator>
            out_arcs(const Vertex& v) 
            { return Generator::out_arcs(v); }
    };
}

#endif
