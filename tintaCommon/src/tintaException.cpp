/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaException.h"
#include "tintaLogger.h"
#include "tintaCommon.h"

namespace Tinta {


    tintaException::tintaException(const String& descr, const char* src,
        const char* file, long lin) :
        mLine(lin),
        mDescription(descr),
        mSource(toWideChar(src)),
        mFile(toWideChar(file))
    {
        print();

    }

    tintaException::tintaException(const String& descr, const char* src) :
        mLine(0),
        mDescription(descr),
        mSource(toWideChar(src))
    {
        print();
    }

    tintaException::tintaException(const tintaException& rhs)
        : mLine(rhs.mLine),
        mDescription(rhs.mDescription),
        mSource(rhs.mSource),
        mFile(rhs.mFile)
    {
    }

    void tintaException::operator = (const tintaException& rhs)
    {
        mDescription = rhs.mDescription;
        mSource = rhs.mSource;
        mFile = rhs.mFile;
        mLine = rhs.mLine;
    }

    const String& tintaException::getDescr(void) const
    {
        if (mFullDesc.empty()){

            StringStream desc;

            desc << " EXCEPTION: " << mDescription << " in " << mSource;
            if (mLine > 0){
                desc << " at " << mFile << " (line " << mLine << ")";
            }

            mFullDesc = desc.str();
        }

        return mFullDesc;
    }


    void tintaException::print() {

        //String descr(  this->getDescr() );
        if (tintaLogger::getPtr())
            tintaLogger::getPtr()->logMsg(mDescription, msg_critical, true);
        else {
            StringStream name;
            name << getRootPath() << _M("/") << getProcessId() << _M("_exception.log");

#if CORE_PLATFORM == CORE_PLATFORM_WIN32
            stream_fout f( name.str().c_str() );
#else
            stream_fout f( name.str().c_str() );

#endif
            f << mDescription << std::endl;
            // Flush stcmdream to ensure it is written (in case of a crash, we need log to be up to date)
            f.flush();
            f.close();
        }

    }

    const char* tintaException::what() const throw()
    {
        return getDescr().c_str();
    }

}