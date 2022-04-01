/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_VERTEX_BUFFER_H
#define TINTA_VERTEX_BUFFER_H


#include "tintaGraphicaPredefine.h"
#include "tintaAttributes.h"
#include "tintaResource.h"
#include "tintaRGBAColor.h"
#include <Math/tintaVector2.h>
#include <Math/tintaVector3.h>
#include <Math/tintaVector4.h>


namespace Graphica
{

class _CoreExport tintaVertexBuffer
    :public AllocatedMemObject, 
     public tintaResource < BufferID >
{   

public:
    tintaVertexBuffer (const tintaAttributes& attributes, int vQuantity);
    tintaVertexBuffer (const tintaVertexBuffer* rv);
    virtual ~tintaVertexBuffer ();
    
    tintaAttributes& getAttributes();

    const tintaAttributes& getAttributes()const;
    
    int				getVertexSize () const;

    
    int				getVertexQuantity () const;

    
    float*			getPositions (int i);
    const float*	getPositions (int i) const;
   
    
    float*			getNormals (int i);
    const float*	getNormals (int i) const;
        
    int				getChannelQuantity () const;
    float*			getData ();
    const float*	getData () const;

    
    float&			getPosition1 (int i);
    float			getPosition1 (int i) const;
    tintaVector2f&	getPosition2 (int i);
    tintaVector2f	getPosition2 (int i) const;
    tintaVector3f&	getPosition3 (int i);
    tintaVector3f	getPosition3 (int i) const;
    tintaVector4f&	getPosition4 (int i);
    tintaVector4f	getPosition4 (int i) const;
    
    float&			getNormal1 (int i);
    float			getNormal1 (int i) const;
    tintaVector2f&	getNormal2 (int i);
    tintaVector2f	getNormal2 (int i) const;
    tintaVector3f&	getNormal3 (int i);
    tintaVector3f	getNormal3 (int i) const;
    tintaVector4f&	getNormal4 (int i);
    tintaVector4f	getNormal4 (int i) const;

    
        

    float&			getTextureCoord1 (int iUnit, int i);
    float			getTextureCoord1 (int iUnit, int i) const;
    tintaVector2f&   getTextureCoord2 (int iUnit, int i);
    tintaVector2f    getTextureCoord2 (int iUnit, int i) const;
    tintaVector3f&   getTextureCoord3 (int iUnit, int i);
    tintaVector3f    getTextureCoord3 (int iUnit, int i) const;
    tintaVector4f&   getTextureCoord4 (int iUnit, int i);
    tintaVector4f    getTextureCoord4 (int iUnit, int i) const;

    float*       getTCoords (int iUnit, int i);
    const float* getTCoords (int iUnit, int i) const;
    
    void packBuffer( int& channels,  float*& buff) const;
    
    void          setVertexQuantity (int iVQuantity);    

private:
    
    tintaVertexBuffer ();

    tintaAttributes mAttributes;

    int mVertexSize;
    
    int mVertexQuantity;
    
    int    mChannelQuantity;

    float* mData;
};



}

#endif
