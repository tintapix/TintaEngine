/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ATTRIBUTES_H_
#define _TINTA_ATTRIBUTES_H_

#include "tintaGraphicaPredefine.h"

namespace Graphica {


	class _CoreExport tintaAttributes {

	public:
		tintaAttributes ();
		~tintaAttributes ();		

		void setPositionChannels ( int channels );		
		void setNormalChannels (int channels);
		void setTCoordChannels (int unit, int channels);

		
		int getChannelQuantity () const;
		
		int getPositionOffset () const;
		int getPositionChannels () const;
		bool hasPosition () const;
		
		int getNormalOffset () const;
		int getNormalChannels () const;
		bool hasNormal () const;

		int getMaxTCoords () const;
		int getTCoordOffset (int iUnit) const;
		int getTCoordChannels (int iUnit) const;
		bool hasTCoord (int iUnit) const;        		
        		
		bool operator== (const tintaAttributes& attr) const;
		bool operator!= (const tintaAttributes& attr) const;

	private:
		void updateOffsets ();
		
		int mChannelQuantity;

		// positions
		int mPositionOffset;
		int mPositionChannels;		

		// normals
		int mNormalOffset;
		int mNormalChannels;       	
		
		// texture coordinates
		std::vector<int> mTCoordOffset;
		std::vector<int> mTCoordChannels;

	};

}

#endif
