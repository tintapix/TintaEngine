#include "tintaUInt8ImageSeamless.h"
#include <tintaCommon.h>

namespace Tinta
{


    const char * const tintaUInt8ImageSeamless::str_image_seamless = "seamless";

	tintaUInt8ImageSeamless::tintaUInt8ImageSeamless(void){
	}

	tintaUInt8ImageSeamless::~tintaUInt8ImageSeamless(void){	
	}

	tintaUInt8ImageSeamless::tintaUInt8ImageSeamless( size_t width, size_t height )
		:tintaUInt8Image( width, height ){
	}
    tintaUInt8ImageSeamless::tintaUInt8ImageSeamless(tintaUInt8ImageSeamless&& rVal) 
        : tintaUInt8Image(std::move(rVal)){
    }

	bool tintaUInt8ImageSeamless::calcIndex( const coord2dI_t& in_coord, coord2dI_t& out_coord ) const {

		int x_val = in_coord.mValX;
		int y_val = in_coord.mValY;

		out_coord = in_coord;
		m_uint32 wg = getWidth();
		m_uint32 hg = getHeight();

		if(  ( unsigned )x_val >= wg )
			out_coord.mValX = in_coord.mValX % wg;
		
		
		if(  x_val < 0 ) { 
			x_val = TintaMath::abs( x_val );
			if( ( unsigned )x_val > wg )
				//x_val = x_val % mWidth;
				out_coord.mValX  = x_val % wg;
			else			
				out_coord.mValX  = wg - TintaMath::abs(x_val);
		}

		if(  ( unsigned )y_val >= hg )
			out_coord.mValY = in_coord.mValY % hg;	

		if(  y_val < 0 ){

			y_val = TintaMath::abs(y_val);

			if( ( unsigned )y_val > hg )
				//y_val = y_val % mHeight;
				out_coord.mValY  = y_val % hg;
			else			
				out_coord.mValY  = hg - TintaMath::abs(y_val);
		}
		return true;
	}
	
 	color_type tintaUInt8ImageSeamless::getPixel( const coord2dI_t& coord ) const {
 
 		coord2dI_t coord_rez;
 		calcIndex(coord, coord_rez);
 		
		return tintaUInt8Image::getPixel(coord_rez);
 	}
	/*color_type *tintaUInt8ImageSeamless::getPixelPtrFast( const coord2dI_t& coord ){

		coord2dI_t coord_rez;
		calcIndex(coord, coord_rez);
		
		return tintaUInt8Image::getPixelPtrFast(coord_rez);

	}*/
	bool tintaUInt8ImageSeamless::setPixel(const coord2dI_t& coord, const color_type &new_value){

		coord2dI_t coord_rez;
		calcIndex(coord, coord_rez);
		return tintaUInt8Image::setPixel(coord_rez, new_value);
	}

    const StringBasic& tintaUInt8ImageSeamless::getName() const {
        static StringBasic name = tintaUInt8ImageSeamless::str_image_seamless;
        return name;        
    }

	const StringBasic& tintaSeamlessImageFactory::getName( void ) const {
        static StringBasic name = tintaUInt8ImageSeamless::str_image_seamless;
		return name;	
	}
    tintaUInt8ImageSeamless &tintaUInt8ImageSeamless::operator=(const tintaUInt8ImageSeamless &&rval) {
        if (this == &rval)
            return *this;

        tintaUInt8Image::operator=(rval);
        return *this;
    }

    tintaUInt8ImageSeamless &tintaUInt8ImageSeamless::operator=(const tintaUInt8ImageSeamless &rval) {
        if (this == &rval)
            return *this;

        tintaUInt8Image::operator=((tintaUInt8Image&&)rval);
        return *this;
    }


}
