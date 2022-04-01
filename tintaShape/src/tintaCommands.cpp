#include "tintaCommands.h"


namespace Graphica {
    tintaCommands::tintaCommands()
    {
    }


    tintaCommands::~tintaCommands()
    {
    }

    void tintaCommands::push(const StringBasic &command) {
        TINTA_LOCK_RECUR_MUTEX(mStateMutex);
        mCommands.push_back(command);
    }

    StringBasic tintaCommands::popFirst() {
        TINTA_LOCK_RECUR_MUTEX(mStateMutex);
        if ( mCommands.size() == 0 )
            return StringBasic();

        StringBasic retVal = mCommands.front();
        mCommands.pop_front();

        return retVal;
    }

    void tintaCommands::clear() {
        TINTA_LOCK_RECUR_MUTEX(mStateMutex);
        mCommands.clear();
    }

    size_t tintaCommands::quantity() const {        
        return mCommands.size();
    }
}