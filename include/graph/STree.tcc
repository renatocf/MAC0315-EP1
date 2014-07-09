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

namespace graph 
{
    template<
        typename Graph,
        typename Cycle = graph::Cycle<typename Graph::arc_type>
    >class STree
    {
        public:
            typedef typename Graph::arc_type    arc_type;
            typedef typename Graph::arc_list    arc_list;
            typedef typename Graph::vertex_id   vertex_id;
            typedef typename Graph::vertex_type vertex_type;
            
        private:
            Graph*                 base;
            std::vector<vertex_id> parnt;
            std::vector<int>       depth;
            vertex_id              radix;
            int                    max_depth;
            
        public:        
            STree(Graph& base, size_t num_vertices, 
                  vertex_id radix, arc_list arcs)
                : base{&base}, parnt(num_vertices), 
                  depth(num_vertices,-1), radix{}, max_depth{-1}
            {
                for(const arc_type& arc : arcs)
                {
                    if(depth[arc.beg] == -1
                    && depth[arc.end] != -1)
                    {
                        radix = arc.beg;
                        parnt[arc.beg] = arc.end;
                        depth[arc.beg] = depth[arc.end]+1;
                    }
                    else if(depth[arc.beg] != -1
                         && depth[arc.end] == -1)
                    {
                        parnt[arc.end] = arc.beg;
                        depth[arc.end] = depth[arc.beg]+1;
                    }
                    else if(depth[arc.beg] == -1 
                         && depth[arc.end] == -1)
                    {
                        parnt[arc.end] = arc.beg;
                        depth[arc.beg] = 0;
                        depth[arc.end] = 1;
                    }
                    for(int d : depth) 
                        if(d > max_depth) max_depth = d;
                }
            }
            
            Cycle fundamental_cycle(const arc_type& inserted)
            {
                Cycle cycle { inserted };
                vertex_id l { inserted.beg }; 
                vertex_id r { inserted.end };
                int depth = depth[inserted.beg()]; 
                
                if(depth[inserted.end()] > depth) 
                    depth = depth[inserted.beg()];
                l = inserted.beg; r = inserted.end;
                
                // Intermediate arcs 
                // (when with different depths)
                for(vertex_id lp = parnt[l]; depth[l] != depth; l = lp)
                    cycle.push_back( arc_type{lp,l} );
                for(vertex_id rp = parnt[r]; depth[r] != depth; r = rp)
                    cycle.push_front( arc_type{rp,r} );
                
                while(r != l)
                {
                    // Intermediate arcs
                    // (when with the same depth)
                    vertex_id lp = parnt[l];
                    vertex_id rp = parnt[r];
                    cycle.push_back ( arc_type{lp,l} );
                    cycle.push_front( arc_type{rp,r} );
                    r = rp; l = lp;
                }
                
                // Last arc
                cycle.push_back({parnt[l],l});
                
                return cycle;
            }
            
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
}

#endif
