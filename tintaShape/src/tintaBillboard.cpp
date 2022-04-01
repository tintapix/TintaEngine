/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaBillboard.h"

namespace Graphica {

    tintaBillboard::tintaBillboard(bool bAxies):
        mbAxies(bAxies)
    {
    }

    tintaBillboard::tintaBillboard(tintaGLRenderer *rend, bool bAxies )
        :tintaTriMesh( rend ),
         mbAxies(bAxies){        
    }


    tintaBillboard::~tintaBillboard(){
    }

    void tintaBillboard::updateWorldData(  ) {

        const tintaCamera* cam = mRend ? mRend->getCamera() : NULL_M;
        if ( cam ) {
            if ( !mbAxies ) {
                float fAngle = TintaMath::atan2(cam->getPosition().mx, cam->getPosition().mz);
                tintaQuatern q;
                q.fromAngleAxis(fAngle + K_PI, tintaVector3f::yAxis);
                updateOrientation(q);
            }
            else {
                float fAnglex = TintaMath::atan2( cam->getPosition().my, cam->getPosition().mz );
                
                tintaQuatern qx;
                //qx.fromAngleAxis(fAnglex + K_PI, -tintaVector3f::xAxis);
               
                float fAngley = TintaMath::atan2(cam->getPosition().mx, cam->getPosition().mz);
                qx.set(0.f, fAngley + K_PI, fAnglex + K_PI);
                updateOrientation(qx);

                //tintaQuatern qy;
                //qy.fromAngleAxis(fAngley + K_PI, tintaVector3f::yAxis);                              
                //updateOrientation( qy );               
            }
        }
        tintaGeometry::updateWorldData( );
    }
}
