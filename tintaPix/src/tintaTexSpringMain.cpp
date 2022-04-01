/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaTexSpringMain.h"
#include "tintaImgCanvas.h"

#include "tintaImgStack.h"
#include "tintaFunctions.h"

#include "tintaIImgCodec.h"
#include <tintaCommFunc.h>
#include <OCL/tintaClObjectContainer.h>
#include <OCL/tintaGPUExt.h>
#include "tintaArrayBox.h"
#include <tintaProcessQueue.h>
#include "tintaInteractHandlerImpl.h"
#include <tintaMemoryAllocator.h>

#ifdef USING_GPUCL 
	#include <OCL/tintaClObjectContainer.h>
	#include <OCL/tintaBox2dProgram.h>
#endif



using namespace Tinta;
    

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
,mScriptContext( NULL_M)
,mCmdBufSize( 30 )
,mThreadPool(NULL_M)
,mTaskExecutor( NULL_M )
,mConfig(NULL_M)
,mImages(NULL_M)
,mGPUObjs(NULL_M)
,mWorkUnitsSet(NULL_M)
,mTasksQueue(NULL_M)
#ifndef TINTA_NO_INTERACT
,mInteractUnitSet(NULL_M)
#endif
,bIsAborting(false)
,mTimerExecute(1000, std::bind(&tintaTexSpringMain::timerExecute, this))
{
    
	
	

	m_nameGen.reset();	   


	

	mImages				 = M_NEW Images_t();	
	
	mConfig       = NEW_T(tintaConfigurator)();    
	
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


	
	//mImages->release();
    if( mImages )
        M_DELETE(mImages);
	mImages = NULL_M;		

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

	
	mBoxObjects.clear();

    if( mConfig ){
		DELETE_T(mConfig, tintaConfigurator)		
			mConfig = 0;
	}
	
	if( mRanGen ){
		DELETE_T(mRanGen, tintaRandGen)		
			mRanGen = 0;
	}
	
	for(compValBoxFactories_t::iterator it = mValBoxFacVec.begin(); it!= mValBoxFacVec.end(); it++) 		M_DELETE (*it);		

    for (fontFactories_t::iterator it = mFontFacroties.begin(); it != mFontFacroties.end(); it++) 		M_DELETE(*it);

    mFontFacroties.push_back(M_NEW tintaImageFontFactory());	
	
	deleteGPU();

	if( mWorkUnitsSet )
		DELETE_T  ( mWorkUnitsSet, tintaUnitsSet );

	if( mTasksQueue )
		DELETE_T  ( mTasksQueue, tintaTaskQueue );
    mTasksQueue = NULL_M;

#ifndef TINTA_NO_INTERACT
    	if( mInteractUnitSet )
		    DELETE_T  ( mInteractUnitSet, tintaInteractUnitsSet );    
        mInteractUnitSet = NULL_M;
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

    

	
	if ( gpuEnabled() ){

		if ( mGPUObjs )
			deleteGPU();

		mGPUObjs = NEW_T(tintaClObjectContainer)();
		{ 
			m_ulong32 p = tintaGPUExt::getPtr()->getPlatformsIDs();
			StringUtil::StrStreamType t;
            if( p > 0 )
			    t << "OCL GPU enabled";
			
			Tinta::tintaLogger::getPtr()->logMsg(t.str());
		}

		

		// user defined 
		m_uint32 q = 0;
		const GPUProgramDescrip *cl = mConfig->getGpuScripts(q);
		for (m_uint32 i = 0; i < q; i++){

			String fullPath;
			if ( mConfig->getGpuScriptPath(fullPath, cl[i].mFile ) ){

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

bool tintaTexSpringMain::initialize(const String &configName, tintaConsoleOutput * out, tintaScriptContext* context){
    
    //if( mScriptContext )
     //   DELETE_T( mScriptContext, tintaScriptContext );
    CoreAssert(context, "context == NULL");
    mScriptContext = context;//NEW_T(Tinta::tintaScriptContext)();
   

    Tinta::tintaLogger::getPtr()->addConsole(out);   

    createFunctions();

    reinitContext(mScriptContext);

    
	// ------- initializing configurator 
	mConfig->parserConfig(configName);// nsNameStrings::strTexSpringConfigFileW);
    
	// ------- common names
	//String path_log = Tinta::getRootPath();
	mAppName = mConfig->getAppName();

	// if the name default, getting the process id
	if( _M("Main") == mAppName ){
		StringUtil::StrStreamType logName; 		
		logName<<getProcessId(); 		
		mAppName = logName.str();
	}
    	
    int appHash = StringUtil::hashStrCaseSens(mAppName.c_str());

	mRandGen.setRandSeed((int)time(0) ^ appHash);

	mRanGen = NEW_T(tintaRandGen)((int)time(0) ^ appHash);
	String prefNameGen = mAppName;
    prefNameGen.append(tintaTexSpringMain::str_process_name_prefix);
	m_nameGen.setPrefix(prefNameGen);
	
	tintaProcessQueue* defaultQ = M_NEW tintaProcessQueue( _M("") );		 

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

    mContainers.insert(containersMap_t::value_type(mImages->getName(), mImages));

    String onStart = mConfig->getStartScript();

    if (onStart.length() > 0)
        executeCommand( onStart.c_str(), tintaExecutingTask::enLocalTask );

	return true;
}

void tintaTexSpringMain::clearContainers() {
  
    
    if (mImages)
        mImages->delAllImg();

    mBoxObjects.clear();

}
bool tintaTexSpringMain::gpuEnabled()const{
	return mConfig ? mConfig->getGpuEnabled() : false;
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
#else
	return NULL_M;
#endif
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




void tintaTexSpringMain::createFunctions(){

    clearContainers();

    m_systemFunctions.clear();   

    //system functions
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("units")), Tinta::tintaMainFunctions::units));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("units()")), Tinta::tintaMainFunctions::units));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("tasks")), Tinta::tintaMainFunctions::tasks));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("tasks()")), Tinta::tintaMainFunctions::tasks));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("status")), Tinta::tintaMainFunctions::status));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("status()")), Tinta::tintaMainFunctions::status));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("abort")), Tinta::tintaMainFunctions::abort));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("abort()")), Tinta::tintaMainFunctions::abort));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("state")), Tinta::tintaMainFunctions::state));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("state()")), Tinta::tintaMainFunctions::state));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("cinit")), Tinta::tintaMainFunctions::cinit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("cinit()")), Tinta::tintaMainFunctions::cinit));
	m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("ginit")), Tinta::tintaMainFunctions::ginit));
	m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("ginit()")), Tinta::tintaMainFunctions::ginit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("restart")), Tinta::tintaMainFunctions::restart));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("restart()")), Tinta::tintaMainFunctions::restart));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("reset")), Tinta::tintaMainFunctions::reset));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("reset()")), Tinta::tintaMainFunctions::reset));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("exit")), Tinta::tintaMainFunctions::exit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("exit()")), Tinta::tintaMainFunctions::exit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("x")), Tinta::tintaMainFunctions::exit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("x()")), Tinta::tintaMainFunctions::exit));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("cls")), Tinta::tintaMainFunctions::cls));
    m_systemFunctions.insert(sysfuncs_t::value_type(String(_M("cls()")), Tinta::tintaMainFunctions::cls));
    
   
     
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

        m_uint32 platform = mConfig->getGpuPlatform() > -1 ? (m_uint32)mConfig->getGpuPlatform() : 0;
        m_uint32 device = mConfig->getGpuDevice() > -1 ? (m_uint32)mConfig->getGpuDevice() : 0;
        tintaIClBase *obj = M_NEW Box2dProgram(fullPath, fullPath, kernel, platform, device);

        try {   

            obj->create();

            mGPUObjs->registerObject(obj);
            mGPUPrograms.push_back(obj);

            return true;
        }
        catch ( const Tinta::tintaException &e ) {
            if( Tinta::tintaLogger::getPtr() )
                Tinta::tintaLogger::getPtr()->logMsg( e.getDescr() );  
            M_DELETE obj;
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

    tintaUtilFunc::registerUtilLua(mScriptContext->getStateEx());
    Tinta::tintaMainFunctions::registerEngineLua(mScriptContext->getStateEx());
	
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

void tintaTexSpringMain::executeCommand(const String &command, tintaExecutingTask::TaskType type){
    executeCommand(0, command, type);
}

void tintaTexSpringMain::executeCommand( size_t unitId, const String &command, tintaExecutingTask::TaskType type ){

    auto it = m_systemFunctions.find(command);
    Tinta::tintaTaskQueue *q = Tinta::tintaTexSpringMain::getPtr()->getTaskQueue();

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

    const tintaExecutingUnit *unit = Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit();
    if ( !unit )
        return;

    m_ulong32 unId = unit->getId().mid;

    tintaAsioInteract* send = mInteract ? mInteract->getInteract() : NULL;

    bool localExecute = (unitId == 0); // all
    for (; unit != NULL; unit = Tinta::tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit(&unId)){

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

#ifndef TINTA_NO_INTERACT
tintaInteractUnitsSet* tintaTexSpringMain::getUnitSet() {
    CoreAssert(mInteractUnitSet, "mInteractUnitSet == NULL");
    return mInteractUnitSet;    
}
#endif


tintaImgStack *tintaTexSpringMain::getImageStack() {
    CoreAssert(mImages, "mImages == NULL");
    return mImages;
}

tintaTaskQueue *tintaTexSpringMain::getTaskQueue() {
    CoreAssert(mTasksQueue, "mTasksQueue == NULL");
    return mTasksQueue;
}

tintaUnitsSet  *tintaTexSpringMain::getUnitsSet() {
    CoreAssert(mWorkUnitsSet, "mWorkUnitsSet == NULL");
    return mWorkUnitsSet;
}

tintaBoxContainer *tintaTexSpringMain::getBoxContainer() {   
   return &mBoxObjects;
}

template<> Tinta::tintaTexSpringMain* Tinta::Singleton<tintaTexSpringMain>::mPtr = 0;



void signalHandler(int)
{
	appClosing = 1;
}

//#endif

