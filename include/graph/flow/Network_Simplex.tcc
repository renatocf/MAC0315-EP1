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

#include "graph/flow/Arc.tcc"
#include "graph/flow/Vertex.tcc"
#include "graph/STree.tcc"

namespace graph {
namespace flow 
{
    template<
        typename Graph = graph::Adjacency_list
            <directed,flow::Vertex<>::type,flow::Arc<>::type>,
        typename STree = graph::STree<Graph>
    >
    void network_simplex_algorithm(Graph G, STree initial)
    {
        
    }
}}

#endif
