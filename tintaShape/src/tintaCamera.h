/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CAMERA_H_
#define _TINTA_CAMERA_H_

#include "tintaGraphicaPredefine.h"
#include "tintaAction.h"
#include <tintaCommon.h>
#include <Math/tintaVector3.h>
#include <Math/tintaQuatern.h>
#include "tintaSpatial.h"

namespace Graphica
{

class _CoreExport tintaCamera :
    public  tintaSpatial {

public:
    
   
    tintaCamera();
    virtual ~tintaCamera();
    
    void setFrame (const tintaVector3f& loc, const tintaVector3f& dir,
                  const tintaVector3f& uVec, const tintaVector3f& rVec);
    
    void setDirection(const tintaVector3f& dir);

    void update();

    void setAxes (const tintaVector3f& DVector, 
                        const tintaVector3f& UVector, 
                            const tintaVector3f& RVector);

    tintaVector3f getDVector () const;
    tintaVector3f getUVector () const;
    tintaVector3f getRVector () const;

    enum
    {
        enDMIN = 0,  // near
        enDMAX = 1,  // far
        enUMIN = 2,  // bottom
        enUMAX = 3,  // top
        enRMIN = 4,  // left
        enRMAX = 5,  // right
        enQUANTITY = 6
    };


    bool isPerspective() const;

    void setPerspective( bool perspective );

    void setFrustum(float rightMin, float rightMax, float upMin, float upMax,
        float farMin, float farMax);
    
    void setFrustum (const float* afFrustum);
      
    void setFrustum(float fovDegrees, float aspectRatio, float dMin, float dMax);    
    void getFrustum (float& rMin, float& rMax, float& uMin, float& uMax, float& dMin, float& dMax) const;

    const float* getFrustum () const;    

    bool getFrustum (float& fovDegrees, float& aspectRatio,
        float& dMin, float& dMax) const;
    
    float getDMin () const;
    float getDMax () const;
    float getUMin () const;
    float getUMax () const;
    float getRMin () const;
    float getRMax () const;
    
    
    void setViewport (float left, float right, float top, float bottom);
    void getViewport (float& left, float& right, float& top,
        float& bottom);

    // depth range (contained in [0,1])
    void setDepthRange (float dNear, float dFar);
    void getDepthRange (float& dNear, float& dFar);   

    void onUpdate();
       
    virtual void moveTo(const tintaVector3f& pos, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void moveOn(const tintaVector3f& addPos, bool bUpdateChildren = true, bool updateLocal = true);

    void moveOnOrient(const tintaVector3f& loc, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void  updateOrientation(const tintaQuatern &orient);

    virtual void roll(float angle);

    virtual void pitch(float angle);

    virtual void yaw(float angle);

    virtual void rotate(const tintaVector3f& axis, float angle, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void rotate(const tintaQuatern& q, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void rotate(float angleX, float angleY, float angleZ, bool bUpdateChildren = true, bool updateLocal = true);

protected:
	
    void update(const tintaVector3f& dVec, const tintaVector3f& uVec, const tintaVector3f& rVec);

    tintaVector3f m_DVector, m_UVector, m_RVector;
    
    float mFrustum[enQUANTITY];    

    float m_portLeft, m_portRight, m_portTop, m_portBottom, m_portNear, m_portFar;    
    bool m_perspective;

    friend class tintaGLRenderer;

    tintaGLRenderer* m_pRenderer;
    
};


//typedef Tinta::SharedPtr<tintaCamera> CameraPtr;

}

#endif
