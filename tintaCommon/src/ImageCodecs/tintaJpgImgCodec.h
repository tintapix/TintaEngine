/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef _TINTA_JPG_IMG_CODEC_H_
#define _TINTA_JPG_IMG_CODEC_H_


#include "../tintaIImgCodec.h"
extern "C"
{
    #include "libjpeg/jpeglib.h"
}

namespace Tinta {

    /*
        Version for jpeg codec
    */	
	class  tintaJpgImgCodec 	
		: public tintaIImgCodec
	{
		
	public:
		tintaJpgImgCodec(void);
		virtual ~tintaJpgImgCodec(void);

	public:
		
        /*

         Returns attribs[0] = m_uint32 mHeight;
	             attribs[1] = m_uint32 mWidth;
	             attribs[2] = EnImagesTypes mImageType;
	             attribs[3] = ImageBits mChanneltype;
	             attribs[4] = EnImageColorFormat mColorFormat; 
        */
		virtual bool getAttribs( int attribs[5] );
		
        /*
            Allocates data and reads image
        */
		bool readImage(const String &path);			     

		/*
            Assigns the value. Size of pixel depends of mChanneltype  parameter
        */
        bool setPixel(m_uint32 x, m_uint32 y, const m_uint8 *pixel);


        bool writeData( const m_uint8 *data);
		/*
            Saves image data in to the file
        */
		bool saveImage( const String &image_path );

        /*
            Allocates data 
        */
        bool createImage(int width, int height, ImgChannels color_format, ImageBits channelByte);

		/*
            if image selected return value > 0  
        */
		unsigned getWidth() const;

		/*
            if image selected return value > 0  
        */
		unsigned getHeight() const;		
		
        bool getPixel(m_uint32 x_coord, m_uint32 y_coord, m_uint8 *pixel) const;

        bool getPixel(m_uint32 pos, m_uint8 *pixel) const;
		
		tintaJpgImgCodec& operator=(const tintaJpgImgCodec& rVal); 

        virtual ImgChannels getChannels()const;
			
	protected:	

        void clearContext();

        m_uint32    toIndex(int x, int y) const;

        m_uint8 getChannels( ImageBits channelByte );

        m_uint32 		mHeight;
        m_uint32 		mWidth;        
       
        bool 			mOpened;
        m_uint8 * mData;		
	};

}

#endif

