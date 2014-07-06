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
        typename Properties = no_property,
        typename Directed   = directed
    >struct Arc
    {
        typedef Vertex     vertex_type;
        typedef Properties property_type;
        
        const Vertex& beg;
        const Vertex& end;
        property_type properties;
        
        explicit
        Arc(const Vertex& beg, const Vertex& end, 
            const property_type properties = property_type{})
            : beg{beg}, end{end}, properties{properties} {}
        
        // Copy constructor and assignment
        Arc(Arc& copied)
            : beg{copied.beg}, end{copied.end}, properties{properties} {}
        Arc(const Arc& copied)
            : beg{copied.beg}, end{copied.end}, properties{properties} {}
        
        Arc& operator=(Arc& copied) { return *this; }
        const Arc& operator=(const Arc& copied) const { return *this; }
        
        // Move constructor and assignment
        Arc(Arc&& moved) 
            : beg{moved.beg}, end{moved.end}, 
              properties{std::move(properties)} {}
        Arc(const Arc&& moved) 
            : beg{moved.beg}, end{moved.end}, 
              properties{std::move(properties)} {}
        
        Arc& operator=(Arc&& moved) { return *this; }
        const Arc& operator=(const Arc&& moved) const { return *this; }
        
        // Comparison operators
        bool operator==(const Arc& a) const
        {
            return this->beg        == a.beg
                && this->end        == a.end
                && this->properties == a.properties;
        }
        
        bool operator!=(const Arc& a) const
        {
            return !operator==(a);
        }
        
        friend std::ostream& 
        operator<<(std::ostream& os, const Arc& a)
        {
            os << "{ beg:" << a.beg << ", end:" << a.end;
            os << ", properties:" << a.properties << " }";
            return os;
        }
    };
}

#endif
