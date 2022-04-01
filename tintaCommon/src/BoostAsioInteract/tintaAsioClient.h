/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_ASIO_CLIENT_H_
#define _TINTA_ASIO_CLIENT_H_

#include "tintaPredefine.h"


#include <boost/asio.hpp>
#include "tintaInteractFunction.h"

namespace Tinta {


	typedef boost::asio::ip::tcp::socket socket_t;


	struct IErrorCallback {
		// if any error has occurred 
        virtual void onError(const boost::system::error_code, m_ulong32 id) = 0;
	};

	struct tintaInteractHandler;

	class _CoreExport tintaAsioClient
	{	
	public:			
        	

        virtual m_ulong32     getId() const = 0;

        virtual void          setId( m_ulong32 val ) = 0;        		

		virtual socket_t&     getSocket() = 0;

		virtual void          startInteract() = 0;

		virtual void          setPoolCallBack( IErrorCallback *clb ) = 0;

		virtual tintaInteractHandler* getHandler() = 0;

		virtual void setHandler( tintaInteractHandler* handler ) = 0;

		virtual tintaInteractFunction *getFunc() = 0;  

        virtual void Release() = 0;        

        virtual ~tintaAsioClient(){}
		
	};
}

#endif

#endif