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

#ifndef HPP_GRAPH_ADJACENCY_LIST_DEFINED
#define HPP_GRAPH_ADJACENCY_LIST_DEFINED

// Default libraries
#include <list>
#include <vector>
#include <initializer_list>

namespace graph 
{
    class Vertex
    {
        private:
            static int g_id;
        
        public:
            const int id;
            
            Vertex()
                : id{g_id++} {}
    };
    
    template<typename V = graph::Vertex>
    class Arc
    {
        public: 
            const V& beg;
            const V& end;
            const int id;
            
            Arc(const V& beg, const V& end, const int id = {})
                : beg{beg}, end{end}, id{id} {}
    };
    
    template<typename V = graph::Vertex, 
             typename A = graph::Arc<>,
             template<class C> class Row = std::vector,
             template<class A> class Col = std::list >
    class Adjacency_list
    {
        private:
            Row<Col<A>> adj_list {};
        
        public:
            Adjacency_list(std::initializer_list<A> arcs)
            {
                for(auto& arc : arcs)
                    adj_list[arcs.beg.id].push_back(arc);
            }
    };
}

#endif
