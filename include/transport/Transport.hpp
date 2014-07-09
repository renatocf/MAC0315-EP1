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
#include "graph.tcc"

namespace transport 
{
    class Transport
    {
        typedef graph::flow::Vertex<>::type vertex;
        typedef graph::flow::Arc<>::type    arc;
        typedef typename vertex::id_type    vertex_id;
        
        typedef // Graph to describe problem
        graph::Adjacency_list<graph::directed,vertex,arc> digraph;
        
        typedef graph::STree<digraph> stree; // Spanning tree
        
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
            
            vertex& producer = graph::vertex(producer_id,transport_net);
            producer.properties.demand = -product;
            
            vertex& consumer = graph::vertex(consumer_id,transport_net);
            consumer.properties.demand = product;
            
            while(input >> source >> target >> cost)
                graph::add_arc(arc{source,target,{cost}},transport_net);
        }
        
        //std::pair<int,std::vector<int>> 
        void calculate_best_route()
        {
            std::vector<bool> artificial 
            ( transport_net.num_vertices(), true );

            digraph::out_iterator oit, oit_end;
            std::tie(oit,oit_end) = out_arcs(producer_id,transport_net);
            for(; oit != oit_end; ++oit) 
                artificial[oit->end] = false;

            digraph extra_net { transport_net };

            digraph::arc_iterator ait, ait_end;
            std::tie(ait,ait_end) = arcs(transport_net);

            // Any real arc has cost 0
            for(; ait != ait_end; ++ait) 
                ait->properties.cost = 0;

            // Artificial arcs have cost 1
            for(unsigned int i = 0; i < artificial.size(); ++i)
                if(artificial[i])
                    graph::add_arc(arc{producer_id,i,{1}},extra_net);

            // All flux go by the arc producer->consumer
            graph::arc(producer_id,consumer_id,extra_net).
                properties.flux = product;

            stree initial { 
                extra_net,extra_net.num_vertices(),
                producer_id,out_arcs_list(producer_id,extra_net)
            };

            graph::flow::
            network_simplex_algorithm<digraph>(extra_net,initial);
        }
        
        friend std::ostream& 
        operator<<(std::ostream& os, const Transport& t)
        {
            os << t.transport_net; return os;
        }
    };
}

#endif
