/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaJpgImgCodec.h"
#include "../tintaCommon.h"




namespace Tinta
{

 tintaJpgImgCodec::tintaJpgImgCodec(void):
     tintaIImgCodec(ImgChannels_3, enJPGType, enMaxChannel),
 mHeight(0),
 mWidth(0),  
 mOpened(false),
 mData(NULL_M){
}

tintaJpgImgCodec::~tintaJpgImgCodec(void){	
    clearContext();
}

/// if image selected return value > 0  
unsigned tintaJpgImgCodec::getWidth() const{	
	return mWidth;
}

/// if image selected return value > 0  
unsigned tintaJpgImgCodec::getHeight() const{
	return mHeight;
}


/// prepare image for read
bool tintaJpgImgCodec::readImage( const String &path ){
    
	clearContext(); 

    mImageType = enJPGType;
    
    m_ulong32 data_size;     // length of the file
    //int channels;               
    
    unsigned char * rowptr[1];    // pointer to an array    
    struct jpeg_decompress_struct info; 
    struct jpeg_error_mgr err;              

    //const unsigned  hesrdeSize = 8;
    //char header[hesrdeSize];    // 8 is the maximum size that can be checked
    
    FILE *fp(NULL_M);
    openFile(&fp, path, String(_M("rb")));

    // on WIN OS this version makes trouble
    //openFile( &fp, path, String(_M("rb")) );

    if( !fp ) {       
        return false;
    }

    info.err = jpeg_std_error(& err);     
    jpeg_create_decompress(& info);   //fills info structure
    jpeg_stdio_src(&info, fp);    
    jpeg_read_header(&info, TRUE);   
    jpeg_start_decompress(&info);    
    
    mWidth = info.output_width;
    mHeight = info.output_height;
    mChannels = (ImgChannels)info.num_components;
    
    if (mChannels == ImgChannels_4)
         mChanneltype = en32bits;   
    else if (mChannels == ImgChannels_3)
        mChanneltype = en24bits;   
    else
        mChanneltype = enMaxChannel;

    data_size = mWidth * mHeight * mChannels; 
    mData = ( unsigned char * )malloc( data_size );

    while ( info.output_scanline < info.output_height ){
        // Enable jpeg_read_scanlines() to fill our jdata array
        rowptr[0] = (unsigned char *)mData + mChannels * info.output_width * info.output_scanline; 
        jpeg_read_scanlines(&info, rowptr, 1);
    }    

    jpeg_finish_decompress(&info);

    fclose(fp);    

	mOpened = true;
    return mOpened;
}


m_uint32    tintaJpgImgCodec::toIndex( int x, int y ) const {
    if( !mOpened || mWidth == 0 || mHeight == 0 )
        return 0;
    return y * mWidth + x;
}

bool tintaJpgImgCodec::getPixel(m_uint32 x, m_uint32 y, m_byte * pixel) const {

    if (!mOpened || x >= (m_uint32)mWidth || y >= (m_uint32)mHeight)
 		return false;

    m_uint32 pos = toIndex( x * mChannels, y * mChannels );
    
    getPixel(pos, pixel);

	return true;
}	

bool tintaJpgImgCodec::getPixel(m_uint32 pos, m_byte *pixel) const {
    for (m_uint8 ch = 0; ch < mChannels; ch++, pos++) {
        pixel[ch] = mData[pos];
    }
    return true;
}

ImgChannels tintaJpgImgCodec::getChannels()const {
    return mChannels;
}

bool tintaJpgImgCodec::getAttribs(int attribs[5]){

	if( !mOpened )
		return false;

	attribs[0] = mHeight;
	attribs[1] = mWidth;
	attribs[2] = (int)mImageType;
	attribs[3] = (int)mChanneltype;
    attribs[4] = (int)mChannels;
	return true;
}

bool tintaJpgImgCodec::createImage(int width, int height, ImgChannels color_format, ImageBits channelByte){
	
	clearContext(); 


	mImageType   = enJPGType;
    mChannels    = color_format;
	mChanneltype = channelByte;
	mHeight = height;
	mWidth  = width;
      
    // if we creating image, using only 3 channels 
    mChannels = ImgChannels_3;

    if( mChannels == 0 )
        return false;

    size_t s = mWidth * mHeight * mChannels; 
    mData = ( unsigned char * )malloc( s ); 
	
	mOpened = true;
    return mOpened;
}


m_uint8 tintaJpgImgCodec::getChannels( ImageBits channelByte ) {

    if( mChanneltype == en32bits )
        return 4;  
    else if(mChanneltype == en24bits)
        return 3;

    assert(false);
    return 0;
}

tintaJpgImgCodec& tintaJpgImgCodec::operator=(const tintaJpgImgCodec& rVal)
{
	if ( this == &rVal )
		return *this;

	mHeight = rVal.mHeight;
	mWidth = rVal.mWidth;
	mImageType = rVal.mImageType;
	mChanneltype = rVal.mChanneltype;
    mChannels = rVal.mChannels;

    createImage(mWidth ,mHeight, mChannels, mChanneltype);

	return *this;
}

///set value to the pixel. Size of pixel depends of channelByte  parameter
bool tintaJpgImgCodec::setPixel(m_uint32 x, m_uint32 y, const m_byte *pixel) {

	if( !mOpened )
		return false;

	if( x >= (unsigned)mWidth ||  y >= (unsigned)mHeight )
		return false;   


    m_uint32 pos = toIndex( x * mChannels, y * mChannels );
    for (unsigned ch = 0; ch < (unsigned)mChannels; ch++, pos++){
         mData[pos] = pixel[ ch ];
    }

	return true;

}
bool tintaJpgImgCodec::writeData( const m_byte *data ) {
    if ( !mOpened )
        return false;
    m_uint32 size = mHeight * mWidth * (int)mChannels;
    mlMemcpy( mData, size, data, size);   

    return true;
}
void tintaJpgImgCodec::clearContext() {

    if( mData )
        free( mData );
    mOpened = false;
}

/// saving image to the file
bool tintaJpgImgCodec::saveImage( const String &path ){
	

	if( !mOpened )
		return false;
	
    FILE *fp(NULL_M);
    openFile(&fp, path, String(_M("w+b")));
	
	if ( !fp )
		return false;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
	int quality = 100;
    JSAMPROW row_pointer[1];       

    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);
    
    cinfo.image_width = mWidth;  
    cinfo.image_height = mHeight;
    cinfo.input_components = (int)mChannels;
    cinfo.num_components = (int)mChannels;
    cinfo.in_color_space = JCS_RGB; //color_space;
	cinfo.dct_method = JDCT_FLOAT;    
    jpeg_set_defaults( &cinfo );    
	jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress( &cinfo, TRUE );
    
    while( cinfo.next_scanline < cinfo.image_height )
    {
        row_pointer[0] = &mData[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
        jpeg_write_scanlines( &cinfo, row_pointer, 1 );
    }
    
    jpeg_finish_compress( &cinfo );
    jpeg_destroy_compress( &cinfo );
    fclose( fp );	

	return true;
}

}
