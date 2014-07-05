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
        typename Properties  = graph::no_property,
        typename Cost        = unsigned int,
        typename Flux        = unsigned int,
        typename Capacity    = unsigned int,
        typename Requirement = unsigned int
    >struct Arc_flow
    {
        Cost        cost;
        Flux        flux;
        Capacity    capacity;
        Requirement requirement;
        Properties  properties;
        
        Arc_flow(
            Cost k = Cost{}, Flux f = Flux{}, Capacity c = Capacity{}, 
            Requirement r = Requirement{}, Properties p = Properties{})
            : cost{k}, flux{f}, capacity{c}, 
              requirement{r}, properties{p} {}
        
        bool operator==(const Arc_flow& f) const
        {
            return this->cost        == f.cost
                && this->flux        == f.flux
                && this->capacity    == f.capacity
                && this->requirement == f.requirement
                && this->properties  == f.properties;
        }
        
        bool operator!=(const Arc_flow& f) const
        {
            return !operator==(f);
        }
        
        friend std::ostream& 
        operator<<(std::ostream& os, const Arc_flow& f)
        {
            os << "{ ";
                           os << "cost:"          << f.cost;
                           os << "flux:"          << f.flux;
            if(f.capacity) os << ", capacity:"    << f.capacity; 
            if(f.flux)     os << ", requirement:" << f.flux;
                           os << ", properties:"  << f.properties;
            os << " }";
            return os;
        }
    };
    
    template<
        typename Vertex     = graph::flow::Vertex<>::type,
        typename Properties = graph::no_property,
        typename Directed   = graph::directed
    >struct Arc
    { typedef graph::Arc<Vertex,Arc_flow<Properties>,Directed> type; };
}}

#endif
