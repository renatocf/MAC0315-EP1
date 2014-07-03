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

#ifndef TCC_GRAPH_DIGRAPH_DEFINED
#define TCC_GRAPH_DIGRAPH_DEFINED

// Default libraries
#include <vector>

// Libraries
#include "Arc.tcc"
#include "Tags.tcc"
#include "Vertex.tcc"
#include "Arc_list.tcc"
#include "Vertex_list.tcc"
#include "Adjacency_list.tcc"

namespace graph 
{
    template<
        typename Vertex      = Vertex<>,
        typename Arc         = Arc<Vertex>,
        typename Properties  = no_property,
        typename Vertex_list = Vertex_list<Vertex>,
        typename Arc_list    = Arc_list<Arc>,
        typename Structure   = Adjacency_list
                               <directed,Vertex_list,Arc_list>
    >class Digraph
    {
        private:
            using property_type        = Properties;
            using arc_property_type    = typename Arc::property_type;
            using vertex_property_type = typename Vertex::property_type;
            
            Structure     digraph    {};
            property_type properties {};
            unsigned long vertices   {};
            unsigned long arcs       {};

        public:
            Digraph(Vertex_list vl = {}, Arc_list al = {}, 
                    property_type properties = {})
                : digraph{vl,al}, properties{properties},
                  vertices{vl.size()}, arcs{al.size()} {}
    };
}

#endif
