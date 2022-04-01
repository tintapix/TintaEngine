/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ROOT_H_
#define _TINTA_ROOT_H_

#include "tintaGraphicaPredefine.h"

#include <tintaScriptContext.h>
#include <tintaLogger.h>
#include <tintaSingleton.h>
#include <tintaScriptFunc.h>
#include <tintaImageFont.h>

#include "tintaResContainer.h"
#include "tintaMeshParser.h"
#include "tintaCommands.h"

#include <tintaBoxContainer.h>
#include <OCL/tintaIClBase.h>
#include <tintaFont.h>
#include <fcntl.h>
#include <io.h>
#include <stdint.h>

namespace Graphica {

	class tintaMaterial;
    class tintaGLRenderer;
    class Tinta::tintaLogger;
    class tintaSceneManager;

    class _CoreExport tintaConsoleCmd : public tintaConsoleOutput {
    public:
        ~tintaConsoleCmd(){
        }

        void printMsg( const Tinta::String &msg, msgLevel stat ) {

#if CORE_PLATFORM  == CORE_PLATFORM_WIN32      
            tintaWString print = StringUtil::createUTF16String( msg );

            if ( print.length() != 0 ) {
                  std::wcout << print << std::endl;
                  std::wcout.clear();
            }
            else {

                
                stream_out << msg << _M("\n");
                stream_out.clear();
           }
#else
            stream_out << msg << _M("\n");
            //      stream_out.clear();
            stream_out.clear();
#endif
        }

        virtual void setTitle(const String &title) {};

        virtual void clear( ) {
            
            system("cls");
        }

        void registrate(){
            Tinta::tintaLogger::getPtr()->addConsole(this);
        }

        void remove(){
            Tinta::tintaLogger::getPtr()->removeConsole(this);
        }
    };


	class _CoreExport tintaRoot
			: public Singleton<tintaRoot> 
	{
	public:
		tintaRoot(void);
        tintaRoot(const String &configName);
		virtual ~tintaRoot(void);


	public:
        static const String mDummyName;// = _M("0x00000000");       

		bool initialize();

		tintaScriptContext* getContext();

		const tintaGraphicConfig* getConfigurator() const ;
		
        static tintaRoot* getPtr(void)	{
			return mPtr;
		}

		tintaImage        *getImage( const String &imageName );

        bool               delImage(const String &imageName);

        void  delTexFromMaterials(const String &textureName);

        tintaImage        *addImage(const tintaUInt8Image &image, const String &name);

        // temp image 1x1 black
        tintaImage        *addDummyImage();

        bool               addMaterial( SHAREDPTR(tintaMaterial) matPtr);

        void               setRenderer( tintaGLRenderer *renderer );

        void               setSceneManager( tintaSceneManager  *sManager );

        tintaSceneManager *getSceneManager();

        tintaGLRenderer   *getRenderer();

        bool               oclEnabled() const ;

        tintaMaterial     *findMaterial(const String &name);

        TintaMath::tintaRandomV2	  *getRandGen();

        NameGenerator     *getNamegen();

        tintaMaterial     *addMaterial(const String &path , const String &name = _M("") );

        const tintaMeshData *findMesh( const String &name )const;

        tintaCommands       *getCommands(); 
        
        void createContext();        

        bool registerCl(const String &program, const String &kernel);

        Tinta::tintaIClBase* getOCLProgram(const String &program);
       
        void deleteOCL(const String &program = _M(""));

        // Fonts
        virtual m_uint32 addFont(const String &name, float scaleX = 0, float scaleY = 0, const color_type &color = { 0,0,0,0 });

        virtual bool setFontParameter(m_uint32 id, float sizeX, float sizeY, const color_type &color);

        virtual bool setFont(m_uint32 id);

        virtual  tintaFont* getFont(m_uint32 id);

        virtual  m_uint32 cloneFont(m_uint32 src);

        virtual  tintaFont* getFont();

        virtual  bool deleteFont(m_uint32 id);

        virtual  void deleteFonts();        

        // Sampler buffers 

        virtual tintaSamplerBuffer *getSBuffer( const String &name);

        virtual const tintaSamplerBuffer *getSBuffer(const String &name) const ;

        virtual tintaSamplerBuffer * addSBuffer( const String &name, SBufferDraw type );

        virtual void delSBuffer( const String &name );

        // arrays 

        virtual tintaBoxContainer *getBoxes();

	protected:
               

        void addFunctions();

        typedef std::map<String, tintaMeshData> t_MeshData;

        //Tinta::tintaLogger   *mLog = nullptr;

		tintaScriptContext   *mScrptContext = nullptr;

		tintaGraphicConfig   *mConfig = nullptr;	

        tintaGLRenderer      *mRenderer = nullptr;

        tintaSceneManager    *mSceneManager = nullptr;

        tintaConsoleCmd       mCmdOut;

        NameGenerator         mNameGen = "Entity";

        t_MeshData            mMeshData;

        TintaMath::tintaRandomV2	      mRandGen;

        tintaCommands         mCommands;
	
        typedef std::map<String, SHAREDPTR(tintaMaterial)> t_materials_container;

        typedef std::map<String, tintaImage*> images_t;

        images_t mImages;

        typedef std::map<String, tintaSamplerBuffer*> sbuffers_t;

        sbuffers_t mSBuffers;

        t_materials_container       mMaterials;

        String                      mConfigName;

        // for USING_GPUCL 
        tintaGPUExt					*gpuExt = NULL_M;

        tintaClObjectContainer      *mGPUObjs = NULL_M;

        typedef std::vector< tintaIClBase* >		 gpuPrograms_t;

        gpuPrograms_t			     mGPUPrograms;
        //

        // arrays
        typedef std::vector< tintaArrayBoxObjFactory* > compValBoxFactories_t;

        compValBoxFactories_t			 mValBoxFacVec;

        tintaBoxContainer	             mBoxObjects;

        fontFactories_t				     mFontFacroties;

        Tinta::tintaFontContainer		 mFonts;
       
        
	};
}


#endif