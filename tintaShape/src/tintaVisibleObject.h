/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_VISIBLE_OBJECT_H
#define TINTA_VISIBLE_OBJECT_H

#include "tintaGraphicaPredefine.h"

namespace Graphica {


	class _CoreExport tintaVisibleObject
	{
	public:
		tintaVisibleObject(){}
		tintaDrawable* Object;		

		bool IsDrawable () const;
	};

	inline bool tintaVisibleObject::IsDrawable () const
	{
		return Object != NULL;
	}


}

#endif
