/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaImage.h"
#include "tintaRoot.h"
#include "ImageCodecs/tintaPngImgCodec.h"
#include "ImageCodecs/tintaJpgImgCodec.h"
#include "tintaGraphicConfig.h"
#include "tintaException.h"
#include "tintaGraphicaPredefine.h"


namespace Graphica{


    m_uint32 tintaImage::mBytesComponent[IMG_TYPE_QUANTITY] = {
		1,
        4,
        4
	};

    m_uint32 tintaImage::mComponents[IMG_FORMAT_QUANTITY] =    {
        COMPONENTS_3,
        COMPONENTS_4,
        COMPONENTS_1,
        COMPONENTS_1,
        COMPONENTS_1,
        COMPONENTS_1,
        COMPONENTS_1
    };


    imageFormat tintaImage::getFormat () const{
		return mFormat;
	}

    imageType   tintaImage::getType() const {
        return mType;
    }	

    m_uint32 tintaImage::channels() const {
        return mComponents[mFormat];
    }

    m_uint32   tintaImage::getChannelSize() const {
        return mBytesComponent[mType];
    }
	
	bool tintaImage::isDepth () const{
         return mSize[2] > 1;
	}
	
    bool tintaImage::isCube() const {
        return mDimension == MAX_SAMPLER_TYPES;
    }

    m_uint32 tintaImage::dimention () const	{
		return (m_uint32)mDimension;
	}	
	 
    
    m_uint8* tintaImage::data() const {
		return mData;
	}
	
    m_uint8* tintaImage::data( m_uint32 i ){
		return mData + i * colorSize();
	}

    const m_uint8* tintaImage::data(m_uint32 i) const {
        return mData + i * colorSize();
    }

    m_uint32 tintaImage::width() const {
		return mSize[0];
	}
    m_uint32 tintaImage::height()const{
		return mSize[1];
	}
    m_uint32 tintaImage::depth()const{
		return mSize[2];
	}
    m_uint32 tintaImage::colorSize() const {        
        return mBytesComponent[mType] * mComponents[mFormat];
    }
	/* tintaImage::tintaImage ( const String &name, imageFormat format, imageType type, samplerType    dimension) {
         read( name, format, type, dimension);
	 }*/

	 
	 tintaImage::tintaImage ()
	 {
		 mFormat = IMG_FORMAT_QUANTITY;
         mType = IMG_TYPE_QUANTITY;
		 mDimension = MAX_SAMPLER_TYPES;
         mSize[0] = 1;
         mSize[1] = 1;
         mSize[2] = 1;        
         
         mData = NULL_M;
	 }
	 

	 tintaImage::~tintaImage () {
		 if( mData )
			FREE_T( mData );
         mData = NULL_M;
	 }

	 void tintaImage::setName(const String &name){
         mName = name;
	 }

	 const String &tintaImage::getName()const{
		 return mName;
	 }

	 
	 void tintaImage::AllocateData ()
	 {
		 if ( !mData ) {

             
			 m_uint32 iNumBytes = mSize[0] * mSize[1] * mSize[2] * colorSize();
           
			 mData = ALLOC_T(m_uint8, iNumBytes);
		 }
	 }


     bool tintaImage::read(const String &path, imageFormat format, imageType type, samplerType dim) {


         // extension
         String ex(StringUtil::getFileExt(path));

         StringUtil::toLower(ex);

         if (ex.length() == 0) {

             return false;
         }
         String fullPath;

         const tintaGraphicConfig* config = tintaRoot::getPtr()->getConfigurator();
         CoreAssert(config, "config");


         // absolute 
         if ( Tinta::isPathValid( path ) )
             fullPath = path;
         else {
             if ( !config->getTexturePath( fullPath, path ) ) {
                 StringStream msg;
#if TINTA_LOCAL ==  TINTA_EN			 
                 msg << u8"Cannot find image file: ";
                 msg << path;
#elif TINTA_LOCAL == TINTA_RU             
                 msg << u8"Не найден файл изображения: ";
                 msg << path;
#endif
                 if (Tinta::tintaLogger::getPtr())
                     Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                 else
                     stream_out << msg.str() << _M("\n");

                 return false;
         }
     }
               
         Tinta::tintaIImgCodec* codec = NULL_M;

         if ( ex == _M("png") )
             codec = M_NEW tintaPngImgCodec();
         else if ( ex == _M("jpeg" )
                        || ex == _M("jpg") )
             codec = M_NEW tintaJpgImgCodec();
         else {            
             return false;
         }

         UNIQPTRVIRT( Tinta::tintaIImgCodec, mImgFilePtr, codec );

         mDimension = dim;
         mFormat = format;
         mType = type;
         
         if (mImgFilePtr->readImage( fullPath ) ) {

             int attribs[5];
             mImgFilePtr->getAttribs( attribs );             
             
             
             mSize[0] = mImgFilePtr->getWidth();
             mSize[1] = mImgFilePtr->getHeight();
                          

                AllocateData();
                m_uint8 vals[4] = { 0, 0, 0, 255 };
                
                m_uint32 posData = 0;
                m_uint32 offset = mBytesComponent[mType];
                int hBeg = (int)mSize[1] - 1;
                for ( int h = hBeg; h >= 0; h-- ) {

                    for ( m_uint32 w = 0; w < mSize[0]; w++ ) {

                        mImgFilePtr->getPixel(w, (m_uint32)h, vals);

                        float r = byteToFloat(vals[0]);
                        float g = byteToFloat(vals[1]);
                        float b = byteToFloat(vals[2]);
                        float a = byteToFloat(vals[3]);

                        tintaVector3f hsl = rgbToHsl( { r, g, b } );

                        float valsFinal[5] = { r, g, b, a, hsl.mz };

                        std::vector<m_uint32> pos;

                        switch ( mFormat ) {

                        case IMG_RGB:
                            pos = { 0, 1, 2 };
                            break;
                        case IMG_RGBA:
                            pos = { 0, 1, 2, 3 };
                            break;
                        case IMG_R:
                            pos = { 0 };
                            break;
                        case IMG_G:
                            pos = { 1 };
                            break;
                        case IMG_B:
                            pos = { 2 };
                            break;
                        case IMG_A:
                            pos = { 3 };
                            break;
                        case IMG_LUMINANCE:
                            pos = { 4 };
                            break;
                        default:
                            break;
                        };


                        switch ( mType ) {

                        case IMG_UBYTE:

                            if (mFormat == IMG_LUMINANCE) {
                                // for (m_uint32 i : pos) {
                                m_uint8 val = floatToByte(hsl.mz);
                                memcpy(mData + posData, &val, offset);
                                posData += offset;
                                //}
                            }
                            else {
                                for (m_uint32 i : pos) {

                                    m_uint8 val = vals[i];//(m_uint32)floatToByte(valsFinal[i]);
                                    memcpy(mData + posData, &val, offset);
                                    posData += offset;
                                }
                            }

                            break;
                        case IMG_UINT:

                            if (mFormat == IMG_LUMINANCE) {
                                m_uint32 val = (m_uint32)floatToByte(hsl.mz); //(m_uint32)floatToByte(valsFinal[i]);
                                memcpy(mData + posData, &val, offset);
                                posData += offset;
                            }
                            else {
                                for (m_uint32 i : pos) {
                                    m_uint32 val = (m_uint32)vals[i]; //floatToByte(valsFinal[i]);
                                    memcpy(mData + posData, &val, offset);
                                    posData += offset;

                                }
                            }

                            break;
                        case IMG_FLOAT:
                            for ( m_uint32 i : pos ) {                                
                                memcpy( mData + posData, &valsFinal[i], offset );
                                posData += offset;
                            }
                           
                            break;
                        case IMG_UNSIGNED_INT_8_8_8_8:                        

                            m_uint32 val;
                            // packing channel values
                           // for ( m_uint32 i : pos ) {                                
                            val = packColor4( { valsFinal[0], valsFinal[1], valsFinal[2], valsFinal[3] } );
                            //}
                            memcpy( mData + posData, &val, offset );
                            posData += offset;

                            break;
                        default:
                            break;
                        };
                        //setPixel(coord2dI_t(w, h), color_type({ vals[0], vals[1], vals[2], vals[3] }));
                    }
                }      

             mName = path;
             return true;            
         }

         return false;
     }

     bool tintaImage::read(const Tinta::tintaUInt8Image &img, const String &name, imageFormat format, imageType type, samplerType dim, bool copyData) {

         mDimension = dim;
         mFormat = format;
         mType = type;
           
            mSize[0] = img.getWidth();
            mSize[1] = img.getHeight();            

            AllocateData();

            if ( copyData ) {

                m_uint8 vals[4] = { 0, 0, 0, 255 };

                m_uint32 posData = 0;
                m_uint32 offset = mBytesComponent[mType];

                int hBeg = (int)mSize[1] - 1;
                for (int h = hBeg; h > 0; h--) {

                    for (m_uint32 w = 0; w < mSize[0]; w++) {

                        color_type color = img.getPixel({ (int)w, (int)h });


                        float r = byteToFloat(color.getVal(0));
                        float g = byteToFloat(color.getVal(1));
                        float b = byteToFloat(color.getVal(2));
                        float a = byteToFloat(color.getVal(3));

                        tintaVector3f hsl = rgbToHsl({ r, g, b });

                        float valsFinal[5] = { r, g, b, a, hsl.mz };

                        std::vector<m_uint32> pos;

                        switch (mFormat) {

                        case IMG_RGB:
                            pos = { 0, 1, 2 };
                            break;
                        case IMG_RGBA:
                            pos = { 0, 1, 2, 3 };
                            break;
                        case IMG_R:
                            pos = { 0 };
                            break;
                        case IMG_G:
                            pos = { 1 };
                            break;
                        case IMG_B:
                            pos = { 2 };
                            break;
                        case IMG_A:
                            pos = { 3 };
                            break;
                        case IMG_LUMINANCE:
                            pos = { 4 };
                            break;
                        default:
                            break;
                        };


                        switch (mType) {

                        case IMG_UBYTE:

                            if (mFormat == IMG_LUMINANCE) {
                                // for (m_uint32 i : pos) {
                                m_uint8 val = floatToByte(hsl.mz);
                                memcpy(mData + posData, &val, offset);
                                posData += offset;
                                //}
                            }
                            else {
                                for (m_uint32 i : pos) {

                                    m_uint8 val = color.getVal((m_uint8)i);//(m_uint32)floatToByte(valsFinal[i]);
                                    memcpy(mData + posData, &val, offset);
                                    posData += offset;
                                }
                            }

                            break;
                        case IMG_UINT:

                            if (mFormat == IMG_LUMINANCE) {
                                m_uint32 val = (m_uint32)floatToByte(hsl.mz); //(m_uint32)floatToByte(valsFinal[i]);
                                memcpy(mData + posData, &val, offset);
                                posData += offset;
                            }
                            else {
                                for (m_uint32 i : pos) {
                                    m_uint32 val = (m_uint32)color.getVal((m_uint8)i); //floatToByte(valsFinal[i]);
                                    memcpy(mData + posData, &val, offset);
                                    posData += offset;

                                }
                            }

                            break;
                        case IMG_FLOAT:
                            for (m_uint32 i : pos) {
                                memcpy(mData + posData, &valsFinal[i], offset);
                                posData += offset;
                            }

                            break;
                        case IMG_UNSIGNED_INT_8_8_8_8:

                            m_uint32 val;
                            // packing channel values
                            for (m_uint32 i : pos) {
                                val = packColor4({ valsFinal[0], valsFinal[1], valsFinal[2], valsFinal[3] });
                            }
                            memcpy(mData + posData, &val, offset);
                            posData += offset;

                            break;
                        default:
                            break;
                        };
                        //setPixel(coord2dI_t(w, h), color_type({ vals[0], vals[1], vals[2], vals[3] }));
                    }
                }
            }
            mName = name;
            return true;
         
     }
	 
	 tintaRGBAColor* tintaImage::CreateRGBA () const
	 {
		 
		 return 0;
	 }
	 
	 void tintaImage::CopyRGBA (tintaRGBAColor* akCImage) const
	 {
	 }



}