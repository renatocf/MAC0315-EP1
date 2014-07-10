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

#ifndef TCC_GRAPH_DEPTH_FIRST_SEARCH_DEFINED
#define TCC_GRAPH_DEPTH_FIRST_SEARCH_DEFINED

// Default libraries
#include <tuple>
#include <stack>

// Libraries
#include "graph/Traits.tcc"

namespace graph
{
    enum class color_map { white, gray, black };
    
    template<typename Graph>
    DFSvisitor
    {
        typedef typename graph_traits<Graph>::vertex_type vertex_type;
        typedef typename graph_traits<Graph>::arc_type arc_type;
        
        initialize_vertex    (vertex_type& v, Graph g);
        start_vertex         (vertex_type& v, Graph g);
        discover_vertex      (vertex_type& v, Graph g);
        examine_arc          (arc_type& a,    Graph g);
        tree_arc             (arc_type& a,    Graph g);
        back_arc             (arc_type& a,    Graph g);
        forward_or_cross_arc (arc_type& a,    Graph g);
        finish_vertex        (vertex_type& v, Graph g);
    }
    
    template<typename Graph> 
    struct time_stamper : public DFSvisitor<Graph>
    {
        typedef typename DFSvisitor<Graph>::vertex_type vertex_type;
        typedef typename DFSvisitor<Graph>::arc_type arc_type;
        
        unsigned int time;
        std::vector<unsigned int> start;
        std::vector<unsigned int> end;
        
        time_stamper(Graph g)
            : time{0}, start{num_vertices(g)}, end{num_vertices(g)} {}
        
        initialize_vertex    (vertex_type& v, Graph g) 
        { start[v->id] = end[v->id] = 0; }
        
        discover_vertex      (vertex_type& v, Graph g)
        { start[v->id] = ++time; }
        
        finish_vertex        (vertex_type& v, Graph g)
        { end[v->id] = ++time; }
        
        examine_arc          (arc_type& a, Graph g) {}
        tree_arc             (arc_type& a, Graph g) {}
        back_arc             (arc_type& a, Graph g) {}
        forward_or_cross_arc (arc_type& a, Graph g) {}
    }
    
    template<typename Graph> 
    struct dfs_tree : public DFSvisitor<Graph>
    {
        typedef typename DFSvisitor<Graph>::vertex_type vertex_type;
        typedef typename DFSvisitor<Graph>::arc_type arc_type;
        
        std::vector<vertex_type::vertex_id> parent;
        
        dfs_tree(Graph g)
            : parent{num_vertices(g)} {}
        
        initialize_vertex    (vertex_type& v, Graph g) 
        { parent[v->id] = v->id; }
        
        tree_arc             (arc_type& a,    Graph g) 
        { parent[a->beg] = a->end; }
        
        discover_vertex      (vertex_type& v, Graph g) {}
        finish_vertex        (vertex_type& v, Graph g) {}
        examine_arc          (arc_type& a,    Graph g) {}
        back_arc             (arc_type& a,    Graph g) {}
        forward_or_cross_arc (arc_type& a,    Graph g) {}
    }
    
    template<typename Graph> 
    
    template<typename Graph, typename DFSvisitor>
    void depth_first_search(DFSvisitor vis, Graph g)
    {
        typedef typename graph_traits<Graph>::vertex_id vertex_id;
        
        std::vector<color_map> color (num_vertices(g), color_map::white);
        std::stack<vertex_id> visited;
        
        typedef typename graph_traits<Graph>::vertex_iterator vit,vit_end;
        for(std::tie(vit,vit_end) = vertices(g); vit != vit_end; ++vit)
        {
            color[vit->id] = color_map::white;
            vis.initialize_vertex(*vit,g);
        }
        
        for(std::tie(vit,vit_end) = vertices(g); vit != vit_end; ++vit)
        {
            if(color[vit->id] != color_map::white) continue;
            
            vis.start_vertex(*vit,g);
            visited.push(vit->id);
            while(!visited.empty())
            {
                vertex_id vid = visited.pop();
                color[v->id] = color_map::gray;
                discover_vertex(v,g);
    
                typename graph_traits<Graph>
                    ::out_arcs_iterator oit, oit_end;
                
                std::tie(oit,oit_end) = out_arcs(vid,g);
                for(; vit != vit_end; ++vit)
                {
                    vis.examine_arc(*oit,g);
                    switch(color[oit->end])
                    {
                        case color_map::white:
                            vis.tree_arc(*oit,g);
                            visited.push(oit->end);
                            // depth_first_search_r(oit->end,color,g);
                            break;
                        case color_map::grey:
                            vis.back_arc(*oit,g);
                            break;
                        case color_map::black:
                            vis.forward_or_cross_arc(*oit,g);
                        break;
                    }
                }
                color[v->id] = color_map::black;
                vis.finish_vertex(v,g);
                
            } // stack
        } // vertices
    }
}

#endif
