/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_SPHEREBV_H
#define TINTA_SPHEREBV_H


#include "tintaBoundingVolume.h"
#include <Math/tintaSphere3.h>

namespace Graphica
{


class _CoreExport tintaSphereBV
	: public tintaBoundingVolume
{
    

public:
    
    tintaSphereBV ();  

    tintaSphereBV(const tintaSphere3f& sphere);
    
    virtual void setCenter (const tintaVector3f& center);

    virtual void setRadius (float fRadius);

    virtual tintaVector3f getCenter () const;

    virtual float         getRadius () const;

    tintaSphere3f&		  getSphere();

    const tintaSphere3f&   getSphere() const;
    
    virtual void computeFromData (const tintaVertexBuffer* vBuffer);
       
    virtual EnGeomSide whichSide (const tintaPlane& plane) const;        
    
    virtual void copyFrom(const tintaBoundingVolume* sphere);
   
    virtual void growToContain(const tintaBoundingVolume* sphere);

    virtual void scale(float scaleFactor);
    
    virtual bool contains(const tintaVector3f& p) const;

    virtual void setOrientation(const tintaQuatern    &orientation);

protected:
    tintaSphere3f mSphere;

    float mScaleFactor = 1.f;
};






}

#endif
