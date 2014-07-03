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

#ifndef TCC_GRAPH_ARC_LIST_DEFINED
#define TCC_GRAPH_ARC_LIST_DEFINED

// Default libraries
#include <vector>

// Libraries
#include "Arc.tcc"

namespace graph 
{
    template<
        typename Arc = graph::Arc<>
    >class Arc_list
    {
        private:
            std::vector<Arc> al;
        
        public:
            using arc_type      = Arc;
            using vertex_type   = typename Arc::vertex_type;
            
            // Iterators
            using iterator =
            typename std::vector<Arc>::iterator;
            using const_iterator =
            typename std::vector<Arc>::const_iterator;
            
            Arc_list(std::initializer_list<Arc> arcs)
                : al{arcs} {}
            
            unsigned long size() { return al.size(); }
            
            friend std::ostream& 
            operator<<(std::ostream& os, const Arc_list<Arc>& al)
            {
                for(auto it = al.begin(); it != al.end(); ++it)
                    os << *it << endl;
                return os;
            }
            
            // Iterators
            iterator begin() { return al.begin(); }
            iterator end()   { return al.end();   }
            
            const_iterator begin() const { return al.begin(); }
            const_iterator end()   const { return al.end();   }
    };
}

#endif
