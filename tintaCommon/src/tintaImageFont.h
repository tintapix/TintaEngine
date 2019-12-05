/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov
tintapix.com
tintapix@gmail.com  */

#ifndef _TINTA_IMAGE_FONT_H_
#define _TINTA_IMAGE_FONT_H_


#include "tintaFont.h"
#include "tintaFontFactory.h"
#include "tintaObjContainer.h"

namespace Tinta {


    

    class _CoreExport tintaImageFont : public tintaFont {
    public:
        tintaImageFont() {}
        virtual ~tintaImageFont() {}

        static String str_font;
    };



    class _CoreExport tintaImageFontFactory :
        public tintaFontFactory {

    public:
        tintaImageFontFactory() {}
        virtual ~tintaImageFontFactory() {}

        const StringBasic& getName(void) const;

        tintaImageFont *createInstance() const {
            return M_NEW tintaImageFont();
        }

        void releaseInstance(tintaFont * obj) {
            M_DELETE obj;
        }
    };

    typedef tintaObjContainer< tintaFont, tintaFontFactory > tintaFontContainer;
    typedef std::vector<tintaImageFontFactory*> fontFactories_t;
}


#endif 
