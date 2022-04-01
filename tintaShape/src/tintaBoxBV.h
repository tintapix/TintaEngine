/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_BOX_BV_H_
#define _TINTA_BOX_BV_H_

#include <Math/tintaBox3.h>
#include "tintaBoundingVolume.h" 
#include "tintaPlane.h"


namespace Graphica
{
	
	class _CoreExport tintaBoxBV
		: public tintaBoundingVolume
	{

	public:
		
		tintaBoxBV (); 
		tintaBoxBV (const tintaBox3f& bvBox);
		virtual ~tintaBoxBV ();		

		virtual void         setCenter (const tintaVector3f& center);

		virtual void         setRadius (float rad);

		virtual tintaVector3f getCenter () const;

		virtual float        getRadius () const;

		tintaBox3f&           getBox ();

        const tintaBox3f& getBox() const;


		virtual void computeFromData ( const tintaVertexBuffer* vBuff );
        		
        virtual EnGeomSide whichSide ( const tintaPlane& plane ) const;
        		
		virtual bool testIntersection (const tintaVector3f& origin, 
            const tintaVector3f& dir, float vMin, float vMax) const;
        		
		virtual bool testIntersection (const tintaBoundingVolume* box) const;
        		
        virtual void copyFrom(const tintaBoundingVolume* box);
        		
        virtual void growToContain(const tintaBoundingVolume* box);

        virtual void scale(float scaleFactor);
        		
		virtual bool contains (const tintaVector3f& point) const;

        virtual void setOrientation(const tintaQuatern    &orientation);

protected:
		tintaBox3f mBox;

        tintaQuatern    mOrientation;

        float mScaleFactor = 1.0f;       
	};
}

#endif