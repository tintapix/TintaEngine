/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_INTERACT_UNITS_CALLBACK_H_
#define _TINTA_INTERACT_UNITS_CALLBACK_H_

#include "tintaExecutingUnit.h"

namespace Tinta { 

	
	struct tintaInteractUnitsCallback		
	{
		virtual void OnUnitStateReceived(tintaExecutingUnit::UnitState state ) = 0;

		virtual void OnPriorReceived(float prior) = 0;

        virtual void OnNameReceived(const String &name) = 0;

		virtual tintaExecutingUnit::UnitId getUnitId() const  = 0;
	};

}


#endif