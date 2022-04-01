/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_TEXSPRING_MAIN_H_
#define _TINTA_TEXSPRING_MAIN_H_

#include <tintaAsyncTimer.h>
#include <tintaPredefine.h>
#include <tintaStdHeaders.h>
#include <tintaConsoleOutput.h>
#include <tintaLogger.h>
#include <tintaCommon.h>
#include <tintaScriptContext.h>
#include <tintaImageFont.h>
#include <BoostAsioInteract/tintaInteractPool.h>
#include "tintaScriptTaskExecutor.h"
#include "tintaChildProc.h"
#include <tintaScriptFunc.h>
#include <tintaBoxContainer.h>
#include "tintaRandGen.h"
#include <OCL/tintaIClBase.h>
#include "tintaUnitsSet.h"
#include "tintaTaskQueue.h"
#include "tintaInteractUnitsSet.h"
#include "tintaIImgWindow.h"
#include "tintaConfigurator.h"
#include <tintaMemoryAllocator.h>


#ifdef _WIN32
int const sigClosed = SIGBREAK;
#else
int const sigClosed = SIGHUP;
#endif

static volatile  sig_atomic_t appClosing = 0;
_CoreExport void signalHandler(int);


namespace Tinta {

typedef std::deque< String > t_string_queue;	
typedef std::shared_ptr<tintaScriptContext> context_shared_t;

class ConsoleEditBase{

private:
    ConsoleEditBase(const ConsoleEditBase &rv);
    ConsoleEditBase& operator=(const ConsoleEditBase &rv) { return *this; }

protected:	
	//ConsoleEditBase(){}
    ConsoleEditBase(){ resetIter(); }
public:
	enum console_keys{EN_CONSOLE_KEY_ENTER, EN_CONSOLE_KEY_GETLAST};

protected:	
	t_string_queue			 mQueue;
	t_string_queue::iterator m_iter;
    const m_uint16  mCmdBufSize = 10;
public:
	void moveIter(){		
			if( mQueue.size() ){				
                if (m_iter == mQueue.end() || ++m_iter == mQueue.end() )
                    m_iter = mQueue.begin();															
			}		
	};

	// move pointer to first command in queue
	virtual void resetIter(){
        m_iter = mQueue.begin();
	}
	// refresh action depends of GUI
    void addCommand(const String &command){

        t_string_queue::iterator it = std::find(mQueue.begin(), mQueue.end(), command);
        if (command.size() > 0 && it == mQueue.end()) {
            if (mQueue.size() == mCmdBufSize){
                mQueue.pop_back();
                mQueue.push_front(command);
                resetIter();
            }
            else{
                mQueue.push_front(command);
                resetIter();
            }
        }

    }

    String getCommand(){
        if (m_iter == mQueue.end())
            return String();
        return *m_iter;

    }
	// command to execute depends of the GUI
    virtual void executeCommand(const String &command = _M(""), tintaExecutingTask::TaskType type = tintaExecutingTask::enCommonTask) = 0;

    virtual void executeCommand(size_t unitId, const String &command = _M(""), tintaExecutingTask::TaskType type = tintaExecutingTask::enCommonTask) = 0;


};



class tintaTexSpringMain	
	: public Singleton< tintaTexSpringMain > 
	, public ConsoleEditBase {
public:
	
       
	void registerFunction( Tinta::tintaScriptContext *context, tintaScriptFunc* obj ) {
		CoreAssert( context && obj, "context && obj" );

		if( context )
			context->regFunc(  obj->mFunc, obj->mfuncName.c_str() );
	}

	void registerFunction( tintaScriptFunc* obj ) {
		CoreAssert( obj,"obj" );
		registerFunction( mScriptContext, obj );
	}

public:   

	tintaTexSpringMain(bool bIsServ = true);

	virtual ~tintaTexSpringMain(void);
    
	static tintaTexSpringMain* getPtr( void )	{

        CoreAssert(mPtr, "mPtr == NULL");
		return mPtr;
	}   

    void clearContainers();
	
	bool	gpuEnabled() const ;

	virtual tintaIClBase* getGPUProgram( const String &uniqName );	

    virtual void executeCommand(const String &command = _M(""), tintaExecutingTask::TaskType type = tintaExecutingTask::enCommonTask);

    virtual void executeCommand(size_t unitId, const String &command = _M(""), tintaExecutingTask::TaskType type = tintaExecutingTask::enCommonTask);

	virtual Tinta::tintaScriptContext *getContext();
	
	// --- from molyCPrintConsole	---
    virtual bool initialize(const String &configName, tintaConsoleOutput * out, tintaScriptContext* context);

	virtual void clearConsole();

	virtual void exit();
    
	bool isAborted() const ;

	void setAborted(bool bAbort);

	// --- from TaskExecutedCallback ----
	//virtual void executionCompleted(String execution_text, bool result);

    virtual tintaThreadPool	*getThreadPool();
    	

	virtual void setServMode(bool server_mode);

	virtual bool isServMode() const;

	virtual NameGenerator*			getNameGen();

	virtual String					getAppName();

	virtual void					setAppName(const String &name);

	virtual TintaMath::tintaRandomV2*			getRandGen();

	// for clients
	virtual float					getUnitPrior();    

    //virtual void	setWindow(tintaIImgWindow* winp);

    virtual m_uint32 addFont( const String &name, float scaleX = 0, float scaleY = 0, const color_type &color = { 0,0,0,0 } );

    virtual bool setFontParameter( m_uint32 id, float sizeX, float sizeY, const color_type &color );

    virtual bool setFont( m_uint32 id );

    virtual  tintaFont* getFont( m_uint32 id );

    virtual  m_uint32 cloneFont(m_uint32 src);

    virtual  tintaFont* getFont();

    virtual  bool deleteFont( m_uint32 id );

    virtual  void deleteFonts();

#ifndef TINTA_NO_INTERACT
	// TODO 
	virtual tintaChildProc*			getInteract();
#endif

    virtual tintaISerialisableSet         *getSerialisableSet(const StringBasic& name);

    virtual const tintaISerialisableSet   *getSerialisableSet(const StringBasic& name) const;


    virtual const tintaFontContainer       *getFonts()const;
	
	void initGPU();

	virtual void reinitContext(Tinta::tintaScriptContext *scriptContext);

    virtual void resetTimer(m_uint32 period, const String &command );

    virtual void stopTimer();

    void deleteGPU( const String &program = _M("") );

    // process name const prefix
	static const String str_process_name_prefix;   

    bool registerCl( const String &program, const String &kernel );
#ifndef TINTA_NO_INTERACT
    Tinta::tintaInteractUnitsSet* getUnitSet( );
#endif

    tintaImgStack *getImageStack();

    tintaTaskQueue *getTaskQueue();

    tintaUnitsSet  *getUnitsSet();

    tintaBoxContainer *getBoxContainer();
protected:

    virtual void trySendSysCommand(const String &command, size_t unitId, void(*sysComFunc)() );

	void createFunctions();

	//void registerFunctions(Tinta::tintaScriptContext *context);	

    void timerExecute();
	
	typedef std::vector< tintaArrayBoxObjFactory* > compValBoxFactories_t;

	typedef std::vector< tintaIClBase* >		 gpuPrograms_t;

    typedef std::map<StringBasic, tintaISerialisableSet*>  containersMap_t;

	tintaConfigurator				*mConfig;	

	compValBoxFactories_t			 mValBoxFacVec;

    fontFactories_t				     mFontFacroties;

    tintaFontContainer				 mFonts;

	//Tinta::tintaLogger				 *mLog;	

	tintaImgStack			         *mImages;	

	Tinta::tintaScriptContext		 *mScriptContext;    
  

    //SHAREDPTRDEF mScriptContext; // (tintaScriptContext, mScriptContext, NULL);

	// default work queue receiving tasks to execute, main mechanism of multi threading
    tintaThreadPool					 *mThreadPool;

	Tinta::tintaScriptTaskExecutor    *mTaskExecutor;
#ifndef TINTA_NO_INTERACT
	tintaInteractPool				 *mInteractPool;

	tintaChildProc					 *mInteract;	
#endif

	tintaRandGen					 *mRanGen;

	size_t					 mCmdBufSize;

	

	tintaBoxContainer	     mBoxObjects;

    containersMap_t          mContainers;
		
	// for USING_GPUCL 
	tintaClObjectContainer   *mGPUObjs;	

	gpuPrograms_t			  mGPUPrograms;

	// client doesn`t receive responses from other processes
	// server as default
	bool                    mIsServMode;
    		
	// generating uniqal names in current process
	NameGenerator           m_nameGen;
	// using for unique name generating "MainApp" by default
	String                  mAppName;	
	
	TintaMath::tintaRandomV2				 mRandGen;	

	//tintaGPUExt					*gpuExt;

	tintaUnitsSet			    *mWorkUnitsSet;

	tintaTaskQueue			    *mTasksQueue;
#ifndef TINTA_NO_INTERACT
	tintaInteractUnitsSet	    *mInteractUnitSet;	
#endif

	bool						bIsAborting;

	typedef std::map<String, void (*)() > sysfuncs_t;

	sysfuncs_t                  m_systemFunctions;

    tintaAsyncTimer              mTimerExecute;

    String                       mTimerCommand;       
};

}

#endif



