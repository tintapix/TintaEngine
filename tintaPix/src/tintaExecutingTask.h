/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_EXECUTING_TASK_H_
#define _TINTA_EXECUTING_TASK_H_

#include "tintaPredefine.h"
#include "tintaISerialisable.h"

namespace Tinta
{
	/*
		Base executing task
	*/	
	class _CoreExport tintaExecutingTask	
		: public tintaISerialisable {			
	public:
		enum TaskType{			
			enCommonTask = 0,	  // task on the server or on client
			enLocalTask,          // task was created for executing on the local unit only
			enDownTask,           // task was created on the server to process on lower level
            enUpperTask,          // task was created for server on upper level
		};
        tintaExecutingTask(String command, m_ulong32 id, TaskType type, size_t owner = 0);

		tintaExecutingTask( const tintaExecutingTask &rval );
        		
		tintaExecutingTask( void );

		virtual ~tintaExecutingTask( void );

		void setCommand( const String &name );

		String getCommand( )const;

        m_ulong32 getId()const;

        void setId(m_ulong32 id);

		const tintaExecutingTask& operator = ( const tintaExecutingTask &rval );

		// sign that the task executing in parallel
		tintaExecutingTask::TaskType getType() const;
		
		// tintaISerialisable
		virtual size_t GetBufferSize() const;

		virtual size_t packData( m_int8 *data, size_t size ) const;

		virtual size_t		   unpackData(const m_int8 *data, size_t offset);

		void setOwner( size_t ownerId );
		// return 0, if no owner
		size_t getOwner( ) const ;	
		
	protected:
		// command to execute
		String mCommand;	
		// id for the task assigned by manager
        m_ulong32   mId;
		// depends of the executor
		TaskType mTypeTask;
		// if 0 no unit owns this task
		size_t mUnitId;
		
	};
}

#endif
