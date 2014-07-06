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
        typename Vertex = graph::Vertex<>,
        typename Arc    = graph::Arc<Vertex>,
        typename Cycle  = graph::Cycle<Arc>
    >class STree
    {
        public:
            typedef Vertex                   vertex_type;
            typedef Arc                      arc_type;
            typedef typename Vertex::id_type vertex_id;
            
        private:
            std::vector<vertex_id> parnt;
            std::vector<int>       depth;
            vertex_id              radix;
            int                    max_depth;
            
        public:        
            STree(size_t num_vertices, vertex_id radix, 
                  std::initializer_list<Arc> arcs)
                : parnt(num_vertices), 
                  depth(num_vertices,-1), 
                  radix{}, max_depth{-1}
            {
                for(const Arc& arc : arcs)
                {
                    if(depth[arc.beg().id] == -1
                    && depth[arc.end().id] != -1)
                    {
                        radix = arc.beg().id;
                        parnt[arc.beg().id] = arc.end().id;
                        depth[arc.beg().id] = depth[arc.end().id]+1;
                    }
                    else if(depth[arc.beg().id] != -1
                         && depth[arc.end().id] == -1)
                    {
                        parnt[arc.end().id] = arc.beg().id;
                        depth[arc.end().id] = depth[arc.beg().id]+1;
                    }
                    else if(depth[arc.beg().id] == -1 
                         && depth[arc.end().id] == -1)
                    {
                        parnt[arc.end().id] = arc.beg().id;
                        depth[arc.beg().id] = 0;
                        depth[arc.end().id] = 1;
                    }
                    for(int d : depth) 
                        if(d > max_depth) max_depth = d;
                }
            }
            
            Cycle fundamental_cycle(const Arc& inserted)
            {
                Cycle cycle { inserted };
                vertex_id l { inserted.beg().id }; 
                vertex_id r { inserted.end().id };
                int depth = depth[inserted.beg()]; 
                
                if(depth[inserted.end()] > depth) 
                    depth = depth[inserted.beg()];
                l = inserted.beg().id; r = inserted.end().id;
                
                // Intermediate arcs 
                // (when with different depths)
                for(vertex_id lp = parnt[l]; depth[l] != depth; l = lp)
                    cycle.push_back( Arc{lp,l} );
                for(vertex_id rp = parnt[r]; depth[r] != depth; r = rp)
                    cycle.push_front( Arc{rp,r} );
                
                while(r != l)
                {
                    // Intermediate arcs
                    // (when with the same depth)
                    vertex_id lp = parnt[l];
                    vertex_id rp = parnt[r];
                    cycle.push_back ( Arc{lp,l} );
                    cycle.push_front( Arc{rp,r} );
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
