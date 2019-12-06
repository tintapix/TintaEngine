/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaScriptTaskExecutor.h"
#include "tintaTexSpringMain.h"
#include "tintaUnitsSet.h"
#include <tintaException.h>
#include "tintaTaskQueue.h"

namespace Tinta
{

	

    tintaScriptTaskExecutor::tintaScriptTaskExecutor(Tinta::tintaScriptContext *context, tintaThreadPool* pool)
        : mPool(NULL){

		 assert(context);
         assert(pool);
		 mExecutor = context; 

         mPool = pool;
         m_channel = mPool->getChannel(tintaThreadPool::taskChannel);
         mPool->addRequestHandler(m_channel, this);
		if( Tinta::tintaUnitsSet::getPtr() )
            Tinta::tintaUnitsSet::getPtr()->addUnit(this);
		setState( tintaExecutingUnit::enReady );
	}

	tintaScriptTaskExecutor::~tintaScriptTaskExecutor(void){

		t_task_request_queue::iterator i = mTasks.begin();

		for( ; i != mTasks.end(); i++ ){
			DELETE_T( *i, TaskExecutingRequest );
		}
		mTasks.clear();
		
        m_channel = mPool->getChannel(tintaThreadPool::taskChannel);
               

        mPool->removeRequestHandler(m_channel);
        mPool = NULL;

        

	}
	bool tintaScriptTaskExecutor::executeTask(const TaskExecutingRequest* task ){
		
		// one process mode waiting for execution complete
		if( !mbExecutng ) {		
			
			tintaBufferIO data(task->m_data);		

			setExcutionFlag( true );
            // TODO
			DELETE_T (task, TaskExecutingRequest);
			
            mPool->addRequest(m_channel, SCRIPT_TASK, data, 0, false);
			return true;
		}

		return false;
	}

	bool tintaScriptTaskExecutor::execute( const tintaExecutingTask &task ) {		


#if DEBUG_MODE
        //stream_out << THREAD_CURRENT_ID << _M(" %%%%%%% execute ") << task.getCommand()<<_M("\n");
#endif
		mTask = task;		

		tintaScriptTaskExecutor::TaskExecutingRequest *toExecute = NEW_T (tintaScriptTaskExecutor::TaskExecutingRequest) ();
        size_t len = task.getCommand().length();
       
        if ( len > 0 ){
            toExecute->m_data.AllocateBuffer(task.getCommand().length() * sizeof(char_m) + sizeof(char_m));
            WriteToBufferString<char_m>(toExecute->m_data.GetBufferEx(), 0, task.getCommand());
        }
		
		setState( tintaExecutingUnit::enBusy );

        return addTaskExecutingRequest( toExecute );
	}

	tintaExecutingUnit::UnitType tintaScriptTaskExecutor::getType()const{
		return tintaExecutingUnit::enLocalUnit;
	}

	bool tintaScriptTaskExecutor::addTaskExecutingRequest( const TaskExecutingRequest* req ){			

		assert(req);
		if(!req)
			return false;

		if(!executeTask(req ))
			mTasks.push_back(req);

		return true;
	}

	void tintaScriptTaskExecutor::setExcutionFlag( bool b_flag ){        
		mbExecutng = b_flag;
	}

	
	bool tintaScriptTaskExecutor::execute( ){
		// do nothing 
		return true;
	}
     

    bool tintaScriptTaskExecutor::handleRequest(const tintaThreadPool::Request* req, const tintaThreadPool* srcQ){
		
        String str_to_execute; // = AnyType_cast<String>(req->getData());
        size_t length = req->getData().GetSize();
        if ( length > 0 ) {
            
            char_m *dataCommand = ALLOC_T(char_m, length / sizeof(char_m) + 1); // and terminating symbol
            
            ReadArrayFromBuffer< char_m >(req->getData().GetBuffer(), 0, dataCommand, length / sizeof(char_m));
            String command( dataCommand );
            str_to_execute = command;
            FREE_T( dataCommand );
        }

       //StringBasic buffexec = str_to_execute;
		//bool rezult = mContext->executeFile( str_to_execute );
		bool rezult = mExecutor->executeBuffer(str_to_execute.c_str(), str_to_execute.length() );

		if( !rezult ){
			// adding errors
			size_t error_count(0);
			const String *errors  = mExecutor->getErrors( error_count );
 			for( unsigned i = 0; i < error_count; i++,errors++ ){
				mErrors.push_back( *errors );

                Tinta::tintaLogger::getPtr()->logMsg( *errors, msg_error );
 			}
            if ( error_count > 0 )
            {
                tintaTexSpringMain* main = Tinta::tintaTexSpringMain::getPtr();
                if ( main )
                    main->clearContainers();
            }
			mExecutor->resetErrors();
			clearExecutionErrors();
		}

       setExcutionFlag( false );
       //cout << THREAD_CURRENT_ID << " handleRequest enReady " << "\n";
       setState( enReady );
       if ( mClb )
           mClb->onJobsDone(mId, true);        
       
        return true;
	}

	String *tintaScriptTaskExecutor::getExecutingErrors( unsigned &quantity ){
		quantity = mErrors.size();
		if(quantity == 0){
			return 0;
		}
		return &mErrors[0];
	}
	void tintaScriptTaskExecutor::clearExecutionErrors(){
		mErrors.clear();
	}

#ifndef TINTA_NO_INTERACT
	tintaAsioClient * tintaScriptTaskExecutor::getInteractClient(){
		return NULL;
	}

	const tintaAsioClient * tintaScriptTaskExecutor::getInteractClient() const {
		return NULL;
	}
#endif

    void tintaScriptTaskExecutor::setAbort() {
        if (mState == tintaScriptTaskExecutor::enBusy)
            Tinta::tintaTexSpringMain::getPtr()->setAborted( true );
    }

	
}
