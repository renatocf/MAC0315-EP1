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

#ifndef TCC_GRAPH_PROPERTIES_DEFINED
#define TCC_GRAPH_PROPERTIES_DEFINED

// Default libraries
#include <iostream>

namespace graph 
{
    struct no_property
    {
        no_property() {}
        no_property(const no_property& np) {}
        
        bool operator==(const no_property& p) const { return true; }
        bool operator!=(const no_property& p) const { return false; }
        
        friend std::ostream& 
        operator<<(std::ostream& os, const no_property& n)
        {
            os << "none"; return os;
        }
    };
}

#endif
