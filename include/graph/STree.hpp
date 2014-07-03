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
        private:
            std::vector<typename Vertex::id_type> parnt {};
            
        public:        
            STree(size_t n_vertex, std::initializer_list<Arc> arcs)
                : parnt{n_vertex}
            {
                for(const Arc& arc : arcs)
                    parnt[arc.beg.id] = arc.end.id;
            }
            
            using vertex_type = Vertex;
            using arc_type    = Arc;
            
            Cycle fundamental_cycle(const Arc& inserted)
            {
                Cycle cycle { inserted };
                Vertex l    { inserted.beg }; 
                Vertex r    { inserted.end };
                int depth = inserted.beg.depth; 
                
                if(inserted.end.depth > depth) 
                    depth = inserted.beg.depth;
                l = inserted.beg; r = inserted.end;
                
                // Intermediate arcs 
                // (when with different depths)
                for(Vertex lp = this->parent(l); l.depth != depth; l = lp)
                    cycle.push_back( Arc{lp,l} );
                for(Vertex rp = this->parent(l); r.depth != depth; r = rp)
                    cycle.push_front( Arc{rp,r} );
                
                while(r != l)
                {
                    // Intermediate arcs 
                    // (when with the same depth)
                    Vertex lp = this->parent(l);
                    Vertex rp = this->parent(r);
                    cycle.push_back ( Arc{lp,l} );
                    cycle.push_front( Arc{rp,r} );
                    r = rp; l = lp;
                }
                
                // Last arc
                cycle.push_back({this->parent(l),l});
                
                return cycle;
            }
    };
}

#endif
