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

#ifndef TCC_GRAPH_STREE_DEFINED
#define TCC_GRAPH_STREE_DEFINED

// Default libraries
#include <vector>
#include <utility>
#include <iostream>
#include <initializer_list>

// Libraries
#include "Arc.tcc"
#include "Cycle.tcc"
#include "Vertex.tcc"
#include "Traits.tcc"
#include "Exception.tcc"
#include "Adjacency_list.tcc"

namespace graph 
{
    template<
        typename Graph,
        typename Cycle = typename 
                         graph::Cycle<typename Graph::arc_type>::type
    >class STree
    {
        public:
            typedef Graph graph_type;
            typedef Cycle cycle_type;
            typedef typename graph_type::arc_id      arc_id;
            typedef typename graph_type::arc_type    arc_type;
            typedef typename graph_type::arc_list    arc_list;
            typedef typename graph_type::vertex_id   vertex_id;
            typedef typename graph_type::vertex_size vertex_size;
            typedef typename graph_type::vertex_type vertex_type;
            
            // arc_list               arc_ids;
            std::vector<arc_id>    arc_ids;
            
        private:
            graph_type* const      graph;
            vertex_size            n_vertices;
            std::vector<vertex_id> parnt;
            std::vector<int>       depth;
            int                    max_depth;
            
            void calculate_parents()
            {
                std::vector<bool> has_parnt(n_vertices,false);
                for(vertex_id id : this->parnt) this->parnt[id] = id;
                for(const arc_id& aid : arc_ids)
                {
                    if(has_parnt[aid.beg] && !has_parnt[aid.end])
                    {
                        parnt[aid.end] = aid.beg;
                        has_parnt[aid.end] = true;
                    }
                    else if(!has_parnt[aid.beg] && has_parnt[aid.end])
                    {
                        parnt[aid.beg] = aid.end;
                        has_parnt[aid.beg] = true;
                    }
                    else if(!has_parnt[aid.beg] && !has_parnt[aid.end])
                    {
                        parnt[aid.end] = aid.beg;
                        has_parnt[aid.end] = true;
                    }
                }
            }
            
            void calculate_depth()
            {
                typename Graph::vertex_iterator vit, vit_end;
                std::tie(vit,vit_end) = vertices(*graph);
                for(; vit != vit_end; ++vit)
                {
                    vertex_id id = vit->id;
                    while(id != this->parnt[id])
                    { id = this->parnt[id]; ++depth[vit->id]; }
                }
            }
            
        public:        
            STree(graph_type& graph, arc_list arcs)
                : arc_ids{}, graph{&graph}, 
                  n_vertices{graph.num_vertices()},
                  parnt(n_vertices,vertex_id{}), 
                  depth(n_vertices,0), max_depth{-1}
            {
                for(arc_type& arc : arcs)
                    this->arc_ids.emplace_back(arc.beg,arc.end);
                this->calculate_parents(); 
                this->calculate_depth(); 
            }
            
            STree(const arc_type& out_arc_id, const arc_type& in_arc_id,
                  const STree& pattern)
                : arc_ids{}, graph{pattern.graph}, 
                  n_vertices{pattern.n_vertices},
                  parnt(n_vertices,vertex_id{}), 
                  depth(n_vertices,0), max_depth{-1}
            {
                arc_type& out_arc 
                    = graph::arc(out_arc_id.beg,out_arc_id.end,*graph);
                arc_type& in_arc 
                    = graph::arc(in_arc_id.beg,in_arc_id.end,*graph);
                
                std::cerr << "STree OUT_ARC" << out_arc;
                std::cerr << "STree IN_ARC " << in_arc;
                std::cerr << "Old arcs: " << std::endl;
                for(auto& a : pattern.arc_ids)
                    std::cerr << a << " ";
                std::cerr << std::endl;
                
                for(auto& aid : pattern.arc_ids)
                {
                    std::cerr << "Out arc is: " << out_arc.id << std::endl;
                    std::cerr << "This arc is: " << aid << std::endl;
                    if(aid != out_arc.id)
                    { 
                        this->arc_ids.emplace_back(aid);
                        std::cerr << "Will put!" << std::endl;
                    }
                }
                
                this->arc_ids.emplace_back(in_arc.beg,in_arc.end);
                
                std::cerr << "New arcs: " << std::endl;
                for(auto& a : this->arc_ids)
                    std::cerr << a << " ";
                std::cerr << std::endl;
                
                this->calculate_parents(); 
                std::cerr << "Parent: ";
                for(auto& id : this->parnt)
                    std::cerr << id << " ";
                std::cerr << std::endl;
                
                this->calculate_depth();
                std::cerr << "Depth: ";
                for(auto& d : this->depth)
                    std::cerr << d << " ";
                std::cerr << std::endl;
            }
            
            graph_type& base_graph() { return *graph; }
            const graph_type& base_graph() const { return *base_graph; }
            
            vertex_id parent(vertex_id son) const
            { 
                return parnt[son]; 
            }
            
            cycle_type fundamental_cycle(const arc_type& inserted)
            {
                cycle_type cycle { inserted.id }, lcycle {};
                vertex_id l { inserted.beg }; 
                vertex_id r { inserted.end };
                int max_depth = depth[inserted.beg]; 
                
                if(depth[inserted.end] > max_depth) 
                    max_depth = depth[inserted.beg];
                l = inserted.beg; r = inserted.end;
                
                std::cout << "Cycle: max depth: " << max_depth << std::endl;
                
                std::cerr << "Cycle: equaling heights" << std::endl;
                std::cerr << "Cycle: parnt: ";
                for(auto& a : parnt) std::cerr << a << " ";
                std::cerr << std::endl;
                // Intermediate arc_ids 
                // (when with different depths)
                std::cerr << "Cycle: left" << std::endl;
                for(vertex_id lp = parnt[l]; depth[l] != max_depth; l = lp)
                {
                    lp = parnt[l];
                    lcycle.emplace_front(lp,l);
                }
                std::cerr << "Cycle: right" << std::endl;
                for(vertex_id rp = parnt[r]; depth[r] != max_depth; r = rp)
                {
                    rp = parnt[r];
                    std::cerr << "  Cycle: " << r << " " << rp << std::endl;
                    cycle.emplace_back(rp,r);
                }
                
                std::cerr << "Cycle: going up" << std::endl;
                while(r != l)
                {
                    // Intermediate arc_ids
                    // (when with the same depth)
                    vertex_id lp = parnt[l];
                    vertex_id rp = parnt[r];
                    std::cerr << "Cycle: parnt[l]: " << lp 
                              << " parnt[r]: " << rp << std::endl;
                    lcycle.emplace_front(lp,l);
                    cycle.emplace_back(rp,r);
                    r = rp; l = lp;
                }
                
                // Put the arc_ids such that the first arc of the
                // cycle to be returned is the arc which generated
                // the fundamental cycle
                for(arc_id& aid : lcycle) cycle.push_back(aid);
                // cycle.insert(std::begin(lcycle), std::end(lcycle));
                
                return cycle;
            }
            
            vertex_size num_vertices() const 
            { return this->n_vertices; }
            
            friend std::ostream& 
            operator<<(std::ostream& os, const STree& st)
            {
                for(int i = 0; i <= st.max_depth; i++)
                {
                    for(int d : st.depth)
                        if(d == i) os << d << " ";
                    os << std::endl;
                }
                return os;
            }
    };
    
    template<typename STree>
        typename STree::vertex_size num_vertices(const STree st)
    {
        return st.num_vertices();
    }
    
    template<typename STree>
        typename STree::vertex_id
        parent(typename STree::vertex_id vid, STree st)
    {
        return st.parent(vid);
    }
    
    template<typename STree>
        typename STree::arc_type&
        edge(typename STree::vertex_id v1, 
             typename STree::vertex_id v2, STree st)
    {
        try { return arc(v1,v2,st.base_graph()); }
        catch(graph::arc_not_found) {}
        try { return arc(v2,v1,st.base_graph()); }
        catch(graph::arc_not_found) {}
        throw graph::arc_not_found{};
    }
}

#endif
