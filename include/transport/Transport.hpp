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
#include <fstream>

// Libraries
#include "graph/STree.tcc"
#include "graph/Adjacency_list.tcc"
#include "graph/flow/Network_Simplex.tcc"

namespace transport 
{
    class Transport
    {
        typedef graph::flow::Vertex<>::type   vertex_type;
        typedef graph::flow::Arc<>::type      arc_type;
        typedef typename vertex_type::id_type vertex_id;
        typedef typename arc_type::id_type    arc_id;
        
        typedef graph // Graph to describe problem
        ::Adjacency_list<graph::directed,vertex_type,arc_type> digraph;
        
        typedef graph::STree<digraph> stree_type; // Spanning tree
        
        typedef digraph::arc_list arc_list;
        
        digraph transport_net;
        size_t n_nodes; int product;
        vertex_id producer_id, consumer_id;
        
    public:
        Transport(std::string filename)
        {
            std::ifstream input { filename };
            
            vertex_id source, target;
            unsigned int cost;
            
            // Get basic info (# nodes, producer, consumer, # product)
            input >> n_nodes >> producer_id >> consumer_id >> product;
            
            // Create network transport
            this->transport_net = digraph{n_nodes};
            
            vertex_type& producer 
                = graph::vertex(producer_id,transport_net);
            producer.properties.demand = -product;
            
            vertex_type& consumer 
                = graph::vertex(consumer_id,transport_net);
            consumer.properties.demand = product;
            
            while(input >> source >> target >> cost)
                graph::add_arc(arc_type{source,target,{cost}},
                               transport_net);
        }
        
        //std::pair<int,std::vector<int>> 
        void calculate_best_route()
        {
            // Find initial solution
            arc_list values; stree_type candidate(transport_net);
            std::tie(values, candidate) = graph::flow
            ::network_simplex_initial_solution(producer_id,transport_net);
            
            for(arc_type& arc : values)
                graph::arc(arc.beg,arc.end,transport_net)
                    .properties.flux = arc.properties.flux;
            
            // Initial tree is the candidate associated with this graph
            stree_type initial { transport_net, candidate.arc_ids() };
            
            std::cerr << std::endl << "Simplex phase 2" << std::endl;
            std::cerr << transport_net << std::endl;
            graph::flow
            ::network_simplex_algorithm(transport_net,initial);
        }
        
        friend std::ostream& 
        operator<<(std::ostream& os, const Transport& t)
        {
            os << t.transport_net; return os;
        }
    };
}

#endif
