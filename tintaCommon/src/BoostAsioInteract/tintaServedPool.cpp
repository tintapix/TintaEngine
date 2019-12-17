/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#include "tintaServedPool.h"


namespace Tinta {

	tintaServedPool::tintaServedPool(tintaIHandlerFactory * hFactory)
		:mIsRunning( false ),
		 mhFactory( hFactory ),
		 mIds(0){
			 mpartIter = mparticipants.begin();
	}


	tintaServedPool::~tintaServedPool(){
		//mparticipants.clear();
		shutdownAll();
	}

    servedPtr_t tintaServedPool::create( boost::asio::io_service& io_service, socket_t &Socket , clientsKey_t id ){
        return servedPtr_t( new tintaAsioClientImpl(io_service, Socket, id) );
	}

    servedPtr_t tintaServedPool::create(boost::asio::io_service& io_service, clientsKey_t id){
        return servedPtr_t(new tintaAsioClientImpl(io_service,id));
    }
        


	IErrorCallback* tintaServedPool::getPoolCallback() {
		IErrorCallback* thisclb = dynamic_cast<IErrorCallback*>(this);
		return thisclb;	
	}
    m_ulong32 tintaServedPool::createId(){

		if( ++mIds == 0 )
			mIds++;
		return mIds;
	}
 	servedPtr_t  tintaServedPool::join( boost::asio::io_service& io, servedPtr_t newConnection ){

		//LOCK_AUTO_MUTEX
		clientsKey_t id = createId(); //mparticipants.size() + 1;
		
		// identifying connected
		newConnection->setId( id );		
	
		mparticipants.insert( clientspset_t::value_type( id, newConnection) );
		mpartIter = mparticipants.begin();
        if( mhFactory )
		    newConnection->setHandler( mhFactory->createHandler( id ) );
		
		
		startup( id, newConnection->getFunc() );
		Tinta::tintaConnectionEvents *c = mObserved.getFirst(tintaConnectionEvents::enConnect);
		for( ;c;c =mObserved.getNext( tintaConnectionEvents::enConnect ) ){ c->onConnected( id  ); }
        //stream_out << _M("client connected with id: ")<< id ;
		return newConnection;
 	}

	void tintaServedPool::unplug( clientsKey_t id ) {
        
       // stream_out << _M("(") << THREAD_CURRENT_ID << _M(")");
		clientspset_it td = mparticipants.find( id );	
        
        if( mhFactory )
		    mhFactory->removeHandler( id );

        td->second->setHandler( NULL_M );

        //boost::system::error_code ec;
        //td->second->getSocket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        //THREAD_SLEEP(6000);

		if( td != mparticipants.end() ){			
			mparticipants.erase( td );
		}
		mpartIter = mparticipants.begin();
        //THREAD_SLEEP(4000);
		//shutdown(id);

        InteractThreadPool_t::iterator i = mThreads.find(id);
        if (i != mThreads.end()) {
            if (i->second->joinable())
                i->second->join();

            THREAD_DESTROY(i->second);

            mThreads.erase(i);
        }
        
        stream_out << _M("Client disconnected with id: ") << id;
        
		Tinta::tintaConnectionEvents *c = mObserved.getFirst( tintaConnectionEvents::enUnplagged );
		for( ;c;c =mObserved.getNext(tintaConnectionEvents::enUnplagged) ){ c->onUnplagged( id ); }
	}

    void tintaServedPool::shutdown(clientsKey_t id) {

        TINTA_LOCK_RECUR_MUTEX_AUTO;
        clientspset_it td = mparticipants.find(id);        

        unplug( id );     
    }

    void tintaServedPool::shutdownAll() {

        clientspset_it td = mparticipants.begin();
        while (td != mparticipants.end()) {           

            shutdown(td->second->getId());
            td = mparticipants.begin();            
        }
        mpartIter = mparticipants.begin();
        //shutdownall();

    }

	

    servedPtr_t	 tintaServedPool::getFirstClient() {
		mpartIter = mparticipants.begin();
		if( mpartIter == mparticipants.end() )
            return NULL_M;
		return mpartIter->second;
	}

    servedPtr_t	 tintaServedPool::getNextClient() {
		if( mpartIter == mparticipants.end() )
            return NULL_M;

        mpartIter = ++mpartIter;
		if( mpartIter == mparticipants.end() )
            return NULL_M;

		return mpartIter->second;
		
	}

	size_t tintaServedPool::addCallback( tintaConnectionEvents  * v){
		return mObserved.addCallback(v);
	}

	tintaConnectionEvents  * tintaServedPool::removeCallback(tintaConnectionEvents  * v){
		return mObserved.removeCallback(v);
	}


	

	void tintaServedPool::startup( clientsKey_t idClient, tintaInteractFunction *func ){

		THREAD_CREATE( t, *func );
		t->detach();
      
		mThreads.insert( InteractThreadPool_t::value_type( idClient ,t ) );

		mIsRunning = true;
	}

    servedPtr_t tintaServedPool::getClient(clientsKey_t id){

		if( mpartIter != mparticipants.end() && mpartIter->first == id)
			return mpartIter->second;

		clientspset_it td = mparticipants.find(id);
		if(td == mparticipants.end())		
            return NULL_M;
		return td->second;
	}	

	
 
 	m_uint32 tintaServedPool::getSize() const {
        return (m_uint32)mparticipants.size();
 	}

	

	

}

#endif
