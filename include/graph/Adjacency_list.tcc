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
#include <type_traits>
#include <initializer_list>

// Libraries
#include "Arc.tcc"
#include "Tags.tcc"
#include "Vertex.tcc"
#include "Exception.tcc"

namespace graph 
{
    template<typename Selector,typename Value>
    struct container_gen {};
    
    struct vecS {};
    
    template<typename Value>
    struct container_gen<vecS,Value> {
        typedef std::vector<Value> type;
    };
    
    template<typename adj_list>struct Adjacency_list_traits;
    
    template<
        typename Adjacency_list, typename directed, typename Vertex, 
        typename Arc, typename Vertex_list, typename Arc_list
    >class Adjacency_list_gen {};
    
    template<
        typename Adjacency_list, typename Vertex, typename Arc,
        typename Vertex_list, typename Arc_list
    >class Adjacency_list_gen
        <Adjacency_list,directed,Vertex,Arc,Vertex_list,Arc_list>
    {
        private:
            Adjacency_list *self;
            
            typedef Adjacency_list_traits<Adjacency_list> adj_traits;
            typedef typename adj_traits::vertex_id       vertex_id;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_id_list  vertex_id_list;
            typedef typename adj_traits::vertex_property vertex_property;
            
            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_id_list     arc_id_list;
            typedef typename adj_traits::arc_property    arc_property;
        
        protected:
            Adjacency_list_gen() 
                : self(static_cast<Adjacency_list*>(this)) {}
            
            // List iterators
            typedef typename arc_id_list::iterator       arc_iterator;
            typedef typename vertex_id_list::iterator    out_iterator;
            typedef void                                 in_iterator;
            typedef std::pair<arc_iterator,arc_iterator> arc_iterator_pair;
            typedef std::pair<out_iterator,out_iterator> out_iterator_pair;
            typedef void                                 in_iterator_pair;
            
            typedef std::tuple<vertex_type*,arc_id_list,vertex_id_list>
                    vertex_map;
            
            typedef typename container_gen<
                Vertex_list,vertex_map
            >::type type;
            
        public:
            void add_arc(const arc_type& arc)
            {
                std::cout << "Inserting " << arc << std::endl;
                
                // Throws if any vertex does not exist
                vertex(arc.beg,*self); vertex(arc.end,*self);
                
                // Create a copy of the arc with internal references
                all_arcs(*self).push_back(arc);
                
                // Put a pointer in the list of out neighbors
                out_arcs_list(arc.beg,*self).push_back(arc.end);
                
                // Increase beg's list of arcs
                arcs_list(arc.beg,*self).push_back(
                                            all_arcs(*self).back().id);
            }
            
            void remove_arc(vertex_id beg, vertex_id end,
                            arc_property prop, bool ignore_prop)
            {
                // Throws if any vertex does not exist
                vertex(beg,*self); vertex(end,*self);
                
                bool has_arc = false;
                
                // Remove from beg's list of arcs
                arc_iterator arc_beg, arc_end;
                std::tie(arc_beg, arc_end) = arcs(beg,*self);
                for(auto it = arc_beg; it != arc_end; ++it)
                    if(arc(*it,*self).beg == beg
                    && arc(*it,*self).end == end
                    && (ignore_prop || arc(*it,*self).properties == prop))
                    {
                        arcs_list(beg,*self).erase(it,it+1);
                        has_arc = true;
                    }
                
                // Does not have the arc, nothing to do
                if(!has_arc) return;
                
                // Remove ids of neighbors
                out_iterator out_beg, out_end;
                std::tie(out_beg, out_end) = out_arcs(beg,*self);
                for(auto it = out_beg; it != out_end; ++it)
                    if(*it == end)
                        out_arcs_list(beg,*self).erase(it,it+1);
                
                // Remove the arc stored in the graph
                typename arc_list::iterator 
                all_arcs_beg = std::begin (all_arcs(*self)),
                all_arcs_end = std::end   (all_arcs(*self));
                for(auto it = all_arcs_beg; it != all_arcs_end; ++it)
                    if(it->beg == beg && it->end == end
                    && (ignore_prop || it->properties == prop))
                        all_arcs(*self).erase(it,it+1);
            }
    };
    
    template<
        typename Adjacency_list, typename Vertex, typename Arc,
        typename Vertex_list, typename Arc_list
    >class Adjacency_list_gen
        <Adjacency_list,undirected,Vertex,Arc,Vertex_list,Arc_list>
    {
        private:
            Adjacency_list *self;
            
            typedef Adjacency_list_traits<Adjacency_list> adj_traits;
            typedef typename adj_traits::vertex_id       vertex_id;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_id_list  vertex_id_list;
            typedef typename adj_traits::vertex_property vertex_property;
            
            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_id_list     arc_id_list;
            typedef typename adj_traits::arc_property    arc_property;
             
        protected:
            Adjacency_list_gen() 
                : self{static_cast<Adjacency_list*>(this)} {}
            
            // List iterators
            typedef typename arc_id_list::iterator       arc_iterator;
            typedef typename vertex_id_list::iterator    out_iterator;
            typedef out_iterator                         in_iterator;
            typedef std::pair<arc_iterator,arc_iterator> arc_iterator_pair;
            typedef std::pair<out_iterator,out_iterator> out_iterator_pair;
            typedef out_iterator_pair                    in_iterator_pair;
            
            typedef std::tuple<vertex_type*,arc_id_list,vertex_id_list>
                    vertex_map;
            
            typedef typename container_gen<
                Vertex_list,vertex_map
            >::type type;
            
            void add_arc(const Arc& arc)
            {
                // Throws if any vertex does not exist
                vertex(arc.beg,*self); vertex(arc.end,*self);
                
                // Create a copy of the arc with internal references
                all_arcs(*self).push_back(arc);
                
                // Put a pointer in the list of out neighbors
                out_arcs_list(arc.beg,*self).push_back(arc.end);
                
                // Increase beg's and end's list of arcs
                arcs_list(arc.beg,*self).push_back(
                                            all_arcs(*self).back().id);
                arcs_list(arc.end,*self).push_back(
                                            all_arcs(*self).back().id);
            }
            
            void remove_arc(vertex_id beg, vertex_id end,
                            arc_property prop, bool ignore_prop)
            {
                // Throws if any vertex does not exist
                vertex(beg,*self); vertex(end,*self);
                
                bool has_arc = false;
                
                // Remove from beg's and end's list of arcs
                arc_iterator arc_beg, arc_end;
                std::tie(arc_beg, arc_end) = arcs(beg,*self);
                for(auto it = arc_beg; it != arc_end; ++it)
                    if(arc(*it,*self).beg == beg
                    && arc(*it,*self).end == end
                    && (ignore_prop || arc(*it,*self).properties == prop))
                    {
                        arcs_list(beg,*self).erase(it,it+1);
                        has_arc = true;
                    }
                
                std::tie(arc_beg, arc_end) = arcs(end,*self);
                for(auto it = arc_beg; it != arc_end; ++it)
                    if(arc(*it,*self).beg == beg
                    && arc(*it,*self).end == end
                    && (ignore_prop || arc(*it,*self).properties == prop))
                    {
                        arcs_list(end,*self).erase(it,it+1);
                        has_arc = true;
                    }
                
                // Does not have the arc, nothing to do
                if(!has_arc) return;
                
                // Remove ids of neighbors
                out_iterator out_beg, out_end;
                std::tie(out_beg, out_end) = out_arcs(beg,*self);
                for(auto it = out_beg; it != out_end; ++it)
                    if(*it == end)
                        out_arcs_list(beg,*self).erase(it,it+1);
                
                // Remove the arc stored in the graph
                typename arc_list::iterator 
                all_arcs_beg = std::begin (all_arcs(*self)),
                all_arcs_end = std::end   (all_arcs(*self));
                for(auto it = all_arcs_beg; it != all_arcs_end; ++it)
                    if(it->beg == beg && it->end == end
                    && (ignore_prop || it->properties == prop))
                        all_arcs(*self).erase(it,it+1);
            }
    };
    
    template<
        typename Adjacency_list, typename Vertex, typename Arc,
        typename Vertex_list, typename Arc_list
    >class Adjacency_list_gen
        <Adjacency_list,bidirectional,Vertex,Arc,Vertex_list,Arc_list>
    {
        private:
            Adjacency_list *self;
            
            typedef Adjacency_list_traits<Adjacency_list> adj_traits;
            typedef typename adj_traits::vertex_id       vertex_id;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_id_list  vertex_id_list;
            typedef typename adj_traits::vertex_property vertex_property;
            
            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_id_list     arc_id_list;
            typedef typename adj_traits::arc_property    arc_property;
        
        protected:
            Adjacency_list_gen() 
                : self{static_cast<Adjacency_list*>(this)} {}
            
            // List iterators
            typedef typename arc_id_list::iterator       arc_iterator;
            typedef typename vertex_id_list::iterator    out_iterator;
            typedef typename vertex_id_list::iterator    in_iterator;
            typedef std::pair<arc_iterator,arc_iterator> arc_iterator_pair;
            typedef std::pair<out_iterator,out_iterator> out_iterator_pair;
            typedef std::pair<in_iterator,in_iterator>   in_iterator_pair;
            
            // Adjacency list type
            typedef std::tuple<vertex_type*,arc_id_list,
                    vertex_id_list,vertex_id_list> vertex_map;
            
            typedef typename 
            container_gen<Vertex_list,vertex_map>::type type;
        
        public:
            void add_arc(const arc_type& arc)
            {
                // Throws if any vertex does not exist
                vertex(arc.beg,*self); vertex(arc.end,*self);
                
                // Create a copy of the arc with internal references
                all_arcs(*self).push_back(arc);
                
                // Put a pointer in the list of out neighbors
                out_arcs_list(arc.beg,*self).push_back(arc.end);
                in_arcs_list (arc.end,*self).push_back(arc.beg);
                
                // Increase beg's and end's list of arcs
                arcs_list(arc.beg,*self).push_back(
                                            all_arcs(*self).back().id);
                arcs_list(arc.end,*self).push_back(
                                            all_arcs(*self).back().id);
            }
            
            void remove_arc(vertex_id beg, vertex_id end,
                            arc_property prop, bool ignore_prop)
            {
                // Throws if any vertex does not exist
                vertex(beg,*self); vertex(end,*self);
                
                bool has_arc = false;
                
                // Remove from beg's and end's list of arcs
                arc_iterator arc_beg, arc_end;
                std::tie(arc_beg, arc_end) = arcs(beg,*self);
                for(auto it = arc_beg; it != arc_end; ++it)
                    if(arc(*it,*self).beg == beg
                    && arc(*it,*self).end == end
                    && (ignore_prop || arc(*it,*self).properties == prop))
                    {
                        arcs_list(beg,*self).erase(it,it+1);
                        has_arc = true;
                    }
                
                std::tie(arc_beg, arc_end) = arcs(end,*self);
                for(auto it = arc_beg; it != arc_end; ++it)
                    if(arc(*it,*self).beg == beg
                    && arc(*it,*self).end == end
                    && (ignore_prop || arc(*it,*self).properties == prop))
                    {
                        arcs_list(end,*self).erase(it,it+1);
                        has_arc = true;
                    }
                
                // Does not have the arc, nothing to do
                if(!has_arc) return;
                
                // Remove ids of neighbors
                out_iterator out_beg, out_end;
                std::tie(out_beg, out_end) = out_arcs(beg,*self);
                for(auto it = out_beg; it != out_end; ++it)
                    if(*it == end)
                        out_arcs_list(beg,*self).erase(it,it+1);
                
                in_iterator in_beg, in_end;
                std::tie(in_beg, in_end) = in_arcs(end,*self);
                for(auto it = in_beg; it != in_end; ++it)
                    if(*it == beg)
                        in_arcs_list(end,*self).erase(it,it+1);
                
                // Remove the arc stored in the graph
                typename arc_list::iterator 
                all_arcs_beg = std::begin (all_arcs(*self)),
                all_arcs_end = std::end   (all_arcs(*self));
                for(auto it = all_arcs_beg; it != all_arcs_end; ++it)
                    if(it->beg == beg && it->end == end
                    && (ignore_prop || it->properties == prop))
                        all_arcs(*self).erase(it,it+1);
            }
    };
    
    template<
        typename Directed    = graph::directed,
        typename Vertex      = graph::Vertex<>,
        typename Arc         = graph::Arc<Vertex>,
        typename Vertex_list = vecS,
        typename Arc_list    = vecS
    >class Adjacency_list
        : public Adjacency_list_gen<
            Adjacency_list<Directed,Vertex,Arc,Vertex_list,Arc_list>,
                Directed,Vertex,Arc,Vertex_list,Arc_list>
    {
        private: 
            typedef Adjacency_list self;
            typedef Adjacency_list_gen
            <self,Directed,Vertex,Arc,Vertex_list,Arc_list> Gen;
            typedef typename Gen::type       Adj_list;
            typedef typename Gen::vertex_map vertex_map;
            // friend class Gen;
            
        public:
            typedef Adjacency_list_traits<self> adj_traits;
            typedef typename adj_traits::vertex_id       vertex_id;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_property vertex_property;
            typedef typename adj_traits::vertex_id_list vertex_id_list;
            
            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_property    arc_property;
            typedef typename adj_traits::arc_id_list    arc_id_list;
            
            typedef typename Gen::in_iterator       in_iterator;
            typedef typename Gen::in_iterator_pair  in_iterator_pair;
            typedef typename Gen::out_iterator      out_iterator;
            typedef typename Gen::out_iterator_pair out_iterator_pair;
            typedef typename Gen::arc_iterator      arc_iterator;
            typedef typename Gen::arc_iterator_pair arc_iterator_pair;
        
        private:
            vertex_list vertices; arc_list arcs;
            Adj_list adj_list;
            
        public:
            Adjacency_list(vertex_list vertices = vertex_list{},
                           arc_list    arcs     = arc_list{})
                : vertices{vertices.size(), vertex_type{0}}, 
                  adj_list{vertices.size()}
            {
                for(vertex_type& v : vertices) this->add_vertex(v);
                for(arc_type& a : arcs)        this->add_arc(a);
            }
            
            inline vertex_map& operator[](vertex_id vid) 
            { return this->adj_list[vid]; }
            
            inline const vertex_map& operator[](vertex_id vid) const 
            { return this->adj_list[vid]; }
            
            void add_vertex(const vertex_type& v)
            {
                this->vertices[v.id] = v;
                std::get<0>(this->adj_list[v.id]) = &(this->vertices[v.id]);
            }
            
            void remove_vertex(vertex_id vid, vertex_property prop, 
                               bool ignore_prop)
            {
                vertex(vid,*this); // To check if id exists
                if(ignore_prop || vertex(vid,*this).properties == prop)
                {
                    while(arcs_list(vid,*this).size() != 0)
                    {
                        for(auto& a : all_arcs())
                            std::cout << ">> " << a << std::endl;
                        std::cout << std::endl;
                        
                        arc_type *back =
                        &(arc(arcs_list(vid,*this).back(),*this));
                        
                        this->remove_arc(
                            back->beg,back->end,back->properties,false
                        );
                    }
                    std::get<0>(this->adj_list[vid]) = nullptr;
                    for(auto it = std::begin(this->vertices);
                        it != std::end(this->vertices); ++it)
                        if(it->id == vid)
                            this->vertices.erase(it,it+1);
                }
            }
            
            inline vertex_list& 
            all_vertices() { return this->vertices; }
            
            inline const vertex_list& 
            all_vertices() const { return this->vertices; }
            
            inline arc_list& 
            all_arcs() { return this->arcs; }
            
            inline const arc_list& 
            all_arcs() const { return this->arcs; }
            
            size_t num_vertices () { return vertices.size(); }
            size_t num_arcs     () { return arcs.size();     }
    };
    
    template<
        typename Directed, typename Vertex, typename Arc,
        typename Vertex_list, typename Arc_list
    >struct Adjacency_list_traits<
        Adjacency_list<Directed,Vertex,Arc,Vertex_list,Arc_list>>
    {
        typedef Directed directed;
            
            typedef Vertex vertex_type;
            typedef typename vertex_type::id_type vertex_id;
            typedef typename vertex_type::property_type vertex_property;
            typedef typename container_gen<Vertex_list,vertex_type>::type 
                    vertex_list;
            typedef typename container_gen<Vertex_list,vertex_id>::type 
                    vertex_id_list;
            
            typedef Arc arc_type;
            typedef typename arc_type::id_type arc_id;
            typedef typename arc_type::property_type arc_property;
            typedef typename container_gen<Arc_list,arc_type>::type 
                    arc_list;
            typedef typename container_gen<Arc_list,arc_id>::type       
                    arc_id_list;
        };

#define ADJ_TEMPL \
        typename D,typename V,typename A,typename VL,typename AL
#define ADJ_LIST Adjacency_list<D,V,A,VL,AL>
        
        template<ADJ_TEMPL>
            inline void 
            add_vertex(typename ADJ_LIST::vertex_type v, ADJ_LIST& g) 
            { g.add_vertex(v); }
        
        template<ADJ_TEMPL>
            inline void 
            remove_vertex(typename ADJ_LIST::vertex_type v, ADJ_LIST& g) 
            { g.remove_vertex(v.vid,v.properties,false); }
        
        template<ADJ_TEMPL>
            inline void 
            remove_vertex(typename ADJ_LIST::vertex_id vid, ADJ_LIST& g) 
            { g.remove_vertex(vid,{},true); }
        
        template<ADJ_TEMPL>
            inline void 
            add_arc(typename ADJ_LIST::arc_type a, ADJ_LIST& g) 
            { g.add_arc(a); }
        
        template<ADJ_TEMPL>
            inline void 
            remove_arc(typename ADJ_LIST::arc_type a, ADJ_LIST& g) 
            { g.remove_arc(a.beg,a.end,a.properties,false); }
        
        template<ADJ_TEMPL>
            inline void 
            remove_arc(typename ADJ_LIST::vertex_id beg, 
                       typename ADJ_LIST::vertex_id end, ADJ_LIST& g) 
            { g.remove_arc(beg,end,{},true); }
        
        // All vertices
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::vertex_list& 
            all_vertices(ADJ_LIST& g) { return g.all_vertices(); }
        
        template<ADJ_TEMPL>
            inline const typename ADJ_LIST::vertex_list& 
            all_vertices(const ADJ_LIST& g) { return g.all_vertices(); }
        
        // All arcs
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::arc_list& 
            all_arcs(ADJ_LIST& g) { return g.all_arcs(); }
        
        template<ADJ_TEMPL>
            inline const typename ADJ_LIST::arc_list& 
            all_arcs(const ADJ_LIST& g) { return g.all_arcs(); }
        
        // Vertex function
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::vertex_type& vertex
            (typename ADJ_LIST::vertex_id vid, ADJ_LIST& g) 
            { 
                if(!std::get<0>(g[vid])) throw graph::id_not_found{};
                return *std::get<0>(g[vid]); 
            }

        template<ADJ_TEMPL>
            inline const typename ADJ_LIST::vertex_type& vertex
            (typename ADJ_LIST::vertex_id vid, const ADJ_LIST& g)
            { 
                if(!std::get<0>(g[vid])) throw graph::id_not_found{};
                return *std::get<0>(g[vid]); 
            }
        
        // Arc function
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::arc_type& arc
            (typename ADJ_LIST::arc_id aid, ADJ_LIST& g) 
            { return *aid; }

        template<ADJ_TEMPL>
            inline const typename ADJ_LIST::arc_type& arc
            (typename ADJ_LIST::arc_id aid, const ADJ_LIST& g)
            { return *aid; }
        
        // Arcs list
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::arc_iterator_pair
            arcs(const typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
            {
                static_assert(
                    !std::is_void<typename ADJ_LIST::arc_iterator_pair>(),
                    "Adjacency list type does not support out arcs list"
                );
                return { std::begin (arcs_list(vid,g)) ,
                         std::end   (arcs_list(vid,g)) };
            }
            
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::arc_id_list& arcs_list
            (typename ADJ_LIST::vertex_id vid, ADJ_LIST& g) 
            { 
                static_assert(
                    !std::is_void<typename ADJ_LIST::arc_iterator_pair>(),
                    "Adjacency list type does not support out arcs list"
                );
                return std::get<1>(g[vid]); 
            }

        template<ADJ_TEMPL>
            inline const typename ADJ_LIST::arc_id_list& arcs_list
            (typename ADJ_LIST::vertex_id vid, const ADJ_LIST& g)
            { 
                static_assert(
                    !std::is_void<typename ADJ_LIST::arc_iterator_pair>(),
                    "Adjacency list type does not support out arcs list"
                );
                return std::get<1>(g[vid]); 
            }

        // Out arcs
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::out_iterator_pair
            out_arcs(const typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
            {
                static_assert(
                    !std::is_void<typename ADJ_LIST::out_iterator_pair>(),
                    "Adjacency list type does not support out arcs list"
                );
                return { std::begin (out_arcs_list(vid,g)) ,
                         std::end   (out_arcs_list(vid,g)) };
            }
                
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::vertex_id_list& out_arcs_list
            (const typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
            { 
                static_assert(
                    !std::is_void<typename ADJ_LIST::out_iterator_pair>(),
                    "Adjacency list type does not support out arcs list"
                );
                return std::get<2>(g[vid]); 
            }

        template<ADJ_TEMPL>
            inline const typename ADJ_LIST::vertex_id_list& out_arcs_list
            (const typename ADJ_LIST::vertex_id& vid, const ADJ_LIST& g)
            { 
                static_assert(
                    !std::is_void<typename ADJ_LIST::out_iterator_pair>(),
                    "Adjacency list type does not support out arcs list"
                );
                return std::get<2>(g[vid]); 
            }

        // In arcs
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::in_iterator_pair 
            in_arcs(const typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
            {
                static_assert(
                    !std::is_void<typename ADJ_LIST::in_iterator_pair>(),
                    "Adjacency list type does not support in arcs list"
                );
                return { std::begin (in_arcs_list(vid,g)) ,
                         std::end   (in_arcs_list(vid,g)) };
            }
        
        template<ADJ_TEMPL>
            inline typename ADJ_LIST::vertex_id_list& in_arcs_list
            (const typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
            { 
                static_assert(
                    !std::is_void<typename ADJ_LIST::in_iterator_pair>(),
                    "Adjacency list type does not support in arcs list"
                );
                return std::get<3>(g[vid]); 
            }

        template<ADJ_TEMPL>
            inline const typename ADJ_LIST::vertex_id_list& in_arcs_list
        (const typename ADJ_LIST::vertex_id& vid, const ADJ_LIST& g)
        { 
            static_assert(
                !std::is_void<typename ADJ_LIST::in_iterator_pair>(),
                "Adjacency list type does not support in arcs list"
            );
            return std::get<3>(g[vid]); 
        }

#undef ADJ_LIST
#undef ADJ_TEMPL

} // namespace graph

#endif
