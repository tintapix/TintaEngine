/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_SPATIAL_H
#define _TINTA_SPATIAL_H

#include <Math/tintaQuatern.h>
#include <Math/tintaMatrix33.h>

namespace Graphica
{

    class _CoreExport tintaSpatial
        : public AllocatedObjectSpatialObject
    {

    public:

        enum MoveAxies{
            enAxisX,
            enAxisY,
            enAxisZ,
        };

        tintaSpatial();

        virtual ~tintaSpatial();

        virtual void roll(float angle);

        virtual void pitch(float angle);

        virtual void yaw(float angle);
        
        virtual void rotate(const tintaVector3f& axis, float angle, bool bUpdateChildren = true, bool updateLocal = true);

        virtual void rotate(const tintaQuatern& q, bool bUpdateChildren = true, bool updateLocal = true);

        virtual void rotate(float angleX, float angleY, float angleZ, bool bUpdateChildren = true, bool updateLocal = true);

        virtual void moveTo(const tintaVector3f& pos, bool bUpdateChildren = true, bool updateLocal = true);

        virtual void moveOnOrient(const tintaVector3f& addPos, bool bUpdateChildren = true, bool updateLocal = true);

        virtual void moveOn(const tintaVector3f& addPos, bool bUpdateChildren = true, bool updateLocal = true);

        virtual void move(MoveAxies ax, float val, bool bUpdateChildren = true, bool updateLocal = true);

        virtual void mulScale(const tintaVector3f& scale, bool bUpdateChildren = true);

        virtual void setScale(const tintaVector3f& scale, bool bUpdateChildren = true);

        virtual const tintaQuatern  &getOrientation() const;

        virtual void                 setOrientation(const tintaQuatern &orient, bool bUpdateChildren = true);

        virtual const tintaQuatern  &getLocalOrientation() const;

        virtual tintaVector3f getScale() const;

        virtual void  updateOrientation( const tintaQuatern &orient, bool bUpdateChildren = true, bool updateLocal = true);

        virtual void  shiftPosition(const tintaVector3f &shift);        

        virtual const tintaVector3f &getPosition() const;

        virtual const tintaVector3f &getLocalPosition() const;

        virtual void addAction(tintaAction *action);

        virtual tintaBasicStringVector updateActions(m_uint32 mskElapsed);

        virtual void deleteActions();
       
        virtual const tintaMatrix44 &getWorld();

        virtual tintaDrawable*  getParent();

        virtual tintaBoundingVolume *getBoundary();

        virtual const tintaBoundingVolume *getBoundary() const ;

        virtual void createBoundary( BVType bv ); 

        virtual void updateModel(bool bUpdateNormals = false);
       

    protected:

        virtual void  setPosition(const tintaVector3f &pos);

        virtual void updateChildData();

        tintaQuatern    mLocalOrientation;       

        tintaVector3f   mLocalPosition;

        entityActions_t mActions;         
        
        tintaVector3f mScale = { 1.f, 1.f, 1.f };

        tintaMatrix44 mHomWorld;

        tintaBoundingVolume *mBound = NULL_M;

    private:

        tintaQuatern    mOrientation;

        tintaVector3f   mPosition;
       
    };

}
#endif


