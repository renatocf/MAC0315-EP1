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

// Default library
#include <utility>

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
            typedef Vertex                         vertex_type;
            typedef typename vertex_type::id_type  vertex_id;
                                                   
            struct id_type 
            { 
                vertex_id beg, end; 
                id_type(const vertex_id& beg, const vertex_id& end)
                    : beg{beg}, end{end} {}
                
                bool operator==(const id_type& id) const
                { return this->beg == id.beg && this->end == id.end; }
                
                bool operator!=(const id_type& id) const
                { return !operator==(id); }
                
                friend std::ostream& 
                operator<<(std::ostream& os, const id_type& id)
                { os << "(" << id.beg << "," << id.end << ")"; return os; }
            };
            
            typedef Properties                     property_type;
            typedef size_t                         size_type;
            
            id_type       id;
            vertex_id&    beg, end;
            property_type properties;
            
            explicit
            Arc(const id_type& base_id,
                const property_type properties = property_type{})
                : id{base_id}, beg{id.beg}, end{id.end},
                  properties{properties} {}
            
            explicit
            Arc(const vertex_type& beg, const vertex_type& end, 
                const property_type properties = property_type{})
                : id{beg.id,end.id}, beg{id.beg}, end{id.end},
                  properties{properties} {}
            
            explicit
            Arc(const vertex_id beg, const vertex_id end, 
                const property_type properties = property_type{})
                : id{beg,end}, beg{id.beg}, end{id.end},
                  properties{properties} {}
            
            Arc(const Arc& p)
                : id{p.id}, beg{id.beg}, end{id.end},
                  properties{p.properties} {}
            
            Arc(const Arc&& p)
                : id{std::move(p.id)}, beg{id.beg}, end{id.end},
                  properties{std::move(p.properties)} {}
            
            Arc& operator=(const Arc& copied)
            {
                this->id  = copied.id;
                this->properties = copied.properties;
                return *this;
            }

            Arc& operator=(const Arc&& moved)
            {
                this->id = std::move(moved.id); 
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
