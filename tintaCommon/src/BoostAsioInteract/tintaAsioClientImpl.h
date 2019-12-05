/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_ASIO_CLIENT_IMPL_H_
#define _TINTA_ASIO_CLIENT_IMPL_H_

#include "tintaPredefine.h"
#include "tintaLogger.h"
#include "tintaInteractHandler.h"
#include "tintaObserved.h"
#include "tintaConnectionEvents.h"
#include "tintaIHandlerFactory.h"
//#include "tintaExecutingUnit.h"
#include "tintaAsioClient.h"


namespace Tinta {


    class tintaAsioClientImpl
        :public tintaAsioClient,
         public tintaIInteracted {
    public:
        AUTO_MUTEX;

        // used by client
        tintaAsioClientImpl( boost::asio::io_service& io, socket_t &Socket, unsigned long id, tintaInteractHandler* handler = NULL );

        // used by client in servers pool
        tintaAsioClientImpl( boost::asio::io_service& io, unsigned long id, tintaInteractHandler* handler = NULL );
        
        virtual ~tintaAsioClientImpl();

        virtual unsigned long  getId() const;

        virtual void setId(unsigned long val);

        virtual void release();

        virtual socket_t&  getSocket();

        virtual void startInteract();

        virtual void setPoolCallBack(IErrorCallback *clb);

        virtual tintaInteractHandler* getHandler();

        virtual void setHandler(tintaInteractHandler* handler);

        virtual tintaInteractFunction *getFunc();

        virtual void Release();

               

    private:
        socket_t					  mSocketHolder;

        enum {
            DefBufferSize = 0xFF,
        };

        m_int8 m_defBuffer[DefBufferSize];
        

    protected:

        boost::asio::io_service		  &mIoService;        

        socket_t					  &mSocket;      

        unsigned long				 		   mid;

        IErrorCallback 				  *mClientsclb;

        void handleData(const boost::system::error_code& error, size_t bytes_transferred);

        tintaInteractHandler 			       *mInterHandler;

        tintaInteractFunction			*mFunc;    

        std::atomic<bool> mHandling = false;
               

        MUTEX_RECURSIVE_T mDataMutex;
    };

}

#endif
#endif