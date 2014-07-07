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

#ifndef TCC_GRAPH_VERTEX_DEFINED
#define TCC_GRAPH_VERTEX_DEFINED

// Libraries
#include "Properties.tcc"

namespace graph
{
    template<
        typename Properties = no_property
    >struct Vertex
    {
        typedef unsigned long id_type;
        typedef Properties    property_type;
        
        id_type       id;
        property_type properties;
        
        explicit
        Vertex(const id_type id, 
               property_type properties = property_type{})
            : id{id}, properties{properties} {}
        
        explicit
        Vertex(const id_type id, const Vertex& prototype)
            : id{id}, properties{prototype.properties} {}
        
        bool operator==(const Vertex& v) const
        {
            return this->id == v.id 
                && this->properties == v.properties;
        }
        
        bool operator!=(const Vertex& v) const
        {
            return !operator==(v);
        }
        
        friend std::ostream& 
        operator<<(std::ostream& os, const Vertex& v)
        {
            os << "{" << v.id << ", " << v.properties << "}";
            return os;
        }
    };
}

#endif
