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
        {std::cerr << "Num_vertices st: " << num_vertices(st) << std::endl;}
        
        void end_visits(vertex_id id, stree_type st)
        {
            std::cerr << "Visiting end arc ";
            arc_type  arc  = edge(id,parent(id,st),st);
            std::cerr << arc;
            cost_type cost = arc.properties.cost;
            std::cerr << "..." << std::endl;
            
            price[id] = price[parent(id,st)]
                      + (arc.end == id ? cost : -cost);
            
            std::cerr << "Prices: ";
            for(auto& p : price) std::cerr << p << " ";
            std::cerr << std::endl;
        }
        
        bool visit_parent_cond(vertex_id id, stree_type st)
        { 
            bool b =  price[parent(id,st)] ==
                      std::numeric_limits<cost_type>::max();
            if(b) std::cerr << "== RECURSIVE CALL == ";
            return b;
        }
        
        void radix(vertex_id id, stree_type st) 
        { 
            std::cerr << "Visiting radix..." << std::endl;
            price[id] = 0; 
        }
        
        void start_vertex(vertex_id id, stree_type st) 
        {
            std::cerr << "Starting vertices..." << std::endl;
        }
        void before_parent (vertex_id id, stree_type st) {}
        void after_parent  (vertex_id id, stree_type st) {}
    };
    
    template<
        typename Graph = graph::Adjacency_list
            <directed,flow::Vertex<>::type,flow::Arc<>::type>,
        typename STree = graph::STree<Graph>
    >STree network_simplex_algorithm(Graph& g, STree& initial)
    {
        typedef typename Graph::vertex_id vertex_id;
        typedef typename Graph::arc_type arc_type;
        typedef typename Graph::arc_property
            ::flux_type flux_type;
        
        typedef typename STree::cycle_type cycle_type;
        
        typedef typename graph_traits<Graph>
            ::arc_iterator arc_iterator;
        
        std::cerr << ">>> STARTING SIMPLEX ITERATION <<<" << std::endl;
        
        // Step 1: Find prices (y)
        std::cerr << ">> FINDING PRICES <<" << std::endl;
        prices_calculator<STree> prices{initial};
        std::cerr << "> SEARCH <" << std::endl;
        stree_search(prices,initial);
        
        std::cerr << "Prices: ";
        for(auto& p : prices.price) std::cerr << p << " "; 
        std::cerr << std::endl;

        // Step 2: Find (i,j) : y[i] + c[i,j] < y[j]
        std::cerr << ">> FINDING IN_ARC <<" << std::endl;
        arc_iterator ait, ait_end;
        for(std::tie(ait,ait_end) = arcs(g); ait != ait_end; ++ait)
        {
            if(prices.price[ait->beg] + ait->properties.cost
               < prices.price[ait->end]) break;
        }

        if(ait == ait_end) { std::cerr << ">> FINISH! <<" << std::endl;
                             return initial; }
        arc_type in_arc { *ait };
        std::cerr << "IN_ARC: " << in_arc << std::endl;

        // Step 3: Find fundamental cycle
        std::cerr << ">> FINDING CYCLE <<" << std::endl;
        cycle_type cycle { initial.fundamental_cycle(in_arc) };
        for(arc_type& a : cycle)
            std::cerr << a << std::endl;

        // Step 4: Find arc to be removed
        std::cerr << ">> FINDING OUT_ARC <<" << std::endl;
        flux_type min_delta { cycle.front().properties.capacity };
        arc_type  out_arc   { cycle.front()                     };
        vertex_id pivot     { cycle.front().beg                 };
        for(arc_type& arc : cycle)
        {
            flux_type delta;
            if(pivot == arc.beg)
            {
                // Arc in cycle direction
                delta = arc.properties.capacity - arc.properties.flux;
                pivot = arc.end;
            }
            else // pivot == arc.end
            {
                // Arc against cycle direction
                delta = arc.properties.flux - arc.properties.requirement;
                pivot = arc.beg;
            }

            if(delta < min_delta) { min_delta = delta; out_arc = arc; }
        }
        std::cerr << "MIN_DELTA: " << min_delta << std::endl;

        pivot = cycle.front().beg;
        for(arc_type& arc : cycle)
        {
            if(pivot == arc.beg)
            {
                graph::arc(arc.beg,arc.end,g).properties.flux += min_delta; 
                pivot = arc.end;
            }
            if(pivot == arc.end)
            { 
                graph::arc(arc.beg,arc.end,g).properties.flux -= min_delta; 
                pivot = arc.beg; 
            }
        }
        
        std::cerr << "PRINTING TREE: " << std::endl;
        std::cerr << g << out_arc << std::endl;
        
        std::cerr << "OUT_ARC: " << out_arc << std::endl;

        if(out_arc == in_arc)
            return network_simplex_algorithm(g, initial);
        else
        {
            STree modified { out_arc,in_arc,initial };
            return network_simplex_algorithm(g, modified);
        }
    }
}}

#endif
