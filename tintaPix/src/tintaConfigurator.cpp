/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaConfigurator.h"
#include <tintaCommon.h>


using namespace Tinta;


const char * const tintaConfigurator::strTexSpringConfigFile = "config.lua";
const char * const tintaConfigurator::strSectionScriptFolders = "Scripts_Folders";
const char * const tintaConfigurator::strSectionGPUFolders = "GPUPrograms_Folders";
const StringBasic tintaConfigurator::strGPUScripts = "GPUPrograms.path";
const char * const tintaConfigurator::strSectionIpAdress = "Ip_Adress";
const char * const tintaConfigurator::strStartScript = "Start_script";
const char * const tintaConfigurator::strSectionPort = "Port";
const char * const tintaConfigurator::strLocalEnabled = "Local_Enabled";
const char * const tintaConfigurator::strGpuEnabled = "Gpu_Enabled";
const char * const tintaConfigurator::strGpuPlatform = "Gpu_Platform";
const char * const tintaConfigurator::strGpuDevice = "Gpu_Device";
const char * const tintaConfigurator::strLogName = "Log_Name";
const char * const tintaConfigurator::strAppName = "App_Name";
const char * const tintaConfigurator::strUnitPrior = "Unit_Priority";

// time in mlSec to send not sent packets after error
const char * const tintaConfigurator::strReConnectTime = "Unit_Reconnect_Time";


tintaConfigurator::tintaConfigurator(void)
	: mLocalEnabled(true)
	, mGpuEnabled(true)	
    , mGpuPlatform(-1)
    , mGpuDevice(-1)
	, mAppName( _M("Main") )
	, mUnitPrior(0.f)	
	, mTimeReconnect(0)
	, mGlobalValsKey("GlobalVals")
{
	
	
	//parserConfig();	
}

tintaConfigurator::~tintaConfigurator(void)
{
	//CORE_DELETE m_logMgr;
}



void tintaConfigurator::dumpErr() {
	size_t error_count;
	const String *errors  = mScriptConfig.getErrors( error_count );
    if( error_count > 0 )
        Tinta::tintaLogger::getPtr()->logMsg(_M("Some errors occured while parsing config file."));

	for( unsigned i = 0; i < error_count; i++,errors++ )		
        Tinta::tintaLogger::getPtr()->logMsg(*errors, msg_error);
	
	mScriptConfig.resetErrors();
}
void tintaConfigurator::validatePath(const String &path) {
    if( !Tinta::isPathValid( path ) || path.length() == 0 || !isFile( path ) )
        EXCEPTION( "Can`t find configuration file." );
}

bool tintaConfigurator::hasErrors() const {
    return mScriptConfig.hasErrors();
}
void tintaConfigurator::fillArrayData( const Tinta::t_string_array &srcData, 
									  Tinta::t_string_array &distData, const String &rootPath ) const {

    Tinta::t_string_array::const_iterator it = srcData.begin();
    for (; it != srcData.end(); it++){
		   String full_path = rootPath;
		   if( Tinta::isPathValid( *it  ) ){
			   distData.push_back( *it );
		   }
		   else{
               Tinta::StringUtil::addLeaf(full_path, (*it));
			   if( isPathValid( full_path ) )
				   distData.push_back(full_path);
		   }
	   }

}
bool tintaConfigurator::parserConfig( const String &name )
{
	String config_path = Tinta::getRootPath();
	const String root_path = config_path; //Tinta::getRootPath();
    Tinta::StringUtil::addLeaf(config_path, name);


	mConfigPath = config_path;
    validatePath(mConfigPath);
	mScriptConfig.setFile(mConfigPath.c_str());

	dumpErr();

	mScriptsPaths.clear();
	mCLObjectsPaths.clear();
	mClObjects.clear();

    // not required 
    // parsing cl kernels paths 
    bool get_kernel = true;
    int count = 1;
	StringStreamBasic t;
    while( get_kernel ){
      
		t << tintaConfigurator::strGPUScripts;
		t << count;
        Tinta::t_string_array  clkernels;
        get_kernel = mScriptConfig.getArrayExp( clkernels, t.str().c_str() );
        if( get_kernel ){ 
            if( clkernels.size() == 3 )
				mClObjects.push_back( GPUProgramDescrip(clkernels[0], 
				clkernels[1], clkernels[2]));            
        }
		count++;
		t.flush();
		t.str("");
    }
    // clear errors for not required configurations 
    mScriptConfig.resetErrors();

	Tinta::t_string_array scrips_folders;
	mScriptConfig.getArray(scrips_folders, tintaConfigurator::strSectionScriptFolders );

	
	fillArrayData( scrips_folders, mScriptsPaths, root_path );

	// parsing cl kernels paths
	Tinta::t_string_array clkernels_folders;
	mScriptConfig.getArray(clkernels_folders, tintaConfigurator::strSectionGPUFolders );
		
	fillArrayData( clkernels_folders, mCLObjectsPaths, root_path );
	
	String ip;
	mScriptConfig.getGlobVar( ip, tintaConfigurator::strSectionIpAdress );

	mIpAdress = ip;
	
	mScriptConfig.getGlobVar(mPort, tintaConfigurator::strSectionPort);

	mScriptConfig.getGlobVar(mLocalEnabled, tintaConfigurator::strLocalEnabled);
    
	mScriptConfig.getGlobVar(mGpuEnabled, tintaConfigurator::strGpuEnabled);

    mScriptConfig.getGlobVar(mGpuPlatform, tintaConfigurator::strGpuPlatform);

    mScriptConfig.getGlobVar(mGpuDevice, tintaConfigurator::strGpuDevice);
	
	mScriptConfig.getGlobVar(mAppName, tintaConfigurator::strAppName);

    mScriptConfig.getGlobVar(mStartScript, tintaConfigurator::strStartScript);

	double val = 0.;
	mScriptConfig.getGlobVar(val, tintaConfigurator::strUnitPrior);
    mUnitPrior = TintaMath::min(std::abs((float)val), 1.f);

	
	mScriptConfig.getGlobVar(mTimeReconnect, tintaConfigurator::strReConnectTime);

    dumpErr();
	return true;
}

float tintaConfigurator::getUnitPrior()const{
	return mUnitPrior;
}


String  tintaConfigurator::getStartScript() const {
    return mStartScript;
}

void tintaConfigurator::getAllScriptsPaths(Tinta::t_string_array &paths ) const {

	paths.resize( mScriptsPaths.size() );
	std::copy(mScriptsPaths.begin(), mScriptsPaths.end(), paths.begin() );

}

m_uint32 tintaConfigurator::getTimeReconnect()const {
	return mTimeReconnect;
}

bool tintaConfigurator::isLocalEnabled() const{
	return mLocalEnabled;
}
int tintaConfigurator::getPort( ) const  {
	return mPort;	
}


m_int32 tintaConfigurator::getResetDuration() const {
    return 5000;
}

String tintaConfigurator::getIpAdress( ) const {
	return mIpAdress;
}

bool tintaConfigurator::getScriptPath(String &path,const String &script) const {
	String str_to_test;
	str_to_test = Tinta::getRootPath();
    Tinta::StringUtil::addLeaf(str_to_test, script);
	// root directory
	if( Tinta::isPathValid(str_to_test) ){
		path = str_to_test;
		return true;
	}
	str_to_test.clear();
	Tinta::t_string_array::const_iterator it = mScriptsPaths.begin();
	for( ; it != mScriptsPaths.end(); it++ ){
		str_to_test = *it;

        Tinta::StringUtil::addLeaf(str_to_test, script);

		if( Tinta::isPathValid(str_to_test) ) {
			path = str_to_test;
			return true;
		}       
	}

	return false;
}	

bool tintaConfigurator::getGpuScriptPath(String &path, const String &fileName) const {
	String str_to_test;
	str_to_test = Tinta::getRootPath();
	Tinta::StringUtil::addLeaf(str_to_test, fileName);
	// root directory
	if( Tinta::isPathValid(str_to_test) ){
		path = str_to_test;
		return true;
	}
	Tinta::t_string_array::const_iterator it = mCLObjectsPaths.begin();
	for( ; it != mCLObjectsPaths.end(); it++ ){
		str_to_test = *it;
		Tinta::StringUtil::addLeaf(str_to_test, fileName);
		if( Tinta::isPathValid( str_to_test ) ) {
			path = str_to_test;
			return true;
		}	
	}

	return false;
}

String tintaConfigurator::getAppName() const{
	return mAppName;
}



bool tintaConfigurator::getLocalEnabled()const{
	return mLocalEnabled;
}

bool tintaConfigurator::getGpuEnabled()const{
	return mGpuEnabled;
}

int   tintaConfigurator::getGpuPlatform() const {
    return mGpuPlatform;
}

int   tintaConfigurator::getGpuDevice() const {
    return mGpuDevice;
}


const GPUProgramDescrip *tintaConfigurator::getGpuScripts( m_uint32 &count ) const {
	count = mClObjects.size();
	if(count == 0 )
		return 0;
	return &mClObjects[0];
}


tintaConfigurator* tintaConfigurator::getPtr(void)
{
	return mPtr;
}

template<> tintaConfigurator* Singleton<tintaConfigurator>::mPtr = 0;
