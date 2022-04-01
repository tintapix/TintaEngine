/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */
#ifndef TINTA_NO_INTERACT
    #define TINTA_NO_INTERACT
#endif

//#include <propkeydef.h>
#include <tintaTexSpringMain.h>
#include <iostream>
#include <cctype>
#if CORE_PLATFORM  == CORE_PLATFORM_WIN32
#include <conio.h>
#endif

#include <OCL/tintaGPUExt.h>
#include <tintaUInt8Image.h>
#include <tintaTreeConfig/tintaTreeConfig.h>
#include <signal.h>
#include <tintaSignalHandler.h>
//#include <tintaPixVer.h>
#if CORE_PLATFORM  == CORE_PLATFORM_WIN32
#include "tintaImgWindow.h"
#endif

#ifdef USING_GPUCL 



#include <CL\cl.h>


#define _WIN32_WINNT 0x0501
#define WINVER 0x0501

#if CORE_ARCH_TYPE == CORE_ARCHITECTURE_64
//#pragma comment(lib, "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v6.0\\lib\\x64\\OpenCL.lib")
#endif
#if CORE_ARCH_TYPE == CORE_ARCHITECTURE_32
//ragma	comment(lib, "C:\Program Files (x86)\NVIDIA GPU Computing Toolkit\CUDA\v3.2\lib\Win32\OpenCL.lib")
#endif


#endif /*USING_GPUCL*/

using namespace Tinta;
using namespace std;

static const char_m *strTexSpringConfigFileW  = _M("config.lua");

#define BUFF_SIZE 1024

//#define TEST_
#ifdef TEST_

#include <dirent.h>



struct mystream
{
    std::stringstream stream;
public:
    template<typename T>
    istream& operator>>(T& x)
    {
       
        return stream.str().empty()?cin>>x:stream>>x;
    }
    template<typename T>
    ostream& operator<<(const T& x)
    {
        return stream<<x;
    }
} myin;


//#include <testClass.h>
int main( int argc, char *argv[] )
{
	getFileProp  p = { false, _M(""), -1 };
	int q{ 0 };
	StringVector r = getFiles_(_M("/home/mixa/foto"), p );
	for( String i  : r ){
		stream_out<<i<<_M("\n");
	}
}



//tintaMatrix33_ mm;
#else


#define KEY_ENTER     13
#define KEY_ARR_UPPDOWN   224
	#define KEY_SUBARR_UPP   72
	#define KEY_SUBARR_DOWN  80
#define KEY_BACKSPACE 8
#define KEY_ESCAPE    27



class molyConsoleCmd : public tintaConsoleOutput {
public:
	~molyConsoleCmd(){		
	}

    virtual void printMsg( const Tinta::String &msg, msgLevel stat ) {    
#if CORE_PLATFORM  == CORE_PLATFORM_WIN32       
        tintaWString print = StringUtil::createUTF16String(msg);
        if (print.length() != 0) {
            std::wcout << StringUtil::createUTF16String(msg) << std::endl;
            std::wcout.clear();
        }
        else {
            stream_out << msg << _M("\n");
            stream_out.clear();
        }
#else
        stream_out << msg << _M("\n");
        stream_out.clear();
  //      stream_out.clear();
#endif
        
	}

    virtual void clear() {
        
        system("cls");
        
    }

	void registrate(){
        if ( Tinta::tintaLogger::getPtr() )
		    Tinta::tintaLogger::getPtr()->addConsole(this);
	}

	void remove(){
        if(Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->removeConsole(this);
	}
    virtual void setTitle(const String &title) {
        //SetConsoleTitle(_M("TexSpring Console"));
    }
};

struct Command {
    int mUnit;
    String mCommand;
};

Command parseCommand(const String &buffer) {
    
    
   Command rez;

   rez.mUnit = 0;
   rez.mCommand = buffer;

   if ( buffer.size() != 0 ) {
       
       
       int it = 0;
       String unitStr;
       char ch;
       while ( it < buffer.size()  ) {           
           
           ch = buffer.at( it );

           if ( std::isdigit(static_cast<unsigned char>(ch) ) ) {           
               unitStr.push_back(ch);
               it++;
           }
           else
               break;
          
       }

       if ( unitStr.size() > 0 ) {
           rez.mUnit = std::stoi(unitStr);
           rez.mCommand = buffer.substr(it);
       }
   }
   return rez;
}

//typedef 



#include <Math/tintaCatRomSpline.h>    

int main( int argc, char *argv[] )
{      

    setlocale(LC_CTYPE, "");

    THREAD_TYPE                  mRenderWindow;


    stream_out << _M("Tinta pix library version ")<<
    #include "tintapixver";
        ;
    stream_out << "\n";

    //stream_out << _M("Tinta pix library version ")<< v <<  _M("\n");

 	StringBasic         buffer;	
	
	molyConsoleCmd      cmdOut;
    Tinta::tintaLogger   *log = M_NEW Tinta::tintaLogger();
    tintaScriptContext* context = NEW_T(tintaScriptContext)();
    tintaGPUExt* gpuExt = NEW_T(tintaGPUExt)();

    tintaSignalHandler signalHandler;
    signalHandler.setupExitSignalHandler();

    tintaTexSpringMain *console = NEW_T( tintaTexSpringMain )( );
	bool isServer = true;		
	
	console->setServMode( isServer );

    bool inited = console->initialize(strTexSpringConfigFileW, &cmdOut, context);
        
#if CORE_PLATFORM  == CORE_PLATFORM_WIN32
    //locale loc("rus_rus.866");
    //system("color 0A");
    CONSOLE_FONT_INFOEX info = { 0 };
    info.cbSize = sizeof(info);

    const tintaConfigurator * config = Tinta::tintaConfigurator::getPtr();
    if (config) {

        info.dwFontSize.Y = (short)config->getFontSize(); // leave X as zero
        info.FontWeight = FW_NORMAL;

        String name = config->getFontName();
        std::wstring wName(name.begin(), name.end());
        if( name.size() > 0  )
            wcscpy( info.FaceName, wName.c_str() );// L"Lucida Console");

        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
    }

#endif //CORE_PLATFORM  == CORE_PLATFORM_WIN32

#if 0
    //Get a console handle
    HWND myconsole = GetConsoleWindow();
    //Get a handle to device context
    HDC mydc = GetDC(myconsole);

    int pixel = 0;

    //Choose any color
    COLORREF COLOR = RGB(255, 0, 255);

    //Draw pixels
    for (double i = 0; i < (int)(3.14 * 4); i += 0.05)
    {
        SetPixel(mydc, pixel, (int)(50 + 25 * cos(i)), COLOR);
        pixel += 1;
    }

    ReleaseDC(myconsole, mydc);
#endif
    //print_other("starting other\n");



    if ( inited ){
        if ( isServer ){
           
            String command; 

            while ( !signalHandler.gotExitSignal() ){                   

                //size_t unitId{ 0 };
                
                char buff[BUFF_SIZE];
                stream_in.getline(buff, BUFF_SIZE);
                
                command = buff;

                Command c = parseCommand(command);

                if (c.mCommand == _M("help") ){
                    stream_out << _M("(") << THREAD_CURRENT_ID << _M(")") << _M("\n") << helpCommand << _M("\n"); //<<_M("\r")<< flush;                 
                }                
                else {

                    
                    if ( StringUtil::getFileExt(c.mCommand) == "lua") {
                        StringStream full;
                        full << "main.f(\"" << c.mCommand << "\")";
                        c.mCommand = full.str();
                    }

                    tintaTexSpringMain::getPtr()->executeCommand(c.mUnit, c.mCommand, tintaExecutingTask::enLocalTask);
                    tintaTexSpringMain::getPtr()->addCommand(c.mCommand);
                }
                c = Command();
            }            
		} 		
	}

	cmdOut.remove();

    DELETE_T(console, tintaTexSpringMain);
    M_DELETE log;    
    DELETE_T( gpuExt, tintaGPUExt );
    DELETE_T( context, tintaScriptContext );
	


	return 0;

}

#endif
