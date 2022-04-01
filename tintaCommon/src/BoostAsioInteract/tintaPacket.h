/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_PACKET_H_
#define _TINTA_PACKET_H_

#include "tintaBufferIO.h"

namespace Tinta {

/*
	Packet for interprocess interaction
	Packet structure: header( len(size==int) + type(size==short) ) + data( body(size==len) )
*/
class _CoreExport tintaPacket {

public:	

	enum {
		// size of the  packets data
		enDataLenSize = sizeof(int),
		// data type size
		enDataTypeSize = sizeof(short),	
		enHeaderSize = enDataLenSize + enDataTypeSize,
	};

	
	// tintaPacket();

	tintaPacket( );


    tintaPacket( tintaPacket&& rVal );


	~tintaPacket();	

	const m_int8* getData() const;

		  m_int8* getData();

	const m_int8* getHeader() const;

	m_int8*       getHeader();

	size_t         lenData() const;

	size_t         lenHeader() const;
    
	void		   setLenData(size_t v);
	
	void			clear();
	//virtual void create( const tintaBufferIO *data ) = 0;
protected:
	//char data_[header_length + max_body_length];
    size_t mHeaderSize = 0;

    size_t mDataSize = 0;    
	
	tintaBufferIO mBuffHead;

	tintaBufferIO mBuffData;
};

}

#endif
#endif