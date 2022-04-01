/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaGLSLProgram.h"
#include "tintaRoot.h"
#include "tintaGraphicConfig.h"



namespace Graphica {


tintaGLSLProgram::tintaGLSLProgram(void)
:mLinked( false ),
 mCompiled( false ),
 mID(0){
}

tintaGLSLProgram::~tintaGLSLProgram(void)
{       

    //if (linker_log!=0) free(linker_log);
    unLinkShaders( true );
    //mShaders.clear();
      
    if( mID!= 0 ){

        glDeleteShader( mID );   
        tintaGLRenderer::printGLError();
    }        
      
}


bool tintaGLSLProgram::addShaders(const char *fragBuff, m_uint32 fragLen, const String &fragName,
    const char *vertBuff, m_uint32 vertLen, const String &vertName) {


    //tintaGLSLShader *fShader   = M_NEW tintaGLSLFragShader ();
    UNIQPTRVIRT(tintaGLSLShader, fShader, M_NEW(tintaGLSLFragShader)(fragBuff, fragLen, fragName));
    //tintaGLSLShader *vShader  = M_NEW tintaGLSLVertexShader ();
    UNIQPTRVIRT(tintaGLSLShader, vShader, M_NEW(tintaGLSLVertexShader)(vertBuff, vertLen, vertName));


    mShaders.push_back(std::move(fShader));
    mShaders.push_back(std::move(vShader));

    return true;

}

#if 0
bool tintaGLSLProgram::addShaders( const String &fragPath, const String &vertexPath ) {        
  
    

   /* String vFullPath;
    String fFullPath;
    bool rfind = tintaRoot::getPtr()->getConfigurator()->getShaderPath(vFullPath, vertexPath);
    rfind =     rfind && tintaRoot::getPtr()->getConfigurator()->getShaderPath(fFullPath, fragPath);
    if ( !rfind || !isFile(vFullPath) || !isFile(fFullPath) )
        return false;
        */

   //tintaGLSLShader *fShader   = M_NEW tintaGLSLFragShader ();
    UNIQPTRVIRT( tintaGLSLShader, fShader, M_NEW(tintaGLSLFragShader)( fFullPath, fragPath) );
   //tintaGLSLShader *vShader  = M_NEW tintaGLSLVertexShader ();
    UNIQPTRVIRT( tintaGLSLShader, vShader, M_NEW(tintaGLSLVertexShader)( vFullPath, vertexPath) );

      
    mShaders.push_back( std::move( fShader ) );
    mShaders.push_back( std::move( vShader ) );

    return true;
}
#endif


bool tintaGLSLProgram::compileShaders( ) {
    if (mCompiled)
        return true;

    glslShadersit_t i = mShaders.begin();
    for (; i != mShaders.end(); i++) {
        if (!(*i)->createShader() || !(*i)->compileShader()) {
            (*i)->deleteShader();
            return false;
        }
    }    

    mCompiled = true;

    return mCompiled;
}

bool tintaGLSLProgram::isCompiled() const {
    return mCompiled;
}

bool tintaGLSLProgram::isLinked() const{
    return mLinked;
}
bool tintaGLSLProgram::enable(){
    glUseProgram( mID );
    return tintaGLRenderer::printGLError() == 0;   
}

bool tintaGLSLProgram::disable(){
    glUseProgram(0);
    

    return tintaGLRenderer::printGLError() == 0;
}



void tintaGLSLProgram::unLinkShaders( bool delShaders ){

    if ( mLinked ){       

        glslShadersit_t i = mShaders.begin();

        for ( ;i!= mShaders.end(); i++ ){

            GLSLProgramID * id = (*i)->GetId();
            if (id) {
                glDetachShader(mID, id->mId);
                tintaGLRenderer::printGLError();
            }
        }

        if( delShaders ){
            glslShadersit_t e = mShaders.begin();
            for ( ;e!= mShaders.end(); e++ ){
                (*e)->deleteShader();
                tintaGLRenderer::printGLError();
            }
        }

        glDeleteProgram(mID);
        tintaGLRenderer::printGLError();
        mID = 0;
        mShaders.clear();
    }
}

bool tintaGLSLProgram::linkShaders( ){

    if ( !mCompiled )
        return false;

    tintaGLRenderer *r = tintaRoot::getPtr()->getRenderer();
    if ( !r || !r->glewInited() ) {
        Tinta::tintaLogger::getPtr()->logMsg("Glew is not inited");
        return false;
    }     
    int err(0);
    if( mID == 0 ){
        mID = glCreateProgram();
        err = tintaGLRenderer::printGLError();
    }

    if ( mLinked ){
        unLinkShaders();
    }

    if ( err == 0 ) {
        glslShadersit_t i = mShaders.begin();
        for (; i != mShaders.end(); i++) {

            GLSLProgramID *id = (*i)->GetId();
            if (id) {
                glAttachShader(mID, id->mId);
                err = tintaGLRenderer::printGLError();
                if (err != 0)
                    break;
            }
        }
    }

    GLint linked(0);

    if (err == 0) {

        glLinkProgram(mID);

        err = tintaGLRenderer::printGLError();
    }
    
    if (err == 0)
        glGetProgramiv(mID, GL_LINK_STATUS, &linked);
    err = tintaGLRenderer::printGLError();


    mLinked = linked > 0;
   
    if( !mLinked )        
       Tinta::tintaLogger::getPtr()->logMsg( getLinkerLog()  );
  

    return mLinked;
}


String tintaGLSLProgram::getLinkerLog(){

    GLint len(0);
    GLsizei slen(0);

    if (mID > 0) {
        glGetProgramiv(mID, GL_INFO_LOG_LENGTH , &len);
        tintaGLRenderer::printGLError();

        GLcharARB* log(NULL_M);
        if ( len > 1 ) {    
            
            
            //log = (GLcharARB*)malloc(len);    

            log = (GLcharARB*)MALLOC_RAW( len );

            glGetProgramInfoLog( mID, len, &slen, log );
            tintaGLRenderer::printGLError();
        
            StringStreamBasic msg_;
            msg_<<(char*) log;

            String  text( toWideChar( msg_.str() ) );

            //free( log );
            FREE_T( log );

            return text;
        }   
    }
    return String();
}



bool tintaGLSLProgram::setUniform1f(const GLcharARB* name, GLfloat v, GLint index) {
    if ( !mLinked )
        return false;

    GLint loc;
    if ( name )
        loc = GetUniformLocation(name);
    else
        loc = index;

    if (loc == -1)
        return false;  

    glUniform1f( loc, v );
    return true;
}

bool tintaGLSLProgram::setUniform1i(const GLcharARB* name, GLint v, GLint index){

    if (!mLinked)
        return false;

    GLint loc;
    if ( name )
        loc = GetUniformLocation( name );
    else
        loc = index;

    if ( loc == -1 )
        return false;

    
    glUniform1i(loc, v);

    return true;
}

bool tintaGLSLProgram::setUniform1ui(const GLcharARB* name, GLuint v, GLint index){
    if (!mLinked)
        return false;

    GLint loc = -1;
    if (name)
        loc = GetUniformLocation(name);
    else
        loc = index;

    if (loc == -1)
        return false;

    glUniform1ui(loc, v);
    return true;
}

bool tintaGLSLProgram::setUniform1d(const GLcharARB* name, GLdouble v, GLint index){

    if (!mLinked)
        return false;

    GLint loc;
    if (name)
        loc = GetUniformLocation(name);
    else
        loc = index;

    if (loc == -1)
        return false;

    glUniform1d(loc, v);
    return true;
}


bool tintaGLSLProgram::setUniform2fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index) {
    if (!mLinked)
        return false;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;

    glUniform2fv(loc, count, value);
    return true;
}

bool tintaGLSLProgram::setUniform2uiv(const GLcharARB* varname, GLsizei count, GLuint *value, GLint index) {
    if (!mLinked)
        return false;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;

    glUniform2uiv(loc, count, value);
    return true;
}


bool tintaGLSLProgram::setUniform2iv(const GLcharARB* varname, GLsizei count, GLint *value, GLint index) {
    if (!mLinked)
        return false;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;

    glUniform2iv(loc, count, value);
    return true;
}


bool tintaGLSLProgram::setUniform4fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index){
    if( !mLinked )
        return false;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc==-1) 
        return false; 

    glUniform4fv(loc, count, value);

    return true;
}

bool tintaGLSLProgram::setUniform3fv(const GLcharARB* varname, GLsizei count, GLfloat *v, GLint index)
{
    if( !mLinked )
         return false;

    GLint loc(0);
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc==-1) 
        return false;  

    glUniform3fv(loc, count, v);

    return true;
}


bool tintaGLSLProgram::setUniform2dv(const GLcharARB* varname, GLsizei count, GLdouble *v, GLint index) {
    if (!mLinked)
        return false;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  

    glUniform2dv(loc, count, v);

    return true;
}


bool  tintaGLSLProgram::setUniform4dv(const GLcharARB* varname, GLsizei count, GLdouble *v, GLint index) {

    if (!mLinked)
        return false;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false; 

    glUniform4dv(loc, count, v);

    return true;
}

bool  tintaGLSLProgram::setUniform3dv(const GLcharARB* varname, GLsizei count, GLdouble *v, GLint index) {
    if (!mLinked)
        return false;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  

    glUniform3dv(loc, count, v);
    return true;
}

bool tintaGLSLProgram::setUniformMatrix3fv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *v, GLint index){
    if (!mLinked)
        return false;

    GLint loc(0);
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;

    glUniformMatrix3fv(loc, count, transpose, v);

    return true;
}

bool tintaGLSLProgram::setUniformMatrix3dv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLdouble *v, GLint index){
    if (!mLinked)
        return false;

    GLint loc(0);
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;

    glUniformMatrix3dv(loc, count, transpose, v);

    return true;
}

bool tintaGLSLProgram::setUniformMatrix4fv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *v, GLint index){
    if( !mLinked )
        return false;

    GLint loc(0);
    if (varname)
        loc = GetUniformLocation( varname );
    else
        loc = index;

    if (loc==-1) 
        return false; 

    glUniformMatrix4fv(loc, count, transpose, v);

    return true;
}

bool tintaGLSLProgram::setUniformMatrix4dv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLdouble *v, GLint index){
    if (!mLinked)
        return false;

    GLint loc(0);
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  

    glUniformMatrix4dv(loc, count, transpose, v );

    return true;
}

GLint tintaGLSLProgram::GetUniformLocation(const GLcharARB *name)
{
    GLint loc(0);

    loc = glGetUniformLocation( mID, name );
    if (loc == -1) 
    {
        cout << "Error: can't find uniform variable \"" << name << "\"\n";
    }
    tintaGLRenderer::printGLError();
    return loc;
}



// bool tintaGLSLProgram::compileProgram( ){    
// 
//     mCompiled = false;
// 
//     GLint compiled = 0;
// 
//     if (!mSrc) return false;
// 
//     GLint	length = (GLint) strlen( (const char*)mSrc );
// 
//     glShaderSourceARB(ID, 1, (const GLcharARB **)&mSrc, &length);
//     //CHECK_GL_ERROR();
//     glCompileShaderARB(ID); 
//     //CHECK_GL_ERROR();
//     glGetObjectParameterivARB(ID, GL_COMPILE_STATUS, &compiled);
//     //CHECK_GL_ERROR();
//     if (compiled) 
//         mCompiled = true;
// 
//     return mCompiled;
// }

}