/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GLSL_SHADER_H_
#define _TINTA_GLSL_SHADER_H_

#include "tintaGraphicaPredefine.h"

namespace Graphica {

class _CoreExport tintaGLSLShader:
    public AllocatedMemObject,
    public tintaResource< GLSLProgramID >
{
public:
    tintaGLSLShader(void);

    tintaGLSLShader(const String &path, const String  &name);

    tintaGLSLShader(const char *buff, m_uint32 len, const String &name);


    virtual ~tintaGLSLShader(void);

    bool loadShader();

    bool loadShader( const String &path, const String  &name );

    bool loadShader(const char *buff, m_uint32 len, const String &name);

    bool compileShader( );   

    //String getName() const;

    // binding the shader
    virtual bool createShader() = 0;

    StringBasic getCompilerLog();    

    void deleteShader();
    
protected:
    bool     mCompiled;    
    GLubyte* mSrc;
    String   mName;
    String   mPath;
    String   mBuffer;
};

class tintaGLSLFragShader 
    : public tintaGLSLShader {
public:
        tintaGLSLFragShader();
        tintaGLSLFragShader(const String &path, const String  &name);
        tintaGLSLFragShader(const char *buff, m_uint32 len, const String &name);
        ~tintaGLSLFragShader();

        virtual bool createShader();

};


class tintaGLSLVertexShader 
        : public tintaGLSLShader {
public:
        tintaGLSLVertexShader();
        tintaGLSLVertexShader(const String &path, const String  &name);
        tintaGLSLVertexShader(const char *buff, m_uint32 len, const String &name);
        ~tintaGLSLVertexShader();
        virtual bool createShader();

};




}


#endif