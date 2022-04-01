/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_EXECUTING_UNIT_H_
#define _TINTA_EXECUTING_UNIT_H_

#include "tintaPredefine.h"
#include "tintaScriptContext.h"
#include "tintaExecutingTask.h"
#include "BoostAsioInteract/tintaAsioClient.h"

namespace Tinta
{


	class tintaExecutingUnitCallback;
	/*
		Base class for all executing units
	*/	
	class _CoreExport tintaExecutingUnit
	{
	public:
		struct UnitId {

            UnitId(m_ulong32 id, String name)
				:mid(id),
				mname (name){
			}

			UnitId(const UnitId &rv)
				:mid(rv.mid),
				mname (rv.mname){
			}

			bool operator== (const UnitId& rval) const {
				
				// if both names exist comparing names
				if( rval.mname.length() > 0 && mname.length() > 0 ){
					return rval.mname == mname;
				}				
				return mid == rval.mid;				

			}

		//	friend stream_o& operator<< (stream_o & out, tintaExecutingUnit::UnitId &id);


			UnitId()
				:mid(0){
			}
            m_ulong32 mid;
			// alternative id, searching linear in set
			String mname;
		};

	public:

		// available states 
		enum UnitState {
			enNoState = 0,  
			enReady ,		// ready to execute
			enBusy  ,		// executing
			enReserved,     // reserved for executing
			enMaxState = enReserved
		};

		enum UnitType {
			enNoType = 0,
			enLocalUnit = 1,	// Local unit			
			enInteractUnit = 3, // Interact unit on server
		};

		tintaExecutingUnit( const UnitId &id );

		tintaExecutingUnit( void );

		virtual ~tintaExecutingUnit( void );
		

		virtual void                    setState( UnitState state );

		UnitState                       getState(  ) const;

		void                            setName( const String &name );		
		
		void                            setPrior(float p);

		float                           getPrior() const;

        void                            setId(m_ulong32 id);

		UnitId	                        getId()const;

		void                            setCallback( tintaExecutingUnitCallback *clb);

		virtual bool                    execute( const tintaExecutingTask &task ) = 0;

		// keeping task after first execution and trying to execute task again
		virtual bool                    execute( ) = 0;

		virtual UnitType                getType() const = 0;

        virtual void                    setAbort();

		virtual const tintaExecutingTask  *getSavedTask()const;
		
		virtual const tintaScriptContext *getContext() const ;

		// for interact units returns associated Client description and NULL for others

#ifndef TINTA_NO_INTERACT
		virtual tintaAsioClient        *getInteractClient() = 0;	

		virtual const tintaAsioClient  *getInteractClient() const = 0;     
#endif
        
        virtual void onStateUpdate();
        

	protected:
		UnitId                     mId;
		tintaExecutingUnitCallback *mClb;
		UnitState                  mState;
		// unit priority from 0 to 1.f
		float                      mPrior;
		// by default it`s NULL, assigned only in local executors
		tintaScriptContext        *mExecutor;
		tintaExecutingTask          mTask;
        //TINTA_MUTEX(mStateUpdateMutex);
	};	

	class tintaExecutingUnitCallback {
		public:
			virtual void onStateChanged(  tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state  ) = 0;

			virtual void onJobsDone(  tintaExecutingUnit::UnitId id, bool success ) = 0;

			virtual void onNameChanged(tintaExecutingUnit::UnitId id, String name) = 0;

			virtual void onPriorChanged(tintaExecutingUnit::UnitId id, float prior) = 0;

	};
}

#endif
