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

#ifndef HPP_TRANSPORT_TRANSPORT_DEFINED
#define HPP_TRANSPORT_TRANSPORT_DEFINED

// Default libraries
#include <string>
#include <fstream>
#include <exception>

// Libraries
#include "graph/STree.tcc"
#include "graph/Adjacency_list.tcc"
#include "graph/flow/Arc.tcc"
#include "graph/flow/Vertex.tcc"
#include "graph/flow/Network_Simplex.tcc"

namespace transport 
{
    class Transport
    {
        public:
            typedef graph::flow::Vertex<>::type   vertex_type;
            typedef graph::flow::Arc<>::type      arc_type;
            typedef typename vertex_type::id_type vertex_id;
            typedef typename arc_type::id_type    arc_id;
            
            typedef graph // Graph to describe problem
            ::Adjacency_list<graph::directed,vertex_type,arc_type> digraph;
            
            typedef graph::STree<digraph> stree_type; // Spanning tree
            
            typedef digraph::arc_list arc_list;
            
        private:
            digraph transport_net;
            size_t n_nodes; int product;
            vertex_id producer_id, consumer_id;
            
        public:
            Transport(std::string filename);
            
            std::pair<arc_list,int> calculate_best_route();
            
            friend std::ostream& 
            operator<<(std::ostream& os, const Transport& t)
            {
                os << t.transport_net; return os;
            }
    };
}

#endif
