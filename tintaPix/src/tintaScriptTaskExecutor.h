/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_SCRIPT_TASK_EXECUTOR_H_
#define _TINTA_SCRIPT_TASK_EXECUTOR_H_

#include <tintaPredefine.h>
#include <tintaThreadPool.h>
#include "tintaExecutingUnit.h"

#include <tintaBufferIO.h>

namespace Tinta
{
	class _CoreExport tintaScriptTaskExecutor :
		public AllocatedTaskExecutor, 
        public tintaThreadPool::RequestHandler,
		public tintaExecutingUnit
	{

	public:		
		struct TaskExecutingRequest {
			String m_channel;			
			tintaBufferIO m_data;			
		};

		enum {
			SCRIPT_TASK = 1
		};

        tintaScriptTaskExecutor(Tinta::tintaScriptContext *context, tintaThreadPool* wq);
		virtual ~tintaScriptTaskExecutor(void);

		//virtual void initialise( Tinta::tintaScriptContext *context, ThreadPool* wq );

		virtual bool addTaskExecutingRequest( const TaskExecutingRequest* req ); 		

        virtual bool handleRequest(const tintaThreadPool::Request* req, const tintaThreadPool* srcQ);
        
		virtual String *getExecutingErrors(unsigned &quantity);

		virtual void clearExecutionErrors();
	
		// keeping task after first execution and trying to execute task again
		virtual bool execute( );

		// from tintaExecutingUnit
		virtual bool execute( const tintaExecutingTask &task );
		virtual tintaExecutingUnit::UnitType getType()const;
        		
#ifndef TINTA_NO_INTERACT
		virtual tintaAsioClient * getInteractClient();	

		virtual const tintaAsioClient * getInteractClient() const;
#endif

        virtual void setAbort();
		
	protected:
	
		bool executeTask(const TaskExecutingRequest* task );
		void setExcutionFlag(bool b_flag);

		//Tinta::tintaScriptContext *mExecutor;
        tintaThreadPool* mPool;

		unsigned short m_channel;
		// command executing flag
		bool mbExecutng = false;
		typedef std::vector<const TaskExecutingRequest*> t_task_request_queue;
		typedef std::vector<String>					     t_error_stack;	

		// keeping tasks for execution if the mode is multiprocess, all tasks sending on avalible machines, and waiting callbacks
		t_task_request_queue     mTasks;
		t_error_stack			 mErrors;
                

	};
}

#endif
