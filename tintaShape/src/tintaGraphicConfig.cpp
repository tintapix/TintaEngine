/*   Copyright (C) 2011 - 2017 Evdokimov Mikhail
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaGraphicConfig.h"
#include <tintaLogger.h>
#include <tintaException.h>

using namespace Tinta;


//String Graphica::tintaGraphicConfig::mConfigName = _M("shape.mfg");



namespace Graphica {


    tintaGraphicConfig::tintaGraphicConfig(void)
        :mFullscreen(true)         
	{		      
	}

	tintaGraphicConfig::~tintaGraphicConfig(void){
	}

	bool tintaGraphicConfig::parseConfig(const String &configFile)	{
//		bool result = false;

        if ( configFile.length() > 0 )
            mConfigName = configFile;

        String root = Tinta::getRootPath();
        Tinta::StringUtil::addLeaf(root, mConfigName);

        if ( !Tinta::isPathValid(root) ) {
            StringStream msg;
#if TINTA_LOCAL ==  TINTA_EN            
            msg << _M("Can`t find configuration file: ");
            msg << mConfigName;
#elif TINTA_LOCAL == TINTA_RU            
            msg << u8"Не найден конфигурационный файл: ";
            msg << mConfigName;
#endif
            EXCEPTION(msg.str());
        }
       

        if ( !mconf.parse( mConfigName ) )
        {
                
            String err = mconf.getError( );
            if ( err.length() > 0 ) {
                StringUtil::StrStreamType msg;
                msg << "File error: " << mConfigName << " Error: "<< err;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
            }

            return false;

        }
        //tintaStringValVector shaders
        mconf.getValues(_M("Shader_paths.*"), mShaders);
        mconf.getValues(_M("Texture_paths.*"), mTextures);
        mconf.getValues(_M("Material_paths.*"), mMaterials);
        mconf.getValues(_M("Meshes_paths.*"), mMeshes);
        mconf.getValue(_M("is_fullscreen"), mFullscreen);
        mconf.getValue(_M("is_titled"), mTitled);
        mconf.getValue(_M("is_perspective"), mPerspective);
        mconf.getValue(_M("show_console"), mConsole);
        mconf.getValue(_M("start_script"), mStartScript);

        double dWidth, dHeight;
        mconf.getValue(_M("width"), dWidth);
        mconf.getValue(_M("height"), dHeight);

        double posx, posy;
        bool posValid = false;
        posValid = mconf.getValue(_M("window_pos_x"), posx);
        posValid = posValid && mconf.getValue(_M("window_pos_y"), posy);
        if (posValid) {
            mPosition = { (int)posx,(int)posy };
        }

        mconf.getValue(_M("ocl_enabled"), mOclEnabled );

        mWidth = (int)dWidth;
        mHeight = (int)dHeight;

        mconf.getValue(_M("screenshot_type"), mScreenShot);   
        if ( mWidth <= 0 || mHeight <= 0 ){
               
            StringUtil::StrStreamType msg;
            msg << "Wrong \"width\" or \"height\" parameters in configuration file";
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                
            return false;
        }
		
		return true;
	}

    String tintaGraphicConfig::startScript() const {
        return mStartScript;
    }


    bool tintaGraphicConfig::oclEnabled() const {
        return mOclEnabled;
    }

	bool tintaGraphicConfig::getPath(const StringVector &in, String &path, const String &name )const {

		//bool result = true;

		//bool result = getPath( path, name, m_shaders );
		String str_to_test;
		String name_ = name;
		str_to_test = Tinta::getRootPath();
		String root = str_to_test; //Tinta:.getVal(0)ootPath();
        Tinta::StringUtil::addLeaf(str_to_test, name_);
		// root directory
		if( Tinta::isPathValid(str_to_test) ){
			path = str_to_test;
			
		}
		else {
			auto it = in.begin();
			for( ; it != in.end(); it++ ){
				str_to_test = root;
                Tinta::StringUtil::addLeaf(str_to_test, *it);
                Tinta::StringUtil::addLeaf(str_to_test, name_);
				if( Tinta::isPathValid(str_to_test) ) {
					path = str_to_test;					
				}
			}
		}
		if( path.length() == 0 )	{
			StringStream msg;
#if TINTA_LOCAL == TINTA_RU  
            msg << u8"Не найден файл: " << name;
#elif TINTA_LOCAL ==  TINTA_EN	
			msg << _M("File not found: ") << name;	
#endif
            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg( msg.str() , msg_error);

            return false;
			//EXCEPTION( msg.str() );
		}
			
		
		return true;
	
	}

	bool tintaGraphicConfig::getShaderPath( String &path, const String &name ) const {		
        return getPath(mShaders, path, name);
	}	
	bool tintaGraphicConfig::getTexturePath( String &path, const String &name ) const {
        return getPath(mTextures, path, name);
	}

    int tintaGraphicConfig::width() const {
        return mWidth;
    }

    int tintaGraphicConfig::height() const {
        return mHeight;
    }
   
    const tintaVector2i* tintaGraphicConfig::windowPos() const {

        if (mPosition.mx == MAX_INT32 || mPosition.my == MAX_INT32)
            return NULL_M;

        return &mPosition;
    }

	bool tintaGraphicConfig::isFullScreen()const{
        return mFullscreen;
	}

    bool tintaGraphicConfig::withTitle()const {
        return mTitled;
    }

    bool tintaGraphicConfig::isPerspective()const {
        return mPerspective;
    }

    bool tintaGraphicConfig::showConsole() const {
        return mConsole;
    }

    const StringVector &tintaGraphicConfig::getMaterialPaths() const {        
        return mMaterials;
    }   

    const StringVector &tintaGraphicConfig::getMeshesPaths()const {
        return mMeshes;
    }

    String tintaGraphicConfig::getScreenShotType() const {
        return mScreenShot;
    }


}