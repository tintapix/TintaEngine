/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaTriMesh.h"
#include "tintaRoot.h"


namespace Graphica {

 
 tintaTriMesh::tintaTriMesh(tintaGLRenderer *rend)
    :tintaTriangles(rend){

     mGeomType = GT_TRIMESH;
}


 tintaTriMesh::~tintaTriMesh()
{
}

void tintaTriMesh::createVBuffer( const tintaAttributes &attrib, int verts) {
    tintaGeometry::createVBuffer( attrib,verts );
    updateNormals(); 
}

bool tintaTriMesh::getTriangle (int i, int& v0, int& v1, int& v2) const
{
    if ( mIndexBuf && 0 <= i && i < getTriangleQuantity() )
    {
        const int* index = &mIndexBuf->GetData()[3*i];
        v0 = *index++;
        v1 = *index++;
        v2 = *index;
        return true;
    }
    return false;
}

int tintaTriMesh::getTriangleQuantity () const
{
    if( !mIndexBuf )
        return 0;

	return mIndexBuf->GetIndexQuantity() / 3;
}


tintaTriMesh* tintaTriMesh::createFromMesh(const String &name) {

    const tintaMeshData * data = tintaRoot::getPtr()->findMesh( name );
    if ( !data )
        return NULL_M;

    tintaVertexBuffer *vBuff{ NULL_M };
    int* aiIndex{ NULL_M };
    tintaIndexBuffer *iBuff{ NULL_M };

    tintaAttributes attr;
    attr.setPositionChannels(3);
    attr.setTCoordChannels(0, 2);

    tintaTriMesh* pGeometry = M_NEW tintaTriMesh();
    

    pGeometry->createVBuffer(attr, data->mVertices.size());
    vBuff = pGeometry->getVBuffer();    
    
    for (size_t i = 0; i < data->mVertices.size(); i++){
        vBuff->getPosition3(i) = data->mVertices[i];
    }
    
  
    pGeometry->createIBuffer( data->mIndices.size() );
    iBuff = pGeometry->getIBuffer();

    aiIndex = iBuff->GetData();
    
    
    for (size_t i = 0; i < data->mIndices.size(); i++){
        aiIndex[i] = data->mIndices[i];
    }
  
    pGeometry->setCullMode( tintaDrawable::CULL_NEVER );

    return pGeometry;
    
}

}
