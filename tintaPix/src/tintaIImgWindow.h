/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov 
tintapix.com
tintapix@gmail.com  */

#ifndef _TINTA_IIMG_WINDOW_H_
#define _TINTA_IIMG_WINDOW_H_


#include <tintaPixel.h>
#include <Math/tintaVector2.h>

namespace Tinta
{

    class tintaUInt8Image;
    
    struct tintaIImgWindow {             

        virtual void create() = 0;

        virtual void updateData(m_uint32 key) = 0;

        virtual void show(bool bShow) = 0;

        virtual bool shown() const = 0;        

        virtual void addData(m_uint32 key, const tintaUInt8Image * data) = 0;

        virtual void deleteData( m_uint32 key = ZERO_ID ) = 0;

        virtual void showImage(m_uint32 key) = 0;

        virtual void setData( m_uint32 key, m_uint32 index,  const tintaPixel24b *color ) = 0;

        virtual void setData(m_uint32 key, const tintaUInt8Image * data) = 0;

        virtual void setData( m_uint32 key, m_uint32 x, m_uint32 y, const tintaPixel24b *color) = 0;

        virtual bool keyState( int key , bool &bValid ) const  = 0;

        virtual void addHandler(const String &type, const String &execute ) = 0;

        virtual void delHandlers() = 0;
        
        virtual tintaVector2i mouseWinPos() const = 0;

        virtual tintaVector2i mouseImgPos() const = 0;

        virtual ~tintaIImgWindow() {};

    }; 

}

#endif
