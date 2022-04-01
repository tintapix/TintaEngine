#ifndef TINTA_GEOMETRY_H_
#define TINTA_GEOMETRY_H_

#include "tintaGraphicaPredefine.h"
#include "tintaDrawable.h"
#include "tintaVertexBuffer.h"
#include "tintaIndexBuffer.h"
#include "tintaBoundingVolume.h"

namespace Graphica {

	class _CoreExport tintaGeometry
		: public tintaDrawable {	

	public:
		
		virtual ~tintaGeometry (); 		

			
		
        /*
            Call after model data change: updates bound and normals
        */
		virtual void updateModel ( bool bUpdateNormals = true );

        virtual void                     createVBuffer( const tintaAttributes &attrib, int verts, bool update = false );

        virtual tintaVertexBuffer        *getVBuffer();

        virtual const tintaVertexBuffer  *getVBuffer() const;

        virtual void                      createIBuffer( int verts);

        virtual       tintaIndexBuffer    *getIBuffer();

        virtual const tintaIndexBuffer    *getIBuffer() const;   
      
	protected:
      

		tintaGeometry ();

        tintaGeometry( tintaGLRenderer *rend = NULL_M );	
        
		virtual void getVisibleSet ( tintaCuller& culler, bool bNoCull );

        tintaVertexBuffer *mVertexBuf = NULL_M;
        tintaIndexBuffer  *mIndexBuf = NULL_M;

        GeometryType  mGeomType;

        virtual void updateModelBound();
       
        // internal use
	public:		

		virtual void updateWorldData ();      

        virtual GeometryType getGeomType();
        
	};

	//inline void tintaGeometry::updateNormals (){		
	//}
}


#endif