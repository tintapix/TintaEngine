#ifndef TINTA_BILLBOARD_H_
#define TINTA_BILLBOARD_H_

#include "tintaTriMesh.h"


namespace Graphica {

    class tintaBillboard :
        public tintaTriMesh
    {
    public:
        tintaBillboard(bool bAxies = false);
        tintaBillboard(tintaGLRenderer *rend, bool bAxies = false );
        virtual ~tintaBillboard();

        virtual void updateWorldData( );

    private:
        // if true fully faces to camera like particle
        bool mbAxies = false;

    };
}

#endif
