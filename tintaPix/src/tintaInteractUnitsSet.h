/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_INTERACT_UNITS_SET_H_
#define _TINTA_INTERACT_UNITS_SET_H_

#include <tintaPredefine.h>
#include <tintaSingleton.h>
#include <tintaConnectionEvents.h>
#include "tintaInteractUnit.h"

namespace Tinta { 	

	class tintaInteractUnitsSet:
		public tintaConnectionEvents,
		public Singleton<tintaInteractUnitsSet> 
	{

	public:
		tintaInteractUnitsSet(void);			

		virtual ~tintaInteractUnitsSet(void);		

		virtual void onUnplagged( m_ulong32 id );

		virtual  void onErrorOccured( m_ulong32 id , long error );	

		virtual m_uint32 getObservMask()const;

		virtual void onConnected( m_ulong32 id );

		virtual const tintaInteractUnit* findUnit( m_ulong32 id )const;
		
		//virtual bool updateUnitState( m_ulong32 id );	

        static tintaInteractUnitsSet* getPtr(void);
			
		

	protected:
		// pair - clients id, from connections pool/Executing Unit
		typedef std::map< m_ulong32, tintaInteractUnit* > interactunits_t;

		interactunits_t mInteractUnint;
		
	};

}


#endif
#endif