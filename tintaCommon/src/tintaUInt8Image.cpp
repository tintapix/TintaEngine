/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaUInt8Image.h"
#include "tintaException.h"
#include "tintaCommon.h"

namespace Tinta
{
    const StringBasic tintaUInt8Image::str_common = "basic";

    tintaUInt8Image::tintaUInt8Image(void) {
    }

    tintaUInt8Image::tintaUInt8Image(const tintaUInt8Image& to_copy) {
        *this = to_copy;
    }

    tintaUInt8Image::tintaUInt8Image(m_uint32 w, m_uint32 h, ImgChannels ch)
        :mChannels(ch) {       

        createImage(w, h, mChannels );
    }

    tintaUInt8Image::tintaUInt8Image(tintaUInt8Image&& rVal)  {
        *this = std::move(rVal);
    }

    tintaUInt8Image::~tintaUInt8Image(void) {
    }

    // bool tintaUInt8Image::isMaskMode() const	{
    // 	return mSelectMaskValue != 0x0;
    // }
    m_uint32    tintaUInt8Image::toIndex(const coord2dI_t& coord) const {              
        return coord.mValY * mImgWidth + coord.mValX;
    }

    color_type tintaUInt8Image::getPixel(const coord2dI_t& coord) const {

        if (!validPos(coord))
            return color_type();

        int index = toIndex(coord);

        color_type pix = { 0,0,0,255 };
        int begin = index * mChannels;
        for ( auto i = 0; i < mChannels; i++ ) {
            pix.setVal((m_uint8)i, mData.at(begin + i ) );
        }

        return pix;
    }

    color_type  tintaUInt8Image::getPixel(m_uint32 index) const {

        if (index >= getSize())
            return  color_type();

        color_type pix = { 0,0,0,255 };
        size_t begin = index * mChannels;
        for (auto i = 0; i < mChannels; i++) {
            pix.setVal((m_uint8)i, mData.at(begin + i));
        }

        return pix;
    }

    bool tintaUInt8Image::calcIndex(const coord2dI_t& in_coord, coord2dI_t& out_coord) const {

        if (!validPos(in_coord))
            return false;
        // actually do nothing for this image type
        out_coord = in_coord;
        return true;
    }

    size_t  tintaUInt8Image::getSize() const {
        return mData.size() / mChannels;
    }

    ImgChannels tintaUInt8Image::channels() const {
        return  mChannels;
    }


    bool tintaUInt8Image::validPos(const coord2dI_t& in_coord) const {
        if (in_coord.mValX < 0 || (m_uint32)in_coord.mValX >= mImgWidth
                               || in_coord.mValY < 0 || (m_uint32)in_coord.mValY >= mImgHeight)
            return false;

        return true;
    }

    bool tintaUInt8Image::validPos(m_uint32 index) const {
        return index <  getSize();
    }

    m_uint32 tintaUInt8Image::getImageBufferSize() const {
        return mData.size() * mChannels;
    }

    bool tintaUInt8Image::setPixel(const coord2dI_t& coord, const color_type &new_value) {
        if (!validPos(coord))
            return false;
        int index = toIndex(coord);

        //mXYRGBCanvas.at(index) = new_value; 
        setPixel(index, new_value);

        return true;
    }



    void tintaUInt8Image::setPixel(const color_type &new_value) {
        size_t count = getSize();
        
        for (size_t i = 0; i < count; i++) {
            setChannel( i, 0, new_value.getVal( color_type::channel_r ) );
            setChannel( i, 1, new_value.getVal( color_type::channel_g ) );
            setChannel( i, 2, new_value.getVal( color_type::channel_b ) );
        }
        
    }

    bool tintaUInt8Image::setPixel(m_uint32 linear_index, const color_type &new_value) {

        if (!validPos(linear_index))
            return false;

        size_t begin = linear_index * mChannels;
        for (auto i = 0; i < mChannels; i++) {
            mData.at(begin + i) = new_value.getVal((m_uint8)i);
        }
        return true;
    }

    bool tintaUInt8Image::setChannel(const coord2dI_t& coord, int ch, m_uint8 value) {

        int index = toIndex(coord);

        if (!validPos(index) || mChannels <= ch )
            return false;

        size_t begin = index * mChannels;
        mData.at(begin + ch) = value;

        return true;
    }

    bool tintaUInt8Image::setChannel(m_uint32 linear_index, int ch, m_uint8 value) {

        if (!validPos(linear_index) || mChannels <= ch )
            return false;
        
        mData.at( linear_index * mChannels + ch ) = value;

        return true;
    }

    void tintaUInt8Image::setChannel(int ch, m_uint8 value) {
        size_t count = getSize();
        for (size_t i = 0; i < count; i++) {
            setChannel(i, ch, value);
        }
    }


    bool tintaUInt8Image::createImage(m_uint32 w, m_uint32 h, ImgChannels ch)
    {
        if ((w + h) == 0)
            return false;            

        mChannels = ch;
        mData.resize(w*h*mChannels);

        mImgWidth = w;
        mImgHeight = h;

        return true;

    }

    m_uint32   tintaUInt8Image::getImageSize() const {
        return mImgWidth * mImgHeight;
    }

    bool tintaUInt8Image::readFromFile(Tinta::tintaIImgCodec* imgFile, const String &path)
    {
        CoreAssert(imgFile, "imgFile==NULL");

        if (!imgFile)
            return false;


        if (imgFile->readImage(path)) {

            int attribs[5];
            imgFile->getAttribs(attribs);

            if ( createImage(imgFile->getWidth(), imgFile->getHeight(), imgFile->getChannels() ) ) {

                m_uint32 wg = mImgWidth;
                m_uint32 hg = mImgHeight;

                m_uint8 vals[4] = { 0, 0, 0, 255 };
                for (m_uint32 h = 0; h < hg; h++) {
                    for (m_uint32 w = 0; w < wg; w++) {
                        imgFile->getPixel(w, h, vals);
                        setPixel(coord2dI_t(w, h), color_type({ vals[0], vals[1], vals[2], vals[3] }));
                    }
                }

                return true;
            }
        }

        return false;
    }
    bool tintaUInt8Image::saveToFile(Tinta::tintaIImgCodec* imgFile, const String &file) {

        CoreAssert(imgFile, "imgFile==NULL");
        if (!imgFile)
            return false;

        m_uint32 wg = mImgWidth;
        m_uint32 hg = mImgHeight;

        if (imgFile->createImage(wg, hg, mChannels, (tintaIImgCodec::ImageBits)(mChannels * 8))) {
            for (m_uint32 y = 0; y < hg; y++) {
                for (m_uint32 x = 0; x < wg; x++) {

                  
                    color_type pSrc = getPixel(coord2dI_t(x, y));

                    m_uint8 pixel[4] = {0,0,0,255};

                    pixel[0] = pSrc.getVal(color_type::channel_r);
                    pixel[1] = pSrc.getVal(color_type::channel_g);
                    pixel[2] = pSrc.getVal(color_type::channel_b);
                    pixel[3] = pSrc.getVal(color_type::channel_a);

                    bool rez = imgFile->setPixel(x, y, pixel);
                    CoreAssert(rez, "rez==false");
                }
            }           

            return imgFile->saveImage(file);
        }
        return false;
    }


    void tintaUInt8Image::mixImage(const tintaUInt8Image *imgSecond, int x_offset, int y_offset, m_float32 factor)
    {

        assert(imgSecond);
        CoreAssert(imgSecond, "imgSecond==NULL");
        if (!imgSecond) return;


        m_uint32 index_src = 0;
        m_uint32 quantity = imgSecond->getImageSize();
        //const pixelp_t pixPtr = image_src->getPixelFirst( quantity );
        for (; index_src < quantity; index_src++)
        {
            coord2dI_t coord = imgSecond->getCoord(index_src);
            color_type pix = imgSecond->getPixel(coord);

            coord.mValX += x_offset;
            coord.mValY += y_offset;

            color_type cNew = pix;
            color_type cOrigin = getPixel(coord);

            m_uint8 a = pix.getVal(color_type::channel_a);

            /// aditional factor
            m_float32 af = byteToFloat(a) * factor;

            cNew.setVal( color_type::channel_r, (m_uint8)(cOrigin.getVal(color_type::channel_r) * (1 - af) + cNew.getVal(color_type::channel_r) * af));
            cNew.setVal( color_type::channel_g, (m_uint8)(cOrigin.getVal(color_type::channel_g) * (1 - af) + cNew.getVal(color_type::channel_g) * af));
            cNew.setVal( color_type::channel_b, (m_uint8)(cOrigin.getVal(color_type::channel_b) * (1 - af) + cNew.getVal(color_type::channel_b) * af));
            cNew.setVal( color_type::channel_a, (m_uint8)(cOrigin.getVal(color_type::channel_a) * (1 - af) + cNew.getVal(color_type::channel_a) * af));

            setPixel(coord, cNew);
        }

    }


    void tintaUInt8Image::mixChannel(const tintaUInt8Image *imgSecond, int x_offset, int y_offset, m_uint8 channel, m_float32 factor)
    {

        CoreAssert(imgSecond, "imgSecond==NULL");
        if (!imgSecond) return;

        m_uint32 index_src = 0;
        m_uint32 quantity = imgSecond->getImageSize();
        for (; index_src < quantity; index_src++)
        {
            coord2dI_t coord = imgSecond->getCoord(index_src);
            color_type pix = imgSecond->getPixel(coord);

            coord.mValX += x_offset;
            coord.mValY += y_offset;

            color_type color_new = pix;
            color_type color_origin = getPixel(coord);

            /// additional factor
            m_float32 alphaf = byteToFloat(color_origin.getVal(channel)) * factor;

            color_new.setVal(channel, (m_uint8)((color_new.getVal(channel) * (1 - alphaf) + color_new.getVal(channel) * alphaf) * factor));

            setPixel(coord, color_new);
        }

    }



    coord2dI_t tintaUInt8Image::getCoord(m_uint32 index) const {

        int y_pos = (int)(index / mImgWidth);
        int x_pos = index - mImgWidth * y_pos;

        return coord2dI_t(x_pos, y_pos);
    }

    bool tintaUInt8Image::setPixelAlpha(const coord2dI_t& coord, m_float32 in_value)
    {
        if (validPos(coord))
            setChannel(coord, 3, floatToByte(in_value));

        return true;
    }

    bool tintaUInt8Image::setPixelAlpha(m_uint32 index, m_float32 in_value) {

        //pixelp_t pixel = tintaUInt8Image::getPixelPtr(index);
        if (validPos(index))
            setChannel(index, 3, floatToByte(in_value));

        return true;
    }

    bool tintaUInt8Image::setPixelAlpha(const coord2dI_t& coord, m_uint8 in_value)
    {
        return setChannel(coord, 3, in_value);
        //pixelp_t pixel = tintaUInt8Image::getPixelPtr(index);
        //pixel->setVal(3, in_value);
        //m_x_transp_factor_canvas.at(index) = in_value;

    }

    bool tintaUInt8Image::setPixelAlpha(m_uint32 index, m_uint8 in_value) {

        return setChannel(index, 3, in_value);

    }


    size_t      tintaUInt8Image::getMemSize() const {
        return mData.size();
    }
    const void *tintaUInt8Image::getMemPtr() const {
        if (mData.size() == 0)
            return NULL;

        return (void*)&mData[0];
    }

    void *tintaUInt8Image::getMemPtrEx() {
        if (mData.size() == 0)
            return NULL;

        return (void*)&mData[0];
    }


    const m_uint8 *tintaUInt8Image::getData() {
        if (mData.size() == 0)
            return NULL;

        return&mData[0];
    }

    m_uint8 *tintaUInt8Image::getDataEx() {
        if (mData.size() == 0)
            return NULL;

        return &mData[0];
    }

    int tintaUInt8Image::getWidth() const
    {
        return (int)mImgWidth;
    }
    int tintaUInt8Image::getHeight() const
    {
        return (int)mImgHeight;
    }

    void    tintaUInt8Image::scaleImage( int newW, int newH ) {

        if ( newW < 1 || newH < 1 )
            return;

        CoreAssert(newW > 0 && newH > 0,"newW <= 0 && newH <= 0");

        tintaUInt8Image old(*this);

        float factorX = (float)(old.getWidth() - 1) / (float)(newW - 1);
        float factorY = (float)(old.getHeight() - 1) / (float)(newH - 1);


        createImage(newW, newH);
        //(mExtrem.mXMin, mExtrem.mXMin + newW - 1,
          //  mExtrem.mYMin, mExtrem.mYMin + newH - 1, false ) ) {
            for ( int i = 0; i < newH; i++ ) {
                for ( int k = 0; k < newW; k++ ) {
                    
                    color_type pixel = old.getPixel(coord2dI_t((m_uint32)std::floor(k * factorX), (m_uint32)std::floor(i * factorY)));                                                                       
                    setPixel({ k, i }, pixel);
                }
            }
        
        //scaleBox( newW, newH );
    }

    m_uint8		tintaUInt8Image::getChannel( m_uint32 linear_index, int ch ) const {
        color_type pixel = getPixel(linear_index);
        return pixel.getVal( (m_uint8)ch );
    }

    m_uint8 tintaUInt8Image::getChannel(const coord2dI_t& coord, int ch) const {
        int index = toIndex(coord);

        if (!validPos(index) || mChannels <= ch)
            return 0;

        return getChannel( index, ch );
    }

    size_t tintaUInt8Image::GetBufferSize() const {

        //mXSize; + mYSize + mExtrem + mData 
        return  sizeof(sizetype_t) + sizeof(sizetype_t) ;// getDataSize();
    }

    size_t tintaUInt8Image::packData(m_int8 *data, size_t offset) const {

        size_t iCurOffset = offset;

        size_t elements = 0;
        CoreAssert(false, "tintaUInt8Image::packData TODO");
        /*iCurOffset = WriteToBuffer<sizetype_t>(data, iCurOffset, width());
        iCurOffset = WriteToBuffer<sizetype_t>(data, iCurOffset, height());

       
        const m_uint8* dataPtr = getPtr( elements );
        //iCurOffset = WriteToBuffer<sizetype_t>( data, iCurOffset,   sizeData );	
        mlMemcpy(data + iCurOffset, elements * sizeof(m_uint8), dataPtr, elements * sizeof(m_uint8));
        */
        return iCurOffset + elements * sizeof(m_uint8);

    }

    size_t  tintaUInt8Image::unpackData(const m_int8 *data, size_t offset)	{


        //sizetype_t width = 0;
        //sizetype_t height = 0;
        size_t elements = 0;

        size_t iCurOffset = offset;
        CoreAssert(false, "tintaUInt8Image::unpackData TODO");
        /*
        iCurOffset = ReadFromBuffer< sizetype_t >(data, iCurOffset, width);
        iCurOffset = ReadFromBuffer< sizetype_t >(data, iCurOffset, height);



        if (!createImage(width, height))
            return iCurOffset;

        size_t elements = 0;
        m_uint8* dataPtr = getPtr(elements);
        //size_t s = sizeof(color_type);
        mlMemcpy(dataPtr, elements * sizeof(m_uint8), data + iCurOffset, elements * sizeof(m_uint8));
        */
        return iCurOffset + elements * sizeof(m_uint8);
    }

    const StringBasic& tintaUInt8Image::getName() const {
        static StringBasic name = tintaUInt8Image::str_common;
        return name;        
    }


tintaUInt8Image &tintaUInt8Image::operator=(const tintaUInt8Image &rval)
{
	if(this == &rval)
		return *this;

    mData = rval.mData;
    mImgWidth = rval.mImgWidth;
    mImgHeight = rval.mImgHeight;
    mChannels = rval.mChannels;
	return *this;
}

tintaUInt8Image &tintaUInt8Image::operator=(const tintaUInt8Image &&rval) {
    if (this == &rval)
        return *this;    

    mData = std::move(rval.mData);
    mImgWidth = std::move(rval.mImgWidth);
    mImgHeight = std::move(rval.mImgHeight);
    mChannels = std::move(rval.mChannels);

    return *this;
}

	




}
