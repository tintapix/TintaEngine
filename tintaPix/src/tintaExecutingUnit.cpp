/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaExecutingUnit.h"
#include "tintaException.h"
#include "tintaLogger.h"

namespace Tinta
{

	tintaExecutingUnit::tintaExecutingUnit()
		:mClb( NULL )
		,mState(enNoState)
		,mPrior(1.f)
		,mTask()
	{			
	}

/*
	stream_o& tintaExecutingUnit::UnitId::operator<< (stream_o & out, tintaExecutingUnit::UnitId &id)
	{
				if( id.mname == _M("") )
					out << id.mid;
				else
					out << id.mname;

				out<<_M(" ");
					return out;
	}
*/

	tintaExecutingUnit::~tintaExecutingUnit( void ){
	}

	tintaExecutingUnit::tintaExecutingUnit( const UnitId &id )
		:mId( id )
		,mClb( NULL )
		,mState( enNoState )
		,mPrior( 1.f )
		,mTask( ){
	}

    void tintaExecutingUnit::onStateUpdate(){
        if ( mClb )
            mClb->onStateChanged( mId, mState );        
    }
	

	void tintaExecutingUnit::setState( UnitState state ){
        
        
#if DEBUG_MODE        

        //StringStream msg;           
        stream_out << THREAD_CURRENT_ID << " unit id " << mId.mid << " state (1ready,2buzy,3reservred old/new) " << mState<<"/" << state << "\n";
        //if ( Tinta::tintaLogger::getPtr() )
        //    Tinta::tintaLogger::getPtr()->logMsg(msg.str() );        
#endif
         
		if( mState == state ) // && ( mState !=  ) )
		    return;
        
		mState = state;	
        if ( mState == enReady ) {            
            mTask = tintaExecutingTask();
        }
        onStateUpdate();
	}

	tintaExecutingUnit::UnitState tintaExecutingUnit::getState(  ) const{
		return mState;
	}

	void tintaExecutingUnit::setName( const String &name ){
		mId.mname = name;
        if ( mClb )
            mClb->onNameChanged( mId, name );
	}

	
	
	float tintaExecutingUnit::getPrior() const{
		return mPrior;
	}

	void tintaExecutingUnit::setPrior(float p) {
		mPrior = p;
	}


    void tintaExecutingUnit::setId(m_ulong32 id){
		mId.mid = id;
	}

// 	unsigned long tintaExecutingUnit::getId( )const {
// 		return mId.mid;
// 	}

	tintaExecutingUnit::UnitId	tintaExecutingUnit::getId()const{
		return mId;
	}

	void tintaExecutingUnit::setCallback( tintaExecutingUnitCallback *clb) {
		CoreAssert(clb,"clb");
		mClb = clb;
	}
	const tintaExecutingTask *tintaExecutingUnit::getSavedTask()const {
		if( mTask.getCommand().length() != 0 )
			return NULL_M;
		return &mTask;
	}

	const tintaScriptContext *tintaExecutingUnit::getContext() const {
		return mExecutor;
	}

    void  tintaExecutingUnit::setAbort() {
         // stub, nothing
    }

	

}
