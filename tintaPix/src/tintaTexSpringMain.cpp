/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaTexSpringMain.h"

#include "tintaImgCanvas.h"
#include "tintaImgStack.h"
#include "tintaFunctions.h"

#include "tintaIImgCodec.h"
#include "tintaGeomTypes.h"
#include "tintaGeomCatRom.h"
#include <tintaCommFunc.h>
#include <OCL/tintaClObjectContainer.h>
#include <OCL/tintaGPUExt.h>
#include "tintaArrayBox.h"
#include <tintaProcessQueue.h>
#include "tintaInteractHandlerImpl.h"


#ifdef USING_GPUCL 
	#include <OCL/tintaClObjectContainer.h>
	#include <OCL/tintaBox2dProgram.h>
#endif



namespace Tinta {
    

	/*const String tintaTexSpringMain::str_filename_box_gpu = _M("box.cl");
    // for counting all nearest on distance from point
	const StringBasic tintaTexSpringMain::str_kernelname_cntnearpoint = "cntnearpoint";
	const String tintaTexSpringMain::str_uniqname_cntnearpoint = _M("cntnearpoint");

    // for counting all nearest on distance from all points
	const StringBasic tintaTexSpringMain::str_kernelname_cntnearall = "cntnearall";
	const String tintaTexSpringMain::str_uniqname_cntnearall = _M("cntnearall");
    */
    // process name const prefix
	const String tintaTexSpringMain::str_process_name_prefix = _M("Proc");
    

tintaTexSpringMain::tintaTexSpringMain( bool bIsServ )
:ConsoleEditBase(),
 mIsServMode(bIsServ)
,mScriptContext( NULL_M )
,mCmdBufSize( 30 )
,mThreadPool(NULL_M)
,mTaskExecutor( NULL_M )
,mConfig(NULL_M)
,mLog(NULL_M)
,mImages(NULL_M)
,mGPUObjs(NULL_M)
,gpuExt(NULL_M)
,mWorkUnitsSet(NULL_M)
,mTasksQueue(NULL_M)
#ifndef TINTA_NO_INTERACT
,mInteractUnitSet(NULL_M)
#endif
,bIsAborting(false)
,mTimerExecute(1000, std::bind(&tintaTexSpringMain::timerExecute, this))
{
    
	mScriptContext = NEW_T (Tinta::tintaScriptContext)(); 	
	

	m_nameGen.reset();	

	mLog				 = M_NEW Tinta::tintaLogger();


	String path_log = Tinta::getRootPath();
	StringUtil::StrStreamType logName; 
	if( mIsServMode )
		logName<<_M("s_")<< getProcessId(); 
	else
		logName<<_M("c_")<< getProcessId(); 	
	logName << _M(".log");

    Tinta::StringUtil::addLeaf(path_log, logName.str());
    Tinta::tintaLogger::getPtr()->createLog(path_log);


	mImages				 = M_NEW Images_t();	
	
	mConfig       = NEW_T(tintaConfigurator)();

	gpuExt				 = NEW_T(tintaGPUExt)();
	
	
}

tintaTexSpringMain::~tintaTexSpringMain( void ){		
	
   
	// to prevent executing after joining threads in shutdown()
	bIsAborting = true;
       			
	if( mTaskExecutor ){
		M_DELETE mTaskExecutor;
		mTaskExecutor = NULL_M;
	}
	
    

	mThreadPool->shutdown();
	// deleting	before mThreadPool
	if( mThreadPool ) {
		M_DELETE mThreadPool; 		
		mThreadPool= NULL_M;
	}


	if( gpuExt )
		DELETE_T( gpuExt, tintaGPUExt );
	gpuExt = NULL_M;	

	//mImages->release();
    M_DELETE(mImages);
	mImages = NULL_M;		


	if(mScriptContext){
			mScriptContext->closeState();
			DELETE_T( mScriptContext, tintaScriptContext );
	}  


#ifndef TINTA_NO_INTERACT
 	if( mInteract ){
 		//M_DELETE mInteract;
        
		DELETE_T  ( mInteract, tintaChildProc );
 		mInteract = 0;
 	}

    if (mInteractPool){
        //M_DELETE mInteractPool;
        DELETE_T(mInteractPool, tintaInteractPool);
        mInteractPool = 0;
    }
#endif

	//mInteractPool->release();

	


	for( pDescripVec_t::iterator it = mFuncDescrib.begin(); it!= mFuncDescrib.end(); it++ ) 		DELETE_T ( *it , tintaScriptFunc );	
	mFuncDescrib.clear();
	mCompObjects.clear();		
	mBoxObjects.clear();

    if( mConfig ){
		DELETE_T(mConfig, tintaConfigurator)		
			mConfig = 0;
	}
	
	if( mRanGen ){
		DELETE_T(mRanGen, tintaRandGen)		
			mRanGen = 0;
	}
	
	for(compObjfactories_t::iterator it = mCompObjFactVec.begin(); it!= mCompObjFactVec.end(); it++) 		M_DELETE (*it);	
	
	
	for(compValBoxFactories_t::iterator it = mValBoxFacVec.begin(); it!= mValBoxFacVec.end(); it++) 		M_DELETE (*it);		

    for (fontFactories_t::iterator it = mFontFacroties.begin(); it != mFontFacroties.end(); it++) 		M_DELETE(*it);

    


    mFontFacroties.push_back(M_NEW tintaImageFontFactory());
	if( mLog )
		M_DELETE mLog;
	
	deleteGPU();

	if( mWorkUnitsSet )
		DELETE_T  ( mWorkUnitsSet, tintaUnitsSet );

	if( mTasksQueue )
		DELETE_T  ( mTasksQueue, tintaTaskQueue );
#ifndef TINTA_NO_INTERACT
    	if( mInteractUnitSet )
		DELETE_T  ( mInteractUnitSet, tintaInteractUnitsSet );    
#endif
}

void tintaTexSpringMain::deleteGPU( const String &program ){
#ifdef USING_GPUCL
    if (program.length() == 0) {
        gpuPrograms_t::iterator it = mGPUPrograms.begin();
        for (; it != mGPUPrograms.end(); it++) {
            M_DELETE(*it);
        }
        mGPUPrograms.clear();
        if (mGPUObjs) {
            mGPUObjs->clear();
            DELETE_T(mGPUObjs, tintaClObjectContainer);
            mGPUObjs = NULL_M;
        }
    }
    else {

        gpuPrograms_t::iterator i = std::find_if(mGPUPrograms.begin(), mGPUPrograms.end(), [&](tintaIClBase* prog) {
            return prog->getProgramName() == program; });              

        if ( i != mGPUPrograms.end() ) {
            M_DELETE(*i);
            if ( mGPUObjs ) {
                mGPUObjs->remove(program);                
            }
        }
    }
	
#endif
}

void tintaTexSpringMain::initGPU() {
#ifdef USING_GPUCL

	//if (mConfig->isG)
	if ( gpuEnabled() ){

		if ( mGPUObjs )
			deleteGPU();

		mGPUObjs = NEW_T(tintaClObjectContainer)();
		{
			m_ulong32 p = gpuExt->getPlatformsIDs();
			StringUtil::StrStreamType t;

			t << "GPU enabled on : ";
			for (m_ulong32 i = 0; i < p; i++){
				t << gpuExt->getDeviceInfo(i, tintaGPUExt::GPUDevName);
			}
			Tinta::tintaLogger::getPtr()->logMsg(t.str());
		}

		

		// user defined 
		m_uint32 q = 0;
		const GPUProgramDescrip *cl = mConfig->getGpuScripts(q);
		for (m_uint32 i = 0; i < q; i++){

			String fullPath;
			if (mConfig->getGpuScriptPath(fullPath, cl[i].mFile)){

				tintaIClBase *obj = M_NEW Box2dProgram(cl[i].mName,
					fullPath, cl[i].mKernel);
				mGPUObjs->registerObject(obj);
				mGPUPrograms.push_back(obj);
			}
		}
	}

	//	unsigned GpuCount = 0;

#endif
}

bool tintaTexSpringMain::initialize(const String &configName, tintaConsoleOutput * out, tintaIImgWindow * window){
	
    Tinta::tintaLogger::getPtr()->addConsole(out);
	// ------- initializing configurator 
	mConfig->parserConfig(configName);// nsNameStrings::strTexSpringConfigFileW);

	// ------- common names
	String path_log = Tinta::getRootPath();
	mAppName = mConfig->getAppName();

	// if the name default, getting the process id
	if( _M("Main") == mAppName ){
		StringUtil::StrStreamType logName; 		
		logName<<getProcessId(); 		
		mAppName = logName.str();
	}
    
	//

	//if ( mAppName == _M("") )
	//mAppName = appName;

    int appHash = StringUtil::hashStrCaseSens(mAppName.c_str());

	mRandGen.setRandSeed((int)time(0) ^ appHash);

	mRanGen = NEW_T(tintaRandGen)((int)time(0) ^ appHash);
	String prefNameGen = mAppName;
    prefNameGen.append(tintaTexSpringMain::str_process_name_prefix);
	m_nameGen.setPrefix(prefNameGen);
	//--


	tintaProcessQueue* defaultQ = M_NEW tintaProcessQueue( _M("") );	
	//if( mIsServMode ) { // clients mode 

	// ------- initializing working queue

	// full version #define TEXSPRING_THREADS THREAD_HARDWARE_CONCURRENCY
	#define TEXSPRING_THREADS THREAD_HARDWARE_CONCURRENCY 
	unsigned threadCount = TEXSPRING_THREADS;
	defaultQ->setWorkerThreadCount( threadCount ); 	

	//}

	mThreadPool = defaultQ;
	mThreadPool->startup();
	
	
	mImages->initialization();	

	mWorkUnitsSet		 = NEW_T(tintaUnitsSet)();


    tintaUnitsEvents * ueV = dynamic_cast<tintaUnitsEvents *>(out);
    if( ueV )
        mWorkUnitsSet->addListener( ueV );
#ifndef TINTA_NO_INTERACT
	mInteractPool = NEW_T(tintaInteractPool)();

   	mInteract  = NEW_T ( tintaChildProc) ();


 	String ip  = mConfig->getIpAdress();
 	short  port  = (short)mConfig->getPort();
    if ( ip.length() > 0 ) {
        mInteract->initialise(tintaChildProc::InteractData(ip, port), mIsServMode, mAppName);

        StringUtil::StrStreamType t;
        t << _M("mInteractPool->startup");
        Tinta::tintaLogger::getPtr()->logMsg(t.str());

        mInteractPool->addInteract(mInteract->getInteractFunc());
    }
#endif   
    createFunctions();
    reinitContext(mScriptContext);    

	//mCompObjects
	// registering factories
	// for geometry
	mCompObjFactVec.push_back( M_NEW  TintaGeomTypes::tintaCompGeomSplineFactory () );
	mCompObjFactVec.push_back( M_NEW  TintaGeomTypes::tintaCircleFactory () );
	mCompObjFactVec.push_back( M_NEW  TintaGeomTypes::tintaCompGeomPolyLineFactory () );
	mCompObjFactVec.push_back( M_NEW  tintaCatRomFactory() );	

 	for( compObjfactories_t::iterator it = mCompObjFactVec.begin(); it!= mCompObjFactVec.end(); it++ )	{
 		bool rez = 	mCompObjects.registerFactory( *it );	
		CoreAssert( rez ,"bool rez = 	mCompObjects->registerFactory( *it );");
 	}
    
	

	mValBoxFacVec.push_back( M_NEW tintaFloatBoxFactory() );
	mValBoxFacVec.push_back( M_NEW tintaIntBoxFactory() );
	mValBoxFacVec.push_back( M_NEW tintaInt16tBoxFactory() );
	mValBoxFacVec.push_back( M_NEW tintaInt16tVectorFactory() );
	mValBoxFacVec.push_back( M_NEW tintaIntVectorFactory() );
    mValBoxFacVec.push_back( M_NEW tintaInt8VectorFactory());
	mValBoxFacVec.push_back( M_NEW tintaFloatVectorFactory() );
	mValBoxFacVec.push_back( M_NEW tintaStringVectorFactory() );
    mValBoxFacVec.push_back( M_NEW tintaVec3fVectorFactory() );


    mFontFacroties.push_back(M_NEW tintaImageFontFactory());
	

	for( compValBoxFactories_t::iterator it = mValBoxFacVec.begin(); it!= mValBoxFacVec.end(); it++ )	{
		bool rez = 	mBoxObjects.registerFactory( *it );	
		CoreAssert( rez,"bool rez = mBoxObjects->registerFactory( *it );" );
	}

    mContainers.insert(containersMap_t::value_type(mBoxObjects.getName(), &mBoxObjects));

    for (fontFactories_t::iterator it = mFontFacroties.begin(); it != mFontFacroties.end(); it++) {
        bool rez = mFonts.registerFactory(*it);
        CoreAssert(rez, "bool rez = mFonts.registerFactory(*it);");
    }

    
	initGPU();

	
	mTasksQueue = NEW_T(tintaTaskQueue)(mIsServMode);
#ifndef TINTA_NO_INTERACT
	mInteractUnitSet = NEW_T (tintaInteractUnitsSet )();
#endif

    if (mConfig->isLocalEnabled()) {
        mTaskExecutor = M_NEW tintaScriptTaskExecutor(mScriptContext, mThreadPool);
        mTaskExecutor->setName(mAppName);
    }
    else
        mTaskExecutor = NULL;

	if( !tintaImgStack::getPtr() )
		return false;

    mContainers.insert(containersMap_t::value_type(mImages->getName(), mImages));


    mImageWindow = window;

	return true;
}

void tintaTexSpringMain::clearContainers() {
       
  
    tintaImgStack* images = Tinta::tintaImgStack::getPtr();
    if (images)
        images->delAllImg();

    tintaBoxContainer* boxes = tintaBoxContainer::getPtr();
    if (boxes)
        boxes->clear();

    tintaGeomSet *pCompObjs = tintaGeomSet::getPtr(); /*tintaImgStack::getPtr()->getCompObjectContainer()*/;
    if (pCompObjs)
        pCompObjs->clear();
       

}
bool tintaTexSpringMain::gpuEnabled()const{
	return mConfig ? mConfig->getGpuEnabled() : false;
}


tintaIImgWindow* tintaTexSpringMain::getWindow() {
    return mImageWindow;
}

void	tintaTexSpringMain::setWindow( tintaIImgWindow* winp ) {
    mImageWindow = winp;
}

bool tintaTexSpringMain::setFont( m_uint32 id ) {
    return mFonts.selectObj( id );
}

bool tintaTexSpringMain::setFontParameter(m_uint32 id, float scaleX, float scaleY, const color_type &color) {

    tintaFont* font = mFonts.getObjEx(id);

    if ( !font )
        return false;

    if (scaleX > 0.f || scaleY > 0.f) {
        font->setSize(scaleX, scaleY);
    }

    if (color != color_type{ 0, 0, 0, 0 }) {
        font->setColor(color);
    }

    return true;

}

 m_uint32 tintaTexSpringMain::addFont( const String &name, float sizeX, float sizeY, const color_type &color ) {

     m_uint32 added =  mFonts.addObj( tintaImageFont::str_font );
     tintaFont* font = mFonts.getObjEx(added);

     if (!font)
         return ZERO_ID;

     if ( sizeX > 0 || sizeY > 0 ) {                  
         font->setSize(sizeX, sizeY);                  
     }

     if ( color != color_type{ 0, 0, 0, 0 } ) {         
             font->setColor(color);         
     }

     if ( !font->create( name ) ) {
          mFonts.delObj(added);
         return ZERO_ID;
     }

     return added;
}

 tintaFont* tintaTexSpringMain::getFont( m_uint32 id ) {
     return mFonts.getObjEx(id);
 }

 m_uint32 tintaTexSpringMain::cloneFont(m_uint32 src) {


     tintaFont* srcFont = mFonts.getObjEx(src);

     if (!srcFont)
         return ZERO_ID;

     m_uint32 added = mFonts.addObj(tintaImageFont::str_font);
     tintaFont* font = mFonts.getObjEx(added);

     *font = *srcFont;
     return added;
 }

 tintaFont* tintaTexSpringMain::getFont() {
     return mFonts.getObjEx( mFonts.getIndexSel());
 }

 bool tintaTexSpringMain::deleteFont( m_uint32 id ) {
     return mFonts.delObj( id );
 }

 void tintaTexSpringMain::deleteFonts() {
     mFonts.clear();
 }

tintaIClBase* tintaTexSpringMain::getGPUProgram( const String &uniqName ){

#ifdef USING_GPUCL
	if( !mGPUObjs )
		return NULL;

	return( mGPUObjs->findObject( uniqName ) );
#endif
	return NULL_M;
}

tintaThreadPool* tintaTexSpringMain::getThreadPool(){
	return mThreadPool;
}


bool tintaTexSpringMain::isServMode() const{
	return mIsServMode;
}

void tintaTexSpringMain::setServMode(bool server_mode){
	mIsServMode = server_mode;
}


String tintaTexSpringMain::getAppName(){
	return mAppName;
}

void tintaTexSpringMain::setAppName(const String &name){
	mAppName = name;
}

NameGenerator* tintaTexSpringMain::getNameGen() {
	return &m_nameGen;
}

TintaMath::tintaRandomV2* tintaTexSpringMain::getRandGen(){
	return &mRandGen;
}
float tintaTexSpringMain::getUnitPrior(){
	return mConfig->getUnitPrior();
}

#ifndef TINTA_NO_INTERACT
tintaChildProc*	tintaTexSpringMain::getInteract(){
	return mInteract;
}
#endif

tintaISerialisableSet   *tintaTexSpringMain::getSerialisableSet(const StringBasic& name) {

    containersMap_t::iterator i = mContainers.find(name);
    if (i == mContainers.end() )
        return NULL_M;

    return i->second;
}

const tintaISerialisableSet   *tintaTexSpringMain::getSerialisableSet(const StringBasic& name) const {


    containersMap_t::const_iterator i = mContainers.find(name);
    if (i == mContainers.end())
        return NULL_M;

    return i->second;
}

const tintaFontContainer       *tintaTexSpringMain::getFonts() const {
    return &mFonts;
}

void tintaTexSpringMain::clearConsole(){	
}

void tintaTexSpringMain::exit(){
	signal(sigClosed, signalHandler);
	raise(sigClosed);
}


void tintaTexSpringMain::setExecutor(Tinta::tintaScriptContext *lua_wrapper){
	mScriptContext = lua_wrapper;
}

void tintaTexSpringMain::registerFunctions(Tinta::tintaScriptContext *context) {
 	for( pDescripVec_t::iterator it = mFuncDescrib.begin(); it!= mFuncDescrib.end(); it++ ) 		
 		registerFunction( context, *it ); 
}
void tintaTexSpringMain::createFunctions(){

    clearContainers();

    m_systemFunctions.clear();

    for (pDescripVec_t::iterator it = mFuncDescrib.begin(); it != mFuncDescrib.end(); it++) 		DELETE_T(*it, tintaScriptFunc);
    mFuncDescrib.clear();

    //system functions
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("units")), Tinta::tintaScriptFunctions::units));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("units()")), Tinta::tintaScriptFunctions::units));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("tasks")), Tinta::tintaScriptFunctions::tasks));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("tasks()")), Tinta::tintaScriptFunctions::tasks));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("status")), Tinta::tintaScriptFunctions::status));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("status()")), Tinta::tintaScriptFunctions::status));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("abort")), Tinta::tintaScriptFunctions::abort));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("abort()")), Tinta::tintaScriptFunctions::abort));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("state")), Tinta::tintaScriptFunctions::state));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("state()")), Tinta::tintaScriptFunctions::state));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("cinit")), Tinta::tintaScriptFunctions::cinit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("cinit()")), Tinta::tintaScriptFunctions::cinit));
	m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("ginit")), Tinta::tintaScriptFunctions::ginit));
	m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("ginit()")), Tinta::tintaScriptFunctions::ginit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("restart")), Tinta::tintaScriptFunctions::restart));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("restart()")), Tinta::tintaScriptFunctions::restart));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("reset")), Tinta::tintaScriptFunctions::reset));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("reset()")), Tinta::tintaScriptFunctions::reset));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("exit")), Tinta::tintaScriptFunctions::exit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("exit()")), Tinta::tintaScriptFunctions::exit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("x")), Tinta::tintaScriptFunctions::exit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("x()")), Tinta::tintaScriptFunctions::exit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("cls")), Tinta::tintaScriptFunctions::cls));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("cls()")), Tinta::tintaScriptFunctions::cls));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("window")), Tinta::tintaScriptFunctions::window));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("window()")), Tinta::tintaScriptFunctions::window));


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setpixelb", tintaScriptFunctions::c_setpixelb));
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setpixelf", tintaScriptFunctions::c_setpixelf));
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_clearimage", tintaScriptFunctions::c_clearimage));
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getcolors", tintaScriptFunctions::c_getcolors));
   
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_priorcolor", tintaScriptFunctions::c_priorcolor));
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_priorcolor2", tintaScriptFunctions::c_priorcolor2));    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_poster", tintaScriptFunctions::c_poster));
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_calcgeom", tintaScriptFunctions::c_calcgeom));
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getimgsel", tintaScriptFunctions::c_getimgsel));   

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_delimg", tintaScriptFunctions::c_delimg));
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_selimg", tintaScriptFunctions::c_selimg));
       

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_saveimg", tintaScriptFunctions::c_saveimg));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_resizeimg", tintaScriptFunctions::c_resizeimg));
    


    //registerFunction(pFunc, tintaScriptFunctions::c_saveimg); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_printver", tintaScriptFunctions::c_printver));
    
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_assert", tintaScriptFunctions::c_assert));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_assert); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getwidth", tintaScriptFunctions::c_getwidth));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getwidth); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getheight", tintaScriptFunctions::c_getheight));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getheight); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getpixelb", tintaScriptFunctions::c_getpixelb));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getpixelb); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getpixelf", tintaScriptFunctions::c_getpixelf));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getpixelf); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getindpixelb", tintaScriptFunctions::c_getindpixelb));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getindpixel); 


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getindpixelf", tintaScriptFunctions::c_getindpixelf));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getindpixelf); 	


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fillimgb", tintaScriptFunctions::c_fillimgb));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fillimgf", tintaScriptFunctions::c_fillimgf));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_fillimgb); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getcoordimg", tintaScriptFunctions::c_getcoordimg));
    
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_f", tintaScriptFunctions::c_f));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fp", tintaScriptFunctions::c_fp));  
    
    
    //registerFunction(pFunc, tintaScriptFunctions::c_drawrec); 	

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getangle", tintaScriptFunctions::c_getangle));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getangle); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getcoordtopoint", tintaScriptFunctions::c_getcoordtopoint));
        

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_createimg", tintaScriptFunctions::c_createimg));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_start", tintaScriptFunctions::c_start));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_stop", tintaScriptFunctions::c_stop));
    
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_test", tintaScriptFunctions::c_test));
    
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_readimg", tintaScriptFunctions::c_readimg));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_readimg); 
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_rotpoint", tintaScriptFunctions::c_rotpoint));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_rotpoint); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getdist", tintaScriptFunctions::c_getdist));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getdist); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setgeomprop1f", tintaScriptFunctions::c_setgeomprop1f));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_setgeomprop1f); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setgeomprop1b", tintaScriptFunctions::c_setgeomprop1b));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_setgeomprop1b); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setgeomprop1s", tintaScriptFunctions::c_setgeomprop1s));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_setgeomprop1s); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_addgeomprop2f", tintaScriptFunctions::c_addgeomprop2f));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_addgeomprop2f); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_addgeomprop1f", tintaScriptFunctions::c_addgeomprop1f));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_addgeomprop1f); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_addgeomprop2i", tintaScriptFunctions::c_addgeomprop2i));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_addgeomprop2i); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_addgeomprop4i", tintaScriptFunctions::c_addgeomprop4i));
    
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_addgeomprop2f", tintaScriptFunctions::c_addgeomprop2f));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_addgeomprop2f); 
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_delgeom", tintaScriptFunctions::c_delgeom));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_delgeom); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_delallgeom", tintaScriptFunctions::c_delallgeom));
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fillalphab", tintaScriptFunctions::c_fillalphab));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fillalphaf", tintaScriptFunctions::c_fillalphaf));
    
            
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setalphaf", tintaScriptFunctions::c_setalphaf));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setalphab", tintaScriptFunctions::c_setalphab));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_setalpha); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_injectimg", tintaScriptFunctions::c_injectimg));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_injectimg); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_injectimga", tintaScriptFunctions::c_injectimga));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_injectimga); 


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getpoint", tintaScriptFunctions::c_getpoint));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getpoint); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_pointscount", tintaScriptFunctions::c_pointscount));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_pointscount); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_cleargeom", tintaScriptFunctions::c_cleargeom));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_cleargeom); 


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_copygeom", tintaScriptFunctions::c_copygeom));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_copygeom); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_delallimg", tintaScriptFunctions::c_delallimg));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_delallimg); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setrandseed", tintaScriptFunctions::c_setrandseed));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_setrandseed); 
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_randint", tintaScriptFunctions::c_randint));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_randintex", tintaScriptFunctions::c_randintex));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_randint); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_randfloat", tintaScriptFunctions::c_randfloat));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_randfloat); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_calchash", tintaScriptFunctions::c_calchash));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_calchash); 
      

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getname", tintaScriptFunctions::c_getname));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getname);
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_uniqid", tintaScriptFunctions::c_uniqid));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_uniqid); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getseed", tintaScriptFunctions::c_getseed));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getseed); 
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_creategeom", tintaScriptFunctions::c_creategeom));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_creategeom); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_calculate", tintaScriptFunctions::c_calculate));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_calculate); 	

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_createbox", tintaScriptFunctions::c_createbox));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_createbox); 	

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_pushbackboxi16", tintaScriptFunctions::c_pushbackboxi16));
    


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_pushbackboxi32", tintaScriptFunctions::c_pushbackboxi32));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_pushbackboxs", tintaScriptFunctions::c_pushbackboxs));
    





    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_pushbackboxf", tintaScriptFunctions::c_pushbackboxf));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_popbackboxf", tintaScriptFunctions::c_popbackboxf));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_popbackbox3f", tintaScriptFunctions::c_popbackbox3f));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_popbackboxs", tintaScriptFunctions::c_popbackboxs));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_popbackboxi32", tintaScriptFunctions::c_popbackboxi32));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_popbackboxi16", tintaScriptFunctions::c_popbackboxi16));
    



    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_boxelements", tintaScriptFunctions::c_boxelements));
    


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_pushbackbox3f", tintaScriptFunctions::c_pushbackbox3f));
    




    //pFunc = NEW_T (tintaScriptFunc)("c_popbackboxi16", tintaScriptFunctions::c_popbackboxi16);	
    //



    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_delbox", tintaScriptFunctions::c_delbox));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_delbox); 	

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_delallboxes", tintaScriptFunctions::c_delallboxes));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_delallboxes); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getvalboxf", tintaScriptFunctions::c_getvalboxf));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getvalbox3f", tintaScriptFunctions::c_getvalbox3f));
    

    //registerFunction(pFunc, tintaScriptFunctions::c_getvalboxf); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getvalboxi16", tintaScriptFunctions::c_getvalboxi16));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getvalboxi16);

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getvalboxs", tintaScriptFunctions::c_getvalboxs));
    


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getvalboxi32", tintaScriptFunctions::c_getvalboxi32));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_getvalboxi32);

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_resizebox", tintaScriptFunctions::c_resizebox));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_resizebox); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setvalboxf", tintaScriptFunctions::c_setvalboxf));
    


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setvalbox3f", tintaScriptFunctions::c_setvalbox3f));
    


    //registerFunction(pFunc, tintaScriptFunctions::c_setvalboxf); 

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setvalboxi32", tintaScriptFunctions::c_setvalboxi32));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setvalboxs", tintaScriptFunctions::c_setvalboxs));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_setvalboxi32);

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setvalboxi16", tintaScriptFunctions::c_setvalboxi16));
    


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_setvalboxui8", tintaScriptFunctions::c_setvalboxui8));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_getvalboxui8", tintaScriptFunctions::c_getvalboxui8));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_popbackboxui8", tintaScriptFunctions::c_popbackboxui8));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_pushbackboxui8", tintaScriptFunctions::c_pushbackboxui8));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_setvalboxi16);

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_clearbox", tintaScriptFunctions::c_clearbox));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_clearbox);   

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_isserver", tintaScriptFunctions::c_isserver));
    


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_images", tintaScriptFunctions::c_images));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_channels", tintaScriptFunctions::c_channels));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_images);


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_geoms", tintaScriptFunctions::c_geoms));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_geoms);


    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_boxes", tintaScriptFunctions::c_boxes));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_boxes);
       

    //pFunc = NEW_T(tintaScriptFunc)("c_executefunc", tintaScriptFunctions::c_executefunc);
    //
    //registerFunction(pFunc, tintaScriptFunctions::c_executefunc);

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_exit", tintaScriptFunctions::c_exit));
    
    //registerFunction(pFunc, tintaScriptFunctions::c_exit);

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_readfile", tintaScriptFunctions::c_readfile));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_writetofile", tintaScriptFunctions::c_writetofile));
    
    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_executeshell", tintaScriptFunctions::c_executeshell));
    

    //pFunc = NEW_T(tintaScriptFunc)("c_sleep", tintaUtilFunc::c_sleep);
    //   
       

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_regextoken", tintaScriptFunctions::c_regextoken));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_regextokenex", tintaScriptFunctions::c_regextokenex));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_wincursor", tintaScriptFunctions::c_wincursor));
      

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_cursorimg", tintaScriptFunctions::c_cursorimg));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_keystate", tintaScriptFunctions::c_keystate));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_regexmatch", tintaScriptFunctions::c_regexmatch));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fd", tintaScriptFunctions::c_fd));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fl", tintaScriptFunctions::c_fl));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fu", tintaScriptFunctions::c_fu));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_e", tintaScriptFunctions::c_e));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_el", tintaScriptFunctions::c_el));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_ed", tintaScriptFunctions::c_ed));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_winupdate", tintaScriptFunctions::c_winupdate));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_winhandler", tintaScriptFunctions::c_winhandler));
     

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_winsetimg", tintaScriptFunctions::c_winsetimg));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_eu", tintaScriptFunctions::c_eu));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_tasks", tintaScriptFunctions::c_tasks));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_addfont", tintaScriptFunctions::c_addfont));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_copyfont", tintaScriptFunctions::c_copyfont));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_selfont", tintaScriptFunctions::c_selfont));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_delfont", tintaScriptFunctions::c_delfont));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_delallfont", tintaScriptFunctions::c_delallfont));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_fontcolor", tintaScriptFunctions::c_fontcolor));
    

    mFuncDescrib.push_back(NEW_T(tintaScriptFunc)("c_drawtext", tintaScriptFunctions::c_drawtext));
    
}

bool tintaTexSpringMain::registerCl(const String &program, const String &kernel){
	// 	Tinta::ImageBlurFilterCL *test_procces = new Tinta::ImageBlurFilterCL();
	// 	test_procces->initialize("kernel.cl", "image_test");

    String fullPath = program;
    String prog = program;

    if ( !isPathValid(prog) ) {

        fullPath.append(Tinta::getRootPath());
        fullPath.append(_M("/"));
        fullPath.append(prog);
        
        if ( !isPathValid( fullPath ) ) {
            fullPath.clear();
            mConfig->getGpuScriptPath( fullPath, prog);
        }       
             
    }        

    if ( isPathValid( fullPath ) ) {
#ifdef USING_GPUCL
        tintaIClBase* clProg = Tinta::tintaTexSpringMain::getPtr()->getGPUProgram(fullPath);
        if (clProg) {
            Tinta::tintaTexSpringMain::getPtr()->deleteGPU(fullPath);
        }        

        try {

            tintaIClBase *obj = M_NEW Box2dProgram(fullPath, fullPath, kernel );

            mGPUObjs->registerObject(obj);
            mGPUPrograms.push_back(obj);

            return true;
        }
        catch ( const Tinta::tintaException &e ) {
            if( Tinta::tintaLogger::getPtr() )
                Tinta::tintaLogger::getPtr()->logMsg( e.getDescr() );          
        }
#endif
    }
    return false;
}

void tintaTexSpringMain::timerExecute() {
    tintaTexSpringMain::getPtr()->executeCommand( 0, mTimerCommand, tintaExecutingTask::enLocalTask );
}

void tintaTexSpringMain::reinitContext( Tinta::tintaScriptContext *scriptContext){
	
	mScriptContext->createState();

    tintaUtilFunc::registerUtilLua(mScriptContext->getState_ex());

	Tinta::tintaScriptFunctions::registerGpuLua(mScriptContext->getState_ex());
	
	registerFunctions( mScriptContext );

    Tinta::tintaLogger::getPtr()->logMsg(_M("Script context was recreated."));    
}

void tintaTexSpringMain::resetTimer(m_uint32 period, const String &command) {
    mTimerCommand = command;
    mTimerExecute.reset( period, true );
}

void tintaTexSpringMain::stopTimer() {
    mTimerExecute.stop();
    mTimerCommand = String();
}


Tinta::tintaScriptContext *tintaTexSpringMain::getContext() {
	CoreAssert(mScriptContext, "mScriptContext");
	return mScriptContext;
}

UNIQPTRDEF_T(Tinta::tintaScriptContext) tintaTexSpringMain::createContext() {

	//tintaScriptContext * scriptContext = NEW_T (Tinta::tintaScriptContext)(); 	

    UNIQPTRDEF(Tinta::tintaScriptContext, scriptContext, NEW_T(Tinta::tintaScriptContext)() );

    reinitContext( scriptContext.get() );
	return scriptContext;
}
void tintaTexSpringMain::executeCommand(const String &command, tintaExecutingTask::TaskType type){
    executeCommand(0, command, type);
}

void tintaTexSpringMain::executeCommand( size_t unitId, const String &command, tintaExecutingTask::TaskType type ){

    auto it = m_systemFunctions.find(command);
    Tinta::tintaTaskQueue *q = Tinta::tintaTaskQueue::getPtr();

    CoreAssert(q, "Tinta::tintaTaskQueue * q");
    if (it != m_systemFunctions.end()){       
       
        trySendSysCommand( command, unitId, it->second );
        return;
    }   

    CoreAssert( mScriptContext, "mScriptContext" );     

    m_ulong32 id = 0;
    q->pushTask( command, type, id, unitId );

}

void tintaTexSpringMain::trySendSysCommand(const String &command, size_t unitId, void(*sysComFunc)() ) {

#ifndef TINTA_NO_INTERACT
    tintaBufferIO buff;

    buff.AllocateBuffer(StringPackSize<String>(command));
    PackString<char_m, String>(buff.GetBufferEx(), 0, command);

    tintaBufferIOUnqPtr buffPacket = tintaInteractHandlerImpl::allocatePacketDataPtr( tintaInteractHandlerImpl::enSystemCommand, &buff );  

    const tintaExecutingUnit *unit = tintaUnitsSet::getPtr()->getNextUnit();
    if ( !unit )
        return;

    m_ulong32 unId = unit->getId().mid;

    tintaAsioInteract* send = mInteract ? mInteract->getInteract() : NULL;

    bool localExecute = (unitId == 0); // all
    for (; unit != NULL; unit = tintaUnitsSet::getPtr()->getNextUnit(&unId)){

        unId = unit->getId().mid;
        StringStream msg;       
        if ( unitId == 0 || (m_ulong32)unitId == unId ){
            const tintaAsioClient * client = unit->getInteractClient();


            if ( client && send
                && unit->getType() == tintaExecutingUnit::enInteractUnit )  {
                m_ulong32 key = client->getId();
                size_t rezId{ 0 };
                send->sendData( buffPacket.get(), rezId, key );
            }
            else
                localExecute = true; // unitId is local 


        }        
    }
    if ( localExecute )
        sysComFunc();
#else
    sysComFunc();
#endif
    

    /*
    size_t id{0};
    tintaAsioInteract* send = mInteract ? mInteract->getInteract() : NULL;
    if ( send ){
        servedPtr_t clientPtr = send->getFirstClient();
        while (clientPtr){
            clientsKey_t key = clientPtr->getId();
            if (unitId == 0 || unitId == key ){
                send->sendData(sendbuff.get(), id, &key);
                clientPtr = send->getNextClient();
            }
        }
    }
    */


}

bool tintaTexSpringMain::isAborted() const {
	return bIsAborting;
}

void tintaTexSpringMain::setAborted( bool bAbort ){
    Tinta::tintaLogger::getPtr()->logMsg(_M("set  aborted"));
   
	bIsAborting = bAbort;
}


template<> Tinta::tintaTexSpringMain* Tinta::Singleton<tintaTexSpringMain>::mPtr = 0;

}

void signalHandler(int)
{
	appClosing = 1;
}

//#endif

