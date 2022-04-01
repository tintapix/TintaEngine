/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_SCRIPT_FUNC_H_
#define _TINTA_SCRIPT_FUNC_H_

#include "tintaPredefine.h"
#include <tintaScriptContext.h>


namespace Tinta
{
	struct	tintaScriptFunc {

		typedef int(*ScriptFunc)( SCRIPT_STATE *L );

		typedef ScriptFunc ScriptFuncPtr;

		tintaScriptFunc()
            : mfuncName(""),
            mFunc(NULL_M){}
		tintaScriptFunc(const StringBasic &func_name , ScriptFuncPtr func )
			: mfuncName( func_name ),
			  mFunc( func ){}
		Tinta::StringBasic		 mfuncName;		
		ScriptFuncPtr mFunc;		
	};
	typedef std::vector<tintaScriptFunc*> pDescripVec_t;



}
#endif

