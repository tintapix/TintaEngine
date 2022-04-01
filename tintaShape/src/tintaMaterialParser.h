/*  Copyright (C) 2011 - 2017 Evdokimov Mikhail
tintapix.com
tintapix@gmail.com  */

#ifndef _TINTA_MATERIAL_PARSER_H_
#define _TINTA_MATERIAL_PARSER_H_

#include "tintaGraphicaPredefine.h"
#include "GL/tintaGLSLProgram.h"
#include "tintaSamplerBuffer.h"
#include "tintaTreeConfig/tintaTreeConfig.h"

namespace Graphica {

struct MaterialParseCallback;
namespace mf = TreeConfig;

class _CoreExport tintaMaterialParser {

public:
    
    // mostly corresponding to GLSL types
    enum MaterialDataType {
        matBool,
        matInt,
        matUInt,
        matFloat,
        matDouble,
        matbVec2,
        matbVec3,
        matbVec4,
        matiVec2,
        matiVec3,
        matiVec4,
        matuVec2,
        matuVec3,
        matuVec4,
        matfVec2,
        matfVec3,
        matfVec4,
        matdVec2,
        matdVec3,
        matdVec4,
        matfMat2,
        matdMat2,
        matfMat3,
        matdMat3,
        matfMat4,
        matdMat4,         
        matTexture,
        matSamplerBuffer,
        matSize
    };    

    enum MaterialVarType {
        // Setting value manually
        varManual,
        // Setting value automatically 
        varAuto,
    };

    // Auto Variables in materials
    // fill MaterialAutoTypes
    enum MaterialAutoVar {

        autoCameraView,
        autoWorldMatrix,
        autoViewMatrix,
        autoProjectMatrix,
		autoTimeElapsed,
        autoCameraPosition,
        autoRandReal,        
        autoCenterPosition,
        autoRandInt,
       // autoTest,
        autoMaxElement
    };
  

    static String   MaterialDataTypeStr[ matSize ];

    static MaterialDataType MaterialAutoData[ autoMaxElement ];

    static String   MaterialAutoTypes[ autoMaxElement ];    
    
    tintaMaterialParser( );

    tintaMaterialParser( const String &matName );

    tintaMaterialParser(const char *buff, m_uint32 len, const String &matName );

    virtual ~tintaMaterialParser(void);

    void parse();   

    void setCallback( MaterialParseCallback *c );

private:

    typedef std::map<String, MaterialDataType> DataTypeNames_t;
    typedef std::map<String, MaterialAutoVar> DataAutoTypeNames_t;

    String mMatName;    

    String mBuffer;

    MaterialParseCallback  *mClb;

    DataTypeNames_t         mDataName;

    DataAutoTypeNames_t     mAutoNames;

     mf::tintaTreeConfig mConf;

     int mTextureCount = 0;

    bool parseName( const mf::tintaConfNode *node );   

    bool fillShaderData( const mf::tintaTreeConfig &conf, const mf::tintaConfNode *node );  



    bool onParseBool( const StringBasic  &varName,MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node );

    bool onParseFloat(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseUInt(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseInt(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseDouble(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsebVec2(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsebVec3(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsebVec4(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseiVec2(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseiVec3(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseiVec4(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseuVec2(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseuVec3(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseuVec4(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsefVec2(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsefVec3(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsefVec4(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsedVec2(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsedVec3(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsedVec4(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsefMat2(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsedMat2(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsefMat3(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsedMat3(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsefMat4(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParsedMat4(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node);

    bool onParseTexture(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node, int count );

    bool onParseSBuffer(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node, int count);
};

struct MaterialParseCallback {


    //virtual void onAuto( const String  &varName, tintaMaterialParser::MaterialAutoVar  var, tintaMaterialParser::MaterialAutoVar autoVar ) = 0;

    virtual void onFloat(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, float v, bool update = false) = 0;

    virtual void onDouble(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, double v, bool update = false) = 0;

    virtual void onUInt(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, m_uint32  v, bool update = false) = 0;

    virtual void onInt(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, int  v, bool update = false) = 0;

    virtual void onBool(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, bool  v, bool update = false) = 0;

   // virtual void onbVec2( const tintaVector2f &v) = 0;

   // virtual void onbVec3( ) = 0;

    //virtual void oniVec2( ) = 0;

   // virtual void oniVec3( ) = 0;

  //  virtual void oniVec4( ) = 0;

  //  virtual void onuVec2( ) = 0;

  //  virtual void onuVec3( ) = 0;

   // virtual void onuVec4( );

    virtual void onfVec2(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, const tintaVector2f &v, bool update = false) = 0;

    virtual void onfVec3(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, const tintaVector3f &v, bool update = false) = 0;

    virtual void onfVec4(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, const tintaVector4f &v, bool update = false) = 0;

    //virtual void ondVec2( );

   // virtual void ondVec3( );

    //virtual void ondVec4( );

    //virtual void onfMat2( const tintaMatrix33 &v);

    //virtual void ondMat2( );

    virtual void onfMat3(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, const tintaMatrix33 &v, bool update = false) = 0;

    //virtual void ondMat3( );

    virtual void onfMat4(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, const tintaMatrix44 &v, bool update = false) = 0;

    virtual void onTexture(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar type, const tintaImage * img, int index, bool update = false) = 0;

    virtual void onVertexShaderName( const String &name ) = 0;

    virtual void onVertexShaderSrc(const String &buffer, const String &name) = 0;

    virtual void onFragmentShaderName(const String &name ) = 0;

    virtual void onFragmentShaderSrc(const String &buffer, const String &name) = 0;

    virtual void onSamplerBuffer(const StringBasic  &varName,
        tintaMaterialParser::MaterialAutoVar  var, tintaSamplerBuffer* sb, int index, bool update = false) = 0;

    //virtual void ondMat4( );

};


}

#endif