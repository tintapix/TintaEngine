/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_FONT_H_
#define _TINTA_FONT_H_

#include "tintaPredefine.h"
#include "tintaTreeConfig/tintaTreeConfig.h"
#include "tintaUInt8Image.h"
#include "ImageCodecs/tintaPngImgCodec.h"
#include "tintaLogger.h"


namespace Tinta {
 
    
    class tintaFont
        : public AllocatedObjectCommon {
    public:

        tintaFont();

        virtual ~tintaFont(){}

        virtual void setSize(float scaleX, float scaleY);

        virtual void setColor(const color_type &color);

        virtual bool created() const;

        tintaFont& operator = (const tintaFont &rv);
        tintaFont& operator = (const tintaFont &&rv);

        struct tintaSymbol {

            String mSymbol;            

            int vertOffset = 0;                    

            tintaUInt8Image mImage;          
        };


        const tintaSymbol* getSymbolData( const String &symbol ) const;

        bool create( const String &name = _M(""), float scaleX = 1.f, float scaleY = 1.f);


        String getName() const;       

    private:        
        String mName; 

        float mScaleX = 1.f;
        float mScaleY = 1.f;

        color_type  mColor = { 0,0,0,255 };

        typedef std::map< String, tintaSymbol > symbolSet_t;

        symbolSet_t mSymbols;        
    };



    
}


#endif 
