/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaUnitsSet.h"
#include "tintaException.h"
#include "tintaLogger.h"

namespace Tinta
{


	struct unitsSortPred {
		bool operator()( tintaExecutingUnit * lv, tintaExecutingUnit *rv ){
			if( lv && rv ){
				return lv->getPrior() > rv->getPrior();
			}
			return false;
		}
	};

	tintaUnitsSet::~tintaUnitsSet(void){
		mUnits.clear();
		idGen = 0;
	}

	tintaUnitsSet::tintaUnitsSet():
	idGen( 0 ){
	}


	/*
			adds unit and returns id in the container
	*/
    m_ulong32 tintaUnitsSet::addUnit(tintaExecutingUnit *unit){

		CoreAssert(unit,"unit");
		
		if( !unit )
			return 0;
        		
		auto it = mUnits.begin();
		for ( ;it != mUnits.end(); ++it ){
			if( unit->getPrior() >= (*it)->getPrior() )
				break;			
		}
		++idGen;
		unit->setId(idGen);
		unit->setCallback(this);		

        StringUtil::StrStreamType t;
        t << _M("Unit added: ") << unit->getId().mname << _M("/") << unit->getId().mid;

        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(t.str());

		mUnits.insert( it, unit );
		
		// saving for fast searching all server local units
		if( unit->getType() == tintaExecutingUnit::enLocalUnit )
			mUnitsLocal.push_back( unit );
		
        tintaUnitsEvents *listener = mObserved.getFirst();
        for( ;listener;listener =mObserved.getNext() ){ listener->onUnitAdded( tintaExecutingUnit::UnitId(idGen,_M("")) ); }

		return idGen;
	}


	tintaUnitsSet::executingunits_it tintaUnitsSet::findUnit( tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitType type ){
		return tintaUnitsSet::findUnit( id.mid, type );
	}

    tintaUnitsSet::executingunits_it tintaUnitsSet::findUnit(m_ulong32 id, tintaExecutingUnit::UnitType type){

		auto itf = mUnits.begin();
		for ( ;itf != mUnits.end(); ++itf ){
			if( (*itf)->getId().mid == id && (*itf)->getType() == type )
				break;			
		}
		
		return itf;	
	}



	tintaUnitsSet::executingunits_it tintaUnitsSet::findUnit( tintaExecutingUnit::UnitId id )  {
				return tintaUnitsSet::findUnit(id.mid);
	}

    tintaUnitsSet::executingunits_it tintaUnitsSet::findUnit(m_ulong32 id)  {

		auto itf = mUnits.begin();
		for ( ;itf != mUnits.end(); ++itf ){
			if( (*itf)->getId().mid == id )
				break;			
		}		
		return itf;		
	}

    tintaUnitsSet::executingunits_cit tintaUnitsSet::findUnit(tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitType type)const{
        return tintaUnitsSet::findUnit(id.mid, type);
    }

    tintaUnitsSet::executingunits_cit tintaUnitsSet::findUnit(m_ulong32 id, tintaExecutingUnit::UnitType type)const{

        auto itf = mUnits.begin();
        for (; itf != mUnits.end(); ++itf){
            if ((*itf)->getId().mid == id && (*itf)->getType() == type)
                break;
        }
        
        return itf;
    }



    tintaUnitsSet::executingunits_cit tintaUnitsSet::findUnit(tintaExecutingUnit::UnitId id) const {
        return tintaUnitsSet::findUnit(id.mid);
    }

    tintaUnitsSet::executingunits_cit tintaUnitsSet::findUnit(m_ulong32 id) const  {

        auto itf = mUnits.begin();
        for (; itf != mUnits.end(); ++itf){
            if ((*itf)->getId().mid == id)
                break;
        }
        //tintaExecutingUnit *rez = NULL;

        return itf;
    }

	

	tintaExecutingUnit *tintaUnitsSet::findUnitFree( size_t  id )  {

		auto itf = mUnits.begin();

		for( ;itf != mUnits.end(); itf++ ){
			tintaExecutingUnit::UnitState state =  (*itf)->getState();
			
			if( id != 0  ){ // trying find the reserved unit by id
				// tintaExecutingUnit::UnitId nameid( (*itf)->getId() );
                if ( id == (*itf)->getId().mid 
                    && (state == tintaExecutingUnit::enReady || state == tintaExecutingUnit::enReserved ))
						 break;
			}
			else { // any free unit
				if( state == tintaExecutingUnit::enReady )
					break;
			}
		}

		if( itf == mUnits.end() )
			return NULL;

		return 	*itf;		
	}

    tintaExecutingUnit *tintaUnitsSet::findUnitFree( tintaExecutingUnit::UnitType type )   {


        auto itf = mUnits.begin();

        for( ;itf != mUnits.end(); itf++ ){
            tintaExecutingUnit::UnitState state =  (*itf)->getState();
            tintaExecutingUnit::UnitType typeUnit =  (*itf)->getType();
           
            // tintaExecutingUnit::UnitId nameid( (*itf)->getId() );
            if( state == tintaExecutingUnit::enReady && typeUnit == type ) 
                break;
            
        }

        if( itf == mUnits.end() )
            return NULL;

        return 	*itf;	
    }

	/*
		Removes unit by id(fast search)
	*/
	void tintaUnitsSet::removeUnit( tintaExecutingUnit::UnitId id ){

		executingunits_t::iterator itf = mUnits.begin();
		for ( ;itf != mUnits.end(); ++itf ){
			if( (*itf)->getId().mid == id.mid )
				break;			
		}

		if( itf == mUnits.end() )
			return;

		
		if( itf != mUnits.end() )
			mUnits.erase(itf);		

				
		
		itf = mUnitsLocal.begin();
		for ( ;itf != mUnitsLocal.end(); ++itf ){
			if( (*itf)->getId().mid == id.mid )
				break;			
		}		

		if( itf != mUnitsLocal.end() )
			mUnitsLocal.erase(itf);	

        tintaUnitsEvents *listener = mObserved.getFirst();
        for( ;listener;listener =mObserved.getNext() ){ listener->onUnitRemoved( id ); }
	}

	

	/*
		Returns unit for information
	*/
	const tintaExecutingUnit *tintaUnitsSet::getUnit( tintaExecutingUnit::UnitId id ) {
		return *(findUnit( id )) ;
	}


    const tintaExecutingUnit *tintaUnitsSet::reservUnit(tintaExecutingTask * task, tintaExecutingUnit::UnitType type, size_t ownerId ){

		tintaExecutingUnit * rez = NULL_M; 
        if( type != tintaExecutingUnit::enNoType )
            rez = findUnitFree( type );
        else
            rez = findUnitFree( ownerId );

		if( rez ){

            if ( task ) {
//#if DEBUG_MODE
//                String text(task->getCommand(), 0,min((int)task->getCommand().length(), 15));
//                stream_out << _M("!!!!!! reservUnit setOwner task text: ") << text << _M(" setOwner: ") << rez->getId().mid << _M("\n");
//#endif
                task->setOwner( rez->getId().mid );
            }
			rez->setState(tintaExecutingUnit::enReserved);
           
			return rez;
		}

		return NULL;
	}


	void tintaUnitsSet::onNameChanged( tintaExecutingUnit::UnitId id, String name ){
        tintaUnitsEvents *listener = mObserved.getFirst();
        for (; listener; listener = mObserved.getNext()) { listener->onUnitNameChanged(id, name); }
	}

	void tintaUnitsSet::onPriorChanged( tintaExecutingUnit::UnitId id, float prior ){
		mUnits.sort( unitsSortPred() );
	}

	void tintaUnitsSet::onStateChanged(  tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state  ) {		
#if DEBUG_MODE
        {
            //StringStream msg;
            //msg << THREAD_CURRENT_ID << _M(" !!!!!! BEGIN tintaUnitsSet onStateChanged ") << id.mid << _M(" ") << state;

            //if (Tinta::tintaLogger::getPtr())
            //    Tinta::tintaLogger::getPtr()->logMsg(msg.str());
        }
#endif
        
        TINTA_LOCK_RECUR_MUTEX( mStateMutex );
		tintaUnitsEvents *listener = mObserved.getFirst();
		for( ;listener;listener =mObserved.getNext() ){ listener->onUnitStateChanged( id, state ); }
        
#if DEBUG_MODE
        {
            // StringStream msg;
           //  msg << THREAD_CURRENT_ID << _M(" !!!!!! END tintaUnitsSet onStateChanged ") << id.mid << _M(" ") << state;

            // if (Tinta::tintaLogger::getPtr())
            //     Tinta::tintaLogger::getPtr()->logMsg(msg.str());
        }
#endif
        
	}

	void tintaUnitsSet::onJobsDone(  tintaExecutingUnit::UnitId id, bool success ){
		//tintaUnitsEvents *listener = mObserved.getFirst();
		//for( ;listener;listener =mObserved.getNext() ){ listener->onUnitJobsDone( id, success ); }
	}

	void tintaUnitsSet::removeListener( tintaUnitsEvents *listener ){
		mObserved.removeCallback(listener);
	}

	void tintaUnitsSet::addListener( tintaUnitsEvents *listener ){
		mObserved.addCallback(listener);
	}

	tintaExecutingUnit *tintaUnitsSet::findUnitWithParallelTask() const {
		
		// first trying find in local 
		auto itf = mUnitsLocal.begin();
		for ( ;itf != mUnitsLocal.end(); ++itf ){
			const tintaExecutingTask *pt = (*itf)->getSavedTask();
			if( pt && pt->getType() != tintaExecutingTask::enCommonTask){
				return *itf;	
			}
		}

		
		itf = mUnits.begin();
		for ( ;itf != mUnits.end(); ++itf ){
			const tintaExecutingTask *pt = (*itf)->getSavedTask();
			if( pt && pt->getType() != tintaExecutingTask::enCommonTask ){
				return *itf;	
			}
		}
		//tintaExecutingUnit *rez = NULL;

		return NULL;	
	}

	/*
		Trying to execute Task on any possible unit, returns pointer to this unit or NULL if no one unit is available
	*/
	const tintaExecutingUnit *tintaUnitsSet::executeTask( const tintaExecutingTask &task ){
		 return executeOnUnit(NULL, &task );
	}
    
    void tintaUnitsSet::resetUnit( tintaExecutingUnit::UnitId id ) {

        executingunits_it uPtr = findUnit( id );
        if ( uPtr != mUnits.end() 
                    && (*uPtr)->getState() != tintaExecutingUnit::enBusy ) {

            (*uPtr)->onStateUpdate();
        }
    }

	/*
			Trying to execute Task on reserved or free unit by id, returns pointer to this unit or NULL if no one unit is available
	*/
	const tintaExecutingUnit *tintaUnitsSet::executeTask( const tintaExecutingTask &task, tintaExecutingUnit::UnitId id ){
		
		return executeOnUnit(&id, &task );
	}

	const tintaExecutingUnit * tintaUnitsSet::executeOnUnit(const tintaExecutingUnit::UnitId *id, const tintaExecutingTask *task  ) {
		tintaExecutingUnit * rezUnit = NULL_M;
        const tintaExecutingTask *taskExecute = task;
        if ( taskExecute != NULL ){
            if ( id ) // reserved or free
                rezUnit = findUnitFree(id->mid);
            else	// all free
                rezUnit = findUnitFree();
        }
        else if ( id ){ // execute the task that unit owned
            executingunits_it r = findUnit(id->mid);
            if ( r != mUnits.end() ){
                rezUnit = *r;
                taskExecute = (*r)->getSavedTask();

            }
        }  
        

        if ( rezUnit && taskExecute ){			 
            rezUnit->execute( *taskExecute );			
		}
        else {

            StringStream msg;
            msg << _M("Executing error on unit:  ");
            if ( rezUnit )
                msg << rezUnit->getId().mid;
            msg << _M(" task Id:  ");
            if ( taskExecute )
                msg << taskExecute->getId();
            if ( Tinta::tintaLogger::getPtr() )
                Tinta::tintaLogger::getPtr()->logMsg(msg.str());

        }

        return rezUnit;
	}

	const tintaExecutingUnit *tintaUnitsSet::executeTask( tintaExecutingUnit::UnitId id ) {
			return executeOnUnit( &id, NULL );
	}
    const tintaExecutingUnit *tintaUnitsSet::getNextUnit( const m_ulong32 *id ) const {

		if( mUnits.size() == 0 )
			return NULL;

		auto itf = mUnits.begin();

		if( id == NULL )
			return *itf;
		
		itf = findUnit( *id );


		if(itf == mUnits.end())
			return NULL;
		itf++;


		if( itf == mUnits.end() )
			return NULL;

		return *itf;
	
	}

    tintaExecutingUnit *tintaUnitsSet::getNextUnit( const m_ulong32 *id ) {

        if ( mUnits.size() == 0 )
            return NULL;

        auto itf = mUnits.begin();

        if (id == NULL)
            return *itf;

        itf = findUnit(*id);


        if (itf == mUnits.end())
            return NULL;
        itf++;


        if (itf == mUnits.end())
            return NULL;

        return *itf;

    }

	template<> tintaUnitsSet* Singleton<tintaUnitsSet>::mPtr = 0;

    tintaUnitsSet* tintaUnitsSet::getPtr(void)	{
		return mPtr;
	}	
}
