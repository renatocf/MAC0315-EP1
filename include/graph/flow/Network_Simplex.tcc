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

#ifndef TCC_GRAPH_FLOW_NETWORK_SIMPLEX_DEFINED
#define TCC_GRAPH_FLOW_NETWORK_SIMPLEX_DEFINED

// Default libraries
#include <tuple>
#include <limits>
#include <iostream>

// Libraries
#include "graph/flow/Arc.tcc"
#include "graph/flow/Vertex.tcc"
#include "graph/Traits.tcc"
#include "graph/STree.tcc"
#include "graph/stree_search.tcc"

namespace graph {
namespace flow 
{
    template<typename STree>
    struct prices_calculator : public STvisitor<STree>
    {
        typedef STree stree_type;
        typedef typename stree_type::vertex_id vertex_id;
        typedef typename stree_type::arc_type  arc_type;
        typedef typename arc_type::property_type::cost_type cost_type;
        
        std::vector<cost_type> price;
        
        prices_calculator(stree_type st)
            : price(num_vertices(st),std::numeric_limits<cost_type>::max())
        {}
        
        void end_visits(vertex_id id, stree_type st)
        {
            arc_type  arc  = edge(id,parent(id,st),st);
            cost_type cost = arc.properties.cost;
            
            price[id] = price[parent(id,st)]
                      + (arc.end == id ? cost : -cost);
        }
        
        bool visit_parent_cond(vertex_id id, stree_type st)
        { 
            return price[parent(id,st)] ==
                   std::numeric_limits<cost_type>::max();
        }
        
        void radix(vertex_id id, stree_type st) { price[id] = 0; }
        
        void start_vertex  (vertex_id id, stree_type st) {}
        void before_parent (vertex_id id, stree_type st) {}
        void after_parent  (vertex_id id, stree_type st) {}
    };
    
    template<
        typename Graph = graph::Adjacency_list
            <directed,flow::Vertex<>::type,flow::Arc<>::type>,
        typename STree = graph::STree<Graph>
    >void network_simplex_algorithm(Graph g, STree initial)
    {
        // Step 1: Find prices (y)
        std::vector<int> y ( graph::num_vertices(g), 
                             std::numeric_limits<int>::max() );
        
        typedef typename Graph::vertex_id vertex_id;
        typedef typename Graph::arc_type arc_type;
        typedef typename Graph::arc_property
            ::flux_type flux_type;
        
        typedef typename STree::cycle_type cycle_type;
        
        typedef typename graph_traits<Graph>
            ::arc_iterator arc_iterator;
        
        prices_calculator<STree> prices{initial};
        stree_search(prices,initial);
        
        for(auto& p : prices.price) std::cout << p << " "; 
        std::cout << std::endl;

        // Step 2: Find (i,j) : y[i] + c[i,j] < y[j]
        arc_iterator ait, ait_end;
        for(std::tie(ait,ait_end) = arcs(g); ait != ait_end; ++ait)
        {
            if(prices.price[ait->beg] + ait->properties.cost
               < prices.price[ait->end]) break;
        }

        // if(ait == ait_end) TERMINOU!!!
        arc_type in_arc { *ait };
        std::cout << "IN_ARC: " << in_arc << std::endl;

        // Step 3: Find fundamental cycle
        cycle_type cycle { initial.fundamental_cycle(in_arc) };
        for(arc_type& a : cycle)
            std::cout << a << std::endl;

        // Step 4: Find arc to be removed
        flux_type min_delta { cycle.front().properties.capacity };
        arc_type  out_arc   { cycle.front()                     };
        vertex_id pivot     { cycle.front().beg                 };
        for(arc_type& arc : cycle)
        {
            flux_type delta;
            if(pivot == arc.beg)
                delta = arc.properties.capacity - arc.properties.flux;
            else // pivot == arc.end
                delta = arc.properties.flux - arc.properties.requirement;

            if(delta < min_delta) { min_delta = delta; out_arc = arc; }
            pivot = arc.end;
        }
        std::cout << min_delta << std::endl;

        pivot = cycle.front().beg;
        for(arc_type& arc : cycle)
        {
            if(pivot == arc.beg) arc.properties.flux += min_delta;
            if(pivot == arc.end) arc.properties.flux -= min_delta;
        }
        std::cout << "OUT_ARC: " << in_arc << std::endl;

        if(out_arc == in_arc)
            /*return*/ network_simplex_algorithm(g, initial);
        else
        {
            // STree modified { out_arc,in_arc,initial };
            // network_simplex_algorithm(g, modified);
        }
    }
}}

#endif
