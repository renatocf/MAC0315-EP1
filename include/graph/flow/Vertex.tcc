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
        typename Demand      = int
    >struct Vertex_flow
    {
        Demand     demand     = {};
        Properties properties = {};
        
        Vertex_flow(Demand d = {}, Properties p = {})
            : demand{d}, properties{p} {}
        
        bool operator==(const Vertex_flow& f) const
        {
            return this->demand     == f.demand
                && this->properties == f.properties;
        }
        
        bool operator!=(const Vertex_flow& f) const
        {
            return !operator==(f);
        }
        
        friend std::ostream& 
        operator<<(std::ostream& os, const Vertex_flow& v)
        {
            os << "{ ";
            if(v.demand) 
                os << "demand:" << std::setw(3) << v.demand << ", ";
            os << "properties:" << v.properties << " }";
            return os;
        }
    };
    
    template<
        typename Properties  = graph::no_property,
        typename Id          = unsigned long
    >using Vertex = graph::Vertex<Vertex_flow<Properties>,Id>;
}}

#endif
