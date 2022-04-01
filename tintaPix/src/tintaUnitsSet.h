/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_UNITS_SET_H_
#define _TINTA_UNITS_SET_H_

#include "tintaExecutingUnit.h"
#include "tintaObserved.h"
#include "tintaPredefine.h"
#include "tintaSingleton.h"


namespace Tinta
{
	
	struct tintaUnitsEvents {
		/*
			if Unit was vacated
		*/
		virtual void onUnitStateChanged( tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state ) = 0;

		/*
			Unit has finished the job
		*/
		virtual void onUnitJobsDone( tintaExecutingUnit::UnitId id, bool success ) = 0;

        /*
			Unit was added in to the set
		*/
        virtual void onUnitAdded( const tintaExecutingUnit::UnitId &unitId ) = 0;

        /*
			Unit was removed from the set
		*/
        virtual void onUnitRemoved( const tintaExecutingUnit::UnitId &unitId ) = 0;

        virtual void onUnitNameChanged(tintaExecutingUnit::UnitId id, String name) = 0;

        virtual void onUnitPriorChanged(tintaExecutingUnit::UnitId id, float prior) = 0;
	};
	

	/*
		Container and manager for executing units
	*/	
	class _CoreExport tintaUnitsSet
		: public tintaExecutingUnitCallback		  
	{

	public:	

		tintaUnitsSet( void );

		virtual ~tintaUnitsSet( void );

		/*
			adds unit and returns id in the container
		*/
        virtual m_ulong32 addUnit(tintaExecutingUnit *unit);

		/*
			Removes unit by id(fast search)
			 
		*/
		virtual void removeUnit( tintaExecutingUnit::UnitId id );

		/*
			Returns unit for information
		*/
		virtual const tintaExecutingUnit *getUnit( tintaExecutingUnit::UnitId id );

		
		/*
			Reserving unit and returns information
		*/
        virtual const tintaExecutingUnit *reservUnit(tintaExecutingTask * task, tintaExecutingUnit::UnitType type = tintaExecutingUnit::enNoType, size_t ownerId = 0);


		/*
			Assign buisy state to the unit
		*/
		//virtual void setBusyState( tintaExecutingUnit::UnitId id );



		/*
			Trying to execute Task on any possible unit, returns pointer to this unit or NULL if no one unit is available
		*/
		virtual const tintaExecutingUnit *executeTask( const tintaExecutingTask &task );	

		/*
			Trying to execute Task on reserved or free unit by id, returns pointer to this unit or NULL if no one unit is available
		*/
		virtual const tintaExecutingUnit *executeTask( const tintaExecutingTask &task, tintaExecutingUnit::UnitId id );	

		/*
            Resets unit to the "Ready" state manualy   
        */
        virtual void resetUnit(tintaExecutingUnit::UnitId id);

		/*
			Trying to execute saved task on reserved or free unit by id
		*/
		virtual const tintaExecutingUnit *executeTask( tintaExecutingUnit::UnitId id );	

		virtual void onStateChanged( tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state );

		virtual void onNameChanged(tintaExecutingUnit::UnitId id, String name);

		virtual void onPriorChanged(tintaExecutingUnit::UnitId id, float prior);

		virtual void onJobsDone(  tintaExecutingUnit::UnitId id, bool success );

		void removeListener( tintaUnitsEvents *listener );

		void addListener( tintaUnitsEvents *listener );

		// searching and returning pointer to the first available unit with parallel task on server or NULL
		tintaExecutingUnit *findUnitWithParallelTask() const; 

		/*
			return first or next unit in the set
		*/
        virtual const tintaExecutingUnit *getNextUnit(const m_ulong32 *id = NULL)const;

        virtual tintaExecutingUnit *getNextUnit(const m_ulong32 *id = NULL);

	protected:

		const tintaExecutingUnit * executeOnUnit(const tintaExecutingUnit::UnitId *id = NULL, const tintaExecutingTask *task = NULL );
		//typedef std::map<unsigned long, tintaExecutingUnit *> executingunits_t;

		typedef std::list<tintaExecutingUnit *> executingunits_t;
		typedef executingunits_t::iterator executingunits_it;
        typedef executingunits_t::const_iterator executingunits_cit;

		executingunits_t mUnits;
		// for fast searching
		executingunits_t mUnitsLocal;

		
		executingunits_it findUnit( tintaExecutingUnit::UnitId id );

		executingunits_it findUnit( tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitType type );

        executingunits_it findUnit(m_ulong32 id, tintaExecutingUnit::UnitType type);

        executingunits_it findUnit(m_ulong32 id);

        executingunits_cit findUnit(tintaExecutingUnit::UnitId id)const;

        executingunits_cit findUnit(tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitType type)const;

        executingunits_cit findUnit(m_ulong32 id, tintaExecutingUnit::UnitType type)const;

        executingunits_cit findUnit(m_ulong32 id)const;

		// seraching first Ready or Reserved unit if id is not NULL, compares id
        tintaExecutingUnit *findUnitFree( size_t  id = 0 );

        tintaExecutingUnit *findUnitFree( tintaExecutingUnit::UnitType type )  ;

        m_ulong32 idGen;

		tintaObserved< tintaUnitsEvents > mObserved;      

        MUTEX_RECURSIVE(mStateMutex);
		
	};
}

#endif
