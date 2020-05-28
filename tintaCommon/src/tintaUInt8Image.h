/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_UINT8_IMAGE_H__
#define _TINTA_UINT8_IMAGE_H__

#include "tintaPredefine.h"
#include "tintaPixel.h"
#include "tintaCoord2.h"
#include "tintaIImgCodec.h"
#include "tintaArray2d.h"
#include "tintaISerialisable.h"
#include "tintaFactoryObj.h"

namespace Tinta {

 typedef tintaPixel24b color_type;   
 typedef       color_type* pixelp_t;
 typedef       const color_type* pixelcp_t;
 typedef const color_type* pixelcp_t;
 //typedef tintaArray2d<m_uint8, sizetype_t> tintaArray2dImage_t;

class _CoreExport tintaUInt8Image 
	: public AllocatedObjectImage	
	,public tintaISerialisable
    ,public tintaINamed
{
					
public:

		tintaUInt8Image (void);

		tintaUInt8Image (const tintaUInt8Image& to_copy);

		tintaUInt8Image (m_uint32 width, m_uint32 height, ImgChannels ch = ImgChannels_4);

        tintaUInt8Image(tintaUInt8Image&& rVal); 

virtual ~tintaUInt8Image(void);

		// returns the position of external point depends of image type(seamless) 
virtual bool calcIndex( const coord2dI_t& in_coord, coord2dI_t& out_coord ) const;

virtual bool validPos(const coord2dI_t& in_coord) const;

virtual bool validPos(m_uint32 index) const;

/// add and draw any other image - color interpolating by alpha 
virtual		void   injectImage(const tintaUInt8Image *imgInjected, int x_offset, int y_offset, m_float32 alphaFactor = 1.f);

//injects other image - alpha interpolating. color assigning
virtual		void   injectImageAlpha(const tintaUInt8Image *imgInjected, int x_offset, int y_offset, m_float32 alphaFactor = 1.f);

virtual	m_uint32	getImageBufferSize() const;	

virtual	color_type  getPixel( const coord2dI_t& coord ) const;	

virtual	color_type  getPixel(m_uint32 index) const;

virtual m_uint32    toIndex(const coord2dI_t& coord) const;

// converts index value to coordinate x,y
virtual	coord2dI_t getCoord( m_uint32 index ) const;	

virtual bool		setPixel(m_uint32 linear_index, const color_type &new_value);

virtual bool		setPixel(const coord2dI_t& coord, const color_type &new_value);

virtual bool		setChannel(const coord2dI_t& coord, int ch, m_uint8 value);

virtual bool		setChannel(m_uint32 linear_index, int ch, m_uint8 value);

virtual m_uint8	getChannel(m_uint32 linear_index, int ch) const;

virtual m_uint8	getChannel(const coord2dI_t& coord, int ch) const;

virtual bool		setPixelAlpha(const coord2dI_t& coord, m_float32 in_value);

virtual bool		setPixelAlpha(m_uint32 index, m_float32 in_value);

virtual bool		setPixelAlpha(const coord2dI_t& coord, m_uint8 in_value);

virtual bool		setPixelAlpha(m_uint32 index, m_uint8 in_value);

virtual const void *getMemPtr();

virtual void       *getMemPtrEx();

virtual size_t      getMemSize() const;
	
		
// fill image with transparency 
virtual void       fillAlpha(m_uint8 trans_factor);

//virtual	void	   fillImageMask(m_ulong32 mask_val);

virtual	bool	   createImage(m_uint32 width, m_uint32 height, ImgChannels ch = ImgChannels_4);

// returns width X height
virtual m_uint32   getImageSize( ) const ;

virtual	void	   fillImage(const color_type &new_value, int ch = -1 );

virtual	void	   fillImage(m_uint8 value, int ch );

virtual		bool   saveToFile(Tinta::tintaIImgCodec* imgFile, const String &file);

virtual		bool   readFromFile(Tinta::tintaIImgCodec* imgFile, const String &image_path);
		
virtual		int    getWidth() const;

virtual		int    getHeight() const;	

virtual     void   scaleImage(int newW, int newH);

virtual     size_t getSize() const;

virtual     int channels() const;
		
	tintaUInt8Image &operator=(const tintaUInt8Image &rval);

    tintaUInt8Image &operator=(const tintaUInt8Image &&rval);

// from tintaISerialisable	

virtual size_t GetBufferSize() const;

virtual size_t packData(m_int8 *data, size_t offset ) const;

virtual size_t		   unpackData(const m_int8 *data, size_t offset);
// tintaINamed
virtual const StringBasic& getName() const;

static const StringBasic str_common;

private:

    typedef std::vector< m_uint8 > array_t;

    array_t mData;

    m_uint32 mImgWidth = 0;
    m_uint32 mImgHeight = 0;

    ImgChannels mChannels = Tinta::ImgChannels_4;

};






}

#endif

