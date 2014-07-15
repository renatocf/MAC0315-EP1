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
    struct bad_file{};
    
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
            Transport(std::string filename)
            {
                std::ifstream input { filename };
                if(!input.good()) 
                {
                    std::string msg {"Problems with file " + filename};
                    throw std::invalid_argument{msg};
                }
                
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
            
            std::pair<arc_list,int> calculate_best_route()
            {
                // Find initial solution
                arc_list values; stree_type candidate(transport_net);
                std::tie(values, candidate) = graph::flow
                ::network_simplex_initial_solution<digraph,stree_type>
                    (producer_id,transport_net);
                
                for(arc_type& arc : values)
                    graph::arc(arc.beg,arc.end,transport_net)
                        .properties.flux = arc.properties.flux;
                
                // Initial tree is the candidate associated with this graph
                stree_type initial { transport_net, candidate.arc_ids() };
                
                // Calculate best route (using flux)
                stree_type best(transport_net);
                std::tie(values, best) = graph::flow
                ::network_simplex_algorithm(initial,transport_net);
                
                int total_cost = 0;
                for(arc_type& arc : values)
                    total_cost += arc.properties.cost * arc.properties.flux;
                
                return std::pair<arc_list,int>{values,total_cost};
            }
            
            friend std::ostream& 
            operator<<(std::ostream& os, const Transport& t)
            {
                os << t.transport_net; return os;
            }
    };
}

#endif
