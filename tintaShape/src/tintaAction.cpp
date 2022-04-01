/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaAction.h"
#include "tintaDrawable.H"
#include "tintaRoot.h"
#include <Math/tintaCommonMath.h>


namespace Graphica  {


    tintaAction::tintaAction() :        
        mSpeed(0.f)
    {
    }

    tintaAction::tintaAction(float speed, const StringBasic &onFinish) :
        mSpeed(speed),
        mOnFinish(onFinish)
    {
    }

    tintaAction::~tintaAction()
    {        
    }

	void tintaAction::setSpeed(float speed) {
		mSpeed = speed;
	}

    const StringBasic &tintaAction::getCommand()const {
        return mOnFinish;
    }


    tintaEntityAction::tintaEntityAction() :
        mEnt(NULL_M),
        tintaAction(0.f,"")
    {
    }

    tintaEntityAction::tintaEntityAction(tintaDrawable * ent, float speed, const StringBasic &onFinish) :
        mEnt(ent),
        tintaAction(speed, onFinish)
    {
    }

    tintaEntityAction::~tintaEntityAction()
    {
        mEnt = NULL_M;
    }

    void tintaEntityAction::setEntity(tintaDrawable * ent) {
        mEnt = ent;
    }

    tintaEntityMoveAction::tintaEntityMoveAction() :
        mDestPos{ tintaVector3f::origin },
        minit( false ),
        mFinalLen{0.f},
        mCurLen{0.f}{
    }


    tintaEntityMoveAction::tintaEntityMoveAction(tintaDrawable * ent, const tintaVector3f &fromPos,
                                const tintaVector3f &destPos, float speed, const StringBasic &onFinish) :
        tintaEntityAction(ent, speed, onFinish),
        mFromPos{ fromPos },
        mDestPos{ destPos }{
        minit = true;
        mFinalLen = distance<float>(fromPos, destPos);
        mCurLen = 0.f;
    }


    tintaAction::ActionType tintaEntityMoveAction::getType() const {
        return enMove;
    }



    tintaEntityMoveAction::~tintaEntityMoveAction() {

    }

    bool tintaEntityMoveAction::onAction(float mlsElapsed) {

        if (!minit || !mEnt)
            return false;

        if ( TintaMath::isZero(mLastTime) )
            mLastTime = mlsElapsed;

        float toMove = ( mlsElapsed - mLastTime ) * mSpeed;
       
        mCurLen += toMove;

        float fact = TintaMath::min(mCurLen / mFinalLen, 1.f);
       
        tintaVector3f r = lerp<float>( mFromPos, mDestPos, fact );
        
        tintaVector3f curPos;

        curPos = mEnt->getLocalPosition();        
       
        if ( mDestPos.equal( curPos, 0.001f ) )
            return true;

        mLastTime = mlsElapsed;
        
        mEnt->moveTo(r, true, true);

        return false;    
    }


    void tintaEntityMoveAction::setPositions(const tintaVector3f &fromPos, const tintaVector3f &destPos){
        mFromPos = fromPos;
        mDestPos = destPos;
        mFinalLen = distance<float>(fromPos, destPos);
        minit = true;
    }



    tintaEntityRotateInfiniteAction::tintaEntityRotateInfiniteAction(){

    }

    tintaEntityRotateInfiniteAction::tintaEntityRotateInfiniteAction( tintaDrawable * ent, float xSpeed, 
                                                float ySpeed, float zSpeed ) :
        tintaEntityAction( ent, 0.f, "" ),
        //mStart{ ent->getOrientation() },
        mXSpeed( xSpeed ),
        mYSpeed( ySpeed ),
        mZSpeed( zSpeed )
       {
    }

    tintaEntityRotateInfiniteAction::~tintaEntityRotateInfiniteAction(){

    }

    bool tintaEntityRotateInfiniteAction::onAction(float mlsElapsed){
        if ( !mEnt )
            return true;

        if ( TintaMath::isZero(mLastTime))
            mLastTime = mlsElapsed;

        float time = (mlsElapsed - mLastTime);
        float angleX = mXSpeed * time;
        float angleY = mYSpeed * time;
        float angleZ = mZSpeed * time;
        mLastTime = mlsElapsed;

        mEnt->rotate( angleX, angleY, angleZ , true );

        return false;       
    }

    void tintaEntityRotateInfiniteAction::setSpeed(float x, float y, float z){
        if ( !mEnt )
            return;

        mXSpeed = x;
        mYSpeed = y;
        mZSpeed = z;
    }

    tintaAction::ActionType tintaEntityRotateInfiniteAction::getType() const {
        return enRotate;
    }


    tintaEntityRotateAction::tintaEntityRotateAction() {

    }

    tintaEntityRotateAction::tintaEntityRotateAction( tintaDrawable * ent, float xAngle,
        float yAngle, float zAngle, float speed, const StringBasic &onFinish ) :
        tintaEntityAction( ent, 0.f, onFinish ){  

        tintaQuatern qX;
        qX.fromAngleAxis(xAngle, tintaVector3f::xAxis);
        tintaQuatern qY;
        qY.fromAngleAxis(yAngle, tintaVector3f::yAxis);
        tintaQuatern qZ;
        qZ.fromAngleAxis(zAngle, tintaVector3f::zAxis);
        
        mDest =  { qX * qY * qZ };     

        
        mDest.normalize();
        //mDest = mEnt->getOrientation() * mDest;

        
        mSpeed = speed;

    }

    tintaEntityRotateAction::~tintaEntityRotateAction() {

    }

    bool tintaEntityRotateAction::onAction(float mlsElapsed) {
        if ( !mEnt )
            return true;

        if ( TintaMath::isZero(mLastTime ) )
            mLastTime = mlsElapsed;
        
        float toRotate = (mlsElapsed - mLastTime) * mSpeed;
        //std::cout << toRotate << "\n";

        mSumRotate += toRotate;

        bool finished = false;

        if ( mSumRotate > 1.f ) {            
            toRotate = 1.f - ( mSumRotate - toRotate );
            finished = true;
        }

        tintaQuatern q =  slerp( mFrom, mDest, toRotate );

        mEnt->rotate( q, true, true );

        if ( finished )
            return true;

        mLastTime = mlsElapsed;
     
        return false;
    }

   

    tintaAction::ActionType tintaEntityRotateAction::getType() const {
        return enRotate;
    }


    tintaCameraRotateAction::tintaCameraRotateAction() {

    }

    tintaCameraRotateAction::tintaCameraRotateAction(tintaCamera * cam, float xAngle,
        float yAngle, float zAngle, float speed, const StringBasic &onFinish) :

        tintaCameraAction( cam, 0.f, onFinish ) {
       
        tintaQuatern qX;
        qX.fromAngleAxis(xAngle, tintaVector3f::xAxis);
        tintaQuatern qY;
        qY.fromAngleAxis(yAngle, tintaVector3f::yAxis);
        tintaQuatern qZ;
        qZ.fromAngleAxis(zAngle, tintaVector3f::zAxis);

        mDest = { qX * qY * qZ };        
        mDest.normalize();        
        mSpeed = speed;
    }

    tintaCameraRotateAction::~tintaCameraRotateAction() {

    }

    bool tintaCameraRotateAction::onAction(float mlsElapsed) {

        if (!mCam)
            return true;

        if (TintaMath::isZero(mLastTime))
            mLastTime = mlsElapsed;

        float toRotate = (mlsElapsed - mLastTime) * mSpeed;
       
       

        mSumRotate += toRotate;

        std::cout <<mSumRotate <<" " <<toRotate << "\n";

        bool finished = false;

        if ( mSumRotate > 1.f ) {
            toRotate = 1.f - (mSumRotate - toRotate);
            finished = true;
        }
       

        tintaQuatern q = slerp(mFrom, mDest, toRotate);

        mCam->rotate(q, false, false);

        if (finished)
            return true;

        mLastTime = mlsElapsed;

        return false;
    }



    tintaAction::ActionType tintaCameraRotateAction::getType() const {
        return enRotate;
    }

    tintaCameraAction::tintaCameraAction() :
        mCam(NULL_M),
        tintaAction(0.f, "")
    {
    }

    tintaCameraAction::tintaCameraAction(tintaCamera * cam, float speed, const StringBasic &onFinish) :
        mCam(cam),
        tintaAction(speed, onFinish)
    {
    }


    tintaCameraAction::~tintaCameraAction()
    {
        mCam= NULL_M;
    }

    void tintaCameraAction::setCamera(tintaCamera * ent) {
        mCam = ent;
    }


	tintaCameraMoveAction::tintaCameraMoveAction() :
		mDestPos{ tintaVector3f::origin },
		minit(false),
		mFinalLen{ 0.f },
		mCurLen{ 0.f }{
	}


	tintaCameraMoveAction::tintaCameraMoveAction(tintaCamera * cam, const tintaVector3f &fromPos,
		const tintaVector3f &destPos, float speed, const StringBasic &onFinish) :
		tintaCameraAction(cam, speed, onFinish),
		mFromPos{ fromPos },
		mDestPos{ destPos }{
		minit = true;
		mFinalLen = distance<float>(fromPos, destPos);
		mCurLen = 0.f;
	}

	tintaCameraMoveAction::~tintaCameraMoveAction() {

	}

    bool tintaCameraMoveAction::onAction( float mlsElapsed ) {

        if (!minit || !mCam)
            return false;

        if (TintaMath::isZero(mLastTime))
            mLastTime = mlsElapsed;

        float toMove = (mlsElapsed - mLastTime) * mSpeed;

        mCurLen += toMove;

        float fact = TintaMath::min(mCurLen / mFinalLen, 1.f);
        //cout << " toMove " << toMove << " mCurLen " << mCurLen << " mFinalLen " << mFinalLen << "fact" << fact << "\n";
        tintaVector3f r = lerp<float>(mFromPos, mDestPos, fact);


        tintaVector3f curPos;
       
       curPos = mCam->getPosition();
       

        if (mDestPos.equal(curPos, 0.001f))
            return true;

        mLastTime = mlsElapsed;

        // v = mDestPos - r;
        
        mCam->moveTo( r );

        return false;
	}

	void tintaCameraMoveAction::setPositions(const tintaVector3f &fromPos, const tintaVector3f &destPos){
		mFromPos = fromPos;
		mDestPos = destPos;
		mFinalLen = distance<float>(fromPos, destPos);
		minit = true;
	}

    tintaAction::ActionType tintaCameraMoveAction::getType() const {
        return enMove;
    }

}