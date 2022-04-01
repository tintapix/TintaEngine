/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_TRIANGLES_H
#define TINTA_TRIANGLES_H


#include "tintaGeometry.h"

namespace Graphica {

class _CoreExport tintaTriangles
	: public tintaGeometry
{    

public:    
    virtual ~tintaTriangles();

    virtual int getTriangleQuantity () const = 0;

    virtual bool getTriangle (int i, int& riV0, int& riV1, int& riV2) const = 0;      

    void genNormals ();

protected:    

    tintaTriangles(tintaGLRenderer *rend = NULL_M);

    //Triangles (tintaVertexBuffer* pkVBuffer, tintaIndexBuffer* pIBuffer);
    virtual void updateNormals ();
};


//typedef Tinta::SharedPtr<Triangles> TrianglesPtr;

}

#endif
