#ifndef __BRAIN_CONSTANTS__H_
#define __BRAIN_CONSTANTS__H_

/*LICENSE_START*/
/* 
 *  Copyright 1995-2011 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 

#include <stdint.h>

namespace caret {

    
    /// Constants for use in the Brain
    class BrainConstants  {
        
    public:
        enum {
            /// Maximum number of browser windows
            MAXIMUM_NUMBER_OF_BROWSER_WINDOWS = 10,
            
            /// Maximum number of browser tabs
            MAXIMUM_NUMBER_OF_BROWSER_TABS = 100,
            
            /// Maximum number of overlays
            MAXIMUM_NUMBER_OF_OVERLAYS = 50,
            
            /// Minimum number of overlays
            MINIMUM_NUMBER_OF_OVERLAYS = 3,
            
            /// Minimum number of volume surface outlines 
            MINIMUM_NUMBER_OF_VOLUME_SURFACE_OUTLINES = 4,
            
            /// Maximum number of volume surface outlines 
            MAXIMUM_NUMBER_OF_VOLUME_SURFACE_OUTLINES = 25
        };        
    private:
        BrainConstants();
        
        ~BrainConstants();
        
        BrainConstants(const BrainConstants&);

        BrainConstants& operator=(const BrainConstants&);
        
    };
    
#ifdef __BRAIN_CONSTANTS_DECLARE__
#endif // __BRAIN_CONSTANTS_DECLARE__

} // namespace
#endif  //__BRAIN_CONSTANTS__H_