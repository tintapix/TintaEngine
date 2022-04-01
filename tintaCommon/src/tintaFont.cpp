/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaFont.h"


namespace Tinta {

    tintaFont::tintaFont() {

    }
    void tintaFont::setSize(float scaleX, float scaleY) {

        mScaleX = scaleX;
        mScaleY = scaleY;

        if (created()) {

            for (auto s : mSymbols) {

                auto val = &s.second;                

                if (TintaMath::isZero(1.f) && TintaMath::isZero(1.f))
                    continue;
                val->mImage.scaleImage((int)scaleX, (int)scaleY);

                val->vertOffset = (int)((float)val->vertOffset * scaleY);
            }
        }
    }

    void tintaFont::setColor( const color_type &color ) {

        mColor = color;

        if ( created() ) {
            for (auto &s : mSymbols) {

                size_t count = s.second.mImage.getSize();

                for (size_t i = 0; i < count; i++) {
                    color_type color_origin = s.second.mImage.getPixel(i);
                    mColor.setVal( color_type::channel_a, color_origin.getVal( color_type::channel_a ) );

                    s.second.mImage.setPixel(i, mColor);
                    
                }
            }
        }
    }

    struct tintaSymbol {

        int leftUpX = 0;
        int leftUpY = 0;
        int rightDownX = 0;
        int rightDownY = 0;
        int vertOffset = 0;

        String mSymbol;
        tintaUInt8Image mImage;
    };

    const tintaFont::tintaSymbol* tintaFont::getSymbolData( const String &symbol ) const {

        auto rez = mSymbols.find(symbol);
        if ( rez == mSymbols.end() ) {
            return NULL_M;
        }

        return &( rez->second );
    }


    bool tintaFont::created() const {
        return mSymbols.size() > 0 ;
    }


    tintaFont& tintaFont::operator = (const tintaFont &rv) {
        if ( this == &rv )
            return *this;

        mName = rv.mName; 
        mScaleX = rv.mScaleX;
        mScaleY = rv.mScaleY;
        mColor = rv.mColor;
        mSymbols = rv.mSymbols;

        return *this;
    }

    tintaFont& tintaFont::operator = (const tintaFont &&rv) {

        if ( this == &rv )
            return *this;

        mName = std::move(rv.mName);
        mScaleX = std::move(rv.mScaleX);
        mScaleY = std::move(rv.mScaleY);
        mColor = std::move(rv.mColor);
        mSymbols = std::move(rv.mSymbols);

        return *this;
    }

    bool tintaFont::create( const String &name, float scaleX , float scaleY ) {

        if (name.length() > 0) {
            mName = name;
        }

        if ( scaleX != 1.f )
            mScaleX = scaleX;

        if (scaleY != 1.f)
            mScaleY = scaleY;

        mSymbols.clear();

        TreeConfig::tintaTreeConfig localFile;

        localFile.parse( mName );

        String err = localFile.getError();
        if ( err.length() > 0 ) {

            StringStream msg;

            msg << _M("Font create error: ") << mName;
            msg << ". Error: " << err;

            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
            else {
                std::cout << msg.str().c_str() << _M("\n");
            }

            return false;
        }

        String pathImg;
        localFile.getValue(_M("path"), pathImg);

        if (pathImg.length() == 0) {

            StringStream msg;

            msg << _M("Font create error: ") << mName;
            msg << ". Wrong \"path\" parameter: " << pathImg;

            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
            else {
                std::cout << msg.str().c_str() << _M("\n");
            }

            return false;
        }

        tintaUInt8Image image;
        Tinta::tintaIImgCodec* codec = M_NEW Tinta::tintaPngImgCodec();

        UNIQPTRVIRT(Tinta::tintaIImgCodec, codecPtr, codec);
        image.readFromFile(codecPtr.get(), pathImg);

        if (image.getWidth() == 0 || image.getHeight() == 0) {

            StringStream msg;

            msg << _M("Font create error: ") << mName;
            msg << ". Wrong image file: " << pathImg;

            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
            else {
                std::cout << msg.str().c_str() << _M("\n");
            }
            return false;
        }

        size_t symbols = localFile.childQuantity(_M("data"));

        for (size_t i = 0; i < symbols; i++) {
            tintaSymbol symbol;

            StringStream dataSymbol;
            dataSymbol << "data.[" << i << "].[0]";
            localFile.getValue(dataSymbol.str(), symbol.mSymbol);
            auto  quote = symbol.mSymbol.find('"');
            if ( quote != std::string::npos ) {
                symbol.mSymbol.replace(symbol.mSymbol.begin(), symbol.mSymbol.end(), _M("\""));
            }

            dataSymbol.str("");

            dataSymbol << "data.[" << i << "].[1]";
            double val;
            localFile.getValue(dataSymbol.str(), val);
            int leftUpX = (int)val;
            dataSymbol.str("");
            dataSymbol << "data.[" << i << "].[2]";
            localFile.getValue(dataSymbol.str(), val);
            int leftUpY = (int)val;
            dataSymbol.str("");
            dataSymbol << "data.[" << i << "].[3]";
            localFile.getValue(dataSymbol.str(), val);
            int rightDownX = (int)val;
            dataSymbol.str("");
            dataSymbol << "data.[" << i << "].[4]";
            localFile.getValue(dataSymbol.str(), val);
            int rightDownY = (int)val;

            dataSymbol.str("");
            dataSymbol << "data.[" << i << "].[5]";
            localFile.getValue(dataSymbol.str(), val);
            symbol.vertOffset = (int)val;

            if ((rightDownX - leftUpX) <= 0
                || (rightDownY - leftUpY) <= 0) {

                StringStream msg;

                msg << _M("Font create error: ") << mName;
                msg << _M(". Wrong symbol position for symbol: ") << symbol.mSymbol << _M(" must be [4] > [2] parameter and [5]  > [3] ");

                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                else {
                    std::cout << msg.str().c_str() << _M("\n");
                }
                mSymbols.clear();
                return false;
            }

            symbol.mImage.createImage(rightDownX + 1 - leftUpX, rightDownY + 1 - leftUpY);
            symbol.mImage.setChannel(color_type::channel_a, 0 );
           
            int xLoc = 0;
            int yLoc = 0;

            for ( int y = leftUpY; y <= rightDownY; y++, yLoc++ ) {
                xLoc = 0;
                for (int x = leftUpX; x <= rightDownX; x++, xLoc++ ) {

                    color_type pixel = image.getPixel(coord2dI_t(x, y));

                    if ( pixel.getVal(3) > 0 ) {
                        symbol.mImage.setPixel({ xLoc, yLoc }, 
                                        {mColor.getVal(0), mColor.getVal(1), mColor.getVal(2), pixel.getVal(3) });
                    }
                }
            }

            int newWidth = (int)((float)symbol.mImage.getWidth() * mScaleX);
            int newHeight = (int)((float)symbol.mImage.getHeight() * mScaleY);

            symbol.mImage.scaleImage(newWidth, newHeight);
            symbol.vertOffset = (int)((float)symbol.vertOffset * mScaleY);
            

            mSymbols.emplace(symbolSet_t::value_type(symbol.mSymbol, symbol));
        }

        return true;
    }

    String tintaFont::getName() const {
        return mName;
    }

  
}

