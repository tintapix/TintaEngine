/*  Copyright (C) 2011 - 2017 Evdokimov Mikhail
tintapix.com
tintapix@gmail.com  */

#ifndef _TINTA_MATERIAL_H_
#define _TINTA_MATERIAL_H_

#include "tintaGraphicaPredefine.h"
#include "GL/tintaGLSLProgram.h"
#include "tintaMaterialParser.h"
#include "tintaSamplerBuffer.h"

namespace Graphica {
    
    class tintaGLRenderer;

//     void for functions that do not return a value
//         bool a conditional type, taking on values of true or false
//         int a signed integer
//         uint an unsigned integer
//         float a single-precision floating-point scalar
//         double a double-precision floating-point scalar
//         vec2 a two-component single-precision floating-point vector
//         vec3 a three-component single-precision floating-point vector
//         vec4 a four-component single-precision floating-point vector
//         dvec2 a two-component double-precision floating-point vector
//         dvec3 a three-component double-precision floating-point vector
//         dvec4 a four-component double-precision floating-point vector
//         bvec2 a two-component Boolean vector
//         bvec3 a three-component Boolean vector
//         bvec4 a four-component Boolean vector
//         ivec2 a two-component signed integer vector
//         ivec3 a three-component signed integer vector
//         ivec4 a four-component signed integer vector
//         uvec2 a two-component unsigned integer vector
//         uvec3 a three-component unsigned integer vector
//         uvec4 a four-component unsigned integer vector
//         mat2 a 2?2 single-precision floating-point matrix
//         mat3 a 3?3 single-precision floating-point matrix
//         mat4 a 4?4 single-precision floating-point matrix
//         mat2x2 same as a mat2
//         mat2x3 a single-precision floating-point matrix with 2 columns and 3 rows
//         mat2x4 a single-precision floating-point matrix with 2 columns and 4 rows
//         mat3x2 a single-precision floating-point matrix with 3 columns and 2 rows
//         mat3x3 same as a mat3
//         mat3x4 a single-precision floating-point matrix with 3 columns and 4 rows
//         mat4x2 a single-precision floating-point matrix with 4 columns and 2 rows
//         mat4x3 a single-precision floating-point matrix with 4 columns and 3 rows
//         mat4x4 same as a mat4
//         dmat2 a 2?2 double-precision floating-point matrix
//         dmat3 a 3?3 double-precision floating-point matrix
//         dmat4 a 4?4 double-precision floating-point matrix
//         dmat2x2 same as a dmat2
//         dmat2x3 a double-precision floating-point matrix with 2 columns and 3 rows
//         dmat2x4 a double-precision floating-point matrix with 2 columns and 4 rows
//         dmat3x2 a double-precision floating-point matrix with 3 columns and 2 rows
//         dmat3x3 same as a dmat3
//         dmat3x4 a double-precision floating-point matrix with 3 columns and 4 rows
//         dmat4x2 a double-precision floating-point matrix with 4 columns and 2 rows
//         dmat4x3 a double-precision floating



    class _CoreExport tintaMaterial : public MaterialParseCallback {

public:

    /*
        Describes information about the material variable for packing it in to the array
    */
    struct  MaterialData{


        ~MaterialData(){}

        MaterialData() 
            :mValType(tintaMaterialParser::matSize),
             mAutoType(tintaMaterialParser::autoMaxElement)             
             {}

        MaterialData(tintaMaterialParser::MaterialDataType vType
                    ,tintaMaterialParser::MaterialAutoVar  aType)
            :mValType(vType),
            mAutoType(aType){}

        // Value type        
        tintaMaterialParser::MaterialDataType mValType;
        // AytoType Variable type        
        tintaMaterialParser::MaterialAutoVar  mAutoType;        
        //Packed value
        tintaBufferIO mValBuff;        
    };

    struct MaterialTexture {
        tintaTexture* mTexture;
        int           mUnit;
    };

    struct MaterialSBuffer {
        tintaSamplerBuffer* mBuffer;
        int           mUnit;
    };
     

    tintaMaterial(const String &path, tintaGLRenderer *renderer = NULL_M);

    tintaMaterial(const String &path, const String &name, tintaGLRenderer *renderer = NULL_M);

    tintaMaterial(const char *buff, m_uint32 len, const String &matName, tintaGLRenderer *renderer = NULL_M);

    tintaMaterial( );   
    
    bool assignVariable( const String &name, const  MaterialData *data );

    bool assignTexture( const String &name, tintaTexture* texture, int index  = 0);

    bool assignBuffer(const String &name, tintaSamplerBuffer* buff, int index, SBufferDraw type);

    const tintaTexture* getTextureByVariable( const String &variableName ) const ;

    int getTextureUnit(const String &variableName) const;

    bool removeTexture( const String &name );

    bool removeSBuffer(const String &name);


    virtual ~tintaMaterial( void );

    void setRenderer( tintaGLRenderer *renderer );
    
    String getName()const;

    void   setName( const String &name );

    tintaMaterialParser::MaterialDataType   getVarType(const String &variable) const ;

    void   setPasses(size_t passes);

    // already compiled and linked
    bool   binded() const;  

    // try to compile and link shaders
    void   bind(); 

    bool   enable(size_t pass);

    bool   disable(size_t pass);

    const tintaGLSLProgram* getProgram(size_t pass) const ;

    // dettach shaders
    void   unbind(); 

    bool   parse();
    


    template< class T >
    static MaterialData* allocateData(tintaMaterialParser::MaterialDataType valType, tintaMaterialParser::MaterialAutoVar  varType, const T &data) {
        MaterialData *m = NEW_T(MaterialData)(valType, varType);
        m->mValBuff.AllocateBuffer(sizeof(data));
        WriteToBuffer(m->mValBuff.GetBufferEx(), 0, data);
        return m;
    }


protected:

    //virtual void onAuto( const String  &varName, tintaMaterialParser::MaterialAutoVar  var, tintaMaterialParser::MaterialAutoVar autoVar );

    virtual void onFloat(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, float v, bool update = false);

    virtual void onDouble(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, double v, bool update = false);

    virtual void onUInt(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, m_uint32  v, bool update = false);

    virtual void onInt(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, int  v, bool update = false);

    virtual void onBool(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, bool  v, bool update = false);

    virtual void onfVec2(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, const tintaVector2f &v, bool update = false);

    virtual void onfVec3(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, const tintaVector3f &v, bool update = false);

    virtual void onfVec4(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, const tintaVector4f &v, bool update = false);

    virtual void onfMat3(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, const tintaMatrix33 &v, bool update = false);

    virtual void onfMat4(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, const tintaMatrix44 &v, bool update = false);

    virtual void onTexture(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, const tintaImage * img, int index, bool update = false);

    virtual void onSamplerBuffer(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, tintaSamplerBuffer* sb, int index, bool update = false);

      

        typedef std::map<StringBasic, MaterialData*> data_t;
        typedef std::map<StringBasic, MaterialTexture> textureData_t;

        typedef std::map<StringBasic, MaterialSBuffer> sBuffersData_t;

        typedef data_t::iterator data_t_it;        
        typedef std::pair<data_t_it, bool> pair_add_t;

        typedef std::vector<UNIQPTRDEF_T(tintaGLSLProgram)> tintaGLSLProgramVec_t;
        typedef tintaGLSLProgramVec_t::iterator tintaGLSLProgramit_t;


protected:


    
    /*
      Assigning variables to the shader program before enabling
    */
    bool assignVariables();

    bool assignTextures();

    bool assignBuffers();

    void clear();

    bool updateAuto(const StringBasic &name, MaterialData* data);

    template< class T >
    T handleVariable(const tintaBufferIO &Buff ){
        T val;
        ReadFromBuffer( Buff.GetBuffer(), 0, val);
        return val;
    }

    virtual void onVertexShaderName(const String &name);

    virtual void onFragmentShaderName(const String &name);

    virtual void onVertexShaderSrc(const String &src, const String &name);

    virtual void onFragmentShaderSrc(const String &src, const String &name);
        
    String mName;

    String mFullPath;
    // material file is parsed 
    bool mParsed;
    // materials shaders are binded
    bool mBinded;

    data_t mVariables;
    textureData_t mTextures;

    sBuffersData_t mSBuffers;

    String mBuffer;

    tintaGLSLProgramVec_t mGLSLPrograms;
    // extension for material file
    String mMaterialEx;   

    m_uint32 mTimeElapsed;

    String mVertexShaderName;
    String mFragmentShaderName;

    String mVertexShaderSrc;
    String mFragmentShaderSrc;

    tintaGLRenderer *mRenderer = NULL_M;

    //tintaSamplerBuffer mTestBuffer;

}; 


}

#endif