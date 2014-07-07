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
            typedef typename adj_traits::vertex_ptr      vertex_ptr;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_property vertex_property;
            
            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_property    arc_property;
        
        protected:
            Adjacency_list_gen() 
                : self(static_cast<Adjacency_list*>(this)) {}
            
            // List iterators
            typedef typename arc_list::iterator          out_iterator;
            typedef void                                 in_iterator;
            typedef std::pair<out_iterator,out_iterator> out_iterator_pair;
            typedef void                                 in_iterator_pair;
            
            typedef std::tuple<vertex_ptr,arc_list> vertex_map;
            
            typedef typename container_gen<
                Vertex_list,vertex_map
            >::type type;
            
        public:
            static const int out_position {1}, in_position {0};
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
            typedef typename adj_traits::vertex_ptr      vertex_ptr;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_property vertex_property;

            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_property    arc_property;

        protected:
            Adjacency_list_gen() 
                : self{static_cast<Adjacency_list*>(this)} {}

            // List iterators
            typedef typename arc_list::iterator          out_iterator;
            typedef typename arc_list::iterator          in_iterator;
            typedef std::pair<out_iterator,out_iterator> out_iterator_pair;
            typedef out_iterator_pair                    in_iterator_pair;
            
            typedef std::tuple<vertex_ptr,arc_list> vertex_map;
            typedef typename container_gen<
                Vertex_list,vertex_map
            >::type type;
            
        public:
            static const int out_position = 1, in_position = 1;
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
            typedef typename adj_traits::vertex_ptr      vertex_ptr;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_property vertex_property;

            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_property    arc_property;

        protected:
            Adjacency_list_gen() 
                : self{static_cast<Adjacency_list*>(this)} {}
            
            // List iterators
            typedef typename arc_list::iterator          out_iterator;
            typedef typename arc_list::iterator          in_iterator;
            typedef std::pair<out_iterator,out_iterator> out_iterator_pair;
            typedef std::pair<in_iterator,in_iterator>   in_iterator_pair;
            
            // Adjacency list type
            typedef std::tuple<vertex_ptr,arc_list,
                    vertex_list,vertex_list> vertex_map;
            
            typedef typename 
            container_gen<Vertex_list,vertex_map>::type type;
            
        public:
            static const int out_position {1}, in_position {2};
            
            // void add_arc(const arc_type& arc)
            // {
            //     // Throws if any vertex does not exist
            //     vertex(arc.beg,*self); vertex(arc.end,*self);
            //
            //     // Put a pointer in the list of out neighbors
            //     out_arcs_list(arc.beg,*self).push_back(arc);
            //     in_arcs_list(arc.end,*self).push_back(arc);
            // }
            //
            // void remove_arc(vertex_id beg, vertex_id end,
            //                 arc_property prop, bool ignore_prop)
            // {
            //     // Throws if any vertex does not exist
            //     vertex(beg,*self); vertex(end,*self);
            //     
            //     // Remove from beg's list of out arcs
            //     out_iterator out_arc_beg, out_arc_end;
            //     std::tie(out_arc_beg, out_arc_end) = out_arcs(beg,*self);
            //     for(auto it = out_arc_beg; it != out_arc_end; ++it)
            //         if(it->beg == beg && it->end == end
            //         && (ignore_prop || it->properties == prop))
            //             out_arcs_list(beg,*self).erase(it,it+1);
            //     
            //     // Remove from end's list of in arcs
            //     in_iterator in_arc_beg, in_arc_end;
            //     std::tie(in_arc_beg, in_arc_end) = in_arcs(beg,*self);
            //     for(auto it = in_arc_beg; it != in_arc_end; ++it)
            //         if(it->beg == beg && it->end == end
            //         && (ignore_prop || it->properties == prop))
            //             in_arcs_list(beg,*self).erase(it,it+1);
            // }
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
            friend Gen;
            
        public:
            typedef Adjacency_list_traits<self> adj_traits;
            typedef typename adj_traits::vertex_id       vertex_id;
            typedef typename adj_traits::vertex_ptr      vertex_ptr;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_property vertex_property;
            
            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_property    arc_property;
            
            typedef typename Gen::in_iterator       in_iterator;
            typedef typename Gen::in_iterator_pair  in_iterator_pair;
            typedef typename Gen::out_iterator      out_iterator;
            typedef typename Gen::out_iterator_pair out_iterator_pair;
        
        private:
            Adj_list adj_list;
            unsigned long n_vertices, n_arcs;
            
            vertex_id max_id(vertex_list vl) 
            { 
                vertex_id max_id {};
                for(const vertex_type& v : vl)
                    if(v.id > max_id) max_id = v.id;
                return max_id;
            }
            
        public:
            Adjacency_list(vertex_list vertices = vertex_list{},
                           arc_list    arcs     = arc_list{})
                : adj_list{this->max_id(vertices)+1},
                  n_vertices{vertices.size()}, n_arcs{arcs.size()}
            {
                for(vertex_type& v : vertices) this->add_vertex(v);
                for(arc_type& a : arcs)        add_arc(a,*this);
            }
            
            inline vertex_map& operator[](vertex_id vid) 
            { return this->adj_list[vid]; }
            
            inline const vertex_map& operator[](vertex_id vid) const 
            { return this->adj_list[vid]; }
            
            void add_vertex(const vertex_type& v)
            {
                if(v.id > this->num_vertices())
                    this->adj_list.resize(v.id);
                std::get<0>(this->adj_list[v.id])
                    = vertex_ptr{new vertex_type{v.id,v}};
                n_vertices++;
            }
            
            void remove_vertex(vertex_id vid, vertex_property prop, 
                               bool ignore_prop)
            {
                vertex(vid,*this); // To check if id exists
                if(ignore_prop || vertex(vid,*this).properties == prop)
                {
                    for(auto& map : this->adj_list)
                    {
                        if(std::get<1>(map)->id == vid) 
                            std::get<1>(map).clear();
                        for(const arc_type& a : std::get<1>(map))
                            if(a.beg == vid || a.end == vid)
                                remove_arc(a,*this);
                        
                        std::get<0>(this->adj_list[vid]) = nullptr;
                        
                        // Remove from vertex list
                        for(auto it = std::begin(this->vertices);
                            it != std::end(this->vertices); ++it)
                            if(it->id == vid)
                                this->vertices.erase(it,it+1);
                    }
                }
                n_vertices--;
            }
            
            size_t num_vertices () { return this->n_vertices; }
            size_t num_arcs     () { return this->n_arcs;     }
    };
        
    template<
        typename Directed, typename Vertex, typename Arc,
        typename Vertex_list, typename Arc_list
    >struct Adjacency_list_traits<
        Adjacency_list<Directed,Vertex,Arc,Vertex_list,Arc_list>>
    {
        typedef Directed directed;
            
            typedef Vertex vertex_type;
            typedef std::shared_ptr<Vertex> vertex_ptr;
            typedef typename vertex_type::id_type vertex_id;
            typedef typename vertex_type::property_type vertex_property;
            typedef typename container_gen<Vertex_list,vertex_type>::type 
                    vertex_list;
            
            typedef Arc arc_type;
            typedef typename arc_type::id_type arc_id;
            typedef typename arc_type::property_type arc_property;
            typedef typename container_gen<Arc_list,arc_type>::type 
                    arc_list;
        };

#define ADJ_TEMPL \
        typename D,typename V,typename A,typename VL,typename AL
#define ADJ_LIST Adjacency_list<D,V,A,VL,AL>
    
    // VERTEX FUNCTIONS ////////////////////////////////////////////////
    
    // Get vertex
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
        
    // Add vertex
    template<ADJ_TEMPL>
        inline void 
        add_vertex(typename ADJ_LIST::vertex_type v, ADJ_LIST& g) 
        { g.add_vertex(v); }
    
    // Remove vertex
    template<ADJ_TEMPL>
        inline void 
        remove_vertex(typename ADJ_LIST::vertex_type v, ADJ_LIST& g) 
        { g.remove_vertex(v.vid,v.properties,false); }
    
    template<ADJ_TEMPL>
        inline void 
        remove_vertex(typename ADJ_LIST::vertex_id vid, ADJ_LIST& g) 
        { g.remove_vertex(vid,{},true); }
    
    // ARC FUNCTIONS ///////////////////////////////////////////////////
    
    // Get arc
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::arc_type& arc
        (typename ADJ_LIST::vertex_id beg,
         typename ADJ_LIST::vertex_id end, ADJ_LIST& g) 
        { 
            typename ADJ_LIST::out_iterator arc_beg, arc_end;
            std::tie(arc_beg, arc_end) = out_arcs(beg,g);
            for(auto it = arc_beg; it != arc_end; ++it)
                if(it->beg == beg && it->end == end) return *it;
        }
        
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::arc_type& arc
        (typename ADJ_LIST::vertex_id beg,
         typename ADJ_LIST::vertex_id end, 
         typename ADJ_LIST::arc_property prop, ADJ_LIST& g) 
        {
            typename ADJ_LIST::out_iterator arc_beg, arc_end;
            std::tie(arc_beg, arc_end) = out_arcs(beg,g);
            for(auto it = arc_beg; it != arc_end; ++it)
                if(it->beg == beg && it->end == end
                && it->properties == prop) return *it;
        }
    
    // Add arcs
    template<ADJ_TEMPL,int out,int in>
        struct add_arc_impl {
            static void 
            add_arc(typename ADJ_LIST::arc_type a, ADJ_LIST& g) {
                // Throws if any vertex does not exist
                vertex(a.beg,g); vertex(a.end,g);
                add_arc_impl<D,V,A,VL,AL,out,0>::add_arc(a,g);
                add_arc_impl<D,V,A,VL,AL,0,in>::add_arc(a,g);
        }};
    
    template<ADJ_TEMPL,int out>
        struct add_arc_impl<D,V,A,VL,AL,out,0> {
            static void 
            add_arc(typename ADJ_LIST::arc_type a, ADJ_LIST& g) {
                out_arcs_list(a.beg,g).push_back(a);
        }};
    
    template<ADJ_TEMPL,int in>
        struct add_arc_impl<D,V,A,VL,AL,0,in> {
            static void 
            add_arc(typename ADJ_LIST::arc_type a, ADJ_LIST& g) {
                in_arcs_list(a.end,g).push_back(a);
        }};
    
    template<ADJ_TEMPL>
        inline void 
        add_arc(typename ADJ_LIST::arc_type a, ADJ_LIST& g) 
        { 
            add_arc_impl<D,V,A,VL,AL,ADJ_LIST::out_position,
                ADJ_LIST::in_position>::add_arc(a,g); 
        }
    
    template<ADJ_TEMPL>
        inline void 
        add_arc(typename ADJ_LIST::vertex_type beg, 
                typename ADJ_LIST::vertex_type end, ADJ_LIST& g) 
        { 
            add_arc_impl<D,V,A,VL,AL,ADJ_LIST::out_position,
                ADJ_LIST::in_position>
                    ::add_arc(ADJ_LIST::arc_type(beg,end),g); 
        }
    
    // Remove arc
    template<ADJ_TEMPL,int out,int in>
        struct remove_arc_impl {
            static void remove_arc
            (typename ADJ_LIST::vertex_id beg, 
             typename ADJ_LIST::vertex_id end,
             typename ADJ_LIST::arc_property prop, 
             bool ignore_prop, ADJ_LIST& g)
            {
                // Throws if any vertex does not exist
                vertex(beg,g); vertex(end,g);
                remove_arc_impl<D,V,A,VL,AL,out,0>
                    ::remove_arc(beg,end,prop,ignore_prop);
                remove_arc_impl<D,V,A,VL,AL,0,in>
                    ::remove_arc(beg,end,prop,ignore_prop);
            }
        };
    
    template<ADJ_TEMPL,int out>
        struct remove_arc_impl<D,V,A,VL,AL,out,0> {
            static void 
            remove_arc(typename ADJ_LIST::vertex_id beg, 
                       typename ADJ_LIST::vertex_id end,
                       typename ADJ_LIST::arc_property prop, 
                       bool ignore_prop, ADJ_LIST& g)
            {
                // Remove from beg's list of arcs
                typename ADJ_LIST::out_iterator arc_beg, arc_end;
                std::tie(arc_beg, arc_end) = out_arcs(beg,g);
                for(auto it = arc_beg; it != arc_end; ++it)
                    if(it->beg == beg && it->end == end
                    && (ignore_prop || it->properties == prop))
                        out_arcs_list(beg,g).erase(it,it+1);
            }
        };
    
    template<ADJ_TEMPL,int in>
        struct remove_arc_impl<D,V,A,VL,AL,0,in> {
            static void 
            remove_arc(typename ADJ_LIST::vertex_id beg, 
                       typename ADJ_LIST::vertex_id end,
                       typename ADJ_LIST::arc_property prop, 
                       bool ignore_prop, ADJ_LIST& g)
            {
                // Remove from end's list of in arcs
                typename ADJ_LIST::in_iterator in_arc_beg,in_arc_end;
                std::tie(in_arc_beg, in_arc_end) 
                    = in_arcs(beg,g);
                for(auto it = in_arc_beg; it != in_arc_end; ++it)
                    if(it->beg == beg && it->end == end
                    && (ignore_prop || it->properties == prop))
                        in_arcs_list(beg,g).erase(it,it+1);
            }
        };
    
    template<ADJ_TEMPL>
        inline void 
        remove_arc(typename ADJ_LIST::arc_type a, ADJ_LIST& g) 
        {
            remove_arc_impl<D,V,A,VL,AL,
                ADJ_LIST::out_position,ADJ_LIST::in_position>
                ::remove_arc(a.bed,a.end,a.properties,false,g);
        }
    
    template<ADJ_TEMPL>
        inline void 
        remove_arc(typename ADJ_LIST::vertex_id beg, 
                   typename ADJ_LIST::vertex_id end, ADJ_LIST& g) 
        {
            remove_arc_impl<D,V,A,VL,AL,
                ADJ_LIST::out_position,ADJ_LIST::in_position>
                ::remove_arc(beg,end,{},false,g);
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
        inline typename ADJ_LIST::arc_list& out_arcs_list
        (const typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
        { 
            static_assert(
                !std::is_void<typename ADJ_LIST::out_iterator_pair>(),
                "Adjacency list type does not support out arcs list"
            );
            return std::get<ADJ_LIST::out_position>(g[vid]); 
        }

    template<ADJ_TEMPL>
        inline const typename ADJ_LIST::arc_list& out_arcs_list
        (const typename ADJ_LIST::vertex_id& vid, const ADJ_LIST& g)
        { 
            static_assert(
                !std::is_void<typename ADJ_LIST::out_iterator_pair>(),
                "Adjacency list type does not support out arcs list"
            );
            return std::get<ADJ_LIST::out_position>(g[vid]); 
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
        inline typename ADJ_LIST::arc_list& in_arcs_list
        (const typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
        { 
            static_assert(
                !std::is_void<typename ADJ_LIST::in_iterator_pair>(),
                "Adjacency list type does not support in arcs list"
            );
            return std::get<ADJ_LIST::in_position>(g[vid]); 
        }

    template<ADJ_TEMPL>
        inline const typename ADJ_LIST::arc_list& in_arcs_list
        (const typename ADJ_LIST::vertex_id& vid, const ADJ_LIST& g)
        { 
            static_assert(
                !std::is_void<typename ADJ_LIST::in_iterator_pair>(),
                "Adjacency list type does not support in arcs list"
            );
            return std::get<ADJ_LIST::in_position>(g[vid]); 
        }

#undef ADJ_LIST
#undef ADJ_TEMPL

} // namespace graph

#endif
