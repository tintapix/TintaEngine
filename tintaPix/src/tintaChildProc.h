/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */
#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_CHILD_PROC_H_
#define _TINTA_CHILD_PROC_H_

#include <BoostAsioInteract/tintaAsioInteract.h>
#include <BoostAsioInteract/tintaInteractHandler.h>
#include <BoostAsioInteract/tintaIHandlerFactory.h>
#include <BoostAsioInteract/tintaIInteracted.h>
#include <BoostAsioInteract/tintaInteractFunction.h>
#include "tintaUnitsSet.h"


namespace Tinta
{


   
	class _CoreExport tintaChildProc 
		 // on connection to clients events from clients pool
		: public tintaConnectionEvents ,	
		 // allows create packets
		  public tintaIHandlerFactory,
		  // units events reaction
		  public tintaUnitsEvents ,
		  // on send reaction
		  public tintaISendEvents ,

		  public tintaConsoleOutput,

          public tintaIInteracted {
	public:	

		/// Thread function
 		struct InteractData {

			InteractData()
                :mIp(tintaChildProc::strZeroIp)
				,mPort( 0 ){

			}
			InteractData(  String ip, short port )
				: mIp(ip),
				  mPort(port){
			} 			
			String  mIp;
			short		mPort; 			
 		};

          

		tintaChildProc(void);
		virtual ~tintaChildProc(void);

		virtual void				 initialise( const InteractData &req, bool bServer, const String &procName );

		virtual void				 startInteract( ); 

        tintaInteractFunction *getInteractFunc();


		// ---- from tintaConnectionEvents
		virtual void onUnplagged( m_ulong32 id );

		virtual  void onErrorOccured( m_ulong32 id , long error );	

		virtual  void onConnected( m_ulong32 id );

		virtual m_uint32 getObservMask()const;

		
		// ---- from tintaIHandlerFactory ----
		virtual tintaInteractHandler * createHandler( m_ulong32 id );

		virtual void removeHandler( m_ulong32 id );	

		virtual tintaAsioInteract* getInteract();

		// from tintaUnitsEvents
		virtual void onUnitStateChanged( tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state );

		virtual void onUnitJobsDone( tintaExecutingUnit::UnitId id, bool success );

         virtual void onUnitAdded( const tintaExecutingUnit::UnitId &unitId );

        /*
			Unit was removed from the set
		*/
        virtual void onUnitRemoved( const tintaExecutingUnit::UnitId &unitId );

		// from tintaISendEvents	

		virtual void onAfterSend(size_t sendId);

        virtual void onUnitNameChanged(tintaExecutingUnit::UnitId id, String name);

        virtual void onUnitPriorChanged(tintaExecutingUnit::UnitId id, float prior);
		
		// from tintaConsoleOutput
		virtual void printMsg( const Tinta::String &msg , msgLevel stat);

        virtual void clear();

        static const char_m * const strZeroIp; // = _M("0.0.0.0");

	protected:		
		
		tintaAsioInteract	    *mSocketInteract;	
        //tintaInteractFunction			*mFunc;
		InteractData			 mReqData;
		bool					 mbServer;
		
		typedef std::map<long, tintaInteractHandler*> handlers_t;

		handlers_t mHandlers;	

		// saving send state id and last state
		size_t					  mIdSendingState;	
		// unit changed the state
		tintaExecutingUnit::UnitId mUnitClientId;
		tintaExecutingUnit::UnitState mClientState;

	};
}

#endif
#endif