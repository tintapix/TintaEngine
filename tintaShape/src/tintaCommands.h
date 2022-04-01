/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_COMMANDS_H_
#define _TINTA_COMMANDS_H_


#include "tintaGraphicaPrecompile.h"

namespace Graphica {

    class tintaCommands
    {
    public:
        tintaCommands();

        virtual ~tintaCommands();

        void push(const StringBasic &command);

        StringBasic popFirst();

        void clear();

        size_t quantity() const;

    private:
        MUTEX_RECURSIVE(mStateMutex);

        std::list<StringBasic> mCommands;

    };

}

#endif