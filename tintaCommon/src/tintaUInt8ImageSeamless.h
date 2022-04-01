/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_UINT8_IMAGE_SEAMLESS_H_
#define _TINTA_UINT8_IMAGE_SEAMLESS_H_


#include "tintaUInt8Image.h"
#include <tintaImageFactory.h>

namespace Tinta {

	/// Seamleas image
	class _CoreExport tintaUInt8ImageSeamless
		: public tintaUInt8Image
	{
		public :
			tintaUInt8ImageSeamless (void);			
			tintaUInt8ImageSeamless (size_t width, size_t height);
            tintaUInt8ImageSeamless(tintaUInt8ImageSeamless&& rVal);

			virtual ~tintaUInt8ImageSeamless(void);
		public:
			// overloaded - without border test could shoot troubles when coordinate out of range
			virtual	color_type  getPixel( const coord2dI_t& coord ) const;

			// overloaded - without border test could shoot troubles when coordinate out of range
            /*virtual	color_type *getPixelPtrFast( const coord2dI_t& coord );
            */

			// overloaded - without border test could shoot troubles when coordinate out of range
			virtual	bool		setPixel(const coord2dI_t& coord, const color_type &new_value);

            tintaUInt8ImageSeamless(const tintaUInt8ImageSeamless& to_copy) : tintaUInt8Image{ to_copy }{};

            virtual const StringBasic& getName() const;

            static const char * const str_image_seamless;

            tintaUInt8ImageSeamless &operator=(const tintaUInt8ImageSeamless &&rval);

            tintaUInt8ImageSeamless &operator=(const tintaUInt8ImageSeamless &rval);

	protected:
		
		bool calcIndex(const coord2dI_t& in_coord, coord2dI_t& out_coord) const;
	};

	class _CoreExport tintaSeamlessImageFactory: 
		public tintaImageFactory {

	public:
		tintaSeamlessImageFactory(){}
		virtual ~tintaSeamlessImageFactory() {}

		const StringBasic& getName(void) const;

		tintaUInt8ImageSeamless *createInstance(  ) const {
			return M_NEW tintaUInt8ImageSeamless( );
		}	

		void releaseInstance(tintaUInt8Image * obj) {
			M_DELETE obj;
		}
		//virtual void releaseInstance( tintaCompGeom* obj) { M_DELETE obj; }
	};
}


#endif
