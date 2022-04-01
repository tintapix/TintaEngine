/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_INTERACT_UNIT_H_
#define _TINTA_INTERACT_UNIT_H_

#include <tintaPredefine.h>
#include <BoostAsioInteract/tintaAsioClient.h>
#include "tintaExecutingUnit.h"
#include "tintaInteractHandlerImpl.h"
#include "tintaInteractUnitsCallback.h"

namespace Tinta { 
    
    /*
        Interact unit on server(interacts with the local unit on server)
    */
	class tintaInteractUnit: 
		public tintaExecutingUnit,		
		public tintaInteractUnitsCallback
	{

	public:
		tintaInteractUnit();

		tintaInteractUnit( tintaAsioClient * pInteract);

		tintaAsioClient * getInteractClient();	

		const tintaAsioClient * getInteractClient() const ;

		void addtInteractClient(tintaAsioClient *add);
		
		virtual ~tintaInteractUnit(void);		

		virtual bool execute( const tintaExecutingTask &task );

		//virtual void setState(UnitState state);

		virtual tintaExecutingUnit::UnitType getType()const;

		// allowing assign only the reserve state 
		virtual void setState( UnitState state );

		virtual bool execute( );

		// from tintaInteractUnitsCallback
		virtual void OnUnitStateReceived( tintaExecutingUnit::UnitState state );

		virtual void OnPriorReceived(float prior);

		virtual void OnNameReceived(const String &name);

		virtual UnitId getUnitId() const;

		//--
		


	private:
		tintaAsioClient *mPInteract;
	};

}


#endif
#endif