/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaExecutingTask.h"

namespace Tinta
{

	tintaExecutingTask::tintaExecutingTask()
		:mId( 0 ),		 
		 mTypeTask(enCommonTask),
		 mUnitId(0)
		 {			
	}


	tintaExecutingTask::tintaExecutingTask( const tintaExecutingTask &rval ):		
			mCommand( rval.mCommand ),			
			mId( rval.mId ),
			mUnitId( rval.mUnitId )
			{}
	
    tintaExecutingTask::tintaExecutingTask(String command, m_ulong32 id, TaskType type, size_t owner) :
		mCommand( command ),	
		mId(id),
		mTypeTask( type ),
		mUnitId( owner )
		{
	}

	tintaExecutingTask::~tintaExecutingTask(void){
	}	

	const tintaExecutingTask& tintaExecutingTask::operator = ( const tintaExecutingTask &rval ){
		if( &rval == this )
			return *this;

		mCommand = rval.mCommand;
		mId = rval.mId;		
		mTypeTask = rval.mTypeTask;
		mUnitId = rval.mUnitId;

		return *this;
	}

	void tintaExecutingTask::setCommand( const String &command ){
		mCommand = command;
	}

	String tintaExecutingTask::getCommand( )const{
		return mCommand;
	}


    m_ulong32 tintaExecutingTask::getId()const{
		return mId;
	}

    void tintaExecutingTask::setId(m_ulong32 id){
		mId = id;
	}

	size_t tintaExecutingTask::getOwner( ) const{
		return mUnitId;
	}

	void tintaExecutingTask::setOwner( size_t ownerId ){
#if DEBUG_MODE
        stream_out << THREAD_CURRENT_ID << _M("setOwner ")<< ownerId <<_M("\n");
#endif
        // TODO Concurrency race place 
        if( mUnitId == 0 || ownerId == 0 )
		    mUnitId = ownerId;		
	}
	
	tintaExecutingTask::TaskType tintaExecutingTask::getType() const {
		return mTypeTask;
	}
	size_t tintaExecutingTask::GetBufferSize() const {
		// command_String + id + task_type + parent id
        return StringPackSize<String>(mCommand) + sizeof(mId) + sizeof(TaskType) + sizeof(mUnitId);
	}

	size_t tintaExecutingTask::packData( m_int8 *data, size_t offset ) const {
		
        assert(data);
		// preparing string
        //size_t vvv = mCommand.length() * sizeof(char_m) + 1;
        //int iCurOffset = WriteToBuffer<m_uint32>(data, 0, mCommand.length() * sizeof(char_m) + sizeof(char_m));
        //size_t l = StringPackSize<String>(mCommand);
        size_t  iCurOffset = PackString<char_m, String>(data, offset, mCommand);

        iCurOffset = WriteToBuffer<m_ulong32>( data, iCurOffset, mId );
		iCurOffset = WriteToBuffer<TaskType>( data, iCurOffset, mTypeTask );
		iCurOffset = WriteToBuffer<size_t>( data, iCurOffset, mUnitId );
		return iCurOffset;
	}

	size_t tintaExecutingTask::unpackData( const m_int8 *data, size_t offset ){		
        assert(data);
		// preparing string
		//m_uint32  iLength = 0;		
        size_t iCurOffset = tintaIOUtil::UnpackString(data, mCommand, offset);
        
        iCurOffset = ReadFromBuffer< m_ulong32 >(data, iCurOffset, mId);
		iCurOffset = ReadFromBuffer< TaskType >( data, iCurOffset, mTypeTask );
		iCurOffset = ReadFromBuffer< size_t >( data, iCurOffset, mUnitId );
		return iCurOffset;

	}
	

}
