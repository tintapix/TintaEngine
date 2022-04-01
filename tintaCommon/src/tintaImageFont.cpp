/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaImageFont.h"


namespace Tinta {

    String tintaImageFont::str_font = _M("image_font");
    
    const StringBasic& tintaImageFontFactory::getName( void ) const {
        return tintaImageFont::str_font;
    }

}

