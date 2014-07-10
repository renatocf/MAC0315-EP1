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

#ifndef TCC_GRAPH_STREE_SEARCH_DEFINED
#define TCC_GRAPH_STREE_SEARCH_DEFINED

// Libraries
#include "graph/STree.tcc"
#include<iostream>

namespace graph 
{
    template<typename STree>
    struct STvisitor
    {
        typedef STree                          stree_type;
        typedef typename stree_type::vertex_id vertex_id;
        
        void start_vertex      (vertex_id,stree_type);
        void radix             (vertex_id,stree_type);
        bool visit_parent_cond (vertex_id,stree_type);
        void before_parent     (vertex_id,stree_type);
        void after_parent      (vertex_id,stree_type);
        void end_visits        (vertex_id,stree_type);
    };
    
    template<typename STree, typename STvisitor>
    void stree_search(STvisitor& vis, STree st)
    {
        typename graph_traits<typename STree::graph_type>
            ::vertex_iterator vit,vit_end;
        
        std::tie(vit,vit_end) = vertices(st.base_graph());
        for(; vit != vit_end; ++vit)
            vis.start_vertex(vit->id,st);
        
        std::tie(vit,vit_end) = vertices(st.base_graph());
        for(; vit != vit_end; ++vit)
            stree_search_visit(vit->id,vis,st);
    }
    
    template<typename STree, typename STvisitor>
    void stree_search_visit(typename STvisitor::vertex_id vid, 
                            STvisitor& vis, STree st)
    {
        if(vid == parent(vid,st)) 
        {
            vis.radix(vid,st);
        }
        else if(vis.visit_parent_cond(vid,st))
        {
            vis.before_parent(vid,st);
            stree_search_visit(parent(vid,st),vis,st);
            vis.after_parent(vid,st);
        }
        else vis.end_visits(vid,st);
    }
}

#endif
