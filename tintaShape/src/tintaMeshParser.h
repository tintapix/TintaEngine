/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_MESH_PARSER_H_
#define _TINTA_MESH_PARSER_H_

#include "tintaGraphicaPredefine.h"

namespace Graphica {

    struct _CoreExport tintaMeshData {
        VecVector3f_t mVertices;
        VecVector2f_t mTcoordinates;
        intVec_t      mIndices;
        String        mMeshName;
        tintaMeshData() {
        }

        tintaMeshData(tintaMeshData && rv) :
            mVertices( std::move(rv.mVertices) )
            , mTcoordinates( std::move(rv.mTcoordinates) )
            , mIndices( std::move(rv.mIndices) )
            , mMeshName( std::move(rv.mMeshName) ){

            
        }

        tintaMeshData(const tintaMeshData & rv) 
            : mVertices( rv.mVertices )
            , mTcoordinates( rv.mTcoordinates )
            , mIndices( rv.mIndices )
            , mMeshName( rv.mMeshName ){
        }

        const tintaMeshData& operator=(const tintaMeshData & rv){
            if ( this == &rv )
                return *this;

            mVertices = rv.mVertices;
            mTcoordinates = rv.mTcoordinates;
            mIndices = rv.mIndices;
            mMeshName = rv.mMeshName;
            return *this;
        }

        const tintaMeshData& operator=(tintaMeshData && rv){
            if (this == &rv)
                return *this;

            mVertices = rv.mVertices;
            rv.mVertices.clear();

            mTcoordinates = rv.mTcoordinates;
            rv.mTcoordinates.clear();
            mIndices = rv.mIndices;
            rv.mIndices.clear();
            mMeshName = rv.mMeshName;
            rv.mMeshName.clear();
            return *this;
        }
    };

    class tintaMeshParser
    {
    public:
        tintaMeshParser();
        virtual ~tintaMeshParser();

        tintaMeshData parse(const String &path);       
   

    };
}
#endif

