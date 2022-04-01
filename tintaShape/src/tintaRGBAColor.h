#ifndef _TINTA_RGBA_COLOR_H_
#define _TINTA_RGBA_COLOR_H_

#include "tintaGraphicaPredefine.h"
#include <Math/tintaVector4.h>
#include <tintaException.h>

namespace Graphica {


	class  _CoreExport tintaRGBAColor {

	public:
		// Construction.  The components are intended to be in the interval [0,1].
		// For accessing of colors by array index, the map is 0 = red, 1 = green,
		// 2 = blue, and 3 = alpha.
		tintaRGBAColor ();  // initial values (0,0,0,0)
		tintaRGBAColor (float r_val, float g_val, float b_val, float a_val);
		tintaRGBAColor (const float* vals);
		tintaRGBAColor (const tintaRGBAColor& val);
		tintaRGBAColor (const tintaVector4f& val);

		// member access
		operator const float* () const;
		operator float* ();
		float operator[] (int i) const;
		float& operator[] (int i);
		
		
		tintaRGBAColor& operator= (const tintaRGBAColor& r_val);
		tintaRGBAColor& operator= (const tintaVector4f& r_val);

		
		bool operator== (const tintaRGBAColor& r_val) const;
		bool operator!= (const tintaRGBAColor& r_val) const;
		bool operator<  (const tintaRGBAColor& r_val) const;
		bool operator<= (const tintaRGBAColor& r_val) const;
		bool operator>  (const tintaRGBAColor& r_val) const;
		bool operator>= (const tintaRGBAColor& r_val) const;

		// arithmetic operations
		tintaRGBAColor operator+ (const tintaRGBAColor& r_val) const;
		tintaRGBAColor operator- (const tintaRGBAColor& r_val) const;
		tintaRGBAColor operator* (const tintaRGBAColor& r_val) const;
		tintaRGBAColor operator* (float f_scalar) const;
		friend tintaRGBAColor operator* (float fScalar, const tintaRGBAColor& r_val);

		// arithmetic updates
		tintaRGBAColor& operator+= (const tintaRGBAColor& r_val);
		tintaRGBAColor& operator-= (const tintaRGBAColor& r_val);
		tintaRGBAColor& operator*= (const tintaRGBAColor& r_val);
		tintaRGBAColor& operator*= (float f_scalar);

		// Clamp sets negative values to
		// zero and values larger than one to one.  ScaleByMax assumes the color
		// channels are nonnegative, finds the maximum RGB channel, and divides
		// all RGB channels by that value.  The alpha channel is clamped to [0,1].
		//void clamp ();
		//void scale_by_max ();

		static const tintaRGBAColor BLACK;   // = (0,0,0,1) 
		static const tintaRGBAColor WHITE;   // = (1,1,1,1)
		static const tintaRGBAColor INVALID; // = (-1,-1,-1,-1)

	private:
		// support for comparisons
		int compare	(const tintaRGBAColor& r_val) const;

		float m_color[4];
	};


	inline tintaRGBAColor::operator const float* () const {
		return m_color;
	}
	
	inline tintaRGBAColor::operator float* () {
		return m_color;
	}
	
	inline float tintaRGBAColor::operator[] (int i) const {
		CoreAssert(0 <= i && i <= 3,"0 <= i && i <= 3");
		if (i < 0){
			i = 0;
		}
		else if ( i > 3 ){
			i = 3;
		}

		return m_color[i];
	}
	
	inline float& tintaRGBAColor::operator[] (int i)	{
		CoreAssert(0 <= i && i <= 3,"0 <= i && i <= 3");
		if (i < 0){
			i = 0;
		}
		else if ( i > 3 ){
			i = 3;
		}

		return m_color[i];
	}
	
	
}

#endif