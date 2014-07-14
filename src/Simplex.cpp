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

/** 
 * @file  Simplex.cpp
 * @brief Main interface for use of the Network Simplex Algorithm
 */

// Default libraries
#include <tuple>
#include <cstdio>
#include <fstream>
#include <iostream>
using namespace std;

// Libraries
#include "Options.hpp"
#include "graph/graph.tcc"
#include "graph/flow/flow.tcc"
#include "transport/Transport.hpp"

/**
 * @fn Main
 * @param  argc Number of arguments in the command line
 * @param  argv Array of string with the arguments
 * @return 0 if success; non-zero otherwise
 */
int main(int argc, char **argv)
{
    // Process options
    Options options; options.parse_args(argc, argv);
    
    if(argc-optind != 1)
    {
        cerr << "Usage: Simplex [-d] [-v] [-o] input.dat ..." << endl;
        cerr << "Type --help for more information" << endl;
        return EXIT_FAILURE;
    }
    
    typedef transport::Transport::arc_type arc_type;
    typedef transport::Transport::arc_list arc_list;
    
    std::ofstream output {options.output};
    std::ostream& os = options.output.empty() ? cout : output;
    
    for(int i = optind; i < argc; ++i)
    {
        os << endl;
        if(options.verbose)
        {
            os << "Analyzing file " << argv[i] << endl;
            os << "==========================" << endl;
            os << endl;
        }
        
        if(options.verbose) os << "Graph: " << endl;
        transport::Transport problem { argv[i] };
        if(options.verbose) os << problem << endl;
        
        // Best route
        arc_list values; int final_cost;
        std::tie(values,final_cost) = problem.calculate_best_route();
        
        os << "Cost: " << final_cost << endl;
        os << "List of arcs: " << endl;
        for(arc_type& arc : values)
        {
            if(options.verbose) os << arc << std::endl;
            else os << arc.id << " - " << arc.properties.flux << endl;
        }
    }
    
    // typedef graph::flow::Vertex<>::type vertex;
    // typedef graph::flow::Arc<>::type    arc;
    //
    // vertex v1 { 0 }, v2 { 1 };
    // arc a1 { v1, v2 }, a2 { v2, v1 };
    //
    // cout << endl << "Vertex list" << endl;
    // cout << "=======================" << endl;
    // typedef std::vector<vertex> vertex_list;
    // vertex_list vl { v1, v2 };
    // for(const vertex& v : vl)
    //     cout << v << endl;
    //
    // cout << endl << "Arc list" << endl;
    // cout << "=======================" << endl;
    // typedef std::vector<arc> arc_list;
    // arc_list al { a1, a2 };
    // for(const arc& a : al)
    //     cout << a << endl;
    //
    // cout << endl << "Digraph" << endl;
    // cout << "=======================" << endl;
    // typedef graph::Adjacency_list<graph::directed,vertex,arc> digraph;
    //
    // vertex_list tr1_v;
    //
    // vertex::id_type id {};
    // for(size_t i = 0; i < 5; i++) 
    //     tr1_v.emplace_back(id++);
    //
    // arc_list tr1_a { 
    //     arc{ 0, 1, { 9} },
    //     arc{ 0, 3, {10} },
    //     arc{ 1, 2, { 7} },
    //     arc{ 1, 3, { 4} },
    //     arc{ 1, 4, { 5} },
    //     arc{ 2, 4, { 9} },
    //     arc{ 3, 1, { 4} },
    //     arc{ 3, 4, { 9} }
    // };
    //
    // digraph T { tr1_v, tr1_a };
    //
    // typename digraph::vertex_iterator it,it_end;
    // for(std::tie(it,it_end) = T.vertices(); it != it_end; ++it)
    //     cout << *it << endl;
    //
    // typename digraph::arc_iterator ait,ait_end;
    // for(std::tie(ait,ait_end) = T.arcs(); ait != ait_end; ++ait)
    //     cout << *ait << endl;
    //
    // cout << "Before removing" << endl;
    // for(int i = 0; i <= 4; i++)
    // {
    //     cout << "List " << i << endl;
    //     for(auto& a : out_arcs_list(i,T))
    //         cout << a << endl;
    // }
    //
    // add_arc(arc{0,2},T);
    // remove_arc(0,1,T);
    //
    // cout << "After removing" << endl;
    // for(int i = 0; i <= 4; i++)
    // {
    //     cout << "List " << i << endl;
    //     for(auto& a : out_arcs_list(i,T))
    //         cout << a << endl;
    // }
    
    return EXIT_SUCCESS;
}
