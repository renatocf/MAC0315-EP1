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

#ifndef TCC_GRAPH_FUNCTIONS_DEFINED
#define TCC_GRAPH_FUNCTIONS_DEFINED

namespace graph 
{
    // Vertex
    template<typename Graph> inline typename Graph::vertex_type&
    vertex(typename Graph::vertex_type *v, Graph g) { return *v; }
    
    template<typename Graph> inline typename Graph::vertex_type&
    vertex(typename Graph::vertex_type& v, Graph g) { return v; }
    
    template<typename Graph> inline const typename Graph::vertex_type&
    vertex(const typename Graph::vertex_type& v, Graph g) { return v; }
    
    // Arc
    template<typename Graph> inline typename Graph::arc_type&
    arc(typename Graph::arc_type *a, Graph g) { return *a; }
    
    template<typename Graph> inline typename Graph::arc_type&
    arc(typename Graph::arc_type& a, Graph g) { return a; }
    
    template<typename Graph> inline const typename Graph::arc_type&
    arc(const typename Graph::arc_type& a, Graph g) { return a; }
    
    // Source vertex of an arc
    template<typename Graph> inline typename Graph::vertex_type&
    source(typename Graph::arc_type *a, Graph g) { return a->beg(); }
    
    template<typename Graph> inline typename Graph::vertex_type&
    source(typename Graph::arc_type& a, Graph g) { return a.beg(); }
    
    template<typename Graph> inline const typename Graph::vertex_type&
    source(const typename Graph::arc_type& a, Graph g) { return a.beg(); }
    
    // Target vertex of an arc
    template<typename Graph> inline typename Graph::vertex_type&
    target(typename Graph::arc_type *a, Graph g) { return a->end(); }
    
    template<typename Graph> inline typename Graph::vertex_type&
    target(typename Graph::arc_type& a, Graph g) { return a.end(); }
    
    template<typename Graph> inline const typename Graph::vertex_type&
    target(const typename Graph::arc_type& a, Graph g) { return a.end(); }
    
    // Iterator for the arcs that have the vertex as source
    template<
        typename Graph, 
        typename out_iterator = typename Graph::out_arcs_iterator
    >std::pair<out_iterator,out_iterator>
     out_arcs(const typename Graph::vertex_type& v, Graph& g) 
    { return g.out_arcs(v); }
    
    // Iterator for the arcs that have the vertex as target
    template<
        typename Graph, 
        typename in_iterator = typename Graph::in_arcs_iterator
    >std::pair<in_iterator,in_iterator>
     in_arcs(const typename Graph::vertex_type& v, Graph& g) 
    { return g.in_arcs(v); }
}

#endif
