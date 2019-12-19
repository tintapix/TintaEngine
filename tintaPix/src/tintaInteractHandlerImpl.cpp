/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#include "tintaInteractHandlerImpl.h"
#include <ImageCodecs/tintaPngImgCodec.h>
#include "tintaInteractUnitsCallback.h"
#include "tintaTexSpringMain.h"
#include "tintaUInt8ImageSeamless.h"
#include "tintaImgStack.h"
#include "tintaTaskQueue.h"

namespace Tinta {

	
	
	tintaInteractHandlerImpl::tintaInteractHandlerImpl(m_uint32 filter, bool bServer )
		:mMode( enReadingHeader )
		,mDataType(enNoType)
		,mOffset(0)
		,mInvoker(NULL)
		,mfilter(filter)
		, mbServer(bServer)
	    ,mUnitsCallback(NULL){

			

			m_Handlers.Register(tintaInteractHandlerImpl::enUnitName, this, &tintaInteractHandlerImpl::onSetUnitName );

			m_Handlers.Register(tintaInteractHandlerImpl::enSendMessage, this, &tintaInteractHandlerImpl::onMessageReceived );

			m_Handlers.Register(tintaInteractHandlerImpl::enUnitClosed, this, &tintaInteractHandlerImpl::onUnitClosed );

			m_Handlers.Register(tintaInteractHandlerImpl::enDisconnectUnit, this, &tintaInteractHandlerImpl::onDisconnected );

			m_Handlers.Register(tintaInteractHandlerImpl::enTaskToexecute, this, &tintaInteractHandlerImpl::onTaskToExecute );

			m_Handlers.Register(tintaInteractHandlerImpl::enSystemCommand, this, &tintaInteractHandlerImpl::onExecuteSystem );


            m_Handlers.Register(tintaInteractHandlerImpl::enUnitState, this, &tintaInteractHandlerImpl::onStateChanged);

			//m_Handlers.Register(tintaInteractHandlerImpl::enImageSend, this, &tintaInteractHandlerImpl::onSendImage);

			m_Handlers.Register(tintaInteractHandlerImpl::enSendUnitPrior, this, &tintaInteractHandlerImpl::onUnitPrior);

			m_Handlers.Register(tintaInteractHandlerImpl::enUnitError, this, &tintaInteractHandlerImpl::onUnitError);

			//m_Handlers.Register(tintaInteractHandlerImpl::enImageResult, this, &tintaInteractHandlerImpl::onSendImageResult);

			m_Handlers.Register(tintaInteractHandlerImpl::enStateRequest, this, &tintaInteractHandlerImpl::onStateRequest);

            m_Handlers.Register(tintaInteractHandlerImpl::enPackage, this, &tintaInteractHandlerImpl::onPackageReceived);

            
            
	}


	void tintaInteractHandlerImpl::clear() {
			mPacket.clear();
			mMode = enReadingHeader;
			mDataType = enNoType;
			mOffset = 0;
	}
	tintaInteractHandlerImpl::~tintaInteractHandlerImpl(){

	}
    	

	tintaBufferIO *tintaInteractHandlerImpl::allocatePacketData( interactedData type, const tintaBufferIO *data ){

		CoreAssert( data, "const tintaBufferIO *data" );

		if( !data )			
			return NULL;


		tintaBufferIO *packetData = NEW_T  (tintaBufferIO)();


		packetData->AllocateBuffer( sizeof(int) + sizeof(short) + data->GetSize() );//( msg.length() + 1 ) * sizeof(char_m) );

        size_t offset = WriteToBuffer<size_t>(packetData->GetBufferEx(), 0, data->GetSize());
		offset	   = WriteToBuffer<short>( packetData->GetBufferEx(), offset, (short) type );		
		offset	   = WriteArrayToBuffer<m_int8>( packetData->GetBufferEx(), offset, data->GetBuffer(), data->GetSize() );

		return packetData;
	}


    tintaBufferIOUnqPtr tintaInteractHandlerImpl::allocatePacketDataPtr(interactedData type, const tintaBufferIO *data) {


        UNIQPTRDEF(tintaBufferIO, buffPacket, tintaInteractHandlerImpl::allocatePacketData(type, data ));      


        buffPacket->AllocateBuffer(sizeof(int) + sizeof(short) + data->GetSize());//( msg.length() + 1 ) * sizeof(char_m) );

        int offset = WriteToBuffer<size_t>(buffPacket->GetBufferEx(), 0, data->GetSize());
        offset = WriteToBuffer<short>(buffPacket->GetBufferEx(), offset, (short)type);
        offset = WriteArrayToBuffer<m_int8>(buffPacket->GetBufferEx(), offset, data->GetBuffer(), data->GetSize());

        return buffPacket;
    }

	bool tintaInteractHandlerImpl::onParsePacket( const tintaAsioClient *invoker ) {
        

       /* StringUtil::StrStreamType t;
        t<<_M("tintaInteractHandlerImpl::onParsePacket mMode(0-hrader 1-body)")<< mMode <<"\n";
        Tinta::tintaLogger::getPtr()->logMsg( t.str() );
        */


		mInvoker = invoker;
		if( mMode == enReadingHeader ){

			size_t dataSize = 0;	

			mOffset =  ReadFromBuffer<size_t>( mPacket.getHeader(), mOffset, dataSize );

			if(dataSize ==  0 )
				return false;

			mPacket.setLenData( dataSize );

			short dataType = 0;

			ReadFromBuffer<short>( mPacket.getHeader(), mOffset, dataType );	
			interactedData eDataType = static_cast<interactedData> ( dataType );	
			mDataType = eDataType;
			
			mMode = enReadingBody;
			
				
		}
		else if( mMode == enReadingBody ){
/*#if DEBUG_MODE 
 			StringUtil::StrStreamType msg;
            if (mInvoker)
                msg<< THREAD_CURRENT_ID << _M(" From") << mInvoker->getId();
 			msg << _M(" Parsed packet with type: ");			
 			msg << getNameType(mDataType);
 			PrintDebugMsg( msg.str() );
#endif*/
			
            
  

			if( mfilter & mDataType ){
                
				bool rez = m_Handlers.Dispatchering(mDataType);
				
				if( !rez )
					stream_out<<_M("Dispatchering error mDataType: ")<<mDataType<<"\n";

			}

			mMode = enReadingHeader;		
			mOffset = 0;
			
		}
		
		mInvoker = NULL;
		return true;
	}

	// handlers
	bool tintaInteractHandlerImpl::onSetUnitName(){

		if( !mInvoker && mbServer )
			return false;
		

		String name;// ( nameArr ) );
		if( mPacket.getData() && mPacket.lenData() > 0 ) {
			
#if 0 // old
            const m_int8 *buff = mPacket.getData();
            UNIQPTRALLOC(char_m, nameArr, ALLOC_T(char_m, mPacket.lenData() / sizeof(char_m) + 1))			
			ReadArrayFromBuffer<char_m>( buff, 0, nameArr.get(), mPacket.lenData() / sizeof(char_m) );
			name =  nameArr.get(); 
                        
#endif
            String r;
            tintaIOUtil::UnpackString(mPacket.getData(), r);
			if ( mUnitsCallback )
                mUnitsCallback->OnNameReceived( r );
		}	

// #if DEBUG_MODE 
 		//StringUtil::StrStreamType msg; 
 		//msg << _M("Name received: ")<< _M("name: ")<<name;
 		//Tinta::tintaLogger::getPtr()->logMsg( msg.str( ) );
// #endif			
		return true;
	}

	bool tintaInteractHandlerImpl::onDisconnected() {
	return true;
	}

	void tintaInteractHandlerImpl::setUnitsCallback( tintaInteractUnitsCallback *c ){
		mUnitsCallback = c;

	}

	void tintaInteractHandlerImpl::PrintDebugMsg( const String &msg ){		
        Tinta::tintaLogger::getPtr()->logMsg(msg);
	}
	bool tintaInteractHandlerImpl::onMessageReceived(){

		if( !mInvoker && mbServer)
			return false;
#if 0 // old
		String name;
		size_t size = mPacket.lenData()/sizeof( char_m );
		//char_m *data = ALLOC_T( char_m, size );
        UNIQPTRALLOC(char_m, data, ALLOC_T(char_m, size))

		const m_int8 *buff = mPacket.getData();
		mlMemcpy( data.get(), mPacket.lenData(), buff, mPacket.lenData() );
#endif		
        String r;
        tintaIOUtil::UnpackString(mPacket.getData(), r );

		StringUtil::StrStreamType msg; 	
        if (mUnitsCallback)
            msg << _M("[") << mUnitsCallback->getUnitId().mname << _M("/") << mUnitsCallback->getUnitId().mid << _M("]");
        else
            msg << _M("[") << mInvoker->getId() << _M("]");

        msg << r;

		//FREE_T( data );
		//if ( mbServer  )
		Tinta::tintaLogger::getPtr()->logMsg( msg.str() );


		
		return true;
	}

	bool tintaInteractHandlerImpl::onUnitClosed(){
		if( !mInvoker && mbServer)
			return false;

		StringUtil::StrStreamType msg; 		

		msg << _M("Unit disconnecting...") <<  mInvoker->getId();

		Tinta::tintaLogger::getPtr()->logMsg( msg.str() );

		return true;
	}

	bool tintaInteractHandlerImpl::onTaskToExecute(){

		if( !mInvoker && mbServer)
			return false;

		size_t size = mPacket.lenData()/sizeof( char_m );
		if( size == 0 || !mPacket.getData() )
			return false;


		const m_int8 *buff = mPacket.getData();
		//mlMemcpy( data, mPacket.lenData(), buff, mPacket.lenData() );

		tintaExecutingTask parsedTask;
		parsedTask.unpackData( buff, 0 );		
	
        //tintaTexSpringMain::getPtr()->executeCommand( parsedTask.getCommand(), 1 );
        
		if( Tinta::tintaTaskQueue::getPtr() ){
			m_ulong32 id = 0;
            // ignoring parsedTask.getType()
            // old version Tinta::tintaTaskQueue::getPtr()->pushTask( parsedTask.getCommand(),parsedTask.getType(), id, 0 );  
           // stream_out << _M(" Received task to execute ") << parsedTask.getCommand();

            Tinta::tintaTaskQueue::getPtr()->pushTask( parsedTask.getCommand(), tintaExecutingTask::enCommonTask, id, 0 );            
		}		
		
		return true;

	}

    bool tintaInteractHandlerImpl::onStateChanged(){

		if( !mInvoker && mbServer)
			return false;
		
		size_t size = mPacket.lenData()/sizeof( char_m );
		if( size == 0 || !mPacket.getData() )
			return false;
		
		const m_int8 *buff = mPacket.getData();	
        size_t offset = 0;
		// reading unit id
		m_ulong32 id = 0;
		offset  = ReadFromBuffer<m_ulong32>( buff, offset, id );	
		int state = 0;
		offset  = ReadFromBuffer<int>( buff, offset, state );

		if( !Tinta::tintaUnitsSet::getPtr() )
			return false;
		

        //StringUtil::StrStreamType t;
        //t << _M("state received(1-ready 2-busy 3-reserved) ") << state << _M("\n");
        //Tinta::tintaLogger::getPtr()->logMsg(t.str());


		//const tintaExecutingUnit *unit =  Tinta::tintaUnitsSet::getPtr()->getUnit( tintaExecutingUnit::UnitId(id, _M("")) );
		if( state < (int)tintaExecutingUnit::enNoState || state > (int)tintaExecutingUnit::enMaxState ) {
			return false;
		}
#if DEBUG_MODE
        if (mUnitsCallback)
            stream_out << THREAD_CURRENT_ID << _M(" ") << mUnitsCallback->getUnitId().mname;
#endif
		
		if( mUnitsCallback )
			mUnitsCallback->OnUnitStateReceived( (tintaExecutingUnit::UnitState) state  );
		
		return true;		
	}

	m_int8* tintaInteractHandlerImpl::nextHandle( size_t &size ){
		size = 0;
		if( mMode == enReadingHeader ){
			
			size = mPacket.lenHeader();
			if( size > 0 ) 
				return  mPacket.getHeader();
		}
		else if( mMode == enReadingBody ){

			
			size = mPacket.lenData();
			if( size > 0 ) 
				return  mPacket.getData();
		}

		return NULL;
	}
#if 0
	bool tintaInteractHandlerImpl::onSendImage() {

		//StringUtil::StrStreamType msg;
		//msg << _M("tintaInteractHandlerImpl::onSendImage");		
		//Tinta::tintaLogger::getPtr()->logMsg(msg.str());

		if (!mInvoker && mbServer)
			return false;

		size_t size = mPacket.lenData();
		if (size == 0 || !mPacket.getData())
			return false;

		const m_int8 *buff = mPacket.getData();

        size_t offset = 0;
		size_t sizeType = 0;
		offset = ReadFromBuffer<size_t>(buff, offset, sizeType);


		char_m *data = ALLOC_T(char_m, sizeType);
		mlMemcpy(data, sizeType*sizeof(char_m), buff + offset, sizeType*sizeof(char_m));
		offset += sizeType*sizeof(char_m);
		StringUtil::StrStreamType imgType;		
		imgType << data;
		String strImgType = imgType.str();
		FREE_T(data);

		Tinta::tintaIImgCodec::EnImagesTypes type;	

        StringUtil::toLower(strImgType);		
		if (strImgType == _M("png"))
			type = Tinta::tintaIImgCodec::enPNGType;
		else if (strImgType == _M("jpeg") || strImgType == _M("jpg"))
			type = Tinta::tintaIImgCodec::enJPGType;		
		else 
			return false;
		

		size_t sizePath = 0;
		offset = ReadFromBuffer<size_t>(buff, offset, sizePath);
		data = ALLOC_T(char_m, sizePath);
		mlMemcpy(data, sizePath*sizeof(char_m), buff + offset, sizePath*sizeof(char_m));
		offset += sizePath*sizeof(char_m);
		StringUtil::StrStreamType strImgPath;
		strImgPath << data;
		String imgPath = strImgPath.str();
		FREE_T(data);
		
		if( !Tinta::createFolder( imgPath  ) ) {						
			return false;
		}


		//Tinta::tintaLogger::getPtr()->logMsg( _M("Begin saving!"));		

        m_uint32  addedImg = tintaImgStack::getPtr()->addImg(tintaUInt8ImageSeamless::str_image_seamless); //image_width, image_height);					

		molyParallelSave *saver = Tinta::tintaTexSpringMain::getPtr()->getImageSaver();
		assert( saver );
		tintaUInt8Image *pImage = tintaImgStack::getPtr()->getImageEx( addedImg );
		if( pImage ){
			offset =  (int) pImage->unpackData( buff, ( size_t ) offset );
			
			StringUtil::StrStreamType msg;
			msg << _M("Saving image ")<<pImage->width()<<_M("x")<<pImage->height();

			Tinta::tintaLogger::getPtr()->logMsg( msg.str() );	

			saver->doSaving( molyParallelSave::SaveData( addedImg, imgPath, type ) );
		}
		//Tinta::tintaLogger::getPtr()->logMsg( _M("End saving!"));		
		return true;
	}

	bool tintaInteractHandlerImpl::onSendImageResult( ){

		//StringUtil::StrStreamType msg;
		//msg << _M("tintaInteractHandlerImpl::onSendImageResult");		
		//Tinta::tintaLogger::getPtr()->logMsg( msg.str() );

		if ( !mInvoker && mbServer )
			return false;

		size_t size = mPacket.lenData();
		if ( size == 0 || !mPacket.getData() )
			return false;

		const m_int8 *buff = mPacket.getData();

        size_t offset = 0;
		size_t sizeType = 0;
		offset = ReadFromBuffer<size_t>(buff, offset, sizeType);


		//char_m *data = ALLOC_T(char_m, sizeType);
        UNIQPTRALLOC(char_m, data, ALLOC_T(char_m, sizeType))

		mlMemcpy( data.get(), sizeType*sizeof(char_m), buff + offset, sizeType*sizeof(char_m) );

		offset += sizeType*sizeof(char_m);

		StringUtil::StrStreamType imgType;		
		imgType << data.get();
		String strImgType = imgType.str();

        m_uint32  addedImg = tintaImgStack::getPtr()->addImg( tintaUInt8ImageSeamless::str_image_seamless ); //image_width, image_height);			

		tintaUInt8Image *pImage = tintaImgStack::getPtr()->getImageEx( addedImg );
		Tinta::tintaScriptContext *executor = Tinta::tintaTexSpringMain::getPtr()->getContext();
		if( pImage && executor){
				offset =  (int) pImage->unpackData( buff, ( size_t ) offset );					
				const tintaInteractUnit* unit = tintaInteractUnitsSet::getPtr()->findUnit( mInvoker->getId() );
				if( unit ){
					m_ulong32 unitId = unit->getId().mid;
					molyParallelExecutor::getPtr()->executeHandle( unitId , &addedImg, 1, executor );
				}			
		}
				
		//Tinta::tintaLogger::getPtr()->logMsg( _M("Image result!"));				
		//M_DELETE  image;
		return true;

	}
#endif
	bool tintaInteractHandlerImpl::onStateRequest() {

		if( !mInvoker && mbServer )
			return false;

		const tintaExecutingUnit *unit = tintaUnitsSet::getPtr()->getNextUnit( NULL_M );

		m_ulong32 id = unit ? unit->getId().mid : 0;

		for( ;unit; unit = tintaUnitsSet::getPtr()->getNextUnit( &id ) ){
							  //state( unit );
            if (unit->getType() == tintaExecutingUnit::enLocalUnit){
				tintaExecutingUnit::UnitState state = unit->getState();
				tintaChildProc * interact = tintaTexSpringMain::getPtr()->getInteract();                
				interact->onUnitStateChanged( unit->getId(), state );
			}

		}

		return true;
	}

    bool tintaInteractHandlerImpl::onAbort() {

        if (!mInvoker && mbServer)
            return false;

        tintaExecutingUnit *unit = tintaUnitsSet::getPtr()->getNextUnit(NULL_M);
        m_ulong32 id = unit ? unit->getId().mid : 0;
        for ( ; unit != NULL; unit = tintaUnitsSet::getPtr()->getNextUnit(&id) ){

            unit->setAbort();

            StringStream msg;
            id = unit->getId().mid;

            if ( unit->getState() == tintaExecutingUnit::enBusy )
                if ( unit )
                    unit->setAbort();
            //Tinta::tintaTexSpringMain::getPtr()->setAborted( true );						  	

        }

        return true;
    }
	bool tintaInteractHandlerImpl::onSendData( const tintaBufferIO *data ) {



		return true;
	}

	bool tintaInteractHandlerImpl::onUnitError() {

		if( !mInvoker && mbServer )
			return false;

		//String name;
		//size_t size = mPacket.lenData()/sizeof( char_m );
		//char_m *data = ALLOC_T( char_m, size );
#if 0   // old 
        UNIQPTRALLOC(char_m, data, ALLOC_T(char_m, size))
		const m_int8 *buff = mPacket.getData();
		mlMemcpy( data.get(), mPacket.lenData(), buff, mPacket.lenData() );
#endif
        //FREE_T( data );
        //if ( mbServer  )
        String r;
        tintaIOUtil::UnpackString( mPacket.getData(), r );
		StringUtil::StrStreamType msg; 		
		if ( mUnitsCallback )
            msg << mUnitsCallback->getUnitId().mname << _M("/") << mUnitsCallback->getUnitId().mid << _M(" ");
		else
            msg << mInvoker->getId();		

        msg << r;
		
		Tinta::tintaLogger::getPtr()->logMsg( msg.str() , Tinta::msg_error );
        		
		
		return true;
	}

	bool tintaInteractHandlerImpl::onExecuteSystem() {
		if( !mInvoker && mbServer )
			return false;
        String r;
        tintaIOUtil::UnpackString( mPacket.getData(), r );
        // executing like ordinary command
        Tinta::tintaTexSpringMain::getPtr()->executeCommand( r );
		return true;
	}

	bool tintaInteractHandlerImpl::onUnitPrior(){

		if( !mInvoker && mbServer )
			return false;


		float prior(1.f);
		if( mPacket.getData() && mPacket.lenData() > 0 ) {
			//char_m *nameArr = ALLOC_T( char_m, mPacket.lenData()/sizeof(char_m)  + 1 );
			//const m_int8 *buff = mPacket.getData();
            size_t offset = 0;
			offset = ReadFromBuffer<float>( mPacket.getData() , offset, prior);		

			if (mUnitsCallback)
				mUnitsCallback->OnPriorReceived(prior);
		}
		return true;			
	}

    bool tintaInteractHandlerImpl::onPackageReceived() {       
        
        if ( mPacket.getData() && mPacket.lenData() > 0 ) {
            //char_m *nameArr = ALLOC_T( char_m, mPacket.lenData()/sizeof(char_m)  + 1 );
            //const m_int8 *buff = mPacket.getData();
            size_t offset = 0;
            //offset = ReadFromBuffer<float>(mPacket.getData(), offset, prior);
            
            //String objName;
            
            String containerName;
            offset = tintaIOUtil::UnpackString(mPacket.getData(), containerName, offset);
            String objName;
            offset = tintaIOUtil::UnpackString(mPacket.getData(), objName, offset);
            
            tintaISerialisableSet * packer = Tinta::tintaTexSpringMain::getPtr()->getSerialisableSet( containerName );
            if ( packer ) {

                //packer->unpackData()
            }
            else {
                EXCEPTION("Wrong container name.");
            }

            //offset = tintaIOUtil::UnpackString(mPacket.getData(), objName, offset);


            //if (mUnitsCallback)
            //    mUnitsCallback->OnPriorReceived(prior);
        }
        return true;
    }
    
	
}
#endif