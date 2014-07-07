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
#include "Tags.tcc"
#include "Vertex.tcc"
#include "Properties.tcc"

namespace graph 
{
    template<
        typename Vertex     = graph::Vertex<>,
        typename Properties = no_property,
        typename Directed   = directed
    >class Arc
    {
        public:
            typedef Arc*                          id_type;
            typedef Vertex                        vertex_type;
            typedef Properties                    property_type;
            typedef typename vertex_type::id_type vertex_id;
            
            const id_type   id;
            vertex_id       beg;
            vertex_id       end;
            property_type   properties;
            
            explicit
            Arc(const vertex_type& beg, const vertex_type& end, 
                const property_type properties = property_type{})
                : id{this}, beg{beg.id}, end{end.id},
                  properties{properties} {}
            
            explicit
            Arc(const vertex_id beg, const vertex_id end, 
                const property_type properties = property_type{})
                : id{this}, beg{beg}, end{end},
                  properties{properties} {}
            
            Arc(const Arc& p)
                : id{this}, beg{p.beg}, end{p.end},
                  properties{p.properties} {}
            
            Arc(const Arc&& p)
                : id{this}, beg{std::move(p.beg)}, end{std::move(p.end)},
                  properties{std::move(p.properties)} {}
            
            Arc& operator=(const Arc& copied)
            {
                this->beg = copied.beg; this->end = copied.end;
                this->properties = std::move(copied.properties);
                return *this;
            }

            Arc& operator=(const Arc&& moved)
            {
                this->beg = std::move(moved.beg); 
                this->end = std::move(moved.end);
                this->properties = std::move(moved.properties);
                return *this;
            }
            
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
