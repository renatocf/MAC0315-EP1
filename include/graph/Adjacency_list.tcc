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
#include <vector>
#include <initializer_list>

// Libraries
#include "Vertex.tcc"
#include "Arc.tcc"

namespace graph 
{
    template<
        typename Arc = graph::Arc<>,
        typename Col = std::list<Arc>,
        typename Row = std::vector<Col>
    >class Adjacency_list
    {
        private:
            Row adj_list {};
        
        public:
            Adjacency_list(std::initializer_list<Arc> arcs)
            {
                for(auto& arc : arcs)
                    adj_list[arc.beg.id].push_back(arc);
            }
    };
}

#endif
