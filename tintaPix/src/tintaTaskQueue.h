/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_TASK_QUEUE_H_
#define _TINTA_TASK_QUEUE_H_

#include "tintaPredefine.h"
#include "tintaExecutingTask.h"
#include "tintaSingleton.h"
#include "tintaUnitsSet.h"

namespace Tinta
{
	class _CoreExport tintaTaskQueue : 	
	public tintaUnitsEvents {

	public:

		tintaTaskQueue(void);

		tintaTaskQueue(bool isServer );

		virtual ~tintaTaskQueue(void);	

		typedef std::list<tintaExecutingTask*> taskqueue_t;		

		// id - fills with id before begins proccesing task
		void pushTask( const String  &command, tintaExecutingTask::TaskType type, m_ulong32 &id, size_t ownerId = 0 , float priority = 1.f);

		//void pushTask( const tintaExecutingTask &task );

		//void popTask();	

        // Removes all tasks from the queue
        void clearQueue();		

        const tintaExecutingTask* findById(m_ulong32 id) const;

        

		size_t getSize()const;		

		// from tintaUnitsEvents
		virtual void onUnitStateChanged( tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state );

		virtual void onUnitJobsDone( tintaExecutingUnit::UnitId id, bool success );

        /*
			Unit was added in to the set
		*/
        virtual void onUnitAdded(const tintaExecutingUnit::UnitId &unitId);

        /*
			Unit was removed from the set
		*/
        virtual void onUnitRemoved( const tintaExecutingUnit::UnitId &unitId );

        virtual void onUnitNameChanged(tintaExecutingUnit::UnitId id, String name);

        virtual void onUnitPriorChanged(tintaExecutingUnit::UnitId id, float prior);

		//--
		// id - task id
        virtual const tintaExecutingTask *getNextById(const m_ulong32 *taskId = NULL);

	private:

        void eraseTaskById(m_ulong32 id);

        void eraseTaskByUnitId(size_t id);

        void eraseTaskByType(tintaExecutingTask::TaskType typeTask);


		tintaExecutingTask *getNext( size_t ownerId  ) ;		
        m_ulong32 generateId();

        m_ulong32 getId() const;
        bool		  reserveUnit( size_t ownerId = 0 );
		bool		  executeFrontTask(tintaExecutingUnit::UnitId id);

		taskqueue_t mTasks;
        m_ulong32 mIdGen;
		bool mIsServer;

        //MUTEX_RECURSIVE(mStateMutex);
	};
}

#endif
