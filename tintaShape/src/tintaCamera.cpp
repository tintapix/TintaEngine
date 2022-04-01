/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include <Math/tintaCommonMath.h>
#include "tintaCamera.h"


using namespace Graphica;


tintaCamera::tintaCamera():
    m_pRenderer{NULL_M},
    m_perspective{ false } {

    setFrustum( -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 2.0f);
    setViewport( 0.f, 1.0f, 1.f, 0.f);
    setDepthRange(0.0f,1.0f);
    setFrame( tintaVector3f::origin,-tintaVector3f::zAxis, tintaVector3f::yAxis,  tintaVector3f::xAxis );    
}

tintaCamera::~tintaCamera(){
    for ( auto data : mActions ){
        M_DELETE data;
    }
    mActions.clear();
}

void tintaCamera::setFrame(const tintaVector3f& loc, const tintaVector3f& DVector, const tintaVector3f& UVector, const tintaVector3f& RVector){
    setPosition(loc);
    setAxes( DVector,UVector,RVector );    
}

void tintaCamera::moveOn(const tintaVector3f& loc, bool bUpdateChildren, bool updateLocal) {
    tintaSpatial::moveOn( loc );

    if (m_pRenderer){
        m_pRenderer->onFrameChange();
    }
}


void tintaCamera::moveOnOrient(const tintaVector3f& loc, bool bUpdateChildren , bool updateLocal) {


    tintaSpatial::moveOnOrient(loc);
    if (m_pRenderer) {
        m_pRenderer->onFrameChange();
    }
}

void  tintaCamera::updateOrientation(const tintaQuatern &orient) {

    tintaSpatial::updateOrientation(orient);
    if (m_pRenderer) {
        m_pRenderer->onFrameChange();
    }
}
void tintaCamera::moveTo(const tintaVector3f& loc, bool bUpdateChildren, bool updateLocal) {
       
    tintaSpatial::moveTo(loc);

    if (m_pRenderer){
        m_pRenderer->onFrameChange();
    }
}
void tintaCamera::yaw(float angle) {
    //tintaVector3f axis;
    //axis = tintaVector3f::yAxis;
    //mUVector = yAxis;
    rotate(tintaVector3f::yAxis, angle);
    update();
    if (m_pRenderer) {
        m_pRenderer->onFrameChange();
    }   
    
    //update();
}
void tintaCamera::pitch(float angle) {

    tintaVector3f axis;
   // axis = mOrientation * tintaVector3f::zAxis;
    //axis = tintaVector3f::zAxis;
    //m_DVector = zAxis;
    rotate(tintaVector3f::zAxis, angle);
    update();
    if (m_pRenderer) {
        m_pRenderer->onFrameChange();
    }   
    
    //update();
}
void tintaCamera::roll(float angle) {
    //tintaVector3f axis;
    //axis = tintaVector3f::xAxis;
    rotate(tintaVector3f::xAxis, angle);
    update();
    //mRVector = xAxis;
    if (m_pRenderer) {
        m_pRenderer->onFrameChange();
    }   
    
    //update();
}


void tintaCamera::rotate(float angleX, float angleY, float angleZ, bool bUpdateChildren, bool updateLocal) {

    tintaQuatern qX;
    qX.fromAngleAxis(angleX, tintaVector3f::xAxis);
    tintaQuatern qY;
    qY.fromAngleAxis(angleY, tintaVector3f::yAxis);
    tintaQuatern qZ;
    qZ.fromAngleAxis(angleZ, tintaVector3f::zAxis);


    rotate(qX * qY * qZ, false, updateLocal);
}

void tintaCamera::rotate( const tintaVector3f& axis, float angle, bool bUpdateChildren, bool updateLocal ) {
    tintaQuatern q;
    q.fromAngleAxis(angle, axis);
    rotate( q );
}

void tintaCamera::rotate(const tintaQuatern& q, bool bUpdateChildren, bool updateLocal) {

    tintaQuatern qnorm = q;
    qnorm.normalize();

    mLocalOrientation = mLocalOrientation * qnorm;
    setOrientation(mLocalOrientation);

    update();
    //mRVector = xAxis;
    if (m_pRenderer) {
        m_pRenderer->onFrameChange();
    }
}


void tintaCamera::setDirection(const tintaVector3f& dir){
    
    //setAxes(-dir, getUVector(), getRVector());


    tintaVector3f xAxis;
    tintaVector3f yAxis;
    tintaVector3f zAxis;

    getOrientation().toAxes(xAxis, yAxis, zAxis);

    tintaVector3f dest{ dir };

    dest.normalize();

    tintaQuatern qObjLookAt = getOrientation() * getRotationTo<float>(xAxis, dest);

    updateOrientation(qObjLookAt);

    update();
    if (m_pRenderer) {
        m_pRenderer->onFrameChange();
    }

#if 0

    if (dir.isUnit())
            return;

    
    tintaVector3f zDir = -dir;

    zDir.normalize();

    tintaQuatern worldOrient;
        
    


    tintaVector3f axes[3];
    
    mOrientation.toAxes( axes );
    tintaQuatern rotQuat;

    if ( ( axes[2] + zDir ).lengthSquared() <  0.00005f )   { // 180 degree direction       
        rotQuat.fromAngleAxis(  K_PI, axes[1] );
    }
    else {
        // shortest way
        //rotQuat = axes[2.getVal(0)otationTo( zAdjustVec );
        rotQuat = getRotationTo(axes[2], zDir);
    }

    worldOrient = rotQuat * mOrientation;

    mOrientation = worldOrient;

    //update(); 
    //mRVector = xAxis;
    if (m_pRenderer) {
        m_pRenderer->onFrameChange();
    }

#endif
}

void tintaCamera::onUpdate() {
    if (m_pRenderer){
        m_pRenderer->onFrustumChange();
        m_pRenderer->onFrameChange();
        m_pRenderer->onViewportChange();
    }
}
void tintaCamera::update(){

	float det = TintaMath::abs(m_DVector.dot(m_UVector.cross(m_RVector)));
    if (TintaMath::abs(1.0f - det) > 0.01f ){
		tintaVector3f::orthonormalize( m_DVector,m_UVector,m_RVector );
	}
    //tintaQuatern toUpdate;
    getOrientation().toAxes(m_DVector, m_UVector, m_RVector);
   // updateOrientation(toUpdate);

}
void tintaCamera::update(const tintaVector3f& DVector, const tintaVector3f& UVector, const tintaVector3f& RVector){


    m_DVector = DVector;
    m_UVector = UVector;
    m_RVector = RVector;

    float det = TintaMath::abs( DVector.dot( UVector.cross( RVector ) ) );

    if (TintaMath::abs( 1.0f - det ) > 0.01f ) {
        tintaVector3f::orthonormalize(m_DVector, m_UVector, m_RVector);
    }

}


void tintaCamera::setAxes(const tintaVector3f& DVector, const tintaVector3f& UVector, const tintaVector3f& RVector){
	
	tintaQuatern orient(DVector,UVector,RVector);

    updateOrientation(orient);

	update(DVector, UVector, RVector);
    if ( m_pRenderer ){
        m_pRenderer->onFrameChange();
    }
}

bool tintaCamera::isPerspective() const {
    return m_perspective;
}


void tintaCamera::setPerspective( bool perspective ) {
    m_perspective = perspective;
    if (m_pRenderer) {
        m_pRenderer->onFrustumChange();
    }
}


void tintaCamera::setFrustum(float rightMin, float rightMax, float upMin, float upMax,
    float farMin, float farMax)
{
    mFrustum[enDMIN] = farMin;
    mFrustum[enDMAX] = farMax;
    mFrustum[enUMIN] = upMin;
    mFrustum[enUMAX] = upMax;
    mFrustum[enRMIN] = rightMin;
    mFrustum[enRMAX] = rightMax;

    if (m_pRenderer){
        m_pRenderer->onFrustumChange();
    }
}

void tintaCamera::setFrustum(const float* frustum)
{
    size_t uiSize = enQUANTITY*sizeof(float);
    mlMemcpy( mFrustum,uiSize, frustum,uiSize );

    if (m_pRenderer)    
       m_pRenderer->onFrustumChange();    
}

void tintaCamera::setFrustum(float upFov, float aspectRatio, float dMin, float dMax)
{
    float fHalfAngle = TintaMath::toRad( 0.5f * upFov); //*DEG_TO_RAD;
    mFrustum[enUMAX] = dMin * TintaMath::tan(fHalfAngle);
    mFrustum[enRMAX] = aspectRatio*mFrustum[enUMAX];
    mFrustum[enUMIN] = -mFrustum[enUMAX];
    mFrustum[enRMIN] = -mFrustum[enRMAX];
    mFrustum[enDMIN] = dMin;
    mFrustum[enDMAX] = dMax;

    if ( m_pRenderer )    
        m_pRenderer->onFrustumChange();    
}

void tintaCamera::getFrustum( float& rMin, float& rMax, float& uMin,
                                float& uMax, float& dMin, float& dMax ) const
{
    dMin = mFrustum[enDMIN];
    dMax = mFrustum[enDMAX];
    uMin = mFrustum[enUMIN];
    uMax = mFrustum[enUMAX];
    rMin = mFrustum[enRMIN];
    rMax = mFrustum[enRMAX];
}

bool tintaCamera::getFrustum(float& upFovDegrees, float& aspectRatio,
					     float& dMin, float& dMax) const
{
    if ( mFrustum[enRMIN] == -mFrustum[enRMAX]
    &&  mFrustum[enUMIN] == -mFrustum[enUMAX] )
    {
        float tmp = mFrustum[enUMAX]/mFrustum[enDMIN];

        upFovDegrees = TintaMath::toDeg( 2.0f * TintaMath::atan(tmp) );
        aspectRatio = mFrustum[enRMAX]/mFrustum[enUMAX];
        dMin = mFrustum[enDMIN];
        dMax = mFrustum[enDMAX];
        return true;
    }
    return false;
}

void tintaCamera::setViewport(float left, float right, 
    float top,   float bottom)
{
    m_portLeft = left;
    m_portRight = right;
    m_portTop = top;
    m_portBottom = bottom;

    if ( m_pRenderer )    
        m_pRenderer->onViewportChange();    
}

void tintaCamera::getViewport(float& left, float& right, 
                float& top, float& bottom) {
    left = m_portLeft;
    right = m_portRight;
    top = m_portTop;
    bottom = m_portBottom;
}

void tintaCamera::setDepthRange(float portNear, float portFar) {
    m_portNear = portNear;
    m_portFar = portFar;
    if (m_pRenderer) 
        m_pRenderer->onDepthRangeChange();
    
}

void tintaCamera::getDepthRange(float& portNear, float& portFar)
{
    portNear = m_portNear;
    portFar = m_portFar;
}


tintaVector3f tintaCamera::getDVector() const
{
	return m_DVector;
}

tintaVector3f tintaCamera::getUVector() const
{
	return m_UVector;
}

tintaVector3f tintaCamera::getRVector() const
{
	return m_RVector;
}

const float* tintaCamera::getFrustum() const
{
	return mFrustum;
}

float tintaCamera::getDMin() const
{
	return mFrustum[enDMIN];
}

float tintaCamera::getDMax() const
{
	return mFrustum[enDMAX];
}

float tintaCamera::getUMin() const
{
    return mFrustum[enUMIN];
}

float tintaCamera::getUMax() const
{
    return mFrustum[enUMAX];
}

float tintaCamera::getRMin() const
{
    return mFrustum[enRMIN];
}

float tintaCamera::getRMax() const
{
    return mFrustum[enRMAX];
}







