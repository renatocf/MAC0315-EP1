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

#ifndef TCC_GRAPH_FLOW_ARC_DEFINED
#define TCC_GRAPH_FLOW_ARC_DEFINED

// Default libraries
#include <iomanip>

// Libraries
#include "Vertex.tcc"
#include "graph/Arc.tcc"

namespace graph {
namespace flow 
{
    template<
        typename Vertex     = graph::flow::Vertex<>,
        typename Properties = graph::no_property,
        typename Demand     = int
    >struct Arc : public graph::Arc<Vertex,Properties>
    {
        using property_type = Properties;
        
        Demand demand = {};
        
        Arc(const Vertex& beg, const Vertex& end, 
            Demand demand = {}, const property_type properties = {})
            : graph::Arc<Vertex,Properties>{beg,end,properties}, 
              demand{demand} {}
        
        friend std::ostream& 
        operator<<(std::ostream& os, const Arc& a)
        {
            os << "{ beg:" << a.beg << ", end:" << a.end;
            if(a.demand) os << ", demand:" << std::setw(3) << a.demand;
            os << ", properties:" << a.properties << " }";
            return os;
        }
    };
}}

#endif
