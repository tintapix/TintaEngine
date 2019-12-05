/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#ifndef TINTA_ASIO_INTERACT_H_
#define TINTA_ASIO_INTERACT_H_

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "tintaServedPool.h"
#include "tintaObserved.h"
#include "tintaISendEvents.h"
#include "tintaPredefine.h"
#include "tintaAsyncTimer.h"


namespace Tinta {
	

	struct _CoreExport tintaAsioInteract :
		public AllocatedValInteract,
        public tintaIInteracted,
        public IErrorCallback
	{
		
        enum {
            ErrorOnClose = 995,
        };

		virtual ~tintaAsioInteract(void);

	protected:
		tintaAsioInteract( const char *ip, short port, bool isServ, const String &procName,
            m_uint32 timeTryConnect, int maxInQueue = 10);
        tintaAsioInteract() = delete;
	public:

		virtual void startInteract();
 
		virtual void stopInteract();        		

		virtual void addConnectListener( tintaConnectionEvents *pl) = 0;

		virtual void addSendListener(tintaISendEvents *pl);
        
		// returns sendId
        virtual void sendData(const tintaBufferIO *data, size_t &sendId, m_ulong32 clientId);        

        virtual void sendData(const tintaBufferIO *data, size_t &sendId);		
		
		// if not server mode always returns NULL
        virtual  servedPtr_t getClient(m_ulong32 id);
		
        virtual  const servedPtr_t getFirstClient() const;

        virtual  const servedPtr_t getNextClient() const;

        virtual tintaInteractFunction *getInteractFunc();	

        // timer function
        void trysendHandler(const boost::system::error_code& e, boost::asio::deadline_timer* t);

        virtual void onError(const boost::system::error_code, m_ulong32 id) = 0;

	protected:
		struct sendData_t {

			enum statusSend {
				enNotSended,
				enSended,
				enError,
			};

			sendData_t()
			:mIdSend(0)
			,mData(NULL_M)
			,mClientId(0)
			,mStatus(enNotSended)
			,mHasClientId(false)
			{}

            sendData_t(size_t idSend, tintaBufferIO *data, statusSend status, m_ulong32 clientId, bool hasClientId)
				:mIdSend(idSend)
				, mData(data)	
				,mClientId( clientId )
				,mStatus(status)
				,mHasClientId(hasClientId)
				{						
			}
			
			size_t			mIdSend;
			tintaBufferIO *  mData;
            m_ulong32 	    mClientId;
			statusSend 		mStatus;
			bool 			mHasClientId;
		};

		typedef boost::asio::ip::tcp::socket socket_t;
		typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
		typedef std::list<sendData_t> sendqueue_t;
		typedef sendqueue_t::iterator sendqueueit_t;

		sendqueue_t mQueueSend;

		// 
		sendqueue_t mQueueSendError;

		// removes all sended packets from the queue
		void updateQueue();

		bool asyncSend( socket_t &sock, const sendData_t &data );        

        virtual void  handleSend(const boost::system::error_code& error, size_t bytes_transferred, size_t sendId);


		StringBasic         mIp;
		short                    mPort;		

		tintaServedPool         *mServedPool;
		const tintaBufferIO      *mDataBuff;

		boost::asio::ip::tcp::endpoint mEndpoint;
		boost::asio::io_service		   mIoService; 
		//boost::asio::deadline_timer	   mTimer;

		socket_t                    mSocket;
		bool	                    mIsServer;
		String                      mProcName;
		bool                        mStarted;
		bool                        mSendOperation;	
		size_t                      mIdSending;	
		m_uint32                       mTimeTrySend;
		tintaObserved<tintaISendEvents>  mObserved;	
        tintaInteractFunction			*mFunc;
        // max packets in queue to send
        int mMaxInQueue;       

        boost::asio::deadline_timer	   mTimerTrySend;

        MUTEX_RECURSIVE_T mInteractMutex;
        
 	};


	class _CoreExport tintaAsioInteractServer
		: public tintaAsioInteract {

public:
    /*
        if hFactory == NULL_M, works only on send
    */
	tintaAsioInteractServer( const char *ip, short port, const String &procName, 
                            tintaIHandlerFactory * hFactory = NULL_M, m_uint32 timeTryConnect = 0, int maxInQueue = 10);
	virtual ~tintaAsioInteractServer(void);
public:	
    	

	void addSession_t(  servedPtr_t session, const boost::system::error_code& error); //,  socket_ptr newSock );
	
	virtual void addConnectListener(tintaConnectionEvents *pl);
	
    virtual servedPtr_t getClient(m_ulong32 id);

    virtual void onError(const boost::system::error_code, m_ulong32 id);

protected:
	void startSession( );		
	boost::asio::ip::tcp::acceptor mAcceptor;	

};


	class _CoreExport tintaAsioInteractClient
		: public tintaAsioInteract	
        
	{

	public:
        /*
        if hFactory == NULL_M, works only on send
        */
		tintaAsioInteractClient( const char *ip, short port, const String &procName, tintaIHandlerFactory * hFactory  = NULL_M , 
                                                                            m_uint32 timeTrySend = 0, int maxInQueue = 100);

        virtual void onError(const boost::system::error_code, m_ulong32 id);

		virtual ~tintaAsioInteractClient(void);
	public:	
		void acceptSession();

        void onConnect(const boost::system::error_code& error, servedPtr_t served);

		virtual void addConnectListener( tintaConnectionEvents *pl );	
				
	protected:

        //TODO delete this
		tintaIHandlerFactory *mhFactory;

		bool 				 mbConnected;

		tintaInteractHandler *mInterHandler;	

        servedPtr_t mServed;

        void  TryConnect();

        m_uint32 mReconnect;

        boost::asio::deadline_timer	   mConnectTimer;

        //tintaAsyncTimer mConnectTimer;

	};

}

#endif
#endif