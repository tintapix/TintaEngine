#include "tintaThreadPool.h"
#include <tintaBufferIO.h>


namespace Tinta
{
	
    const String   Tinta::tintaThreadPool::taskChannel      = _M("executeTask");
    
    m_uint16 tintaThreadPool::getChannel(const String& channelName){
        TINTA_LOCK_RECUR_MUTEX(mChannelMapMutex);

		auto i = mChannelMap.find(channelName);

		if (i == mChannelMap.end())		{
			i = mChannelMap.insert(ChannelMap::value_type(channelName, mNextChannel++)).first;
		}
		return i->second;
	}
	
    tintaThreadPool::Request::Request(m_uint16 channel, m_uint16 rtype,
                                    const tintaBufferIO& rData, m_uint8 retry, RequestID rid)
		: mChannel(channel), 
          mType(rtype),
          mData(rData), 
          mRetryCount(retry),
          mID(rid)
	{

	}
	
    tintaThreadPool::Request::~Request(){}   
	
	
	DefaultThreadPoolBase::DefaultThreadPoolBase(const String& name)
		: mName(name)
		, mWorkerThreadCount(0)
		, mWorkerRenderSystemAccess(false)
		, mIsRunning(false)		
		, mWorkerFunc(0)
		, mRequestCount(0)		
		, mAcceptRequests(true)
		, mLastRequest(0)
	{
	}
	
	const String& DefaultThreadPoolBase::getName() const
	{
		return mName;
	}
	
	size_t DefaultThreadPoolBase::getWorkerThreadCount() const
	{
		return mWorkerThreadCount;
	}
	
	void DefaultThreadPoolBase::setWorkerThreadCount(size_t c)
	{
		mWorkerThreadCount = c;
	}
	DefaultThreadPoolBase::~DefaultThreadPoolBase()
	{

		for (auto i = mRequestQueue.begin(); i != mRequestQueue.end(); ++i){
			M_DELETE (*i);						
		}
		mRequestQueue.clear();

        
        for (auto i = mProcessQueue.begin(); i != mProcessQueue.end(); ++i){
			M_DELETE(*i);			
		}
		mProcessQueue.clear();	
	}
	
	void DefaultThreadPoolBase::addRequestHandler(m_uint16 channel, RequestHandler* rh)
	{
        TINTA_LOCK_MUTEX(mRequestHandlerMutex);

        auto i = mRequestHandlers.find(channel);
		if (i == mRequestHandlers.end())
			i = mRequestHandlers.insert(RequestHandlerListByChannel::value_type(channel, RequestHandlerList())).first;

		RequestHandlerList& handlers = i->second;
		bool duplicate = false;
		for (auto j = handlers.begin(); j != handlers.end(); ++j){
			if ((*j)->getHandler() == rh)
			{
				duplicate = true;
				break;
			}
		}
		if ( !duplicate )
			handlers.push_back( M_NEW RequestHandlerHolder(rh) );
			//handlers.push_back(new RequestHandlerHolder(rh));			

	}
	
	void DefaultThreadPoolBase::removeRequestHandler(m_uint16 channel, RequestHandler* rh)
	{
        TINTA_LOCK_MUTEX(mRequestHandlerMutex);

        auto i = mRequestHandlers.find(channel);
		if (i != mRequestHandlers.end())
		{
			RequestHandlerList& handlers = i->second;
            for (auto j = handlers.begin(); j != handlers.end(); ++j)	{
				if ( (*j)->getHandler() == rh || !rh ){
					
					(*j)->disconnectHandler();
					M_DELETE (*j);
					handlers.erase(j);	
					break;
				}
			}

		}

	}
	
	
    tintaThreadPool::RequestID DefaultThreadPoolBase::addRequest(m_uint16 channel, m_uint16 requestType,
        const tintaBufferIO& rData, m_uint8 retryCount, bool forceSynchronous)
	{
		Request* req = NULL_M;
		RequestID rid = 0;

		{
            
            TINTA_LOCK_MUTEX(mRequestMutex);
                //Tinta::tintaLogger::getPtr()->logMsg(_M("Locked DefaultThreadPoolBase::addRequest mRequestMutex"), msg_warning);
				if ( !mAcceptRequests )
					return 0;

			rid = ++mRequestCount;
			req = M_NEW Request(channel, requestType, rData, retryCount, rid);

			if (!forceSynchronous && mWorkerThreadCount > 0){
				mRequestQueue.push_back(req);
				notifyWorkers();
				return rid;
			}
		}
		processQueue(req);
		return rid;
	}
	
	
	void DefaultThreadPoolBase::processNext(){
		Request* request = 0;
		{			
            TINTA_LOCK_MUTEX(mProcessMutex);
			{                               
					if (!mRequestQueue.empty())	{
						request = mRequestQueue.front();
						mRequestQueue.pop_front();
						mProcessQueue.push_back( request );
					}
			}
		}

		if (request){			
			processQueue(request);
		}
	}
	
	void DefaultThreadPoolBase::processQueue(Request* r){
	    processRequest(r);

        TINTA_LOCK_MUTEX(mProcessMutex);

        RequestQueue::iterator it = mProcessQueue.begin();
		for( ; it != mProcessQueue.end(); ++it ){
			if( (*it) == r ){
				mProcessQueue.erase( it );
				break;
			}
		}
        if ( r ) 
		    M_DELETE r;	
	}	

	bool DefaultThreadPoolBase::processRequest(Request* r){
		RequestHandlerListByChannel handlerListCopy;
        {      
            TINTA_LOCK_MUTEX(mRequestHandlerMutex);
			handlerListCopy = mRequestHandlers;
		}
        bool rez(false);
		auto i = handlerListCopy.find(r->getChannel());
		if ( i != handlerListCopy.end() )	{
			RequestHandlerList& handlers = i->second;
            auto j = handlers.rbegin();
			for (; j != handlers.rend(); ++j)	{				
                rez = (*j)->handleRequest(r, this);	
                if ( rez )
                    break;
			}
		}
        return rez;
	}
	


	void DefaultThreadPoolBase::WorkerFunc::operator()()
	{
        mQueue->run();
	}

	void DefaultThreadPoolBase::WorkerFunc::run()
	{
		
        mQueue->run();
	}
}
