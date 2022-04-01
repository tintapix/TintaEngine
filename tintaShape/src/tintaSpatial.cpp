
#include "tintaGraphicaPrecompile.h"
#include "tintaSpatial.h"

namespace Graphica {

    tintaSpatial::tintaSpatial(){ //:
       //mMatrix(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f){

    }

    tintaSpatial::~tintaSpatial(){
        deleteActions();

        if (mBound)
            M_DELETE mBound;
    }

    void tintaSpatial::roll(float angle){
        tintaSpatial::rotate(tintaVector3f::zAxis, angle);
    }

    void tintaSpatial::pitch(float angle){
        tintaSpatial::rotate(tintaVector3f::xAxis, angle);
    }

    void tintaSpatial::yaw(float angle){
        tintaSpatial::rotate(tintaVector3f::yAxis, angle);
    }

    void tintaSpatial::rotate(const tintaVector3f& axis, float angle, bool bUpdateChildren, bool updateLocal){
        tintaQuatern q;
        q.fromAngleAxis(angle, axis);
        tintaSpatial::rotate(q, false, updateLocal);
    }

    void tintaSpatial::rotate(const tintaQuatern& q, bool bUpdateChildren, bool updateLocal ){

        tintaQuatern qnorm = q;
        qnorm.normalize();

        mLocalOrientation = mLocalOrientation * qnorm;
        setOrientation( mLocalOrientation );
         
        if ( getParent() && !updateLocal ) {            
            setPosition( getParent()->getPosition() + getParent()->mScale * (getParent()->getOrientation() * mLocalPosition));
        }
    }

    const tintaMatrix44 &tintaSpatial::getWorld() {
        return mHomWorld;
    }

    void tintaSpatial::rotate(float angleX, float angleY, float angleZ, bool bUpdateChildren, bool updateLocal){
            
        tintaQuatern qX;
        qX.fromAngleAxis(angleX, tintaVector3f::xAxis);
        tintaQuatern qY;
        qY.fromAngleAxis(angleY, tintaVector3f::yAxis);
        tintaQuatern qZ;
        qZ.fromAngleAxis(angleZ, tintaVector3f::zAxis);

        
        tintaSpatial::rotate(qX * qY * qZ, false, updateLocal );
    }

   

    void tintaSpatial::updateChildData() {
    }   

    tintaDrawable*  tintaSpatial::getParent() {
        return NULL_M;
    }


    void tintaSpatial::moveTo(const tintaVector3f& pos, bool bUpdateChildren, bool updateLocal) {

        if ( updateLocal )
            mLocalPosition = pos;

        if ( getParent() ) {        

            updateChildData();

            return;
        }

        setPosition(pos);

    }

    void tintaSpatial::moveOn( const tintaVector3f& addPos, bool bUpdateChildren, bool updateLocal ) {

        if (  getParent() ){

            if( updateLocal )
                mLocalPosition += addPos;

            setPosition( getParent()->getPosition() + getParent()->mScale * (getParent()->getOrientation() * mLocalPosition) );

            return;
        }
        mLocalPosition += addPos;
        setPosition(mPosition + addPos);
    }
   
    void tintaSpatial::moveOnOrient(const tintaVector3f& addPos, bool bUpdateChildren, bool updateLocal){
        
        tintaVector3f trans = mOrientation * addPos;          

        if ( getParent() ) {

             //orientedPos = getParent()->getOrientation() * orientedPos;
            // if moving child manualy
            if ( updateLocal ) {                    
                mLocalPosition += getParent()->getOrientation() * trans;
                setPosition( mPosition + trans );
            }           
            else {

                tintaDrawable* p = getParent();
                tintaDrawable* root = NULL_M; // getParent();
                while ( p ) {
                    root = p;
                    p = p->getParent();                    
                }

                setPosition(mPosition + root->getOrientation() * addPos);
            }
            
            return;
        }
        mLocalPosition += trans;
        setPosition(mPosition + trans);
    }

    void tintaSpatial::move(MoveAxies ax, float val, bool bUpdateChildren, bool updateLocal) {

        tintaVector3f loc(0.f, 0.f, 0.f);

        switch (ax) {

        case enAxisX:
            loc.mx = val;
            break;
        case enAxisY:
            loc.my = val;
            break;
        case enAxisZ:
            loc.mz = val;
            break;
        }

        tintaSpatial::moveTo(loc , bUpdateChildren, updateLocal);
    }


    void tintaSpatial::mulScale(const tintaVector3f& scale, bool bUpdateChildren) {
        
        
        mScale *= scale;        

        if ( getParent() ) {                    
            
            setPosition( getParent()->getPosition() + getParent()->mScale * (getParent()->getOrientation() * mLocalPosition) );

            return;
        }

        
        if ( mBound )
            mBound->scale( TintaMath::max(mScale.mx, mScale.my, mScale.mz ) );

       
    }

    void tintaSpatial::setScale(const tintaVector3f& scale, bool bUpdateChildren) {


        mScale = scale;

        if (getParent()) {

            setPosition(getParent()->getPosition() + getParent()->mScale * (getParent()->getOrientation() * mLocalPosition));

            return;
        }


        if (mBound)
            mBound->scale(TintaMath::max(mScale.mx, mScale.my, mScale.mz));


    }

    tintaVector3f tintaSpatial::getScale() const {
        return mScale;
    }

    const tintaQuatern  &tintaSpatial::getOrientation() const{
        return mOrientation;
    }

    const tintaQuatern  &tintaSpatial::getLocalOrientation() const {
        return mLocalOrientation;
    }

       
    void  tintaSpatial::setOrientation(const tintaQuatern &orient, bool bUpdateChildren) {
        mOrientation = orient;        
        if(mBound)
            mBound->setOrientation(mOrientation);
    }
    void   tintaSpatial::updateOrientation( const tintaQuatern &orient, bool bUpdateChildren , bool updateLocal ) {

        if ( getParent() ) {  

            setOrientation( orient );

            setPosition( getParent()->getPosition() + getParent()->mScale * ( getParent()->getOrientation() * mLocalPosition ) );

            return;
        }
        mLocalOrientation = orient;
        setOrientation(orient);
    }


    void  tintaSpatial::setPosition(const tintaVector3f &pos) {
        mPosition = pos;
        if (mBound)
            mBound->setCenter(mPosition);    
    }

   
    void  tintaSpatial::shiftPosition(const tintaVector3f &shift) {
        mPosition += shift;
        mLocalPosition += shift;
    }

    const tintaVector3f &tintaSpatial::getPosition() const{
        return mPosition;
    }

    const tintaVector3f &tintaSpatial::getLocalPosition() const {
        return mLocalPosition;
    }


    void tintaSpatial::updateModel(bool bUpdateNormals ) {

    }

    void tintaSpatial::addAction( tintaAction *action ){

        CoreAssert( action, "action" );

        if ( !action )
            return;               

        entityActions_it it = mActions.begin();
        for ( ; it != mActions.end(); ) {

            if ((*it)->getType() == action->getType() ) {
                M_DELETE *it;
                mActions.erase(it++);
                break;
            }
            else
                ++it;
            
        }
        mActions.push_back(  action );
    }

    tintaBoundingVolume *tintaSpatial::getBoundary() {
        return mBound;
    }

    const tintaBoundingVolume *tintaSpatial::getBoundary() const {
        return mBound;
    }

    void tintaSpatial::createBoundary( BVType bv ) {

        if ( mBound )
            M_DELETE mBound;

        mBound = NULL_M;

        if ( bv == SphereBV )
            mBound = M_NEW tintaSphereBV();
        else if ( bv == BoxBV )
            mBound = M_NEW tintaBoxBV();

        updateModel();
    }

    tintaBasicStringVector tintaSpatial::updateActions( m_uint32 mskElapsed ){

        entityActions_it it = mActions.begin();
        tintaBasicStringVector commands;
        for (; it != mActions.end();){            
            
            if ( (*it)->onAction((float)mskElapsed) ){

                if ( (*it)->getCommand().length() > 0 )
                    commands.push_back( (*it)->getCommand() );

                M_DELETE *it;
                mActions.erase(it++);
            }
            else
                ++it;
        }   
        return commands;
    };

    void tintaSpatial::deleteActions() {
        for (auto data : mActions){
            M_DELETE data;
        }
        mActions.clear();
    }

	
}


