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
#include <typeinfo>

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
            
            typedef typename arc_list::const_iterator const_out_iterator;
            typedef void                              const_in_iterator;
            typedef std::pair<const_out_iterator,const_out_iterator>
                    const_out_iterator_pair;
            typedef void const_in_iterator_pair;
            
            typedef std::tuple<vertex_ptr,arc_list> vertex_map;
            
            typedef typename container_gen<
                Vertex_list,vertex_map
            >::type type;
            
        public:
            static const int out_position = 1, in_position = -1;
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
            
            typedef typename arc_list::const_iterator const_out_iterator;
            typedef typename arc_list::const_iterator const_in_iterator;
            typedef std::pair<const_out_iterator,const_out_iterator>
                    const_out_iterator_pair;
            typedef std::pair<const_in_iterator,const_in_iterator>
                    const_in_iterator_pair;
            
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
            
            typedef typename arc_list::const_iterator const_out_iterator;
            typedef typename arc_list::const_iterator const_in_iterator;
            typedef std::pair<const_out_iterator,const_out_iterator>
                    const_out_iterator_pair;
            typedef std::pair<const_in_iterator,const_in_iterator>
                    const_in_iterator_pair;
            
            // Adjacency list type
            typedef std::tuple<vertex_ptr,arc_list,
                    vertex_list,vertex_list> vertex_map;
            
            typedef typename
            container_gen<Vertex_list,vertex_map>::type type;
            
        public:
            static const int out_position = 1, in_position = 2;
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
            
        public:
            typedef Adjacency_list_traits<self> adj_traits;
            typedef typename adj_traits::vertex_id       vertex_id;
            typedef typename adj_traits::vertex_ptr      vertex_ptr;
            typedef typename adj_traits::vertex_size     vertex_size;
            typedef typename adj_traits::vertex_type     vertex_type;
            typedef typename adj_traits::vertex_list     vertex_list;
            typedef typename adj_traits::vertex_property vertex_property;
            
            typedef typename adj_traits::arc_id          arc_id;
            typedef typename adj_traits::arc_type        arc_type;
            typedef typename adj_traits::arc_size        arc_size;
            typedef typename adj_traits::arc_list        arc_list;
            typedef typename adj_traits::arc_property    arc_property;
            
            typedef typename Gen::in_iterator        in_iterator;
            typedef typename Gen::in_iterator_pair   in_iterator_pair;
            typedef typename Gen::const_in_iterator  const_in_iterator;
            typedef typename Gen::const_in_iterator_pair
                             const_in_iterator_pair;
            
            typedef typename Gen::out_iterator       out_iterator;
            typedef typename Gen::out_iterator_pair  out_iterator_pair;
            typedef typename Gen::const_out_iterator const_out_iterator;
            typedef typename Gen::const_out_iterator_pair
                             const_out_iterator_pair;
            
            struct vertex_iterator
            {
                private:
                typedef typename Adj_list::iterator iterator;
                iterator vmi, vmi_beg, vmi_end;
                
                public:
                typedef typename iterator::iterator_category
                        iterator_category;
                typedef vertex_type                 value_type;
                typedef vertex_type*                pointer;
                typedef vertex_type&                reference;
                typedef std::size_t                 size_type;
                typedef std::ptrdiff_t              difference_type;
                
                vertex_iterator() {}
                
                vertex_iterator(Adjacency_list* adj_list)
                    : vmi{std::begin(adj_list->adj_list)},
                      vmi_beg{std::begin(adj_list->adj_list)},
                      vmi_end{std::end(adj_list->adj_list)}
                { while(vmi != vmi_end && !std::get<0>(*vmi)) ++vmi; }
                
                vertex_iterator(Adjacency_list* adj_list, int)
                    : vmi{std::end(adj_list->adj_list)},
                      vmi_beg{std::begin(adj_list->adj_list)},
                      vmi_end{std::end(adj_list->adj_list)} {}
                
                value_type& operator*()
                { return *std::get<0>(*vmi); }
                
                const value_type& operator*() const
                { return *std::get<0>(*vmi); }
                
                vertex_iterator& operator++()
                { 
                    if(vmi != vmi_end) 
                        do ++vmi; 
                        while(vmi != vmi_end && !std::get<0>(*vmi));
                    return *this; 
                }
                
                vertex_iterator& operator--()
                { 
                    if(vmi != vmi_beg)
                        do --vmi; 
                        while(vmi != vmi_beg && !std::get<0>(*vmi));
                    return *this; 
                }
                
                vertex_iterator operator++(int)
                { vertex_iterator tmp = *this; ++*this; return tmp; }
                
                vertex_iterator operator--(int)
                { vertex_iterator tmp = *this; --*this; return tmp; }
                
                const vertex_ptr& operator->() const
                { return std::get<0>(*vmi); }
                
                bool operator==(vertex_iterator& it)
                { return it.vmi == this->vmi; }
                
                bool operator!=(vertex_iterator& it)
                { return !operator==(it); }
            };
            
            struct const_vertex_iterator
            {
                private:
                typedef typename Adj_list::const_iterator iterator;
                iterator vmi, vmi_beg, vmi_end;
                    
                public:
                typedef typename iterator::iterator_category
                        iterator_category;
                typedef vertex_type    value_type;
                typedef vertex_type*   pointer;
                typedef vertex_type&   reference;
                typedef std::size_t    size_type;
                typedef std::ptrdiff_t difference_type;
                
                const_vertex_iterator() {}
                
                const_vertex_iterator(const Adjacency_list* adj_list)
                    : vmi{std::begin(adj_list->adj_list)},
                      vmi_beg{std::begin(adj_list->adj_list)},
                      vmi_end{std::end(adj_list->adj_list)}
                { while(vmi != vmi_end && !std::get<0>(*vmi)) ++vmi; }
                
                const_vertex_iterator(const Adjacency_list* adj_list, int)
                    : vmi{std::end(adj_list->adj_list)},
                      vmi_beg{std::begin(adj_list->adj_list)},
                      vmi_end{std::end(adj_list->adj_list)} {}
                
                const value_type& operator*() const
                { return *std::get<0>(*vmi); }
                
                const_vertex_iterator& operator++()
                { 
                    if(vmi != vmi_end) 
                        do ++vmi; 
                        while(vmi != vmi_end && !std::get<0>(*vmi));
                    return *this; 
                }
                
                const_vertex_iterator& operator--()
                { 
                    if(vmi != vmi_beg)
                        do --vmi; 
                        while(vmi != vmi_beg && !std::get<0>(*vmi));
                    return *this; 
                }
                
                const_vertex_iterator operator++(int)
                { const_vertex_iterator tmp = *this; ++*this; return tmp; }
                
                const_vertex_iterator operator--(int)
                { const_vertex_iterator tmp = *this; --*this; return tmp; }
                
                const vertex_ptr& operator->() const
                { return std::get<0>(*vmi); }
                
                bool operator==(const_vertex_iterator& it)
                { return it.vmi == this->vmi; }
                
                bool operator!=(const_vertex_iterator& it)
                { return !operator==(it); }
            };
            
            struct arc_iterator
            {
                typedef Adjacency_list::out_iterator    out_iterator;
                typedef Adjacency_list::vertex_iterator vertex_iterator;
                Adjacency_list *adj_list;
                
                vertex_iterator vit, vit_beg, vit_end;
                out_iterator    oit, oit_beg, oit_end; 
                
                typedef std::bidirectional_iterator_tag 
                        iterator_category;
                typedef arc_type       value_type;
                typedef arc_type*      pointer;
                typedef arc_type&      reference;
                typedef std::size_t    size_type;
                typedef std::ptrdiff_t difference_type;
                
                arc_iterator() {}
                
                arc_iterator(Adjacency_list *adj_list)
                    : adj_list{adj_list}
                {
                    std::tie(vit_beg,vit_end) 
                        = adj_list->vertices();
                    std::tie(oit_beg,oit_end) 
                        = out_arcs(vit_beg->id,*adj_list);
                    vit = vit_beg; oit = oit_beg;
                }
                
                arc_iterator(Adjacency_list *adj_list, int)
                {
                    std::tie(vit_beg,vit_end) 
                        = adj_list->vertices();
                    vit = --vit_end; ++vit_end; // last vertex element
                    std::tie(oit_beg,oit_end) 
                        = out_arcs(vit->id,*adj_list);
                    oit = oit_end; vit = vit_end;
                }
                
                value_type& operator*() { return *oit; }
                const value_type& operator*() const { return *oit; }
                
                arc_iterator& operator++()
                {
                    if(vit == vit_end) return *this;
                    ++oit; // Continues untill finish or in empty lists
                    while(oit == oit_end)
                        if(++vit == vit_end) break;
                        else
                        {
                            std::tie(oit_beg,oit_end)
                             = out_arcs(vit->id,*adj_list); 
                            oit = oit_beg;
                        }
                    return *this;
                }
                
                arc_iterator& operator--()
                {
                    if(vit == vit_beg && oit == oit_beg) return *this;
                    --oit; // Continues untill finish or in empty lists
                    while(oit == oit_beg)
                        if(--vit == vit_beg) break;
                        else 
                        { 
                            std::tie(oit_beg,oit_end)
                            = out_arcs(vit->id,*adj_list); 
                            oit = oit_end; --oit; 
                        }
                    return *this;
                }
                
                arc_iterator operator++(int)
                { arc_iterator temp = *this; ++*this; return temp; }
                
                arc_iterator operator--(int)
                { arc_iterator temp = *this; --*this; return temp; }
                    
                out_iterator& operator->()             { return oit; }
                const out_iterator& operator->() const { return oit; }
                
                bool operator==(arc_iterator& it)
                {
                    return it.vit == this->vit && it.oit == this->oit
                        && it.oit_end == this->oit_end
                        && it.oit_beg == this->oit_beg
                        && it.vit_end == this->vit_end
                        && it.vit_beg == this->vit_beg;
                }
                
                bool operator!=(arc_iterator& it)
                { return !operator==(it); }
            };
            
            struct const_arc_iterator
            {
                typedef Adjacency_list::const_out_iterator    
                        const_out_iterator;
                typedef Adjacency_list::const_vertex_iterator
                        const_vertex_iterator;
                const Adjacency_list *adj_list;
                
                const_vertex_iterator vit, vit_beg, vit_end;
                const_out_iterator    oit, oit_beg, oit_end; 
                
                typedef std::bidirectional_iterator_tag 
                        iterator_category;
                typedef arc_type       value_type;
                typedef arc_type*      pointer;
                typedef arc_type&      reference;
                typedef std::size_t    size_type;
                typedef std::ptrdiff_t difference_type;
                
                const_arc_iterator() {}
                
                const_arc_iterator(const Adjacency_list *adj_list)
                    : adj_list{adj_list}
                {
                    std::tie(vit_beg,vit_end) 
                        = adj_list->vertices();
                    std::tie(oit_beg,oit_end) 
                        = out_arcs(vit_beg->id,*adj_list);
                    vit = vit_beg; oit = oit_beg;
                }
                
                const_arc_iterator(const Adjacency_list *adj_list, int)
                {
                    std::tie(vit_beg,vit_end) 
                        = adj_list->vertices();
                    vit = --vit_end; ++vit_end; // last vertex element
                    std::tie(oit_beg,oit_end) 
                        = out_arcs(vit->id,*adj_list);
                    oit = oit_end; vit = vit_end;
                }
                
                const value_type& operator*() const { return *oit; }
                
                const_arc_iterator& operator++()
                {
                    if(vit == vit_end) return *this;
                    ++oit; // Continues untill finish or in empty lists
                    while(oit == oit_end)
                        if(++vit == vit_end) break;
                        else
                        {
                            std::tie(oit_beg,oit_end)
                             = out_arcs(vit->id,*adj_list); 
                            oit = oit_beg;
                        }
                    return *this;
                }
                
                const_arc_iterator& operator--()
                {
                    if(vit == vit_beg && oit == oit_beg) return *this;
                    --oit; // Continues untill finish or in empty lists
                    while(oit == oit_beg)
                        if(--vit == vit_beg) break;
                        else 
                        { 
                            std::tie(oit_beg,oit_end)
                            = out_arcs(vit->id,*adj_list); 
                            oit = oit_end; --oit; 
                        }
                    return *this;
                }
                
                const_arc_iterator operator++(int)
                { const_arc_iterator temp = *this; ++*this; return temp; }
                
                const_arc_iterator operator--(int)
                { const_arc_iterator temp = *this; --*this; return temp; }
                    
                const const_out_iterator& operator->() const 
                { return oit; }
                
                bool operator==(const_arc_iterator& it)
                {
                    return it.vit == this->vit && it.oit == this->oit
                        && it.oit_end == this->oit_end
                        && it.oit_beg == this->oit_beg
                        && it.vit_end == this->vit_end
                        && it.vit_beg == this->vit_beg;
                }
                
                bool operator!=(const_arc_iterator& it)
                { return !operator==(it); }
            };
            
            typedef std::pair<vertex_iterator,vertex_iterator>
                    vertex_iterator_pair;
            typedef std::pair<const_vertex_iterator,const_vertex_iterator>
                    const_vertex_iterator_pair;
            typedef std::pair<arc_iterator,arc_iterator>
                    arc_iterator_pair;
            typedef std::pair<const_arc_iterator,const_arc_iterator>
                    const_arc_iterator_pair;
        
        private:
            Adj_list adj_list;
            
            vertex_id max_id(vertex_list vl)
            {
                vertex_id max_id {};
                for(const vertex_type& v : vl)
                    if(v.id > max_id) max_id = v.id;
                return max_id;
            }
            
        public:
            unsigned long n_vertices, n_arcs;
            
            Adjacency_list(vertex_list vertices = vertex_list{},
                           arc_list    arcs     = arc_list{})
                : adj_list{this->max_id(vertices)+1},
                  n_vertices{0}, n_arcs{0}
            {
                for(vertex_type& v : vertices) this->add_vertex(v);
                for(arc_type& a : arcs)        add_arc(a,*this);
            }
            
            Adjacency_list(size_t n_vertices,
                           vertex_type prototype = vertex_type{0})
                : adj_list{n_vertices}, n_vertices{0}, n_arcs{0}
            {
                vertex_id id {};
                for(size_t m = 0; m < n_vertices; ++m, ++id)
                    this->add_vertex(vertex_type{id,prototype});
            }
            
            vertex_iterator_pair vertices()
            {
                return { vertex_iterator{this},
                         vertex_iterator{this,1} };
            }
            
            const_vertex_iterator_pair vertices() const
            {
                return { const_vertex_iterator{this},
                         const_vertex_iterator{this,1} };
            }
            
            arc_iterator_pair arcs()
            {
                return { arc_iterator{this},
                         arc_iterator{this,1} };
            }
            
            const_arc_iterator_pair arcs() const
            {
                return { const_arc_iterator{this},
                         const_arc_iterator{this,1} };
            }
            
            inline vertex_map& operator[](vertex_id vid)
            { return this->adj_list[vid]; }
            
            inline const vertex_map& operator[](vertex_id vid) const
            { return this->adj_list[vid]; }
            
            void add_vertex(const vertex_type& v)
            {
                if(v.id > this->num_vertices())
                    this->adj_list.resize(v.id+1);
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
                        if(!std::get<0>(map)) continue;
                        if(Gen::out_position != -1)
                        {
                            if(std::get<0>(map)->id == vid)
                                std::get<1>(map).clear();
                            for(const arc_type& a : std::get<1>(map))
                                if(a.beg == vid || a.end == vid)
                                    remove_arc(a,*this);
                        }
                        if(Gen::in_position != -1)
                        {
                            if(std::get<0>(map)->id == vid)
                                std::get<2>(map).clear();
                            for(const arc_type& a : std::get<2>(map))
                                if(a.beg == vid || a.end == vid)
                                    remove_arc(a,*this);
                        }
                    }
                    std::get<0>(this->adj_list[vid]) = nullptr;
                }
                n_vertices--;
            }
            
            vertex_size num_vertices() const { return this->n_vertices; }
            arc_size    num_arcs()     const { return this->n_arcs;     }
            
            friend std::ostream&
            operator<<(std::ostream& os, const Adjacency_list& t)
            {
                // Print vertices
                const_vertex_iterator vit,vit_end;
                std::tie(vit,vit_end) = t.vertices();
                for(; vit != vit_end; ++vit) os << *vit << std::endl;
                // Print arcs
                const_arc_iterator ait,ait_end;
                std::tie(ait,ait_end) = t.arcs();
                for(; ait != ait_end; ++ait) os << *ait << std::endl;
                // Return ostream
                return os;
            }
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
        typedef typename vertex_type::size_type vertex_size;
        typedef typename vertex_type::property_type vertex_property;
        typedef typename container_gen<Vertex_list,vertex_type>::type
                vertex_list;
        
        typedef Arc arc_type;
        typedef typename arc_type::id_type arc_id;
        typedef typename arc_type::size_type arc_size;
        typedef typename arc_type::property_type arc_property;
        typedef typename container_gen<Arc_list,arc_type>::type
                arc_list;
    };

#define ADJ_TEMPL \
        typename D,typename V,typename A,typename VL,typename AL
#define ADJ_LIST Adjacency_list<D,V,A,VL,AL>
    
    // VERTEX FUNCTIONS ////////////////////////////////////////////////
    
    // Vertex list
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::vertex_iterator_pair
        vertices(ADJ_LIST& g) { return g.vertices(); }
    
    // Number of vertices
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::vertex_size
        num_vertices(const ADJ_LIST& g) { return g.num_vertices(); }
    
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
        add_vertex(typename ADJ_LIST::vertex_id vid, ADJ_LIST& g)
        { g.add_vertex(typename ADJ_LIST::vertex_type{vid}); }
        
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
    
    // Arc list
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::arc_iterator_pair
        arcs(ADJ_LIST& g) { return g.arcs(); }
    
    // Number of arcs
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::arc_size
        num_arcs(const ADJ_LIST& g) { return g.num_arcs(); }
    
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
            throw arc_not_found{};
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
            throw arc_not_found{};
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
                g.n_arcs++;
        }};
    
    template<ADJ_TEMPL,int out>
        struct add_arc_impl<D,V,A,VL,AL,out,-1> {
            static void
            add_arc(typename ADJ_LIST::arc_type a, ADJ_LIST& g) {
                out_arcs_list(a.beg,g).push_back(a);
        }};
    
    template<ADJ_TEMPL,int in>
        struct add_arc_impl<D,V,A,VL,AL,-1,in> {
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
                remove_arc_impl<D,V,A,VL,AL,out,0>
                    ::remove_arc(beg,end,prop,ignore_prop);
                remove_arc_impl<D,V,A,VL,AL,0,in>
                    ::remove_arc(beg,end,prop,ignore_prop);
            }
        };
    
    template<ADJ_TEMPL,int out>
        struct remove_arc_impl<D,V,A,VL,AL,out,-1> {
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
        struct remove_arc_impl<D,V,A,VL,AL,-1,in> {
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
                ::remove_arc(a.beg,a.end,a.properties,false,g);
            g.n_arcs--;
        }
    
    template<ADJ_TEMPL>
        inline void
        remove_arc(typename ADJ_LIST::vertex_id beg,
                   typename ADJ_LIST::vertex_id end, ADJ_LIST& g)
        {
            remove_arc_impl<D,V,A,VL,AL,
                ADJ_LIST::out_position,ADJ_LIST::in_position>
                ::remove_arc(beg,end,{},true,g);
            g.n_arcs--;
        }

    // Out arcs
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::out_iterator_pair
        out_arcs(typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
        {
            static_assert(
                !std::is_void<typename ADJ_LIST::out_iterator_pair>(),
                "Adjacency list type does not support out arcs list"
            );
            return { std::begin (out_arcs_list(vid,g)) ,
                     std::end   (out_arcs_list(vid,g)) };
        }
    
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::const_out_iterator_pair
        out_arcs(const typename ADJ_LIST::vertex_id& vid, 
                 const ADJ_LIST& g)
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
        (typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
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
                !std::is_void<typename 
                              ADJ_LIST::const_out_iterator_pair>(),
                "Adjacency list type does not support out arcs list"
            );
            return std::get<ADJ_LIST::out_position>(g[vid]);
        }

    // In arcs
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::in_iterator_pair
        in_arcs(typename ADJ_LIST::vertex_id& vid, ADJ_LIST& g)
        {
            static_assert(
                !std::is_void<typename ADJ_LIST::in_iterator_pair>(),
                "Adjacency list type does not support in arcs list"
            );
            return { std::begin (in_arcs_list(vid,g)) ,
                     std::end   (in_arcs_list(vid,g)) };
        }
    
    template<ADJ_TEMPL>
        inline typename ADJ_LIST::const_in_iterator_pair
        in_arcs(const typename ADJ_LIST::vertex_id& vid, 
                const ADJ_LIST& g)
        {
            static_assert(
                !std::is_void<typename 
                              ADJ_LIST::const_in_iterator_pair>(),
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
