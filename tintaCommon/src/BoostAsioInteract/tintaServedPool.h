/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */
#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_CLIENTS_POOL_H_
#define _TINTA_CLIENTS_POOL_H_

#include "tintaPredefine.h"
#include "tintaLogger.h"
#include "tintaInteractHandler.h"
#include "tintaObserved.h"
#include "tintaConnectionEvents.h"
#include "tintaIHandlerFactory.h"
#include "tintaAsioClientImpl.h"
#include "tintaInteractFunction.h"

#include "memory"

namespace Tinta {


	typedef std::shared_ptr<tintaAsioClientImpl> servedPtr_t;
    typedef m_ulong32 clientsKey_t;
	
 	typedef std::map< clientsKey_t, servedPtr_t > clientspset_t;
	typedef clientspset_t::iterator clientspset_it;
	typedef clientspset_t::const_iterator clientspset_cit;
	typedef std::multimap< String, clientspset_it > clientsnames_t;
	typedef clientsnames_t::iterator clientsnames_it;
 
    /*
        Contains all connected clients
    */
	class _CoreExport tintaServedPool 			 
	{

 	public:
        AUTO_MUTEX;

		tintaServedPool(tintaIHandlerFactory * hFactory);

		virtual ~tintaServedPool();

        static servedPtr_t   create( boost::asio::io_service& io_service, socket_t &Socket , clientsKey_t id );

        static servedPtr_t   create( boost::asio::io_service& io_service,  clientsKey_t id );       

 		servedPtr_t			 join ( boost::asio::io_service& io , servedPtr_t client ); 
 		 		        
        void                 shutdown( clientsKey_t id );

        void                 shutdownAll();

 		m_uint32			 getSize( ) const ;			

        servedPtr_t	         getClient( clientsKey_t id );

		IErrorCallback *getPoolCallback();		

        servedPtr_t          getFirstClient();

        servedPtr_t          getNextClient();

		 size_t              addCallback( tintaConnectionEvents  * v);

		 tintaConnectionEvents  *removeCallback(tintaConnectionEvents  * v);

         void				 unplug(clientsKey_t id);

 	protected:                       

		clientsKey_t createId();

		void         startup( clientsKey_t idClient , tintaInteractFunction *func );
		
		
		typedef std::map< clientsKey_t, THREAD_TYPE* > InteractThreadPool_t;

		InteractThreadPool_t  mThreads;
		
 		clientspset_t		  mparticipants;

		clientspset_it		  mpartIter;

		bool			      mIsRunning;

		tintaIHandlerFactory  *mhFactory;
		
		clientsKey_t		  mIds;

		tintaObservedMasked<tintaConnectionEvents> mObserved;        
 	};


}

#endif
#endif