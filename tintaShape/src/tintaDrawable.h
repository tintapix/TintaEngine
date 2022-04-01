/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_DRAWABLE_H
#define _TINTA_DRAWABLE_H

#include "tintaBoundingVolume.h"
#include "tintaMaterial.h"
#include "tintaSpatial.h"
#include <tintaMemoryAllocator.h>

namespace Graphica
{


class tintaCuller;
class MaterialData;

class _CoreExport tintaDrawable
    : public tintaSpatial
{
    
public:	
    
    typedef UNIQPTRDEF_T(Graphica::tintaMaterial::MaterialData) MaterialDataPtr_t;       
      
    virtual ~tintaDrawable();
       
    enum CullingMode
    {
        CULL_DYNAMIC,

        CULL_ALWAYS,

        CULL_NEVER,

        MAX_CULLING_MODE
    };

    CullingMode mCulling;    
   
    void updateBound ();

    void setCullMode(CullingMode mode);            
        
    void addMaterial( const String &name , tintaMaterial *mat = NULL_M );

    void addMaterial( tintaMaterial *mat );     

    void removeMaterial( const String &name );

    void activateMaterial(const String &name);

    void removeMaterials();     

    bool enableMaterial();

    bool disableMaterial();

    virtual bool isVisible()const ;

    virtual void setVisible( bool visible );

    virtual void moveTo(const tintaVector3f& pos, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void moveOnOrient(const tintaVector3f& addPos, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void moveOn(const tintaVector3f& addPos, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void move(MoveAxies ax, float val, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void setOrientation(const tintaQuatern &orient, bool bUpdateChildren = true);

    virtual void updateOrientation(const tintaQuatern &orient, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void roll(float angle);

    virtual void pitch(float angle);

    virtual void yaw(float angle);

    virtual void rotate(const tintaVector3f& axis, float angle, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void rotate(const tintaQuatern& q, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void rotate(float angleX, float angleY, float angleZ, bool bUpdateChildren = true, bool updateLocal = true);

    virtual void mulScale( const tintaVector3f& scale, bool bUpdateChildren = true );

    virtual void setScale(const tintaVector3f& scale, bool bUpdateChildren = true);

    const tintaMaterial   *getMaterial() const ;

    tintaMaterial         *getMaterial();

    tintaMaterial         *findMaterial(const String &name);            

    const tintaGLRenderer *getRenderer()const ;

    void setRenderer(tintaGLRenderer     *rend);

    const String &getName()const;

    void setName(const String &name );    
   
    virtual tintaDrawable*  getParent();

    virtual const tintaDrawable*  getParent() const;

    tintaDrawable(tintaGLRenderer *rend = NULL_M);    
    
    virtual bool addChildren( tintaDrawable * child );

    virtual tintaDrawable*  getChildren( const String &name, bool deepSearch );

    virtual drawablelist_t& getChildrenList();

    virtual const drawablelist_t& getChildrenList() const;

    virtual tintaDrawable*  getChildren( const String &nextAfter = _M("") );

    virtual bool dettachChildren( const String &name );

    virtual void dettachChildren();

    void getVisibleSet( tintaCuller& culler, bool bNoCull );         

    bool onMatVarTexture(const String &nameMaterial, const String &nameVar, tintaTexture *texture, int index);

    // dynamically changing material variables
    virtual bool onMatVarFloat(const String &matName, const String  &varName, float v);

    virtual bool onMatVarDouble(const String &matName, const StringBasic  &varName, double v);

    virtual bool onMatVarUInt(const String &matName, const StringBasic  &varName, m_uint32  v);

    virtual bool onMatVarInt(const String &matName, const StringBasic  &varName, int  v );

    virtual bool onMatVarBool(const String &matName, const StringBasic  &varName, bool  v );

    virtual bool onMatVarfVec2(const String &matName, const StringBasic  &varName, const tintaVector2f &v );

    virtual bool onMatVarfVec3(const String &matName, const StringBasic  &varName, const tintaVector3f &v );

    virtual bool onMatVarfVec4(const String &matName, const StringBasic  &varName, const tintaVector4f &v);

    virtual bool onMatVarfMat3(const String &matName, const StringBasic  &varName, const tintaMatrix33 &v);

    virtual bool onMatVarfMat4(const String &matName, const StringBasic  &varName, const tintaMatrix44 &v);    

protected:   
       
    struct ObjectMaterials {
        tintaMaterial* material; 

        struct Variable {
            String name;
            Graphica::tintaMaterial::MaterialData *data;
        };

        struct Texture {
            String        name;
            tintaTexture *texture;
            int           index;
            //bool    isManual = false;
        };
        std::list< Variable > matVariables;
        std::list<Texture> matManualTextures;
    };

    typedef std::list<ObjectMaterials> tintaMatvec_t;

    virtual void    updateWorldData ();   

    void            boundToRoot ();      

    drawablelist_t    mChildren;

    tintaMatvec_t     mMaterials;

    tintaGLRenderer  *mRend;

    String            mName;

    bool mVisible = true;

    tintaDrawable*    mpParent;

    virtual void  updateChildData();

public:    

    // parent access (tintaNode calls this during attach/detach of children)
    void setParent(tintaDrawable* parent);

    // culling
    void          onGetVisibleSet (tintaCuller& culler, bool bNoCull);    
};





}


#endif
