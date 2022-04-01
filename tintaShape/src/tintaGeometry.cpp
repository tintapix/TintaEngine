
#include "tintaGraphicaPrecompile.h"
#include "tintaGeometry.h"
#include "tintaCuller.h"

namespace Graphica {





tintaGeometry::tintaGeometry(tintaGLRenderer *rend)
    :tintaDrawable( mRend )    
{
}


tintaGeometry::~tintaGeometry ()
{
    
    if( mIndexBuf ){
        if (mRend)
            mRend->releaseVBuffer(mVertexBuf);
        M_DELETE mIndexBuf;
        mIndexBuf = NULL_M;
    }

    
    if( mVertexBuf ){
        if (mRend)
            mRend->releaseIBuffer(mIndexBuf);
        M_DELETE mVertexBuf;
        mVertexBuf = NULL_M;
    }
}

void tintaGeometry::createVBuffer( const tintaAttributes &attrib, int verts, bool update) {
    if(mVertexBuf ){
        M_DELETE mVertexBuf;        
    } 

    mVertexBuf = M_NEW tintaVertexBuffer( attrib, verts );
    if( update )
        updateModel( false );
}


tintaVertexBuffer * tintaGeometry::getVBuffer(){
    return mVertexBuf;
}

const tintaVertexBuffer * tintaGeometry::getVBuffer()const {
    return mVertexBuf;
}

void tintaGeometry::createIBuffer( int verts) {
    if( verts <= 0 )
        return;

    if( mIndexBuf ){
        M_DELETE mIndexBuf;     
    }

    mIndexBuf = M_NEW tintaIndexBuffer( verts );    
}

tintaIndexBuffer * tintaGeometry::getIBuffer(){
    return mIndexBuf;
}
    

const tintaIndexBuffer * tintaGeometry::getIBuffer()const {
    return mIndexBuf;
}


void tintaGeometry::updateModel (bool bUpdateNormals)
{
    updateModelBound();
        if ( bUpdateNormals ){
        //updateNormals();
    }
}

void tintaGeometry::updateModelBound (){
    if (mBound) {
        mBound->computeFromData(mVertexBuf);
        mBound->setOrientation(getOrientation());
        mBound->scale(1.f);
    }
}

void tintaGeometry::getVisibleSet(tintaCuller& culler, bool v)
{
    culler.Insert(this);
}



GeometryType tintaGeometry::getGeomType() {
    return mGeomType;
}
void tintaGeometry::updateWorldData (){    
    
    tintaMatrix33 rot;
    //tintaQuatern orient;

    getOrientation().toRotMatrix( rot );   
    //updateOrientation( orient );

    mHomWorld[0][0] = mScale[0] * rot[0][0];
    mHomWorld[0][1] = mScale[0] * rot[1][0];
    mHomWorld[0][2] = mScale[0] * rot[2][0];
    mHomWorld[0][3] = 0.0f;
    mHomWorld[1][0] = mScale[1] * rot[0][1];
    mHomWorld[1][1] = mScale[1] * rot[1][1];
    mHomWorld[1][2] = mScale[1] * rot[2][1];
    mHomWorld[1][3] = 0.0f;
    mHomWorld[2][0] = mScale[2] * rot[0][2];
    mHomWorld[2][1] = mScale[2] * rot[1][2];
    mHomWorld[2][2] = mScale[2] * rot[2][2];
    mHomWorld[2][3] = 0.0f;
   
    const tintaVector3f &pos = getPosition();
    mHomWorld[3][0] = pos[0];
    mHomWorld[3][1] = pos[1];
    mHomWorld[3][2] = pos[2];
    mHomWorld[3][3] = 1.0f;
}


}