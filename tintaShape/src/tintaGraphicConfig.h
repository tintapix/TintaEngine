/*   Copyright (C) 2011 - 2017 Evdokimov Mikhail
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CONFIG_PARSE_H_
#define _TINTA_CONFIG_PARSE_H_

#include "tintaGraphicaPredefine.h"
#include <tintaTreeConfig/tintaTreeConfig.h>

namespace Graphica {


	class  _CoreExport tintaGraphicConfig {
		
	public:		

		tintaGraphicConfig(void);

		~tintaGraphicConfig(void);		

        virtual bool parseConfig( const String &configFile = "");

	protected:		

		StringVector mShaders;	

		StringVector mTextures;	

        StringVector mMaterials;

        StringVector mMeshes;

		bool         mFullscreen;

        bool         mTitled = true;

        bool         mPerspective = true;

        bool         mConsole = true;

        String       mStartScript;

        int mWidth = 0;

        int mHeight = 0;

        tintaVector2i mPosition = { MAX_INT32, MAX_INT32 };

        String mScreenShot;
        
		TreeConfig::tintaTreeConfig mconf;

		bool getPath( const StringVector &in, String &path, const String &name  )const;

        bool mOclEnabled = true;

        
	public:		
        String mConfigName;

		bool getShaderPath(  String &path, const String &name ) const;

		bool getTexturePath( String &path, const String &name ) const;     

        int width() const;

        int height() const;

        const tintaVector2i* windowPos() const;       

		bool isFullScreen()const;

        bool withTitle()const;

        bool isPerspective() const;

        bool showConsole() const;

        const StringVector &getMaterialPaths()const;    

        const StringVector &getMeshesPaths()const;

        String getScreenShotType() const;

        String startScript() const;

        bool oclEnabled() const;

	};

}


#endif