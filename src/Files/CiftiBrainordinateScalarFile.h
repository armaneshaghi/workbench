#ifndef __CIFTI_BRAINORDINATE_SCALAR_FILE_H__
#define __CIFTI_BRAINORDINATE_SCALAR_FILE_H__

/*LICENSE_START*/
/*
 * Copyright 2012 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/


#include "BrainConstants.h"
#include "ChartableInterface.h"
#include "CiftiMappableDataFile.h"

namespace caret {

    class ChartTypeEnum;
    class CiftiMappableConnectivityMatrixDataFile;
    class TimeLine;
    
    class CiftiBrainordinateScalarFile : public CiftiMappableDataFile, public ChartableInterface {
    
    public:
        CiftiBrainordinateScalarFile();
        
        virtual ~CiftiBrainordinateScalarFile();
        
        static CiftiBrainordinateScalarFile* newInstanceFromRowInCiftiConnectivityMatrixFile(const CiftiMappableConnectivityMatrixDataFile* ciftiMatrixFile,
                                                                                             AString& errorMessageOut);

        virtual bool isChartingEnabled(const int32_t tabIndex) const;
        
        virtual void setChartingEnabled(const int32_t tabIndex,
                                        const bool enabled);
        
        virtual bool isChartingSupported() const;

        virtual ChartTypeEnum::Enum getDefaultChartType() const;

        virtual void getSupportedChartTypes(std::vector<ChartTypeEnum::Enum> &list) const;
        
        virtual ChartData* loadChartDataForSurfaceNode(const StructureEnum::Enum structure,
                                                                                   const int32_t nodeIndex) throw (DataFileException);
        
        virtual ChartData* loadAverageChartDataForSurfaceNodes(const StructureEnum::Enum structure,
                                                               const std::vector<int32_t>& nodeIndices) throw (DataFileException);
        
        virtual ChartData* loadChartDataForVoxelAtCoordinate(const float xyz[3]) throw (DataFileException);
        

        virtual bool loadAverageChartForSurfaceNodes(const StructureEnum::Enum structure,
                                                     const std::vector<int32_t>& nodeIndices,
                                                     TimeLine& timeLineOut) throw (DataFileException);
        
        virtual bool loadChartForSurfaceNode(const StructureEnum::Enum structure,
                                             const int32_t nodeIndex,
                                             TimeLine& timeLineOut) throw (DataFileException);
        
        virtual bool loadChartForVoxelAtCoordinate(const float xyz[3],
                                                   TimeLine& timeLineOut) throw (DataFileException);
        
        virtual CaretMappableDataFile* getCaretMappableDataFile();
        
        virtual const CaretMappableDataFile* getCaretMappableDataFile() const;
        
    private:
        CiftiBrainordinateScalarFile(const CiftiBrainordinateScalarFile&);

        CiftiBrainordinateScalarFile& operator=(const CiftiBrainordinateScalarFile&);
        
        virtual void saveFileDataToScene(const SceneAttributes* sceneAttributes,
                                         SceneClass* sceneClass);
        
        virtual void restoreFileDataFromScene(const SceneAttributes* sceneAttributes,
                                              const SceneClass* sceneClass);
        
    public:

        // ADD_NEW_METHODS_HERE

    private:

        bool m_chartingEnabledForTab[BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS];
        
        // ADD_NEW_MEMBERS_HERE

    };
    
#ifdef __CIFTI_BRAINORDINATE_SCALAR_FILE_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __CIFTI_BRAINORDINATE_SCALAR_FILE_DECLARE__

} // namespace
#endif  //__CIFTI_BRAINORDINATE_SCALAR_FILE_H__
