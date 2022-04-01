/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef	_TINTA_PLANE_H_	
#define _TINTA_PLANE_H_

#include "tintaGraphicaPredefine.h"
#include <Math/tintaVector3.h>

namespace Graphica
{

class _CoreExport tintaPlane
{
public:
    
    tintaPlane ();  
    tintaPlane (const tintaPlane& plane);
  
    tintaPlane (const tintaVector3f& normal, float len);
    
    tintaPlane(const tintaVector3f& normal, const tintaVector3f& p);
    
    tintaPlane (const tintaVector3f& p0, const tintaVector3f& p1, const tintaVector3f& p2);
    
    tintaPlane& operator= (const tintaPlane& plane);
    
    EnGeomSide whichSide (const tintaVector3f& p) const;
    
    float distanceTo (const tintaVector3f& rv) const;

    tintaVector3f mNormal;
    float        mMag;
};






}

#endif
