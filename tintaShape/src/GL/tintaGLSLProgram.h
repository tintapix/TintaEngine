/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GLSL_PROGRAM_H_
#define _TINTA_GLSL_PROGRAM_H_


#include "tintaGraphicaPredefine.h"
#include "tintaGLSLShader.h"

namespace Graphica {

 class _CoreExport tintaGLSLProgram
{

public:
  
    tintaGLSLProgram(void);
    virtual ~tintaGLSLProgram(void);


    //bool addShaders(const String &fragPath, const String &vertexPath);

    bool addShaders( const char *fragBuff, m_uint32 fragLen, const String &fragName,
                                    const char *vertBuff, m_uint32 vertLen, const String &vertName );

    bool linkShaders( );
    // delShaders - if true calls glDeleteObjectARB fro all shaders
    void unLinkShaders( bool delShaders = false);

    String getLinkerLog();

    bool compileShaders( );

    bool isCompiled() const;

    bool isLinked() const;

    bool enable();

    bool disable();

    // shader ubiform variables access    

    bool       setUniform1f(const GLcharARB* name, GLfloat v, GLint index = -1);  

    bool       setUniform1i(const GLcharARB* name, GLint v, GLint index = -1);

    bool       setUniform1ui(const GLcharARB* name, GLuint v, GLint index = -1);

    bool       setUniform1d(const GLcharARB* name, GLdouble v, GLint index = -1);

    bool       setUniform2fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index = -1);

    bool       setUniform2uiv(const GLcharARB* varname, GLsizei count, GLuint  *value, GLint index = -1);

    bool       setUniform2iv(const GLcharARB* varname, GLsizei count, GLint  *value, GLint index = -1);

    bool       setUniform4fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index = -1);

    bool       setUniform3fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index = -1);

    bool       setUniform2dv(const GLcharARB* varname, GLsizei count, GLdouble *value, GLint index = -1);

    bool       setUniform4dv(const GLcharARB* varname, GLsizei count, GLdouble *value, GLint index = -1);

    bool       setUniform3dv(const GLcharARB* varname, GLsizei count, GLdouble *value, GLint index = -1);

    bool       setUniformMatrix3fv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index = -1);

    bool       setUniformMatrix3dv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLdouble *value, GLint index = -1);

    bool       setUniformMatrix4fv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index = -1);

    bool       setUniformMatrix4dv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLdouble *value, GLint index = -1);

protected:   
    
    typedef std::vector<UNIQPTRVIRT_T(tintaGLSLShader)> glslShaders_t;
    
    typedef glslShaders_t::iterator glslShadersit_t ;
    bool          mLinked;    
    bool          mCompiled;    
    glslShaders_t mShaders;
    m_uint32      mID; 
    GLint       GetUniformLocation(const GLcharARB *name);
};

}

#endif