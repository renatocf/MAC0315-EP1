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
#include <cstdio>
#include <iostream>
using namespace std;

// Libraries
#include "Options.hpp"
#include "graph/graph.tcc"
#include "graph/flow/flow.tcc"

/**
 * @fn Main
 * @param  argc Number of arguments in the command line
 * @param  argv Array of string with the arguments
 * @return 0 if success; non-zero otherwise
 */
int main(int argc, char **argv)
{
    // Process options
    Options::parse_args(argc, argv);
    
    if(argc-optind != 1)
    {
        cerr << "Usage: Simplex [-h] [-d] input.dat ..." << endl;
        cerr << "Type --help for more information"       << endl;

        return EXIT_FAILURE;
    }
    
    typedef graph::flow::Vertex<>::type vertex;
    typedef graph::flow::Arc<>::type    arc;
    
    vertex v1 { 0 }, v2 { 1 };
    arc a1 { v1, v2 }, a2 { v2, v1 };
    
    cout << endl << "Vertex list" << endl;
    cout << "=======================" << endl;
    typedef std::vector<vertex> vertex_list;
    vertex_list vl { v1, v2 };
    for(const vertex& v : vl)
        cout << v << endl;
    
    cout << endl << "Arc list" << endl;
    cout << "=======================" << endl;
    typedef std::vector<arc> arc_list;
    arc_list al { a1, a2 };
    for(const arc& a : al)
        cout << a << endl;
    
    cout << endl << "Digraph" << endl;
    cout << "=======================" << endl;
    typedef graph::Adjacency_list<graph::directed,vertex,arc> digraph;
    // typedef graph::Adjacency_list<graph::bidirectional,vertex,arc> digraph;
    
    vertex_list tr1_v;
    
    vertex::id_type id {};
    for(size_t i = 0; i < 5; i++) 
        tr1_v.emplace_back(id++);
    
    arc_list tr1_a { 
        arc{ tr1_v[0], tr1_v[1], {9 } },
        arc{ tr1_v[0], tr1_v[3], {10} },
        arc{ tr1_v[1], tr1_v[2], {7 } },
        arc{ tr1_v[1], tr1_v[3], {4 } },
        arc{ tr1_v[1], tr1_v[4], {5 } },
        arc{ tr1_v[2], tr1_v[4], {9 } },
        arc{ tr1_v[3], tr1_v[1], {4 } },
        arc{ tr1_v[3], tr1_v[4], {9 } }
    };

    digraph T { tr1_v, tr1_a };
    
    auto it  = out_arcs(tr1_v[0], T).first;
    auto end = out_arcs(tr1_v[0], T).second;
    for(; it != end; it++)
        cout << graph::vertex(*it,T) << endl;
    
    cout << endl << "Spanning Tree" << endl;
    cout << "=======================" << endl;
    typedef graph::STree<vertex,arc> stree;
    
    stree initial { T.num_vertices(), 
                    0, {tr1_a[0], tr1_a[1], tr1_a[7], tr1_a[3]} };
    
    cout << initial << endl;
    
    return EXIT_SUCCESS;
}
