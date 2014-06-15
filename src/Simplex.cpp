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
    
    graph::flow::Vertex<> v1 { 0 };
    graph::flow::Vertex<> v2 { 1 };
    
    graph::flow::Arc<> a1 { v1, v2, -1 };
    graph::flow::Arc<> a2 { v2, v1,  1 };
    
    graph::Vertex_list<graph::flow::Vertex<>> vl { v1, v2 };
    cout << vl << endl;
    
    graph::Arc_list<graph::flow::Arc<>> al { a1, a2 };
    cout << al << endl;
    
    return EXIT_SUCCESS;
}
