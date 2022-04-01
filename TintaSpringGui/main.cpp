#include "tintaGraphicaPrecompile.h"
#include <ImageCodecs/tintaPngImgCodec.h>

#include "tintaSpringGui.h"
#include <tintaCommon.h>

#include <utf8.h>


int main( int args, char* apcArgument[] ){
    
    
    stream_out << _M("Tintashape library version ") <<
#include "tintashapever";
        ;
    stream_out << "\n";

    StringVector argSet;

    for (int i = 0; i < args; i++) {
        argSet.push_back(apcArgument[i]);
    }


    String config = _M("window.cfg");
    //	String configName;
    if ( argSet.size() > 0 && StringUtil::getFileExt(argSet.at(argSet.size() - 1)) == _M("cfg") )
        config = argSet.at(argSet.size() - 1);

    Tinta::tintaLogger   *mLog = M_NEW Tinta::tintaLogger();

    String path_log = Tinta::getRootPath();
    StringUtil::StrStreamType logName;
   
    logName << _M("s_") << getProcessId();
   
    logName << _M(".log");

    Tinta::StringUtil::addLeaf(path_log, logName.str());

    mLog->createLog(path_log);

    tintaApp *app = M_NEW tintaSpringGui(config);
    
    tintaWinApp::mWinApp = (tintaWinApp*)app;
   
    app->Main( args, apcArgument );        

    M_DELETE app;

    if (mLog) {
        M_DELETE(mLog);
        mLog = NULL_M;
    }

    return 0;
}