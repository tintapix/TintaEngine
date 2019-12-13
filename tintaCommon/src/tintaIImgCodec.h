/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef _TINTA_IIMG_CODEC_H_
#define _TINTA_IIMG_CODEC_H_

#include "tintaPredefine.h"
#include "tintaMemoryAllocator.h"


namespace Tinta {

	// base class for image codecs
	class  _CoreExport tintaIImgCodec
		: public AllocatedObjectImgCodec {
	public:
		enum EnImagesTypes
		{
			enBMPType,
			enPNGType,
			enJPGType,
			enTIFFType,
			enMaxType,
		};
		enum ImageBits
		{
            en8bits  = 8,
			en24bits = 24,
			en32bits = 32, // with alpha
			enMaxChannel
		};
        
	public:

        tintaIImgCodec(ImgChannels format, EnImagesTypes type, ImageBits    channels)
            :mChannels(format),
             mImageType(type),
             mChanneltype(channels){}

        static const String mBmp;
        static const String mJpg;
        static const String mPng;

		/// prepare image for save
        virtual bool createImage(int width, int height, ImgChannels color_format, ImageBits channelByte) = 0;
		/// prepare image for read
		virtual bool readImage(const String &image_path ) = 0;		
		/// return image attributes after readImage was called
		virtual bool getAttribs(int attribs [5]) = 0;
		
		///set value to the pixel. Size of pixel depends of channelByte  parameter
        virtual bool setPixel(m_uint32 xCoord, m_uint32 yCoord, const m_uint8 *pixel) = 0;

        virtual bool writeData(const m_uint8 *data) = 0;
		/// saving image to the file
		virtual bool saveImage(const String &image_path) = 0;
		/// if image selected return value > 0  
        virtual m_uint32 getWidth() const = 0;

		/// if image selected return value > 0  
        virtual m_uint32 getHeight() const = 0;

		//virtual void getImage( m_uint8 *image ) = 0;

        virtual bool getPixel(m_uint32 x_coord, m_uint32 y_coord, m_uint8 *pixel) const = 0;

        virtual bool getPixel(m_uint32 pos, m_uint8 *pixel) const = 0;
		

        virtual EnImagesTypes getType() {
            return mImageType;
        }

        virtual ImgChannels getChannels()const = 0;

		static EnImagesTypes getExtByType( const String &extens ){

			//String str_cmp(extens);

            if (extens == mBmp)
				return enBMPType;	
            if (extens == mJpg)
                return enJPGType;	
            else if (extens == mPng)
				return enPNGType;

			return enMaxType;
		}		

        virtual ~tintaIImgCodec(){
        }

    protected:
        ImgChannels      mChannels    = ImgChannels_max;

        EnImagesTypes    mImageType   = enMaxType;

        ImageBits        mChanneltype = enMaxChannel;       
	
	};

}

#endif

