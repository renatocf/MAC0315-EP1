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

#ifndef TCC_GRAPH_TRAITS_DEFINED
#define TCC_GRAPH_TRAITS_DEFINED

namespace graph
{
    template<typename Graph>
    struct graph_traits
    {
        typedef typename Graph
            ::vertex_type           vertex_type;
        typedef typename Graph
            ::vertex_iterator       vertex_iterator;
        typedef typename Graph
            ::const_vertex_iterator const_vertex_iterator;
        
        typedef typename Graph
            ::arc_type              arc_type;
        typedef typename Graph
            ::arc_iterator          arc_iterator;
        typedef typename Graph
            ::const_arc_iterator    const_arc_iterator;
        typedef typename Graph
            ::out_iterator          out_arcs_iterator;
        typedef typename Graph
            ::const_out_iterator    const_out_arcs_iterator;
        typedef typename Graph
            ::in_iterator           in_arcs_iterator;
        typedef typename Graph
            ::const_in_iterator     const_in_arcs_iterator;
    };
}

#endif
