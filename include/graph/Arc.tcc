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

#ifndef TCC_GRAPH_ARC_DEFINED
#define TCC_GRAPH_ARC_DEFINED

// Libraries
#include "Vertex.tcc"

namespace graph 
{
    template<
        typename Vertex     = graph::Vertex<>,
        typename Properties = no_property
    >struct Arc
    {
        using vertex_type   = Vertex;
        using property_type = Properties;
        
        const Vertex& beg;
        const Vertex& end;
        property_type properties {};
        
        Arc(const Vertex& beg, const Vertex& end, 
            const property_type properties = {})
            : beg{beg}, end{end}, properties{properties} {}
    };
}

#endif
