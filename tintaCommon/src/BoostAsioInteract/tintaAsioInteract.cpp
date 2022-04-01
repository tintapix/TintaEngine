/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#include "tintaAsioInteract.h"
#include "tintaException.h"


namespace Tinta {

	
	tintaAsioInteract::~tintaAsioInteract(void){
              
        

        stopInteract();        
           
        while (!mQueueSend.empty())
        {
            if (mQueueSend.front().mData)
                DELETE_T(mQueueSend.front().mData, tintaBufferIO);
            mQueueSend.pop_front();
        }

        if (mFunc) {
            DELETE_T(mFunc, tintaInteractFunction);
            mFunc = NULL_M;
        }           
        
	}

	
	tintaAsioInteract::tintaAsioInteract( const char *ip, short port, bool isServ, const String &procName, m_uint32 timeTryConnect, int maxInQueue ):
		 mPort( port )
		,mIp(ip)
        ,mEndpoint( boost::asio::ip::address::from_string( ip ), (unsigned short)mPort )
		,mIsServer( isServ )
		,mProcName( procName )
		,mStarted(false) 
		,mSocket(mIoService)        
		,mDataBuff(NULL_M)
		,mSendOperation(false)          
		, mIdSending( 1 )
        , mMaxInQueue(maxInQueue)
        , mTimeTrySend(1000)
        , mTimerTrySend(mIoService, boost::posix_time::milliseconds(mTimeTrySend)) {
      mFunc = NEW_T(tintaInteractFunction)(this);
      //mTimerTrySend.async_wait(boost::bind(&tintaAsioInteract::sendData, this, boost::asio::placeholders::error));
	}

	void tintaAsioInteract::startInteract(){
#if DEBUG_MODE 
		StringUtil::StrStreamType msg;
		if(mIsServer)
			msg<<_M("Server ");
		else
			msg<<_M("Client ");

        msg << _M("Start interacting: ip: ") << toWideChar(mIp) << _M(" port: ") << mPort;

        if ( Tinta::tintaLogger::getPtr() )
		    Tinta::tintaLogger::getPtr()->logMsg( msg.str() );	
        else
            stream_out << msg.str() << _M("\n");
#endif
		mIoService.run();	
		
	}	

	void tintaAsioInteract::stopInteract(){
        TINTA_LOCK_RECUR_MUTEX(mInteractMutex);

        if ( mSocket.is_open() )
            mSocket.close();

		if( mIsServer ){
#if DEBUG_MODE 
			StringUtil::StrStreamType msg; 
            msg << _M("Stop interacting: ip: ") << toWideChar(mIp) << _M(" port: ") << mPort;			
            if ( Tinta::tintaLogger::getPtr() )
			    Tinta::tintaLogger::getPtr()->logMsg( msg.str() );    
            else
                stream_out << msg.str() << _M("\n");
#endif
		}
		mStarted = false;		
	}
	void tintaAsioInteract::addSendListener(tintaISendEvents *pl){
		mObserved.addCallback( pl );
	}
	
	void tintaAsioInteract::updateQueue(){
       
        if ( mInteractMutex.try_lock() ) {

            if (mQueueSend.size() > 0) {

                sendqueueit_t i = mQueueSend.begin();
                for (; i != mQueueSend.end(); ) {
                    if (i->mStatus == sendData_t::enSended) {
                        DELETE_T(i->mData, tintaBufferIO);

                        size_t sended = i->mIdSend;
                        i = mQueueSend.erase(i);

                        Tinta::tintaISendEvents *c = mObserved.getFirst();

                        for (; c; c = mObserved.getNext()) { c->onAfterSend(sended); }

                    }
                    else
                        i++;
                }
            }
            mInteractMutex.unlock();
        }
	}
	
    servedPtr_t tintaAsioInteract::getClient(m_ulong32 id) {
		return NULL;
	}

    const servedPtr_t tintaAsioInteract::getFirstClient() const {
		if(mServedPool)
			return mServedPool->getFirstClient();
		return NULL;
	}

    const servedPtr_t tintaAsioInteract::getNextClient() const {
		if(mServedPool)
			return mServedPool->getNextClient();
		return NULL;
	}

    tintaInteractFunction *tintaAsioInteract::getInteractFunc() {
        return mFunc;
    }

	bool tintaAsioInteract::asyncSend( socket_t &sock, const tintaAsioInteract::sendData_t &data ){

        //cout<<" asyncSend begin "<<"\n";
		if( sock.is_open() ){
			sock.async_send( boost::asio::buffer(data.mData->GetBuffer(),
				data.mData->GetSize()), boost::bind(&tintaAsioInteract::handleSend, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred,
				data.mIdSend ) );
			mIoService.reset();
            //cout << " asyncSend end true" << "\n";
			return true;
		}
        //cout << " asyncSend end false" << "\n";
		return false;
	}

 	

    void tintaAsioInteract::sendData(const tintaBufferIO *data, size_t &sendId) {
        sendData(data, sendId, 0);
    }


    void tintaAsioInteract::trysendHandler(const boost::system::error_code& e, boost::asio::deadline_timer* t) {

        TINTA_LOCK_RECUR_MUTEX(mInteractMutex);

            if ( mQueueSend.size() > 0 && mServedPool ) {

                mSendOperation = true;

                sendqueueit_t i = mQueueSend.begin();
                for (; i != mQueueSend.end(); i++) {
                    if (i->mStatus == sendData_t::enNotSended) {

                        const sendData_t &data = *i; //mQueueSend.front();
                        servedPtr_t client = NULL;
                        if (data.mHasClientId && mServedPool) {
                            client = mServedPool->getClient(data.mClientId);
                        }

                        bool rez = false;

                        if (client)
                            rez = asyncSend(client->getSocket(), data);
                        else
                            rez = asyncSend(mSocket, data);

                        if ( !rez ) {
                            StringUtil::StrStreamType msg;
                            msg << _M("Socket not opened for client id: ") << i->mClientId;
                            if (Tinta::tintaLogger::getPtr())
                                Tinta::tintaLogger::getPtr()->logMsg(msg.str());
                            else
                                stream_out << msg.str() << _M("\n");
                        }
                        else {
                            i->mStatus = sendData_t::enSended;
                        }
                    }

                }
            }

        if ( mQueueSend.size() > 0 ) {
            t->expires_at(t->expires_at() + boost::posix_time::milliseconds(mTimeTrySend));
            t->async_wait(boost::bind(&tintaAsioInteract::trysendHandler, this, boost::asio::placeholders::error, t));
        }
        else
            mTimerTrySend.cancel();


    }


    void tintaAsioInteract::sendData(const tintaBufferIO *data, size_t &sendId, m_ulong32 clientId) {

        if ( !data )
            return;

        servedPtr_t client(NULL_M);

		bool bWithClient = clientId > 0;              

        TINTA_LOCK_RECUR_MUTEX(mInteractMutex);

        //stream_out << THREAD_CURRENT_ID << _M(" ") << _M("sendData") << _M("\n");
                
		if( mServedPool && bWithClient){
            
			client = mServedPool->getClient(clientId);

            if ( !client )
                return;
		}       

		tintaBufferIO *dataBuff = NEW_T(tintaBufferIO)();

		dataBuff->AllocateBuffer(data->GetSize());

		dataBuff->SetData(data->GetBuffer(), data->GetSize());

		mQueueSend.push_back( sendData_t(++mIdSending, dataBuff, sendData_t::enNotSended, clientId, bWithClient ) );

        if ( mQueueSend.size() > (size_t)mMaxInQueue ) {

            DELETE_T( mQueueSend.front().mData, tintaBufferIO );

            mQueueSend.pop_front();
        }
                
		sendId = mIdSending;			
            
		if ( !mSendOperation  ){

			mSendOperation = true;	

			bool rez = false;

			if (bWithClient){
				rez = asyncSend(client->getSocket(), mQueueSend.back() );
			}
			else{
				rez = asyncSend( mSocket, mQueueSend.back() );
			}	

            if ( !rez ) {

                mSendOperation = false;
                StringUtil::StrStreamType msg;                    
                msg << _M("Socket not opened for client id: ") << clientId;
                if( Tinta::tintaLogger::getPtr() )
                    Tinta::tintaLogger::getPtr()->logMsg( msg.str() );
                else
                    stream_out << msg.str() << _M("\n");

            }
            else { 
                mQueueSend.back().mStatus = sendData_t::enSended;
            }            
				
		} 		
        if ( mQueueSend.size() > 0 ) {
            mTimerTrySend.expires_at(mTimerTrySend.expires_at() + boost::posix_time::milliseconds(mTimeTrySend));
            mTimerTrySend.async_wait(boost::bind(&tintaAsioInteract::trysendHandler, this, boost::asio::placeholders::error, &mTimerTrySend));
        }
        else
            mTimerTrySend.cancel();
		
	}

	void  tintaAsioInteract::handleSend( const boost::system::error_code& error, size_t bytes_transferred, size_t sendId ){
        
        TINTA_LOCK_RECUR_MUTEX(mInteractMutex);
        

		mSendOperation = false;

		if ( error.value() != 0 ){		
            stream_out << THREAD_CURRENT_ID << _M(" ") << _M("error handleSend") << _M("\n");

 			sendqueueit_t i = mQueueSend.begin();
 			for (; i != mQueueSend.end(); ){
 				
 				if( i->mIdSend == sendId && i->mStatus != sendData_t::enNotSended)
 					break;				
 				i++;
 			}	
 			if( i != mQueueSend.end() ){
 				i->mStatus = sendData_t::enNotSended;
    
                StringUtil::StrStreamType msg;
                msg << _M("Send data error! port: ") << mPort;
                msg << _M(" with id ") << sendId;
                msg << _M(" error ") << error.value();

                if ( Tinta::tintaLogger::getPtr() ) {

                    if ( Tinta::tintaLogger::getPtr() )
                        Tinta::tintaLogger::getPtr()->logMsg( msg.str() );
                }
                else
                    stream_out << msg.str() << _M("\n");
 			}
		}
		updateQueue();
	}

tintaAsioInteractServer::tintaAsioInteractServer( const char *ip, short port , const String &procName, tintaIHandlerFactory * hFactory, m_uint32 timeTryConnect, int maxInQueue):
	tintaAsioInteract( ip, port, true, procName, timeTryConnect, maxInQueue)
	,mAcceptor( mIoService, mEndpoint )	
{	
    mServedPool = NEW_T(tintaServedPool )( hFactory );
	startSession( );		
}

tintaAsioInteractServer::~tintaAsioInteractServer(void)
{
    TINTA_LOCK_RECUR_MUTEX(mInteractMutex);
    if ( mServedPool ) {
        mServedPool->shutdownAll();
        DELETE_T(mServedPool, tintaServedPool);
        mServedPool = NULL_M;
    }
   
}

void tintaAsioInteractServer::startSession(  ) {
    
    servedPtr_t served = tintaServedPool::create(mIoService, 0);
    served->setPoolCallBack(this);

    mAcceptor.async_accept( served->getSocket(),
		boost::bind( &tintaAsioInteractServer::addSession_t
		,this
		,served 
		,boost::asio::placeholders::error		
		) );		
}



servedPtr_t tintaAsioInteractServer::getClient(m_ulong32 id) {
	return mServedPool->getClient( id );
}

void tintaAsioInteractServer::addSession_t( servedPtr_t session, const boost::system::error_code& error ){
	
 	if( !error ) { 
		mServedPool->join( mIoService , session);	 
        startSession();
 	}
    else {
        if ( boost::asio::error::connection_reset != error ) {
#if DEBUG_MODE
            std::string msg = error.message();
            std::cout << msg;
#endif
        }
    }
	
}

void tintaAsioInteractServer::onError(const boost::system::error_code, clientsKey_t id) {
    // calls asynchronous
    //unplug(id);
    TINTA_LOCK_RECUR_MUTEX(mInteractMutex);

    mServedPool->unplug(id);
}


void tintaAsioInteractServer::addConnectListener(tintaConnectionEvents *pl){
    CoreAssert(pl, "pl == NULL");
	mServedPool->addCallback( pl );
}

tintaAsioInteractClient::tintaAsioInteractClient( const char *ip, short port, 
                            const String &procName, tintaIHandlerFactory * hFactory, m_uint32 timeConnectSend, int maxInQueue)
: tintaAsioInteract( ip, port, false, procName, timeConnectSend, maxInQueue )
  ,mhFactory( hFactory )   
  ,mbConnected( false )
  ,mInterHandler( NULL_M )
  ,mReconnect(timeConnectSend)
  ,mConnectTimer(mIoService, boost::posix_time::milliseconds(mReconnect))
{
    mServedPool = NEW_T( tintaServedPool )( hFactory );  

    mServed = tintaServedPool::create( mIoService, mSocket, 0 );

    if( mReconnect > 0 )
        mConnectTimer.async_wait( boost::bind( &tintaAsioInteractClient::TryConnect, this ) );
    else
        TryConnect(); 	
}

tintaAsioInteractClient::~tintaAsioInteractClient(void){
    TINTA_LOCK_RECUR_MUTEX(mInteractMutex);

    servedPtr_t clientPtr = getFirstClient();    

    while ( clientPtr ) {
        clientPtr->Release();
        clientPtr = getNextClient();
    }

    mbConnected = false;
}

void tintaAsioInteractClient::acceptSession() {
}


void tintaAsioInteractClient::onError(const boost::system::error_code, clientsKey_t id) {
    // calls asynchronous
    //unplug(id);
    TINTA_LOCK_RECUR_MUTEX(mInteractMutex);

    mServedPool->unplug(id);
    mbConnected = false;
    //mSocket
    mSocket.close();
    //mServed = tintaServedPool::create(mIoService, mSocket, 0);

    if ( mReconnect > 0 ) {
        mConnectTimer.expires_from_now(boost::posix_time::milliseconds(mReconnect));
        mConnectTimer.async_wait(boost::bind(&tintaAsioInteractClient::TryConnect, this));
    }

}

void tintaAsioInteractClient::onConnect(const boost::system::error_code& error , servedPtr_t served){
	
    TINTA_LOCK_RECUR_MUTEX(mInteractMutex);

	// sending Clients name
	if ( !error ){		

        mConnectTimer.cancel();

		mbConnected = true;		

        served->setPoolCallBack(this);

        mServedPool->join( mIoService, served );
#if DEBUG_MODE     
        StringUtil::StrStreamType t;

        t << _M("Connected as client !!!! ");
        if ( Tinta::tintaLogger::getPtr() )
            Tinta::tintaLogger::getPtr()->logMsg( t.str() );
        else
            stream_out << t.str() << _M("\n");         
#endif
	}	
    else {

        std::string msg = error.message();

        if ( Tinta::tintaLogger::getPtr() )
            Tinta::tintaLogger::getPtr()->logMsg(msg);
        else
            std::cout << msg << _M("\n");

        mbConnected = false;

        if (mReconnect > 0) {
            mConnectTimer.expires_from_now(boost::posix_time::milliseconds(mReconnect));
            mConnectTimer.async_wait(boost::bind(&tintaAsioInteractClient::TryConnect, this));
        }
        
        //TryConnect();
    }
	
}

void tintaAsioInteractClient::addConnectListener( tintaConnectionEvents *pl ) {
    if ( mServedPool )
        mServedPool->addCallback( pl );

}

void  tintaAsioInteractClient::TryConnect() {
    TINTA_LOCK_RECUR_MUTEX(mInteractMutex);
 
    if ( mServed && !mbConnected ) {

        mSocket.async_connect( mEndpoint,
            boost::bind( &tintaAsioInteractClient::onConnect, this,
                boost::asio::placeholders::error, mServed ) );
            
    }
}

}

#endif
