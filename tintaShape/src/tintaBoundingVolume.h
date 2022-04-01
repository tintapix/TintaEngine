/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_BOUNDING_VOLUME_H_
#define _TINTA_BOUNDING_VOLUME_H_


#include <Math/tintaVector3.h>
#include "tintaPlane.h"
#include "tintaVertexBuffer.h"

namespace Graphica {

	

	class  _CoreExport tintaBoundingVolume:
		public AllocatedObjectBoundVolume
	{
	public:
		
		virtual ~tintaBoundingVolume ();
		
		virtual void setCenter (const tintaVector3f& center)	= 0;

		virtual void setRadius (float rad)					= 0;

		virtual tintaVector3f getCenter () const			    = 0;

		virtual float         getRadius () const			    	= 0;		
		
		virtual void computeFromData (const tintaVertexBuffer* vBuffer) = 0;	

        virtual void scale( float scaleFactor ) = 0;

        // for box BV
        virtual void setOrientation( const tintaQuatern    &orientation ) = 0;
				
		virtual EnGeomSide whichSide (const tintaPlane& plane) const = 0;
        		
		virtual void copyFrom (const tintaBoundingVolume* inpust) = 0;
		
		virtual void growToContain (const tintaBoundingVolume* input) = 0;
        		
		virtual bool contains (const tintaVector3f& point) const = 0;

	protected:
		tintaBoundingVolume ();
	};

}
#endif