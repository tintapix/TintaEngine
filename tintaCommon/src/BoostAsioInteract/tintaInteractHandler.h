/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_INTERACT_HANDLER_H_
#define _TINTA_INTERACT_HANDLER_H_



#include "tintaBufferIO.h"


namespace Tinta {

	class tintaAsioClient;
	struct tintaInteractUnitsCallback;
    struct _CoreExport tintaInteractHandler {

		/*  after handling Interactor have to know the data size of the next part and have the access to the allocated buffer		
			may return NULL if the packet have no body
		*/
		virtual m_int8*  nextHandle( size_t &size ) = 0;				

		/*  calls after the packet has parsed	*/
		virtual bool	  onParsePacket( const tintaAsioClient *invoker ) = 0;				


		virtual bool	  onSendData( const tintaBufferIO *data ) = 0;

		// if using connections as executing units
		virtual void	  setUnitsCallback( tintaInteractUnitsCallback *c ) = 0;

		/* clears all data and preparing data reading from the begin*/
		virtual void	  clear() = 0;

        virtual           ~tintaInteractHandler(){}
		
	};



}

#endif

#endif