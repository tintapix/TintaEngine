/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CULLER_H
#define _TINTA_CULLER_H

#include "tintaCamera.h"
#include "tintaVisibleObject.h"
#include "tintaVisibleSet.h"

#include "tintaBoundingVolume.h"

namespace Graphica
{

class tintaDrawable;

class _CoreExport tintaCuller
{
public:
    
    tintaCuller (int iMaxQuantity = 0, int iGrowBy = 0, const tintaCamera* pkCamera = 0);
    virtual ~tintaCuller ();
    
    void setCamera (const tintaCamera* pkCamera);
    const tintaCamera* getCamera () const;
    void setFrustum (const float* afFrustum);
    const float* getFrustum () const;
    tintaVisibleSet& getVisibleSet ();

    virtual void Insert(tintaDrawable* pkObject);
        
    enum { VS_MAX_PLANE_QUANTITY = 32 };
    int GetPlaneQuantity () const;
    const tintaPlane* GetPlanes () const;
    void setPlaneState (m_uint32 uiPlaneState);
    m_uint32 GetPlaneState () const;
    void PushPlane (const tintaPlane& rkPlane);
    void PopPlane ();
       
    bool IsVisible (const tintaBoundingVolume* pkWound);

    
    bool IsVisible (int iVertexQuantity, const tintaVector3f* akVertex,  bool bIgnoreNearPlane);
        
    EnGeomSide whichSide (const tintaPlane& rkPlane) const;
        
    void ComputeVisibleSet(tintaDrawable* pkScene, bool noCull = false);

protected:
    
    const tintaCamera* m_Cam;

    
    float mFrustum[tintaCamera::enQUANTITY];
       
    int m_iPlaneQuantity;
    tintaPlane m_akPlane[VS_MAX_PLANE_QUANTITY];
    m_uint32 m_uiPlaneState;

    
    tintaVisibleSet mVisible;
};




}

#endif
