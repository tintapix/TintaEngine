

#include <tintaConfig.h>

#ifndef TINTA_NO_INTERACT

#include "tintaPacket.h"

namespace Tinta {

tintaPacket::tintaPacket()
: mHeaderSize( enHeaderSize ){
  mBuffHead.AllocateBuffer( mHeaderSize );
}


tintaPacket::tintaPacket( tintaPacket && rVal )
    : mHeaderSize(rVal.mHeaderSize),
      mDataSize(rVal.mDataSize),
    mBuffHead( std::move(rVal.mBuffHead) ),
    mBuffData( std::move(rVal.mBuffData) )
{    
}


tintaPacket::~tintaPacket() {
	//mBuffHead.Release();
	//mBuffData.Release();
}

const m_int8* tintaPacket::getData() const {
	return mBuffData.GetBuffer();
}

m_int8* tintaPacket::getData() {
	return mBuffData.GetBufferEx();
}

const m_int8* tintaPacket::getHeader() const {
	return mBuffHead.GetBuffer();
}

m_int8* tintaPacket::getHeader() {
	return mBuffHead.GetBufferEx();
}

size_t tintaPacket::lenData() const {
	return mDataSize; //header_length + body_length_;
}

size_t tintaPacket::lenHeader( ) const {
	return enHeaderSize;
}

void tintaPacket::clear() {	
	mBuffHead.AllocateBuffer( mHeaderSize );
	mDataSize = 0;
}
void tintaPacket::setLenData( size_t v ){
	mBuffData.AllocateBuffer( v );
	mDataSize = v;
}

}

#endif