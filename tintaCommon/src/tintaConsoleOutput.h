/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CONSOLE_OUTPUT_H_
#define _TINTA_CONSOLE_OUTPUT_H_


#include "tintaPredefine.h"

namespace Tinta
{

	enum msgLevel
	{       
        
		msg_warning = 0,		  // debug mode		        
        msg_critical = 1,		  // EXCEPTIONs and etc.
		msg_error = msg_critical, // script executing errors        
        msg_system,               // system messages
		msg_info			      // trivial messages
	};
    

	struct tintaConsoleOutput {
		virtual void printMsg( const Tinta::String &msg , msgLevel stat) = 0;

        virtual void clear() = 0;

        virtual void setTitle( const String &title  ) = 0;        
	};

    const String helpCommand = _M("Type the command and press Enter  \n \
                                          or type unit id and command after like '1 c_f(\"c.lua\")'\n \
                                                  to execute on certain unit \n \
                                                  type 'x' or 'exit' to exit \n \
                                                  type 'status' to view common status of the system \n \
                                                  type 'c_f(\"filename\")' or 'c_executefile(\"filename\")' to execute file \n \
                                                  type 'units' to view units status \n \
                                                  type 'tasks' to view tasks \n \
                                                  type 'abort' to abort the execution on local unit \n \
                                                  type 'state' to request the state from all interact units \n \
                                                  type 'cinit' to recreate script context(if any required file was changed) on all units \n \
												  type 'ginit' to recreate gpu context(if any gpu src file was changed) on all units \n \
                                                  type 'state' to request the state of the  concrete interact unit \n)\
                                                  type 'reset' to call abort command and turn the unit to the 'Ready' state  \n)\
                                                  type 'l' to execute last command\n")\

                                                  ;
	

}

#endif
