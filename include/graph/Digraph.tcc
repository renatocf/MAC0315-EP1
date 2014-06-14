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
#include "Vertex.tcc"
#include "Arc_list.tcc"
#include "Properties.tcc"
#include "Vertex_list.tcc"
#include "Adjacency_list.tcc"

namespace graph 
{
    template<
        typename Vertex     = graph::Vertex<>,
        typename Arc        = graph::Arc<>,
        typename Structure  = graph::Adjacency_list<>,
        typename Properties = no_property
    >class Digraph
    {
        private:
            using property_type        = Properties;
            using arc_property_type    = typename Arc::property_type;
            using vertex_property_type = typename Vertex::property_type;
            
            Vertex_list<vertex_property_type> vl {};
            Arc_list<Arc> al   {};
            Structure    data {};
            
            property_type properties   {};
            
            unsigned long vertices   {};
            unsigned long arcs       {};

        public:
            Digraph(Vertex_list<vertex_property_type> vl = {}, 
                    Arc_list<Arc> al = {}, property_type properties = {})
                : vl{vl}, al{al}, properties{properties},
                  vertices{vl.size()}, arcs{al.size()} {}
    };   
}

#endif
