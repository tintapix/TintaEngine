/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef _TINTA_PNG_IMG_CODEC_H_
#define _TINTA_PNG_IMG_CODEC_H_


#include "../tintaIImgCodec.h"

#define PNG_DEBUG 3

#include "libpng/png.h"
#include <vector>

namespace Tinta {

    /*
        Version for png codec
    */
	class  _CoreExport tintaPngImgCodec 	
		: public tintaIImgCodec
	{
		
	public:
		tintaPngImgCodec(void);
		virtual ~tintaPngImgCodec(void);

	public:
		
        /*

            Returns attribs[0] = m_uint32 mHeight;
            attribs[1] = m_uint32 mWidth;
            attribs[2] = EnImagesTypes mImageType;
            attribs[3] = ImageBits mChanneltype;
            attribs[4] = EnImageColorFormat mColorFormat;
        */
		virtual bool getAttribs(int attribs[5]);
		
        /*
        Allocates data and reads image
        */
		bool readImage( const String &image_path);		
		
        	
		
        bool setPixel(m_uint32 xCoord, m_uint32 yCoord, const m_byte *pixel);

        bool writeData(const m_byte *data);

		bool saveImage(const String &image_path);

        /*
            Allocates data
        */
        bool createImage(int width, int height, ImgChannels color_format, ImageBits channelByte);

		
		unsigned getWidth() const ;
		
		unsigned getHeight() const ;
		
		
        bool getPixel(m_uint32 x_coord, m_uint32 y_coord, m_byte *pixel) const;

        bool getPixel(m_uint32 pos, m_byte *pixel) const;
		
		tintaPngImgCodec& operator=(const tintaPngImgCodec& rVal);
		
        virtual ImgChannels getChannels()const;
			
	protected:	
        enum openMode{ enRead,enWrite,noMode};
		// pngLib specific funnction
		ImageBits getCannels(int pngLibVal );
        ImgChannels getColorFormat(int pngLibVal);
        m_byte getColorType(ImgChannels format);

        void deleteStructs();

		void clearContext();

		int 		mHeight;
		int 		mWidth;
		
		bool 			mOpened;
		png_structp 	png_ptr;
		png_infop 		info_ptr;
		

		
		unsigned char color_type;
		unsigned char bit_depth;
			
		// call png_create_write_struct or  png_destroy_read_struct, or nothing
        openMode mMode;

		int number_of_passes;
		png_bytep * row_pointers;
	};

}

#endif

