//#include <QCoreApplication>

#include <iostream>
#include <tintaConfig.h>
#include <MemTracker.h>
#include <tintaCommon.h>

using namespace std;
using namespace Tinta;



String getFileExt(const String &path){

    String rez;
    unsigned found = path.find_last_of(_M("."));

    if ( found != String::npos ){
        rez.insert( rez.begin(), path.begin() + found + 1, path.end() );
    }
    return std::move(rez);

}


int main(int argc, char *argv[])
{
    getFileProp propFiles(true, "html", 0);


    StringVector rez = getFiles("/home/mixa", propFiles, NULL);

    for (auto f : rez){
        std::cout << f;
    }


    return 0;
}
