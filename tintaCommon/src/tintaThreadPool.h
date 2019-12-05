/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_THREAD_POOL_H_
#define _TINTA_THREAD_POOL_H_


#include "tintaPredefine.h"
#include "tintaLogger.h"
#include "tintaBufferIO.h"



namespace Tinta
{	
	 class _CoreExport tintaThreadPool :
		public AllocatedObjectCommon 	{
	protected:
		typedef std::map<String, m_uint16> ChannelMap;
		ChannelMap mChannelMap;
		m_uint16 mNextChannel;
        MUTEX_RECURSIVE(mChannelMapMutex)
	public:
		
		typedef m_ulong32 RequestID;
		
		class Request 
			: public AllocatedObjectCommon	{
            friend class tintaThreadPool;
		protected:
			
			m_uint16 mChannel;			
			m_uint16 mType;						
            tintaBufferIO   mData;
			m_byte         mRetryCount;			
			RequestID      mID;			
		public:

            Request(m_uint16 channel, m_uint16 rtype, const tintaBufferIO& rData, m_byte retry, RequestID rid);
			~Request();
			
			
			m_uint16 getChannel() const { return mChannel; }
			
			m_uint16 getType() const { return mType; }
			
            const tintaBufferIO& getData() const { return mData; }
			
			m_byte getRetryCount() const { return mRetryCount; }	

			RequestID getID() const { return mID; }         
            

		};
        	

		
		class _CoreExport RequestHandler 
		{
		public:
			RequestHandler() {}
			virtual ~RequestHandler() {}			
            virtual bool handleRequest(const Request* req, const tintaThreadPool* srcQ) = 0;
		};
        	
		

        tintaThreadPool() : mNextChannel(0) {}
        virtual ~tintaThreadPool() {}
		
		virtual void startup(bool forceRestart = true) = 0;
		
		virtual void addRequestHandler(m_uint16  channel, RequestHandler* rh) = 0;
		
        virtual void removeRequestHandler(m_uint16 channel, RequestHandler* rh = NULL_M) = 0;		

		
        virtual RequestID addRequest(m_uint16 channel, 
                            m_uint16 requestType, const tintaBufferIO& rData, 
                                            m_byte retryCount = 0, bool forceSynchronous = false) = 0;
				
		virtual void shutdown() = 0;
		
		virtual m_uint16 getChannel(const String& channelName);

        static const String   taskChannel;// = _M("executeTask");
	};

	
	class _CoreExport DefaultThreadPoolBase 
        : public tintaThreadPool
	{
	public:		
        
		DefaultThreadPoolBase(const String& name = _M(""));
		virtual ~DefaultThreadPoolBase();
        		
		const String& getName() const;
		
		virtual size_t getWorkerThreadCount() const;
		
		virtual void setWorkerThreadCount(size_t c);		
		
		virtual void processNext();

        virtual void run() = 0;

		virtual bool isShuttingDown() const { return mShuttingDown; }
		
		virtual void addRequestHandler(m_uint16  channel, RequestHandler* rh);
		
		virtual void removeRequestHandler(m_uint16 channel, RequestHandler* rh = NULL_M);
						
        virtual RequestID addRequest(m_uint16 channel, m_uint16 requestType, const tintaBufferIO& rData, m_byte retryCount = 0, bool forceSynchronous = false);
        		
		
	protected:

		class RequestHandlerHolder
					: public AllocatedObjectCommon
				{

				protected:
                    MUTEX_RECURSIVE(mRWMutex);
					RequestHandler* mHandler;
				public:
					RequestHandlerHolder(RequestHandler* handler)
						: mHandler(handler)	{}

					void disconnectHandler(){						
						mHandler = NULL_M;
					}


					RequestHandler* getHandler() {
						return mHandler;
					}
                    bool handleRequest(const Request* r, const tintaThreadPool* srcQ){
                        TINTA_LOCK_RECUR_MUTEX(mRWMutex);
                        bool rez{ false };
						if ( mHandler ){							
                            rez = mHandler->handleRequest(r, srcQ);
						}
                        return rez;
					}
				};

		typedef list< RequestHandlerHolder* > RequestHandlerList;		
		typedef map< m_uint16, RequestHandlerList > RequestHandlerListByChannel;		

		/// Thread function
		struct WorkerFunc{
			DefaultThreadPoolBase* mQueue;
			WorkerFunc(DefaultThreadPoolBase* q)
				: mQueue(q) {
			}
			void operator()();
			void run();
		};

		String mName;
		size_t mWorkerThreadCount;
		bool   mWorkerRenderSystemAccess;
		bool   mIsRunning;
		
		WorkerFunc* mWorkerFunc;
		RequestID   mRequestCount;
		bool        mAcceptRequests;
		Request*    mLastRequest;
		bool        mShuttingDown;
		typedef deque<Request*> RequestQueue;		
		RequestQueue mRequestQueue;
		RequestQueue mProcessQueue;		

		RequestHandlerListByChannel mRequestHandlers;	

        TINTA_MUTEX(mRequestMutex);
        TINTA_MUTEX(mProcessMutex)
        TINTA_MUTEX(mResponseMutex)
        TINTA_MUTEX(mRequestHandlerMutex);       

		void processQueue(Request* r);
		bool processRequest(Request* r);	
		
		virtual void notifyWorkers() = 0;
	};

}

#endif




