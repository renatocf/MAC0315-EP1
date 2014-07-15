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
    
    if(argc-optind < 1)
    {
        cerr << "Usage: Simplex [-v] [-o] input.dat ..." << endl;
        return EXIT_FAILURE;
    }
    
    typedef transport::Transport::arc_type arc_type;
    typedef transport::Transport::arc_list arc_list;
    
    std::ofstream output {options.output};
    std::ostream& os = options.output.empty() ? cout : output;
    
    for(int i = optind; i < argc; ++i)
    try 
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
    catch(const std::invalid_argument& ia)
    {
        cerr << "Invalid argument: " << ia.what() << endl;
    }
    
    return EXIT_SUCCESS;
}
