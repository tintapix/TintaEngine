/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_IMAGE_H
#define TINTA_IMAGE_H


#include "tintaRGBAColor.h"
#include <tintaUInt8Image.h>

namespace Graphica {

class _CoreExport tintaImage
{    

public:
     
	//tintaImage ( const String &path, imageFormat format, imageType type, samplerType    dimension = SAMPLER_2D);
	tintaImage ();

    virtual ~tintaImage ();
    
    imageFormat getFormat () const;

    imageType   getType() const;

    m_uint32 channels() const;

    m_uint32   getChannelSize() const;
    
	m_uint32 width()const;

    m_uint32 height()const;

    m_uint32 depth()const;

    bool isDepth () const;

    bool isCube () const;       

    bool read( const String &path, imageFormat format, imageType type, samplerType    dimension = SAMPLER_2D);

    bool read( const Tinta::tintaUInt8Image &img, const String &name, imageFormat format, imageType type, samplerType    dimension = SAMPLER_2D, bool copyData = true );

    m_uint32 dimention () const;    
    
    m_uint8* data () const;

    m_uint8* data ( m_uint32 i );

    const m_uint8* data(m_uint32 i) const;

	void setName( const String &name );

	const String &getName() const;

    void AllocateData ();
    
    tintaRGBAColor* CreateRGBA () const;
    
    void CopyRGBA (tintaRGBAColor* akCImage) const;
	
    m_uint32 colorSize() const;

protected:	

    imageFormat mFormat;

    imageType   mType;

    samplerType   mDimension;
    
    m_uint8*      mData = NULL_M;   

	String     mName;

    m_uint32 mSize[3] = {1,1,1};    

	static m_uint32 mBytesComponent[IMG_TYPE_QUANTITY];

    static m_uint32 mComponents[IMG_FORMAT_QUANTITY];
	 

};


//typedef Tinta::SharedPtr<tintaImage> tintaImagePtr;


}

#endif
