/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaPngImgCodec.h"
#include "tintaCommon.h"

//#include <fstream>      // std::ifstream

//#include <assert.h>

namespace Tinta
{




tintaPngImgCodec::tintaPngImgCodec(void)
    :tintaIImgCodec(ImgChannels_max, enPNGType, enMaxChannel),
	mHeight(0), 
	mWidth(0),	
	mOpened(false),
	png_ptr( NULL_M ),
    info_ptr(NULL_M),
    mMode(noMode),
    row_pointers(NULL_M)
{

}

void tintaPngImgCodec::clearContext() {

	/* cleanup heap allocation */
	if(mHeight == 0 || mWidth == 0)
		return ;

	if(row_pointers){
		for (int y=0; y<mHeight; y++)
			free(row_pointers[y]);
		free(row_pointers);	
		row_pointers = 0;
	}
    deleteStructs();
}

void tintaPngImgCodec::deleteStructs() {
    if ( mMode != noMode && png_ptr && info_ptr ){
        if (mMode == enWrite)
            png_destroy_write_struct(&png_ptr, &info_ptr);
        else if (mMode == enRead)
            png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);
        png_ptr = NULL_M;
        info_ptr = NULL_M;
    }

    mMode = noMode;
}

tintaPngImgCodec::~tintaPngImgCodec(void){
	clearContext();
}


/// if image selected return value > 0  
unsigned tintaPngImgCodec::getWidth() const {	
	return mWidth;
}

/// if image selected return value > 0  
unsigned tintaPngImgCodec::getHeight() const {
	return mHeight;
}


/// prepare image for read
bool tintaPngImgCodec::readImage( const String &image_path ){
    clearContext();

	mOpened = false;

	mImageType = enPNGType;

	const unsigned  hesrdeSize = 8;

	char header[hesrdeSize];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp(NULL_M); 
	openFile( &fp, image_path, String(_M("rb")) );

	if ( !fp )
		return false;

    FileHandler fh{fp};

    fread(header, 1, hesrdeSize, fh.mFp);
	
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if ( !png_ptr )
		return false;

	info_ptr = png_create_info_struct( png_ptr );

	if ( !info_ptr )
		return false;

	if ( setjmp( png_jmpbuf(png_ptr) ) )
		return false;

    png_init_io(png_ptr, fh.mFp);

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	mWidth = png_get_image_width(png_ptr, info_ptr);

	mHeight = png_get_image_height(png_ptr, info_ptr);

	color_type = png_get_color_type(png_ptr, info_ptr);

	mChanneltype = getCannels(color_type);
    mChannels    = getColorFormat(color_type);

	//const unsigned colorChannels = mChanneltype / 8;
	
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);

	png_read_update_info(png_ptr, info_ptr);	

	if ( setjmp( png_jmpbuf( png_ptr ) ) )
		return false;
	
	row_pointers = ( png_bytep* ) malloc( sizeof( png_bytep ) * mHeight );
	for ( int y = 0; y < mHeight; y++ ){		
		row_pointers[y] = ( png_byte* ) malloc( png_get_rowbytes( png_ptr,info_ptr ) );
	}

	png_read_image( png_ptr, row_pointers ); 	
    	

	mOpened = true;
    mMode = enRead;
    return mOpened;
}


bool tintaPngImgCodec::getPixel(m_uint32 x_coord, m_uint32 y_coord, m_uint8 * pixel) const {

	if( !mOpened )
		return false;
    if (x_coord >= (m_uint32)mWidth || y_coord >= (m_uint32)mHeight)
		return false;
    const m_uint32 colorChannels = (unsigned)mChannels;

	png_byte* row = row_pointers[y_coord];
	png_byte* ptr = &( row[x_coord * colorChannels] );

    for (m_uint32 ch = 0; ch < colorChannels; ch++)
		 pixel[ ch ] = ptr[ch];						

	return true;
}	

bool tintaPngImgCodec::getPixel(m_uint32 pos, m_uint8 *pixel) const {
    
    png_byte* ptr = row_pointers[pos];
    const m_uint32 colorChannels = (unsigned)mChannels;
    for (m_uint32 ch = 0; ch < colorChannels; ch++)
        pixel[ch] = ptr[ch];

    return true;
}

ImgChannels tintaPngImgCodec::getChannels()const {
    return mChannels;
}

//bool tintaPngImgCodec::getAttrib( EnImagesTypes &image_type, m_uint32 &height, m_uint32 &width, EnImageColorFormat &color_format, ImageBits channelByte ){
bool tintaPngImgCodec::getAttribs(int attribs[5]){

	if( !mOpened )
		return false;

	attribs[0] = mHeight;
	attribs[1] = mWidth;
	attribs[2] = (int)mImageType;
	attribs[3] = (int)mChanneltype;
    attribs[4] = (int)mChannels;

	return true;
}

tintaIImgCodec::ImageBits tintaPngImgCodec::getCannels(int pngLibVal ){

	if(PNG_COLOR_MASK_ALPHA & pngLibVal){
		return en32bits;
	}
	
	return en24bits;
	

	
}
m_uint8 tintaPngImgCodec::getColorType(ImgChannels format){

	m_uint8 rez = PNG_COLOR_MASK_COLOR;
    if (format == ImgChannels_4)
		     rez |= PNG_COLOR_MASK_ALPHA;
	return rez;

}
ImgChannels tintaPngImgCodec::getColorFormat(int pngLibVal){

	if(PNG_COLOR_MASK_ALPHA & pngLibVal){
        return ImgChannels_4;
	}
	
    return ImgChannels_3;
	
}


bool tintaPngImgCodec::createImage(int width, int height, ImgChannels color_format, ImageBits channelByte){
	
    if ( height <= 0 || width <= 0 )
        return false;

	mOpened = false; 
	clearContext();

	mImageType = enPNGType;
    mChannels = color_format;
    // png always 4 channels
    //mChannels = ImgChannels_4;

	mChanneltype = channelByte;
	bit_depth = 8;
	mHeight = height;
	mWidth  = width;
	color_type = getColorType(mChannels);
	
    
    

    const unsigned colorChannels = (unsigned)mChannels;

	/*row_pointers = ( png_bytep* ) malloc( colorChannels * mHeight );
	for ( int y = 0; y < mHeight; y++ )
		row_pointers[y] = ( png_byte* ) malloc( colorChannels * mWidth );
        */
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * mHeight);
    for (int y = 0; y < mHeight; y++) {
        row_pointers[y] = (png_byte*)malloc(colorChannels * mWidth);
    }
	
	mOpened = true;
    return mOpened;

}

tintaPngImgCodec& tintaPngImgCodec::operator=(const tintaPngImgCodec& rVal)
{
	if ( this == &rVal )
		return *this;

	mHeight = rVal.mHeight;
	mWidth = rVal.mWidth;
	mImageType = rVal.mImageType;
	mChanneltype = rVal.mChanneltype;
    mChannels = rVal.mChannels;

    createImage(mWidth, mHeight, mChannels, mChanneltype);

	return *this;
}

///set value to the pixel. Size of pixel depends of channelByte  parameter
bool tintaPngImgCodec::setPixel(unsigned xCoord, unsigned yCoord, const m_uint8 *pixel) {

	if( !mOpened )
		return false;
	if( xCoord >= (unsigned)mWidth ||  yCoord >= (unsigned)mHeight )
		return false;   

    const m_uint32 colorChannels = (unsigned)mChannels;

	png_byte* row = row_pointers[yCoord];
	png_byte* ptr = &(row[xCoord * colorChannels]);	

    for (m_uint32 ch = 0; ch < colorChannels; ch++)
		ptr[ch] = pixel[ ch ];

	return true;

}

bool tintaPngImgCodec::writeData(const m_uint8 *data) {

    if (!mOpened)
        return false;
    m_uint32 size = mHeight * mWidth * (int)mChannels;
    mlMemcpy(row_pointers, size, data, size);
    return true;
}

/// saving image to the file
bool tintaPngImgCodec::saveImage( const String &image_path ){
	
	if(!mOpened)
		return false;

	
	FILE *fp;// = fopen(image_path.c_str(), "wb");
	openFile( &fp,image_path.c_str(), String( _M("w+b") ) );
	if ( !fp )
		return false;

    FileHandler fh{ fp };

    deleteStructs();

	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr)
		return false;
    mMode = enWrite;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		return false;

	if (setjmp(png_jmpbuf(png_ptr)))
		return false;

    png_init_io(png_ptr, fh.mFp);


	/* write header */ 
	if (setjmp(png_jmpbuf(png_ptr)))
		return false;

	png_set_IHDR(png_ptr, info_ptr, mWidth, mHeight,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	
    if (setjmp(png_jmpbuf(png_ptr))){
        //fclose(fp);
        return false;
    }

	png_write_image(png_ptr, row_pointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		return false;

	png_write_end(png_ptr, NULL);

	clearContext();

	

	//fclose(fp);

	return true;
}

}
