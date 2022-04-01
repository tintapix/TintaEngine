/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ENTITY_ACTION_H_
#define _TINTA_ENTITY_ACTION_H_

#include <tintaPredefine.h>



namespace Graphica {


    class tintaDrawable;

    struct _CoreExport tintaAction
        : public AllocatedMemObject {


        enum ActionType {
            enMove,
            enRotate
        };


        tintaAction();

        tintaAction(float speed, const StringBasic &onFinish );

        virtual ~tintaAction();

        virtual bool onAction( float mlsElapsed ) = 0;

        virtual ActionType getType() const  = 0;

        virtual void setSpeed( float speed );

        // Get on finish script handler
        const StringBasic &getCommand()const;

    protected:
        float mSpeed = 0.f;

        float mLastTime = 0.f;

        // On finish script handler
        StringBasic mOnFinish;
    };


    struct _CoreExport tintaEntityAction
        : public tintaAction
    {
        tintaEntityAction();

        tintaEntityAction(tintaDrawable * ent, float speed, const StringBasic &onFinish);

        virtual ~tintaEntityAction();

        virtual void setEntity(tintaDrawable * ent);        

        virtual bool onAction(float mlsElapsed) = 0;

       

    protected:
        tintaDrawable * mEnt;
        
    };


    class _CoreExport tintaEntityMoveAction
        : public tintaEntityAction {
    public:
        tintaEntityMoveAction();
        tintaEntityMoveAction(tintaDrawable * ent, const tintaVector3f &fromPos, const tintaVector3f &destPos, float speed
            , const StringBasic &onFinish);
        virtual ~tintaEntityMoveAction();
        
        virtual bool onAction(float mlsElapsed);
        void setPositions(const tintaVector3f &fromPos, const tintaVector3f &destPos);

        virtual ActionType getType() const;

    protected:
        tintaVector3f mFromPos;
        tintaVector3f mDestPos;
        float mFinalLen;
        float mCurLen;
        bool  minit;

    };  


    class _CoreExport tintaEntityRotateInfiniteAction
        : public tintaEntityAction {
    public:
        tintaEntityRotateInfiniteAction();

        // xSpeed - angle speed in radians
        // ySpeed - angle speed in radians
        // zSpeed - angle speed in radians
        tintaEntityRotateInfiniteAction( tintaDrawable * ent, float xSpeed, float ySpeed, float zSpeed );

        virtual ~tintaEntityRotateInfiniteAction();
        virtual bool onAction(float mlsElapsed);
        virtual void setSpeed( float xSpeed, float ySpeed, float zSpeed );

        virtual ActionType getType() const;

    protected: 
       
        float mXSpeed; // radian
        float mYSpeed; // radian
        float mZSpeed; // radian        
    };


    class tintaEntityRotateAction
        : public tintaEntityAction {
    public:
        tintaEntityRotateAction();

        // xAngle - angle in radians
        // ySpeed - angle in radians
        // zSpeed - angle in radians
        tintaEntityRotateAction(tintaDrawable * ent, float xAngle, float yAngle, float zAngle, float speed
            , const StringBasic &onFinish);

        virtual ~tintaEntityRotateAction();
        virtual bool onAction(float mlsElapsed);        

        virtual ActionType getType() const;

    protected:
        float mXAngle; // radian
        float mYAngle; // radian
        float mZAngle; // radian  
        float mSpeed;  // speed        
        
    private:
        
        float mSumRotate = 0.f;

        tintaQuatern mFrom;
        tintaQuatern mDest;
    };



    


    struct _CoreExport tintaCameraAction
        : public tintaAction
    {
        tintaCameraAction();

        tintaCameraAction(tintaCamera * ent, float speed, const StringBasic &onFinish);

        virtual ~tintaCameraAction();

        virtual void setCamera(tintaCamera * ent);        

        virtual bool onAction(float mlsElapsed) = 0;

    protected:
        tintaCamera * mCam;

    };

    
    class _CoreExport tintaCameraMoveAction
		: public tintaCameraAction {
    public:
		tintaCameraMoveAction();
		tintaCameraMoveAction(tintaCamera * cam, const tintaVector3f &fromPos, const tintaVector3f &destPos, float speed
            , const StringBasic &onFinish);
		virtual ~tintaCameraMoveAction();

        virtual bool onAction(float mlsElapsed);
        void setPositions(const tintaVector3f &fromPos, const tintaVector3f &destPos);

        virtual ActionType getType() const;

    protected:
        tintaVector3f mFromPos;
        tintaVector3f mDestPos;
        float mFinalLen;
        float mCurLen;
        bool minit;

    };

    class _CoreExport tintaCameraRotateAction
        : public tintaCameraAction {
    public:
        tintaCameraRotateAction();

        // xAngle - angle in radians
        // ySpeed - angle in radians
        // zSpeed - angle in radians
        tintaCameraRotateAction(tintaCamera * ent, float xAngle, float yAngle, float zAngle, float speed
            , const StringBasic &onFinish);

        virtual ~tintaCameraRotateAction();
        virtual bool onAction(float mlsElapsed);

        virtual ActionType getType() const;

    protected:
        float mXAngle; // radian
        float mYAngle; // radian
        float mZAngle; // radian  
        float mSpeed;  // speed        

    private:

        float mSumRotate = 0.f;

        tintaQuatern mFrom;
        tintaQuatern mDest;
    };
    
    
    typedef std::list<tintaAction*> entityActions_t;
    typedef entityActions_t::iterator entityActions_it;
}


#endif
