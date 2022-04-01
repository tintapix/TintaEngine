/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaTriangles.h"

namespace Graphica {



tintaTriangles::tintaTriangles(tintaGLRenderer *rend)
:tintaGeometry(rend){}


tintaTriangles::~tintaTriangles()
{
}

void tintaTriangles::genNormals (){
#if 0
    if( !mVertexBuf ) 
        return;

    if ( !mVertexBuf->GetAttributes().hasNormal() )
    {
        tintaAttributes attrib = mVertexBuf->GetAttributes();
        attrib.setNormalChannels( 3 );
        
        createVBuffer(attrib, mVertexBuf->GetVertexQuantity());
       
        int iChannels;
        float* data = mVertexBuf->GetData();
        mVertexBuf->packBuffer( attrib,false, iChannels, data );
    }

    updateNormals();
#endif
}

void tintaTriangles::updateNormals ()
{
    if( !mVertexBuf )
        return;


    if (!mVertexBuf->getAttributes().hasNormal())
    {
        return;
    }

    int iVQuantity = mVertexBuf->getVertexQuantity();
    int i;
    for (i = 0; i < iVQuantity; i++)
    {
        mVertexBuf->getNormal3(i) = tintaVector3f::origin;
    }

    int iTQuantity = getTriangleQuantity();
    for ( i = 0; i < iTQuantity; i++ ){
        
        int iV0, iV1, iV2;

        if ( !getTriangle(i,iV0,iV1,iV2) )        
            continue;        
        
        tintaVector3f& v0 = mVertexBuf->getPosition3(iV0);
        tintaVector3f& v1 = mVertexBuf->getPosition3(iV1);
        tintaVector3f& v2 = mVertexBuf->getPosition3(iV2);
                
        tintaVector3f edge1 = v1 - v0;
        tintaVector3f edge2 = v2 - v0;
        tintaVector3f normal = edge1.cross( edge2 );

        mVertexBuf->getNormal3(iV0) += normal;
        mVertexBuf->getNormal3(iV1) += normal;
        mVertexBuf->getNormal3(iV2) += normal;
    }

    for ( i = 0; i < iVQuantity; i++ )    
        mVertexBuf->getNormal3(i).normalize();
    
}


}
