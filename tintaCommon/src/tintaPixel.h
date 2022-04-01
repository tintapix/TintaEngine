/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_PIXEL_H_
#define _TINTA_PIXEL_H_

#include "tintaPredefine.h"

namespace Tinta
{
 


    
template < class T, ImgChannels Tch >
struct tintaPixel {
    
public:

    tintaPixel() {     
        static_assert( Tch >= ImgChannels_1 && Tch <= ImgChannels_max, "Wrong channels quantity!" );
        std::fill(mChannels, mChannels + (m_uint32)Tch, T());
	}

    tintaPixel(std::initializer_list<T> l)
        {
        static_assert(Tch >= ImgChannels_1 && Tch <= ImgChannels_max, "Wrong channels quantity!");
        if (l.size() <= Tch){
            std::copy(l.begin(), l.end(), mChannels);
        }
    }

    tintaPixel(const tintaPixel &new_value){
        static_assert(Tch >= ImgChannels_1 && Tch <= ImgChannels_max, "Wrong channels quantity!");
        *this = new_value;
    }

    virtual  ~tintaPixel(){        
    }


    void setVal(m_uint8 channel, T value){
        assert(channel <= (m_uint8)Tch);
        mChannels[channel] = value;

    }

    T getVal(m_uint8 channel) const {
        assert(channel <= (m_uint8)Tch);
        return mChannels[channel];
    }

    T *getValPtr(m_uint8 channel){
        assert(channel <= (m_uint8)Tch);
        return &mChannels[channel];
    }

	
    bool operator == (const tintaPixel& rVal) const {

         for (auto i = 0; i < Tch; i++){
             if ( !(mChannels[i] == rVal.mChannels[i] ) )
                 return false;
         }
         return true;        			
	}
    bool operator != (const tintaPixel& rVal) const {
		return !(*this == rVal);

	}

     tintaPixel& operator = (const tintaPixel& rVal) 	{
		if( this == &rVal )
			return *this;
        for (auto i = 0; i < Tch; i++){
            mChannels[i] = rVal.mChannels[i];
        }
 		
 		return *this;
 	}
     const tintaPixel&  operator*(float scalar){
		
         for (auto i = 0; i < Tch; i++){
             mChannels[i] = (T)(mChannels[i] * scalar);
         }

		return *this;
	}

     const tintaPixel&  operator+(const tintaPixel& rVal){
         for (auto i = 0; i < Tch; i++){
             mChannels[i] += rVal.mChannels[i];
         }
		return *this;
	}	

    
protected:
    T mChannels[Tch];
public:
    static const m_uint8 channel_r = 0;
    static const m_uint8 channel_g = 1;
    static const m_uint8 channel_b = 2;
    static const m_uint8 channel_a = 3;

    

};

    typedef tintaPixel<m_uint8, ImgChannels_4> tintaPixel32b;
    // with luminance
    typedef tintaPixel<m_uint8, ImgChannels_5> tintaPixel40b;

}

#endif
