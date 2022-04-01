/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaCuller.h"
#include "tintaDrawable.h"
#include <Math/tintaCommonMath.h>

namespace Graphica {


    tintaCuller::tintaCuller(int iMaxQuantity, int iGrowBy, const tintaCamera* pkCamera)
    : mVisible(iMaxQuantity,iGrowBy)
{
    m_Cam = pkCamera;
    m_iPlaneQuantity = 6;    
}

    tintaCuller::~tintaCuller()
{
}

void tintaCuller::setFrustum (const float* afFrustum)
{
    assert(m_Cam);
    if (!m_Cam)
    {
        return;
    }

    // copy the frustum values
    size_t uiSize = tintaCamera::enQUANTITY*sizeof(float);
	Tinta::mlMemcpy(mFrustum,uiSize,afFrustum,uiSize);
    float fDMin2 = mFrustum[tintaCamera::enDMIN]*mFrustum[tintaCamera::enDMIN];
    float fRMin2 = mFrustum[tintaCamera::enRMIN]*mFrustum[tintaCamera::enRMIN];
    float fRMax2 = mFrustum[tintaCamera::enRMAX]*mFrustum[tintaCamera::enRMAX];
    float fUMin2 = mFrustum[tintaCamera::enUMIN]*mFrustum[tintaCamera::enUMIN];
    float fUMax2 = mFrustum[tintaCamera::enUMAX]*mFrustum[tintaCamera::enUMAX];

    // get the tintaCamera coordinate frame
    tintaVector3f kLoc =  m_Cam->getPosition();
    tintaVector3f kDVec = m_Cam->getDVector();
    tintaVector3f kUVec = m_Cam->getUVector();
    tintaVector3f kRVec = m_Cam->getRVector();
    float fDdE = kDVec.dot(kLoc);

    // update the near plane
    m_akPlane[tintaCamera::enDMIN].mNormal = kDVec;
    m_akPlane[tintaCamera::enDMIN].mMag =
        fDdE + mFrustum[tintaCamera::enDMIN];

    // update the far plane
    m_akPlane[tintaCamera::enDMAX].mNormal = -kDVec;
    m_akPlane[tintaCamera::enDMAX].mMag =
        -(fDdE + mFrustum[tintaCamera::enDMAX]);

    // update the bottom plane
	float fInvLength = 1.0f/TintaMath::sqrt(fDMin2 + fUMin2);
    float fC0 = -mFrustum[tintaCamera::enUMIN]*fInvLength;  // D component
    float fC1 = +mFrustum[tintaCamera::enDMIN]*fInvLength;  // U component
    m_akPlane[tintaCamera::enUMIN].mNormal = fC0*kDVec + fC1*kUVec;
    m_akPlane[tintaCamera::enUMIN].mMag =
        kLoc.dot(m_akPlane[tintaCamera::enUMIN].mNormal);

    // update the top plane
    fInvLength = 1.0f/TintaMath::sqrt(fDMin2 + fUMax2);
    fC0 = +mFrustum[tintaCamera::enUMAX]*fInvLength;  // D component
    fC1 = -mFrustum[tintaCamera::enDMIN]*fInvLength;  // U component
    m_akPlane[tintaCamera::enUMAX].mNormal = fC0*kDVec + fC1*kUVec;
    m_akPlane[tintaCamera::enUMAX].mMag =
        kLoc.dot(m_akPlane[tintaCamera::enUMAX].mNormal);

    // update the left plane
    fInvLength = 1.0f/TintaMath::sqrt(fDMin2 + fRMin2);
    fC0 = -mFrustum[tintaCamera::enRMIN]*fInvLength;  // D component
    fC1 = +mFrustum[tintaCamera::enDMIN]*fInvLength;  // R component
    m_akPlane[tintaCamera::enRMIN].mNormal = fC0*kDVec + fC1*kRVec;
    m_akPlane[tintaCamera::enRMIN].mMag =
        kLoc.dot(m_akPlane[tintaCamera::enRMIN].mNormal);

    // update the right plane
    fInvLength = 1.0f/TintaMath::sqrt(fDMin2 + fRMax2);
    fC0 = +mFrustum[tintaCamera::enRMAX]*fInvLength;  // D component
    fC1 = -mFrustum[tintaCamera::enDMIN]*fInvLength;  // R component
    m_akPlane[tintaCamera::enRMAX].mNormal = fC0*kDVec + fC1*kRVec;
    m_akPlane[tintaCamera::enRMAX].mMag =
        kLoc.dot(m_akPlane[tintaCamera::enRMAX].mNormal);

    // all planes are active initially
    m_uiPlaneState = (m_uint32)~0;
}

bool tintaCuller::IsVisible (const tintaBoundingVolume* pkBound)
{
    // Start with the last pushed plane, which is potentially the most
    // restrictive plane.
    int iP = m_iPlaneQuantity - 1;
    m_uint32 uiMask = 1 << iP;

    for (int i = 0; i < m_iPlaneQuantity; i++, iP--, uiMask >>= 1)   {
        if ( m_uiPlaneState & uiMask )    {
            EnGeomSide iSide = pkBound->whichSide(m_akPlane[iP]);

            if ( iSide ==  SIDE_BACK ) {                
                return false;
            }

            if ( iSide ==  SIDE_FRONT )  {                
                m_uiPlaneState &= ~uiMask;
            }
        }
    }

    return true;
}

bool tintaCuller::IsVisible (int iVertexQuantity, const tintaVector3f* akVertex,  bool bIgnoreNearPlane){

    
    int iP = m_iPlaneQuantity - 1;
    for (int i = 0; i < m_iPlaneQuantity; i++, iP--)
    {
        tintaPlane & rkPlane = m_akPlane[iP];
        if (bIgnoreNearPlane && iP == tintaCamera::enDMIN)
        {
            continue;
        }

        int iV;
        for (iV = 0; iV < iVertexQuantity; iV++) {
            EnGeomSide iSide = rkPlane.whichSide(akVertex[iV]);
            if (iSide == SIDE_FRONT || iSide == SIDE_NOSIDE ) {
                
                break;
            }
        }

        if (iV == iVertexQuantity) {
            // polygon is totally outside this plane
            return false;
        }
    }

    return true;
}

EnGeomSide tintaCuller::whichSide (const tintaPlane& rkPlane) const
{
    // The plane is N*(X-C) = 0 where the * indicates dot product.  The signed
    // distance from the tintaCamera location E to the plane is N*(E-C).
    float fNdEmC = rkPlane.distanceTo(m_Cam->getPosition());

    float fNdD = rkPlane.mNormal.dot(m_Cam->getDVector());
    float fNdU = rkPlane.mNormal.dot(m_Cam->getUVector());
    float fNdR = rkPlane.mNormal.dot(m_Cam->getRVector());
    float fFdN = mFrustum[tintaCamera::enDMAX]/mFrustum[tintaCamera::enDMIN];

    int iPositive = 0, iNegative = 0;
    float fSgnDist;

    // check near-plane vertices
    float fPDMin = mFrustum[tintaCamera::enDMIN]*fNdD;
    float fNUMin = mFrustum[tintaCamera::enUMIN]*fNdU;
    float fNUMax = mFrustum[tintaCamera::enUMAX]*fNdU;
    float fNRMin = mFrustum[tintaCamera::enRMIN]*fNdR;
    float fNRMax = mFrustum[tintaCamera::enRMAX]*fNdR;

    // V = E + dmin*D + umin*U + rmin*R
    // N*(V-C) = N*(E-C) + dmin*(N*D) + umin*(N*U) + rmin*(N*R)
    fSgnDist = fNdEmC + fPDMin + fNUMin + fNRMin;
    if (fSgnDist > 0.0f)
    {
        iPositive++;
    }
    else if (fSgnDist < 0.0f)
    {
        iNegative++;
    }

    // V = E + dmin*D + umin*U + rmax*R
    // N*(V-C) = N*(E-C) + dmin*(N*D) + umin*(N*U) + rmax*(N*R)
    fSgnDist = fNdEmC + fPDMin + fNUMin + fNRMax;
    if (fSgnDist > 0.0f)
    {
        iPositive++;
    }
    else if (fSgnDist < 0.0f)
    {
        iNegative++;
    }

    // V = E + dmin*D + umax*U + rmin*R
    // N*(V-C) = N*(E-C) + dmin*(N*D) + umax*(N*U) + rmin*(N*R)
    fSgnDist = fNdEmC + fPDMin + fNUMax + fNRMin;
    if (fSgnDist > 0.0f)
    {
        iPositive++;
    }
    else if (fSgnDist < 0.0f)
    {
        iNegative++;
    }

    // V = E + dmin*D + umax*U + rmax*R
    // N*(V-C) = N*(E-C) + dmin*(N*D) + umax*(N*U) + rmax*(N*R)
    fSgnDist = fNdEmC + fPDMin + fNUMax + fNRMax;
    if (fSgnDist > 0.0f)
    {
        iPositive++;
    }
    else if (fSgnDist < 0.0f)
    {
        iNegative++;
    }

    // check far-plane vertices (s = dmax/dmin)
    float fPDMax = mFrustum[tintaCamera::enDMAX]*fNdD;
    float fFUMin = fFdN*fNUMin;
    float fFUMax = fFdN*fNUMax;
    float fFRMin = fFdN*fNRMin;
    float fFRMax = fFdN*fNRMax;

    // V = E + dmax*D + umin*U + rmin*R
    // N*(V-C) = N*(E-C) + dmax*(N*D) + s*umin*(N*U) + s*rmin*(N*R)
    fSgnDist = fNdEmC + fPDMax + fFUMin + fFRMin;
    if (fSgnDist > 0.0f)
    {
        iPositive++;
    }
    else if (fSgnDist < 0.0f)
    {
        iNegative++;
    }

    // V = E + dmax*D + umin*U + rmax*R
    // N*(V-C) = N*(E-C) + dmax*(N*D) + s*umin*(N*U) + s*rmax*(N*R)
    fSgnDist = fNdEmC + fPDMax + fFUMin + fFRMax;
    if (fSgnDist > 0.0f)
    {
        iPositive++;
    }
    else if (fSgnDist < 0.0f)
    {
        iNegative++;
    }

    // V = E + dmax*D + umax*U + rmin*R
    // N*(V-C) = N*(E-C) + dmax*(N*D) + s*umax*(N*U) + s*rmin*(N*R)
    fSgnDist = fNdEmC + fPDMax + fFUMax + fFRMin;
    if (fSgnDist > 0.0f)
    {
        iPositive++;
    }
    else if (fSgnDist < 0.0f)
    {
        iNegative++;
    }

    // V = E + dmax*D + umax*U + rmax*R
    // N*(V-C) = N*(E-C) + dmax*(N*D) + s*umax*(N*U) + s*rmax*(N*R)
    fSgnDist = fNdEmC + fPDMax + fFUMax + fFRMax;
    if (fSgnDist > 0.0f)
    {
        iPositive++;
    }
    else if (fSgnDist < 0.0f)
    {
        iNegative++;
    }

    if (iPositive > 0)
    {
        if (iNegative > 0)
        {
            // frustum straddles the plane
            return SIDE_NOSIDE;
        }

        // frustum is fully on the positive side
        return SIDE_FRONT;
    }

    // frustum is fully on the negative side
    return SIDE_BACK;
}

void tintaCuller::ComputeVisibleSet(tintaDrawable* scene, bool noCull){
    if ( !scene)
        return;
     
     if ( m_Cam )
     {
         setFrustum(m_Cam->getFrustum());
         mVisible.Clear();
         scene->onGetVisibleSet( *this, noCull );
     }
}


void tintaCuller::setCamera(const tintaCamera* pkCamera)
{
    m_Cam = pkCamera;
}

const tintaCamera* tintaCuller::getCamera() const
{
    return m_Cam;
}

const float* tintaCuller::getFrustum() const
{
    return mFrustum;
}

tintaVisibleSet& tintaCuller::getVisibleSet()
{
    return mVisible;
}

void tintaCuller::Insert(tintaDrawable* pkObject)
{
    mVisible.Insert(pkObject);
}

int tintaCuller::GetPlaneQuantity() const
{
    return m_iPlaneQuantity;
}

const tintaPlane* tintaCuller::GetPlanes() const
{
    return m_akPlane;
}

void tintaCuller::setPlaneState(m_uint32 uiPlaneState)
{
    m_uiPlaneState = uiPlaneState;
}

m_uint32 tintaCuller::GetPlaneState() const
{
    return m_uiPlaneState;
}

void tintaCuller::PushPlane(const tintaPlane& rkPlane)
{
    if (m_iPlaneQuantity < VS_MAX_PLANE_QUANTITY)
    {

        m_akPlane[m_iPlaneQuantity++] = rkPlane;
    }
}

void tintaCuller::PopPlane()
{
    if (m_iPlaneQuantity > tintaCamera::enQUANTITY)
    {
        // Frustum planes may not be removed from the stack.
        m_iPlaneQuantity--;
    }
}


}

