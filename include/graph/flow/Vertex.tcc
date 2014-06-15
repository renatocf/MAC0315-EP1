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

#ifndef TCC_GRAPH_FLOW_VERTEX_DEFINED
#define TCC_GRAPH_FLOW_VERTEX_DEFINED

// Libraries
#include "graph/Vertex.tcc"

namespace graph {
namespace flow 
{
    template<
        typename Properties  = graph::no_property,
        typename Flux        = unsigned int,
        typename Capacity    = unsigned int,
        typename Requirement = unsigned int,
        typename Id          = unsigned long
    >struct Vertex : public graph::Vertex<Properties, Id>
    {
        using id_type       = Id;
        using property_type = Properties;

        Flux flux               = {};
        Capacity capacity       = {};
        Requirement requirement = {};

        Vertex(const Id id, Flux f = {}, Capacity c = {},
               Requirement r = {}, property_type properties = {})
            : graph::Vertex<Properties,Id>{id,properties},
              flux{f}, capacity{c}, requirement{r} {}
        
        Vertex(const Id id, const Vertex& prototype)
            : graph::Vertex<Properties,Id>{id,prototype.properties},
              flux{prototype.flux}, capacity{prototype.capacity},
              requirement{prototype.requirement} {}
        
        friend std::ostream& 
        operator<<(std::ostream& os, const Vertex& v)
        {
            os << "{ id:" << v.id << ", flux:" << v.flux;
            if(v.capacity) os << ", capacity:"    << v.capacity; 
            if(v.flux)     os << ", requirement:" << v.flux;
            os << ", properties:" << v.properties << " }";
            return os;
        }
    };
}}

#endif
