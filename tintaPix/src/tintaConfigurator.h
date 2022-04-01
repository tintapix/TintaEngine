/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CONFIGURATOR_H_
#define _TINTA_CONFIGURATOR_H_


#include "tintaPredefine.h"
#include <tintaScriptContext.h>
#include <tintaLogger.h>
#include "tintaPixel.h"
#include <tintaException.h>


namespace Tinta
{

	struct GPUProgramDescrip{

		GPUProgramDescrip(const String &name, const String &file,
		const StringBasic &kernel)
		:mName(name),
		 mFile(file),
		 mKernel(kernel)
		{}

		String mName;
		String mFile;
		StringBasic mKernel;
		GPUProgramDescrip() = default;
		GPUProgramDescrip(const GPUProgramDescrip&) = default;
	};



	typedef  std::vector<GPUProgramDescrip> gpu_programs_t;
	class _CoreExport tintaConfigurator
		: public Tinta::Singleton< tintaConfigurator >
	{

	private:
		
		typedef  std::map<StringBasic,StringBasic>  t_string_map;
		typedef  std::map<StringBasic,StringBasic>::const_iterator  t_string_map_cit;
		typedef  std::map<StringBasic,StringBasic>::iterator  t_string_map_it;

	public:
		tintaConfigurator(void);
		virtual ~tintaConfigurator(void);
		
		bool parserConfig( const String &name );
		
		/*
			Returns user defined gpu script (parameter GPUPrograms )
		*/
		const GPUProgramDescrip *getGpuScripts( m_uint32 &count ) const;

		String getAppName() const;	

		bool   getLocalEnabled() const;

		bool   getGpuEnabled() const;

        int   getGpuPlatform() const;

        int   getGpuDevice() const;

		float  getUnitPrior() const;
        
        String  getStartScript() const;

		m_uint32 getTimeReconnect()const;

        void dumpErr();
	
        const t_string_array &getAllScriptsPaths() const;

        const t_string_array &getCommands() const;

		/*
		returns first script appropriated path
		beginning from the root
		*/
		bool getScriptPath(String &path, const String &script) const;

        String getFontName() const;

        int getFontSize() const;

		/*
			Trying to find path by filename in GPU folders (parameter GPUPrograms_Folders)
		*/
		bool getGpuScriptPath(String &path, const String &fileName) const;

		int getPort() const;

		m_int32 getResetDuration() const;

		bool hasErrors()const;

		String getIpAdress() const;

		bool isLocalEnabled() const;

		static tintaConfigurator* getPtr(void);

	private:

        const StringBasic mGlobalValsKey;

        void validatePath(const String &path);

		Tinta::tintaScriptContext mScriptConfig;
		
		void fillArrayData( const Tinta::t_string_array &srcData, 
							Tinta::t_string_array &distData, const String &rootPath ) const;
		

		Tinta::t_string_array mScriptsPaths;
		Tinta::t_string_array mCLObjectsPaths;

        Tinta::t_string_array mCommands;

		// with global values. Fills after value was requested		
		gpu_programs_t mClObjects;

		String mIpAdress;

		int	 mPort;	

		bool mLocalEnabled;

		bool mGpuEnabled;		

        int mGpuPlatform;

        int mGpuDevice;

		String mAppName;

		float mUnitPrior;		

		m_uint32 mTimeReconnect;

		String mConfigPath;

        // file or buffer executing after start
        String mStartScript;

        String mFontName;

        int mFontSize;

        static const char * const strTexSpringConfigFile; // = "config.lua";
        static const char * const strSectionScriptFolders;// = "Scripts_Folders";
        static const char * const strSectionCommands; //= "Commands";
        static const char * const strSectionGPUFolders;// = "GPUPrograms_Folders";
        static const char * strGPUScripts;// = "GPUPrograms.path";
        static const char * const strSectionIpAdress;// = "Ip_Adress";
        static const char * const strStartScript;// = "Start_script";
        static const char * const strConsoleFontName;// = "Console_FontName";
        static const char * const strConsoleFontSize;// = "Console_FontSize";
        static const char * const strSectionPort;// = "Port";
        static const char * const strLocalEnabled;// d = "Local_Enabled";
        static const char * const strGpuEnabled;// = "Gpu_Enabled";
        static const char * const strGpuPlatform; //"Gpu_Platform"
        static const char * const strGpuDevice; //"Gpu_Device"
        static const char * const strLogName;// = "Log_Name";
        static const char * const strAppName;// = "App_Name";
        static const char * const strUnitPrior;// = "Unit_Priority";
        // unit reconnect time in mlSec
        static const char * const strReconnect;// = "Unit_Reconnect_Time";

        // time in mlSec to send not sent packets after error
        static const char * const strReConnectTime;// = "Unit_Reconnect_Time";

	


	};

}

#endif
