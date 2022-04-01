/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_TRIMESH_H
#define TINTA_TRIMESH_H


#include "tintaTriangles.h"

namespace Graphica {


class _CoreExport tintaTriMesh
	: public tintaTriangles
{
    
public:   

    virtual ~tintaTriMesh();
      

    tintaTriMesh(tintaGLRenderer *rend = NULL_M);

    
    virtual int getTriangleQuantity () const;

    virtual bool getTriangle(int i, int& v0, int& v1, int& v2) const;
    
    virtual void createVBuffer( const tintaAttributes &attrib, int verts);

    static tintaTriMesh* createFromMesh(const String &name);
};

}

#endif
