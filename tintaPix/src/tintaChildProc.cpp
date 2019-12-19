/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

//#include "tintaPrecompile.h"

#include "tintaChildProc.h"
#include "tintaException.h"
#include "tintaLogger.h"
#include "tintaInteractHandlerImpl.h"
#include "tintaTexSpringMain.h"
#include "tintaConfigurator.h"


namespace Tinta
{

    const char_m * const tintaChildProc::strZeroIp = _M("0.0.0.0");


	tintaChildProc::tintaChildProc(void)
		: mSocketInteract(NULL)
		//, mFunc(NULL)
		, mbServer(false)
		, mIdSendingState(1)		
		{
	}

	tintaChildProc::~tintaChildProc(void){
        if( Tinta::tintaLogger::getPtr() )
            Tinta::tintaLogger::getPtr()->removeConsole(this);
		
		if ( mSocketInteract ){

            mSocketInteract->stopInteract();

			M_DELETE  mSocketInteract;
            mSocketInteract = NULL_M;
		}

		//if (mFunc){
		//	DELETE_T(mFunc, tintaInteractFunction);
		//	mFunc = NULL_M;
		//}		
	}

	void tintaChildProc::initialise( const InteractData &req, bool bServer, const String &procName ) {

		mbServer = bServer;
		if ( !mbServer ){
		
            if ( Tinta::tintaUnitsSet::getPtr() )
                Tinta::tintaUnitsSet::getPtr()->addListener(this);
		}

		

        m_uint32 tryConnectTime = 0;

        if ( Tinta::tintaTexSpringMain::getPtr() ){

            const tintaConfigurator * config = Tinta::tintaConfigurator::getPtr();
			
            if( config && config->getTimeReconnect() > tryConnectTime )
                tryConnectTime =  (m_uint32)config->getTimeReconnect();
		}

		try {
  			if( bServer )
				mSocketInteract = M_NEW tintaAsioInteractServer(req.mIp.c_str(), req.mPort, procName, this, tryConnectTime);
 			else
                mSocketInteract = M_NEW tintaAsioInteractClient(req.mIp.c_str(), req.mPort, procName, this, tryConnectTime);
		}
		catch( std::exception& e ){

			StringStreamBasic msg; 
			msg << "Wrong ip: ";			
            msg << req.mIp;
			msg << " or port: ";			
			msg << req.mPort;			
			msg << ", add in config file. ";					
			msg << " For example: Ip_Adress = \"127.0.0.1\" ";
			msg << " Port = \"5001\" ";
			msg << " ex description: ";
			msg << e.what();

			
			EXCEPTION(  msg.str() );
		}

		//mFunc = NEW_T(tintaInteractFunction)( this );
		mSocketInteract->addConnectListener(this);
		mSocketInteract->addSendListener(this);

		mReqData = req;
        Tinta::tintaLogger::getPtr()->addConsole(this);
	}

	void tintaChildProc::startInteract( ) {
        //if (mSocketInteract)
		//    mSocketInteract->startInteract();
	}
	
	 


    tintaInteractFunction *tintaChildProc::getInteractFunc() {
        if (mSocketInteract)
            return mSocketInteract->getInteractFunc();
        return NULL_M;
	}

	

	void tintaChildProc::onUnplagged( m_ulong32 id ){

		StringUtil::StrStreamType msg;
		msg<<_M("Client: ");
		msg<<id;
		msg<<_M(" has disconnected from: ");
		msg<<mReqData.mIp;
		msg<<_M(" ");
		msg<<mReqData.mPort;
        if( Tinta::tintaLogger::getPtr() )
            Tinta::tintaLogger::getPtr()->logMsg( msg.str() );		
	}

	void tintaChildProc::onErrorOccured( m_ulong32 id , long error ){

		StringUtil::StrStreamType msg;
		msg<<_M("Error received from client: ");
		msg<<id;
		msg<<_M(" : ");
		msg<<error;
		msg<<_M(" on: ");
		msg<<mReqData.mIp;
		msg<<_M(" ");
		msg<<mReqData.mPort;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str());
	}

	void tintaChildProc::onConnected( m_ulong32 id ){
        if ( !mSocketInteract )
            return;

		if( mbServer ){

 			StringUtil::StrStreamType msg;
 			msg<<_M("Connected: ");
 			msg<<id;		
 			msg<<_M(" on: ");
 			msg<<mReqData.mIp;
 			msg<<_M(" ");
 			msg<<mReqData.mPort;
            if ( Tinta::tintaLogger::getPtr() )
                Tinta::tintaLogger::getPtr()->logMsg( msg.str() );

		}
		else {
		
            tintaBufferIO buff;
            String name = Tinta::tintaTexSpringMain::getPtr()->getAppName(); //ng->generate();

            // send name
            buff.AllocateBuffer(StringPackSize<String>(name));
            PackString<char_m, String>(buff.GetBufferEx(), 0, name);
			

            tintaBufferIOUnqPtr sendbuff = tintaInteractHandlerImpl::allocatePacketDataPtr(tintaInteractHandlerImpl::enUnitName, &buff);

			size_t idSend = 0;
			mSocketInteract->sendData(sendbuff.get(), idSend );
			

            // send prior
			buff.AllocateBuffer( sizeof(float) );
            WriteToBuffer(buff.GetBufferEx(), 0, Tinta::tintaTexSpringMain::getPtr()->getUnitPrior());
			
            tintaBufferIOUnqPtr sendbuffPrior = tintaInteractHandlerImpl::allocatePacketDataPtr(tintaInteractHandlerImpl::enSendUnitPrior, &buff);
            			
			idSend = 0;
            mSocketInteract->sendData(sendbuffPrior.get(), idSend);    

           
            // send state
            const tintaExecutingUnit *unit = tintaUnitsSet::getPtr()->getNextUnit(NULL_M);
           // m_ulong32 idUnit = unit ? unit->getId().mid : 0;
            for (; unit; unit = tintaUnitsSet::getPtr()->getNextUnit(&id)) {

                //idUnit = unit->getId().mid;

                buff.AllocateBuffer(sizeof(m_ulong32) * sizeof(tintaExecutingUnit::UnitState));

                size_t offset = 0;
                offset = WriteToBuffer<m_ulong32>(buff.GetBufferEx(), offset, unit->getId().mid);
                offset = WriteToBuffer<tintaExecutingUnit::UnitState>(buff.GetBufferEx(), offset, unit->getState());

                tintaBufferIOUnqPtr sendbuffState = tintaInteractHandlerImpl::allocatePacketDataPtr(tintaInteractHandlerImpl::enUnitState, &buff);
                
                idSend = 0;
                mSocketInteract->sendData(sendbuffState.get(), idSend);
            }           

		}

	}

	m_uint32 tintaChildProc::getObservMask()const {
		return tintaConnectionEvents::enError | tintaConnectionEvents::enUnplagged | tintaConnectionEvents::enConnect;
	}

	
//#if 0
	void tintaChildProc::printMsg( const Tinta::String &msg , msgLevel stat ) {

        if (!mSocketInteract)
            return;

		if( msg.length() > 0 
            && !mbServer 
            && stat <= msg_system ){
#if 0 // old
			tintaBufferIO buff;
			buff.AllocateBuffer(( msg.length() + 1 ) * sizeof( char_m ) );
            WriteToBufferString<char_m>(buff.GetBufferEx(), 0, msg);
#endif
            tintaBufferIO buff;
                       
            buff.AllocateBuffer( StringPackSize<String>( msg ) );
            PackString<char_m, String>( buff.GetBufferEx(), 0, msg );

			//tintaBufferIO *sendbuff = tintaInteractHandlerImpl::allocatePacketData( tintaInteractHandlerImpl::enUnitError, &buff );

            size_t ret(0);

            if ( stat == msg_system ) {
                tintaBufferIOUnqPtr sendbuff = tintaInteractHandlerImpl::allocatePacketDataPtr(tintaInteractHandlerImpl::enSendMessage, &buff);                
                mSocketInteract->sendData(sendbuff.get(), ret);
            }
            else {
                tintaBufferIOUnqPtr sendbuff = tintaInteractHandlerImpl::allocatePacketDataPtr(tintaInteractHandlerImpl::enUnitError, &buff);
                mSocketInteract->sendData(sendbuff.get(), ret);
            }
            
            buff.AllocateBuffer(0);
		}
	}

    void tintaChildProc::clear() {

    }
//#endif
	
	tintaAsioInteract* tintaChildProc::getInteract() {
		return mSocketInteract;
	}

	tintaInteractHandler * tintaChildProc::createHandler( m_ulong32  id )	{

		m_uint32 iflags = tintaInteractHandlerImpl::enNoType;

        if (mbServer)
            iflags = tintaInteractHandlerImpl::enUnitClosed
            | tintaInteractHandlerImpl::enUnitState            
            | tintaInteractHandlerImpl::enSendUnitPrior
            | tintaInteractHandlerImpl::enUnitName
            | tintaInteractHandlerImpl::enUnitError
            | tintaInteractHandlerImpl::enSendMessage
            | tintaInteractHandlerImpl::enPackage
            | tintaInteractHandlerImpl::enTaskToexecute;

        else
            iflags = tintaInteractHandlerImpl::enTaskToexecute
            | tintaInteractHandlerImpl::enSystemCommand
            | tintaInteractHandlerImpl::enStateRequest
            | tintaInteractHandlerImpl::enDisconnectUnit
            | tintaInteractHandlerImpl::enPackage;
			
		tintaInteractHandler	*handler = NEW_T(tintaInteractHandlerImpl)( iflags, mbServer );

		bool added = mHandlers.insert( handlers_t::value_type( id, handler ) ).second;

		CoreAssert(added,"mHandlers.insert(handlers_t::value_type( id, handler ) ).second;");		

		return handler;
	}

    void tintaChildProc::removeHandler(m_ulong32 id) {
        handlers_t::iterator i = mHandlers.find(id);
        if (i != mHandlers.end()) {
            DELETE_T(i->second, tintaInteractHandler);
            mHandlers.erase(i);
        }
    }


	void tintaChildProc::onUnitStateChanged( tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state ){

        if (!mSocketInteract)
            return;
		
 		tintaBufferIO buff;
		buff.AllocateBuffer( sizeof( m_ulong32 ) * sizeof( tintaExecutingUnit::UnitState ) );

        size_t offset = 0;
 		offset = WriteToBuffer<m_ulong32> ( buff.GetBufferEx(), offset,  id.mid );
		offset = WriteToBuffer<tintaExecutingUnit::UnitState> ( buff.GetBufferEx(), offset,  state );

        tintaBufferIOUnqPtr sendbuff = tintaInteractHandlerImpl::allocatePacketDataPtr(tintaInteractHandlerImpl::enUnitState, &buff);

		mSocketInteract->sendData(sendbuff.get(), mIdSendingState);

		mUnitClientId = id;
		mClientState = state;
#if DEBUG_MODE		
        StringUtil::StrStreamType t;
        t<< THREAD_CURRENT_ID << _M("###### tintaChildProc onUnitStateChanged send state: ")<< state;
        
        if ( Tinta::tintaLogger::getPtr() )
            Tinta::tintaLogger::getPtr()->logMsg(t.str());
#endif

	}

	void tintaChildProc::onUnitJobsDone( tintaExecutingUnit::UnitId id, bool success ) {
	}

    void tintaChildProc::onUnitAdded( const tintaExecutingUnit::UnitId &unitId ){
    }

    void tintaChildProc::onUnitRemoved( const tintaExecutingUnit::UnitId &unitId ){
    }

    void tintaChildProc::onUnitNameChanged(tintaExecutingUnit::UnitId id, String name){
    }

    void tintaChildProc::onUnitPriorChanged(tintaExecutingUnit::UnitId id, float prior){
    }

	void tintaChildProc::onAfterSend(size_t sendId){		
	}


	
	
}
#endif