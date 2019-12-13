/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#include "tintaAsioClientImpl.h"
#include "tintaException.h"
#include "tintaAsioInteract.h"

#include <boost/bind.hpp>
#include <iostream>

namespace Tinta {
    
    // used by client
    tintaAsioClientImpl::tintaAsioClientImpl( boost::asio::io_service& io, socket_t &Socket, unsigned long id, tintaInteractHandler* handler )
        : mIoService( io )
        , mSocketHolder( io )
        , mSocket( Socket )
        , mid( id )
        , mClientsclb( NULL_M )
        , mInterHandler( handler ){
        mFunc = NEW_T(tintaInteractFunction)(this);

    }

    // used by clients in servers pool
    tintaAsioClientImpl::tintaAsioClientImpl( boost::asio::io_service& io, unsigned long id, tintaInteractHandler* handler )
        : mIoService( io )
        , mSocketHolder(io)
        , mSocket( mSocketHolder )
        , mid(id)
        , mClientsclb(NULL_M)
        , mInterHandler( handler ){
        mFunc = NEW_T(tintaInteractFunction)(this);

    }

    tintaAsioClientImpl::~tintaAsioClientImpl(){
       
        Release();

        if ( mFunc ){
            DELETE_T(mFunc, tintaInteractFunction);
            mFunc = NULL_M;
        }       
        mInterHandler = NULL_M;
        std::cout << "~tintaAsioClientImpl id: " << mid << "\n";
    }

    tintaInteractFunction *tintaAsioClientImpl::getFunc() {
        return mFunc;
    }

    void tintaAsioClientImpl::Release() {

        mSocket.close();
        mClientsclb = NULL_M;   
        //while (mHandling) {
        //}
    }     
    

    void tintaAsioClientImpl::handleData( const boost::system::error_code& error, size_t bytes_transferred ){    
       // TINTA_LOCK_RECUR_MUTEX(mDataMutex);
       // mHandling.store(true);
       
       // AtomicKeeper<bool> ak( mHandling, false );

        if ( !mSocket.is_open() )
            return;
    
        if ( !mFunc )
            return;        
        
        if ( error.value() == tintaAsioInteract::ErrorOnClose )
            return;
        else if( error.value() != 0 ){           
                
                std::string msg = error.message();

                std::cout << "Client receive data error id: " << mid << " " << msg << "\n";

                if ( mInterHandler )
                    mInterHandler->clear();

                if ( mClientsclb )
                    mClientsclb->onError( error, mid );            
            
            return;
        }
        if ( mInterHandler && mInterHandler->onParsePacket(this) ){
            
            size_t size = 0;

            m_int8 *buff = mInterHandler->nextHandle(size);

            boost::asio::async_read( mSocket,
                boost::asio::buffer( buff, size ),
                boost::bind( &tintaAsioClientImpl::handleData, this, boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred ) );

        }
        else {
            
            std::cout << "Received: ";
            for (auto i = 0; i < DefBufferSize; i++)
                std::cout <<  m_defBuffer[i];

            std::cout << "\n";

            boost::asio::async_read(mSocket,
                boost::asio::buffer(m_defBuffer, DefBufferSize),
                boost::bind(&tintaAsioClientImpl::handleData, this, boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }        
    }

    void tintaAsioClientImpl::startInteract() {

        
        // if no handlers returns ok - no receive data
        if ( mInterHandler ) {
            m_int8 *buff = NULL_M;
            size_t size =0;
           buff = mInterHandler->nextHandle(size);
           boost::asio::async_read(mSocket,
               boost::asio::buffer(buff, size),
               boost::bind(&tintaAsioClientImpl::handleData, this, boost::asio::placeholders::error,
                   boost::asio::placeholders::bytes_transferred));
        }
        else {

            boost::asio::async_read(mSocket,
                boost::asio::buffer(m_defBuffer, DefBufferSize),
                boost::bind(&tintaAsioClientImpl::handleData, this, boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

        }
    }
       

    void tintaAsioClientImpl::setPoolCallBack(IErrorCallback *clb){        

        mClientsclb = clb;
    }

    tintaInteractHandler* tintaAsioClientImpl::getHandler(){
        return mInterHandler;
    }

    void tintaAsioClientImpl::setHandler(tintaInteractHandler* handler){

        mInterHandler = handler;
    }

    unsigned long  tintaAsioClientImpl::getId() const {
        return mid;
    }

    void tintaAsioClientImpl::setId(unsigned long val) {
        mid = val;
    }

    socket_t&  tintaAsioClientImpl::getSocket(){
        return mSocket;
    }


    void tintaAsioClientImpl::release(){
    }   
}
#endif