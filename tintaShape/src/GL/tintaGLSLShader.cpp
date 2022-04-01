/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaGLSLShader.h"
#include <tintaLogger.h>



namespace Graphica {

tintaGLSLShader::tintaGLSLShader(void):
mCompiled( false ),
mSrc( NULL_M ){
    createID();
    //ID = = glCreateProgram();
}

tintaGLSLShader::tintaGLSLShader(const String &path, const String  &name)
    :mCompiled(false),
    mSrc(NULL_M),
    mName(name),
    mPath(path) {
}

tintaGLSLShader::tintaGLSLShader(const char *buff, m_uint32 len, const String &name) :
    mCompiled(false) ,
    mName(name) {
    if (buff && len > 0) {
        mBuffer = buff;
    }
}


tintaGLSLShader::~tintaGLSLShader(void){
     if(mSrc)  FREE_T(mSrc);
     mSrc = NULL_M;
     deleteShader();    
}



bool tintaGLSLShader::loadShader( const char *buff, m_uint32 len, const String &name ) {
    mName = name;

    if ( !buff || len == 0 ) {
        StringStream s;
        s << _M("wrong path for shader ") << name;
        EXCEPTION(s.str());
    }

    if (mSrc)
        FREE_T(mSrc);

    mSrc = (GLubyte*)ALLOC_T(char, len + 1);

    strcpy((char *)mSrc, buff);

    return true;
}


bool tintaGLSLShader::loadShader( const String &path, const String  &name ){
     
     StringBasic srs;
     if( !readUTF8Text( path, srs ) || name.length() == 0 ){
 
         StringStream s;
         s<<_M("wrong path: ")<<path<<_M(" for shader ")<<name;
         EXCEPTION( s.str() );

     }
    return  loadShader(srs.c_str(), srs.length(), name);     
}

bool tintaGLSLShader::loadShader() {    
    return loadShader( mBuffer.c_str(), mBuffer.length(), mName );
}

/*
String tintaGLSLShader::getName() const {
    return mName;
}
*/

StringBasic tintaGLSLShader::getCompilerLog(){

    GLint len(0);
    GLsizei slen(0);

    if (mId > 0) {
        glGetShaderiv(GetId()->mId, GL_INFO_LOG_LENGTH, &len);
        tintaGLRenderer::printGLError( );

        GLcharARB* log(NULL_M);
        if ( len > 1 ) {    

            log = (GLcharARB*)malloc(len);
            if( log ) {

                glGetInfoLogARB(GetId()->mId, len, &slen, log);
                tintaGLRenderer::printGLError();
            }
        }


        if ( log ){
            StringStreamBasic msg_;
            msg_<<(char*) log;
            return msg_.str();
        }   
    }
    return StringBasic();
}

void tintaGLSLShader::deleteShader(){
   
        if ( GetId() ) {
            glDeleteObjectARB(GetId()->mId);           
            releaseID();

            tintaGLRenderer::printGLError();
        }

        mCompiled = false;
        
    
}


bool tintaGLSLShader::compileShader( ){
    if ( mCompiled )
        return true;    

    if( !loadShader() )
        return false;

    GLint compiled = 0;
    GLint	length = (GLint) strlen( (const char*)mSrc );

    glShaderSourceARB(GetId()->mId, 1, (const GLcharARB **)&mSrc, &length);
    int err = tintaGLRenderer::printGLError( );

    if( err == 0 )
        glCompileShaderARB(GetId()->mId);
    err = tintaGLRenderer::printGLError();

    if ( err == 0 )
        glGetObjectParameterivARB(GetId()->mId, GL_COMPILE_STATUS, &compiled);
    err = tintaGLRenderer::printGLError();

    if ( err == 0 && compiled )
        mCompiled = true;
    else {
        StringStream desc;
        desc << _M("Compile error shader: ") << mName <<_M("\n");
        Tinta::tintaLogger::getPtr()->logMsg( desc.str() );
        Tinta::tintaLogger::getPtr()->logMsg( getCompilerLog() );        
    }
    return mCompiled;
}



tintaGLSLFragShader::tintaGLSLFragShader(){
    //createID();
}

tintaGLSLFragShader::tintaGLSLFragShader(const String &path, const String  &name) 
    :tintaGLSLShader(path, name){
    //createID();
}


tintaGLSLFragShader::tintaGLSLFragShader(const char *buff, m_uint32 len, const String &name)
    : tintaGLSLShader(buff, len, name) {
    //createID();
}

 bool tintaGLSLFragShader::createShader() {
     if ( mCompiled )
         return true;

     if (!GetId())
         createID();

     GetId()->mId = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
     int err = tintaGLRenderer::printGLError();
     return err == 0;
}


tintaGLSLFragShader::~tintaGLSLFragShader(){
}


tintaGLSLVertexShader::tintaGLSLVertexShader(const String &path, const String  &name)
    :tintaGLSLShader(path, name) {
    //createID();
}

tintaGLSLVertexShader::tintaGLSLVertexShader(const char *buff, m_uint32 len, const String &name)
    : tintaGLSLShader(buff, len,name) {
    //createID();
}

tintaGLSLVertexShader::tintaGLSLVertexShader(){
    //createID();
}   


bool tintaGLSLVertexShader::createShader() {
    if (mCompiled)
        return true;

    if( !GetId() )
        createID();

    GetId()->mId = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    int err = tintaGLRenderer::printGLError();
    return err == 0;
}

tintaGLSLVertexShader::~tintaGLSLVertexShader(){

}





}
