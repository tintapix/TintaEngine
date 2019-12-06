/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_INTERACT_HANDLER_IMPL_H_
#define _TINTA_INTERACT_HANDLER_IMPL_H_

#include <BoostAsioInteract/tintaInteractHandler.h>
#include <BoostAsioInteract/tintaPacket.h>
#include <BoostAsioInteract/tintaServedPool.h>
#include <tintaBufferIO.h>
#include <tintaDispatcher.h>

namespace Tinta {

	
	class  tintaInteractHandlerImpl
		: public tintaInteractHandler {
	public:		
		
		enum interactedData {

			enNoType			 = 0x00,  // on header parsed
			enUnitName			 = 0x01,  // if connected sending  unit name - client identification
			enSendMessage		 = 0x02,  // text message received
			enUnitClosed		 = 0x04,  // received command that the unit preparing to close
			enDisconnectUnit     = 0x08,  // server sends command to client to disconnect
			enTaskToexecute 	 = 0x10,  // send task to execute
			enUnitState 	     = 0x20,  // Interact unit state was changed			
			enSendUnitPrior		 = 0x80,  // unit priority
			enUnitError			 = 0x100, // unit throws error			
			enSystemCommand		 = 0x400, // server sends to clients system command
			enStateRequest		 = 0x800, // server sends to clients state request
            enPackage            = 0x1000, // send package            
		};
//#if DEBUG_MODE
		String getNameType( interactedData type ){
			switch (type){
			case enNoType:
				return _M("no type");


			case enUnitName:
				return _M("Unit name");

            
            case enSendMessage:
                return _M("Send message");

			case enUnitClosed:
				return _M("Unit closed");


			case enDisconnectUnit:
				return _M("Unit disconnected");


			case enTaskToexecute:
				return _M("Task to execute ");

			case enUnitState:
				return _M("Unit state");		

			case enSendUnitPrior:
				return _M("Unit prior");

			case enUnitError:
				return _M("Unit prior");			

			case enSystemCommand:
				return _M("Execute system command");

			case enStateRequest:
				return _M("State request");

            case enPackage:
                return _M("Package recieved");

			default:
				return String();
			}			
		}
//#endif	
		tintaInteractHandlerImpl( m_uint32 filter, bool bServer );

		virtual ~tintaInteractHandlerImpl();

		virtual m_int8*  nextHandle( size_t &size ); 

		virtual bool	  onSendData( const tintaBufferIO *data );

		virtual bool	  onParsePacket( const tintaAsioClient *invoker );

		virtual  void     clear();

		virtual void	  setUnitsCallback( tintaInteractUnitsCallback *c );
		
	// allocating data for sending
		static  tintaBufferIO *allocatePacketData(interactedData type, const tintaBufferIO *data );

        static  tintaBufferIOUnqPtr allocatePacketDataPtr(interactedData type, const tintaBufferIO *data);
	protected:

		enum modehandle {
				enReadingHeader,
				enReadingBody,
		};

		tintaPacket mPacket;

		// uses if only handler in server mode to know the src of the packet
		const tintaAsioClient *mInvoker;

		interactedData mDataType;

		modehandle mMode; 

		size_t mOffset;

		typedef tintaDispatcher< tintaInteractHandlerImpl, interactedData, bool ( tintaInteractHandlerImpl::* )() > handlers_t;

		handlers_t m_Handlers;

		m_uint32 mfilter;

		bool mbServer;
		
	//	moltInteractUnitCallback *mClb;

		void PrintDebugMsg( const String &msg );

		// handlers on receiving
		bool onSetUnitName();

		bool onMessageReceived();

		bool onMessageToSend();

		bool onDisconnected();

		bool onUnitClosed();

		bool onTaskToExecute();

		bool onStateChanged();

		//bool onSendImage();

	//	bool onSendImageResult();

		bool onUnitPrior();

		bool onUnitError();

		bool onExecuteSystem();

		bool onStateRequest();

        bool onPackageReceived();

        bool onAbort();

       


		tintaInteractUnitsCallback *mUnitsCallback;
		//bool onSendMessage();
	};
}

#endif
#endif