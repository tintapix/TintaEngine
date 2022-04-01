/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"


namespace Graphica {

	tintaAttributes::tintaAttributes(void){

		mChannelQuantity = 0;
		mPositionOffset = -1;
		mPositionChannels = 0;
		mNormalOffset = -1;
		mNormalChannels = 0;				
	}

	tintaAttributes::~tintaAttributes(void){

	}

	int tintaAttributes::getChannelQuantity () const {
		return mChannelQuantity;
	}
	
	int tintaAttributes::getPositionOffset () const	{
		return mPositionOffset;
	}
	
	int tintaAttributes::getPositionChannels () const {
		return mPositionChannels;
	}
	
	bool tintaAttributes::hasPosition () const	{
		return mPositionChannels > 0;
	}
	
		
	int tintaAttributes::getNormalOffset () const {
		return mNormalOffset;
	}
	
	int tintaAttributes::getNormalChannels () const	{
		return mNormalChannels;
	}
	
	bool tintaAttributes::hasNormal () const {
		return mNormalChannels > 0;
	}
	
	int tintaAttributes::getMaxTCoords () const	{
		return (int)mTCoordChannels.size();
	}
	

	 void tintaAttributes::setPositionChannels (int iPositionChannels) {
		 CoreAssert(0 <= iPositionChannels && iPositionChannels <= 4, "0 <= iPositionChannels && iPositionChannels <= 4");

		 mPositionChannels = iPositionChannels;
		 updateOffsets();
	 }	 
		 
	 void tintaAttributes::setNormalChannels (int iNormalChannels) {
		 CoreAssert(0 <= iNormalChannels && iNormalChannels <= 4,"0 <= iNormalChannels && iNormalChannels <= 4");

		 mNormalChannels = iNormalChannels;
		 updateOffsets();
	 }	
	 
	 void tintaAttributes::setTCoordChannels (int unit, int iTCoordChannels) {
		 CoreAssert(0 <= unit && 0 <= iTCoordChannels && iTCoordChannels <= 4,"0 <= iUnit && 0 <= iTCoordChannels && iTCoordChannels <= 4");

		 int iMaxUnits = (int)mTCoordChannels.size();

		 if ( unit >= iMaxUnits )
			 mTCoordChannels.resize(unit +1 );
		 
		 mTCoordChannels[unit] = iTCoordChannels;
		 updateOffsets();
	 }
	 
	 int tintaAttributes::getTCoordOffset (int iUnit) const {
		 if (0 <= iUnit && iUnit < (int)mTCoordOffset.size())
		 {
			 return mTCoordOffset[iUnit];
		 }
		 return -1;
	 }
	 
	 int tintaAttributes::getTCoordChannels (int iUnit) const {
		 if (0 <= iUnit && iUnit < (int)mTCoordChannels.size())
		 {
			 return mTCoordChannels[iUnit];
		 }
		 return 0;
	 }
	 
	 bool tintaAttributes::hasTCoord (int iUnit) const{
		 if (0 <= iUnit && iUnit < (int)mTCoordChannels.size())
		 {
			 return mTCoordChannels[iUnit] > 0;
		 }
		 return false;
	 }
	 
	 
	 bool tintaAttributes::operator== (const tintaAttributes& a) const{
		 
		 if (a.mPositionChannels != mPositionChannels){
			 return false;
		 }
         		 
		 if (a.mNormalChannels != mNormalChannels){
			 return false;
		 }

		 if (a.mTCoordChannels.size() != mTCoordChannels.size()){
			 return false;
		 }

		 for (int i = 0; i < (int)mTCoordChannels.size(); i++){
			 if ( a.mTCoordChannels[i] != mTCoordChannels[i] )			 
				 return false;			 
		 }         		

		 return true;
	 }
	 
	 bool tintaAttributes::operator!= (const tintaAttributes& a) const	 {
		 return !operator==(a);
	 }
	 
	 void tintaAttributes::updateOffsets ()	 {	 
		 mChannelQuantity = 0;
		 mPositionOffset = -1;		 
		 mNormalOffset = -1;	 
		 
		
		 mTCoordOffset.resize( mTCoordChannels.size() );
		 for (int i = 0; i < (int)mTCoordChannels.size(); i++)
		 {
			 mTCoordOffset[i] = -1;
		 }
		
		 if ( mPositionChannels > 0 ) {
			 mPositionOffset = mChannelQuantity;
			 mChannelQuantity += mPositionChannels;
		 }

		 
		 if ( mNormalChannels > 0 )
		 {
			 mNormalOffset = mChannelQuantity;
			 mChannelQuantity += mNormalChannels;
		 }         
	
		 for (int i = 0; i < (int)mTCoordChannels.size(); i++ ) {
			 if (mTCoordChannels[i] > 0) {
				 mTCoordOffset[i] = mChannelQuantity;
				 mChannelQuantity += mTCoordChannels[i];
			 }
		 }         		
	 }

}