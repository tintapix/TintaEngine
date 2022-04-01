/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaTaskQueue.h"
#include "tintaLogger.h"
#include "tintaInteractHandlerImpl.h"
#include "tintaChildProc.h"
#include "tintaTexSpringMain.h"


namespace Tinta
{

	tintaTaskQueue::tintaTaskQueue( void )
		:mIdGen( 0 )
		,mIsServer(true){		
		
		Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->addListener(this);
	}

	tintaTaskQueue::tintaTaskQueue(bool isServer)
		:mIdGen(0)
		, mIsServer(isServer){	
		
        Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->addListener(this);
	}

	tintaTaskQueue::~tintaTaskQueue( void ){		
        
        Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->removeListener(this);
		while ( !mTasks.empty() )
		{
			DELETE_T ( mTasks.front() , tintaExecutingTask );
			mTasks.pop_front();
		}
		
	}

	void tintaTaskQueue::pushTask( const String  &command , tintaExecutingTask::TaskType type, m_ulong32 &id,  size_t ownerId, float priority ){

		tintaExecutingTask * task = NEW_T ( tintaExecutingTask )(command, generateId(), type,  ownerId  );
		mTasks.push_back( task );
		id = mIdGen;
        //cout << THREAD_CURRENT_ID << " tasks in queue " << mTasks.size() << "\n";
        reserveUnit( ownerId );
	}	
    
    void tintaTaskQueue::clearQueue() {
        mTasks.clear();
    }

	tintaExecutingTask *tintaTaskQueue::getNext( size_t ownerId ) {

		if( mTasks.size() ){

			auto itf = mTasks.begin();
            
            for( ;itf != mTasks.end(); itf++ ){                  
            
				if( (*itf)->getOwner() == ownerId ){				
					break;
				}                
			}
			if( itf != mTasks.end() )
				return *itf;            			
		}
		return NULL_M;
	}



    const tintaExecutingTask* tintaTaskQueue::findById(m_ulong32 id) const {
        return NULL_M;
	}

    m_ulong32 tintaTaskQueue::generateId() {
		return ++mIdGen;
	}
	size_t tintaTaskQueue::getSize() const {
		return mTasks.size(); 	
	}

    bool tintaTaskQueue::reserveUnit(size_t ownerId) {

        //TINTA_LOCK_RECUR_MUTEX(mStateMutex);

        tintaExecutingTask * task = getNext( 0 );

        if (!task) {

            return false;
        }
        const tintaExecutingUnit * rez(NULL_M);

        if (task->getType() == tintaExecutingTask::enLocalTask) // parallel task added in to the queue
            rez = Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->reservUnit(task, tintaExecutingUnit::enLocalUnit);
        else if (task->getType() == tintaExecutingTask::enDownTask) { // subtask for clients units
            rez = Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->reservUnit(task, tintaExecutingUnit::enInteractUnit);
        }
        else if ( task->getType() == tintaExecutingTask::enUpperTask ) { 
#ifndef TINTA_NO_INTERACT
            // no units for server
            // sending to the server       
            size_t size = task->GetBufferSize();
            
            UNIQPTRDEF(tintaBufferIO, dataBuff, NEW_T(tintaBufferIO)());

            dataBuff->AllocateBuffer(size);           
                        
            //assert(task->packData(dataBuff->GetBufferEx(), 0) == size);
            CoreAssert(task->packData(dataBuff->GetBufferEx(), 0) == size, "task->packData(dataBuff->GetBufferEx(), 0) != size");

            tintaBufferIOUnqPtr buffPacket = tintaInteractHandlerImpl::allocatePacketDataPtr( tintaInteractHandlerImpl::enTaskToexecute, dataBuff.get() );

            tintaChildProc * chProc = tintaTexSpringMain::getPtr()->getInteract();
            
            tintaAsioInteract* interact = chProc ? chProc->getInteract() : NULL_M;

            if ( interact ) {

                size_t idSend = 0;
                interact->sendData(buffPacket.get(), idSend);
            }
            eraseTaskById( task->getId() );
#else
            rez = Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->reservUnit(task, tintaExecutingUnit::enLocalUnit);
#endif

        }
        else
            rez = Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->reservUnit(task, tintaExecutingUnit::enNoType ,ownerId );
        

        return rez != NULL_M;
	}

	bool tintaTaskQueue::executeFrontTask( tintaExecutingUnit::UnitId id ) {
        //TINTA_LOCK_RECUR_MUTEX(mStateMutex);
#if DEBUG_MODE
       // cout << THREAD_CURRENT_ID << "!!!!!! BEGIN task " << " owner " << id.mid << "\n";
#endif
        tintaExecutingTask * task = getNext( id.mid );

        if ( !task )
            return false;
#if DEBUG_MODE
       // cout<< THREAD_CURRENT_ID << "!!!!!! END task " << task->getId() << " owner " << id.mid<<"\n";
#endif
        ///if( task->getOwner() == 0 )
		 //   task->setOwner( id.mid );		
        return  Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->executeTask( *task, id ) != NULL_M;
	}

    const tintaExecutingTask *tintaTaskQueue::getNextById(const m_ulong32 *taskId){

		if( mTasks.size() == 0 )
            return NULL_M;

		auto itf = mTasks.begin();

        if (taskId == NULL_M)
			return *itf;

		
		for(;itf != mTasks.end(); itf++ ){
			if( (*itf)->getId() == *taskId ){				
				break;
			}
		}
		if( itf != mTasks.end() &&  ++itf != mTasks.end() )
			return *itf;
		
        return NULL_M;
	}

    m_ulong32 tintaTaskQueue::getId() const{
		return mIdGen;
	}

	void tintaTaskQueue::onUnitStateChanged( tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state ){     
            
#if DEBUG_MODE        
        {
            StringStream msg;

            msg << THREAD_CURRENT_ID << _M(" ###### tintaTaskQueue onUnitStateChanged ") << _M(" ") << state;
           // if (Tinta::tintaLogger::getPtr())
            //    Tinta::tintaLogger::getPtr()->logMsg(msg.str());
        }         
#endif       
       // TINTA_LOCK_RECUR_MUTEX( mStateMutex );        
		if( state == tintaExecutingUnit::enReady ){            
            
            reserveUnit( id.mid );			
		}
		else if ( state == tintaExecutingUnit::enReserved ){	
            // trying find reserved before             
			executeFrontTask( id );
		}
		else if( state == tintaExecutingUnit::enBusy ){ 
            // job accepted - deleting task from the queue	
            eraseTaskByUnitId( id.mid );
		}
        
#if DEBUG_MODE
        {
          /*  StringStream msg;
            msg << THREAD_CURRENT_ID << _M(" ###### END tintaTaskQueue tintaUnitsSet onStateChanged ") << id.mid << _M(" ") << state;
            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str());
                */
        }
#endif
        
	}

    void tintaTaskQueue::eraseTaskById( m_ulong32 id ) {

        if ( mTasks.size() == 0 )
            return;

		taskqueue_t::iterator itd = mTasks.begin();
		
		for(;itd != mTasks.end(); ){
			if( (*itd)->getId() == id ){	
                DELETE_T( *itd, tintaExecutingTask );
                itd = mTasks.erase( itd );
			}
			else 
				itd++;
		}
	}

	void tintaTaskQueue::eraseTaskByUnitId( size_t id ) {
//#if DEBUG_MODE
 //       stream_out << THREAD_CURRENT_ID << _M("**** erazing task") << id <<_M("\n");
//#endif
        if ( mTasks.size() == 0 )
            return;
		taskqueue_t::iterator itd = mTasks.begin();

		for( ;itd != mTasks.end(); ){
            if ((*itd)->getOwner() == id ) {
                DELETE_T(*itd, tintaExecutingTask);
                itd = mTasks.erase(itd);
            }
            else
                ++itd;
		}
	}

	void tintaTaskQueue::eraseTaskByType( tintaExecutingTask::TaskType typeTask ){

        if ( mTasks.size() == 0 )
            return;

		taskqueue_t::iterator itd = mTasks.begin();

		for(;itd != mTasks.end();  ){
			if( (*itd)->getType() == typeTask ){	
                DELETE_T( *itd, tintaExecutingTask );
                itd = mTasks.erase( itd );
			}
			else 
				itd++;
		}
	}

	void tintaTaskQueue::onUnitJobsDone( tintaExecutingUnit::UnitId id, bool success ){
		
	}

    void tintaTaskQueue::onUnitAdded(const tintaExecutingUnit::UnitId &unitId){

    }        
    void tintaTaskQueue::onUnitRemoved(const tintaExecutingUnit::UnitId &unitId ){
    
    }

    void tintaTaskQueue::onUnitNameChanged(tintaExecutingUnit::UnitId id, String name){

    }

    void tintaTaskQueue::onUnitPriorChanged(tintaExecutingUnit::UnitId id, float prior){

    }
		
}
