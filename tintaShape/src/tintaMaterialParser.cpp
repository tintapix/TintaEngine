/*  Copyright (C) 2011 - 2017 Evdokimov Mikhail
    tintapix.com
    tintapix@gmail.com  
*/

#include "tintaGraphicaPrecompile.h"
#include "tintaMaterialParser.h"
#include "tintaSamplerBuffer.h"
#include "tintaRoot.h"
#include "tintaException.h"



namespace Graphica {


String tintaMaterialParser::MaterialDataTypeStr[matSize]  = {
    _M("bool"),
    _M("int"),
    _M("uint"),
    _M("float"),
    _M("double"),
    _M("bVec2"),
    _M("bVec3"),
    _M("bVec4"),
    _M("iVec2"),
    _M("iVec3"),
    _M("iVec4"),
    _M("uVec2"),
    _M("uVec3"),
    _M("uVec4"),
    _M("fVec2"),
    _M("fVec3"),
    _M("fVec4"),
    _M("dVec2"),
    _M("dVec3"),
    _M("dVec4"),
    _M("fMat2"),
    _M("dMat2"),
    _M("fMat3"),
    _M("dMat3"),
    _M("fMat4"),
    _M("dMat4"),
    _M("texture"), // texture
    _M("sbuffer")  // sampler buffer name
};
String tintaMaterialParser::MaterialAutoTypes[ autoMaxElement ]  = 
{ _M("cameraView"), 
  _M("worldMatrix"),
  _M("viewMatrix"),
  _M("projectMatrix"),
  _M("timeElapsed"), 
  _M("cameraPosition"), 
  _M("randReal"),
  _M("entityCenter"),
  _M("randInt"),
  //_M("autoTest") 
};


tintaMaterialParser::MaterialDataType tintaMaterialParser::MaterialAutoData[autoMaxElement] =
{ 
      MaterialDataType::matfMat4, // mat4 cameraView
      MaterialDataType::matfMat4, // mat4 worldMatrix
      MaterialDataType::matfMat4, // mat4 viewMatrix
      MaterialDataType::matfMat4, // mat4 projectMatrix
      MaterialDataType::matFloat, // float timeElapsed
      MaterialDataType::matfVec3, // vector3 cameraPosition
      MaterialDataType::matFloat, // float randReal
      MaterialDataType::matfVec3,  // entityCenter
      MaterialDataType::matInt, // int randInt
};


tintaMaterialParser::tintaMaterialParser(void)
:mClb(NULL_M){
}

tintaMaterialParser::tintaMaterialParser( const String &mMatName )
:mMatName( mMatName ),
 mClb( NULL_M ){
}

tintaMaterialParser::tintaMaterialParser( const char *buff, m_uint32 len, const String &matName ) 
    : tintaMaterialParser(matName) {
    if (buff && len > 0) {
        mBuffer = buff;
    }
}

tintaMaterialParser::~tintaMaterialParser(void){

}

bool tintaMaterialParser::parseName( const mf::tintaConfNode *node ) {

    if ( node->getName().length() == 0) {                

        Tinta::StringUtil::StrStreamType msg;        
        msg << _M("Error while parsing material file: ");
        msg << _M(" file ") << mMatName << _M("\n");       
        msg << _M("Empty node in section: ");
        msg << node->getParent()->getName();       

        EXCEPTION( msg.str() );
        return false;
    }

    if (node->getName() == _M("file"))
        return true;
    
    
    MaterialVarType  Vtype( varManual );
    
    
    StringVector tok = StringUtil::split(node->getName(), { _M("#") });

    if( tok.size() ==  0 ){

            return true;
    }

    if( tok[0] == _M("auto") ){
        Vtype = varAuto;
    }    
    else if( tok[0] == _M("manual") ){
        Vtype = varManual;
    }    
    else
        return true;  

    DataTypeNames_t::const_iterator it =  mDataName.find( tok[1] );

    StringBasic  varName;
    if ( it == mDataName.end() && Vtype == varManual ) {

        Tinta::StringUtil::StrStreamType msg;
        
        msg << _M("Error while parsing material file: ");
        msg << _M(" file ") << mMatName << _M("\n");       
        msg << _M("Wrong data type in: ");
        msg << node->getName();

        EXCEPTION(msg.str());
        return false;
    }
        
    MaterialDataType Dtype{ matSize };

    if( it != mDataName.end() )         
        Dtype = it->second;
    else {
        // auto may not have type
        // get auto variable name and type
        varName = tok[1];
    }

    // manual variable or manually typed auto
    if( varName.length() == 0 )
        varName = tok[2];
   

    MaterialAutoVar  autoVar{ autoMaxElement };      
  
    if( Vtype == varAuto ){

        // auto variable reserved name        
        if ( node && node->childQuantity() > 0) {

            String valAuto;

            node->getChild( 0 )->getVal( valAuto );
            DataAutoTypeNames_t::const_iterator itf = mAutoNames.find( valAuto );

            if( itf != mAutoNames.end() )
                autoVar = itf->second;            
           
            if ( autoVar != autoMaxElement ) {

                MaterialDataType data = MaterialAutoData[autoVar];

                if ( Dtype != matSize && data != Dtype ) {  // wrong manually typed auto - break

                    StringStream msg;
                    msg << _M("Error while parsing material file: ");
                    msg << _M(" file ") << mMatName << _M("\n");
                    msg << _M("Wrong data type for auto variable: ");
                    msg << toWideChar(varName);

                    EXCEPTION( msg.str() );
                    return false;
                }
                else
                    Dtype = data;                
            }
            else {

                StringStream msg;                
                msg << _M("Error while parsing material file: ");
                msg << _M(" file ") << mMatName << _M("\n");
                msg << _M( "Wrong auto variable: \"" );
                msg << valAuto <<_M(" \"");

                EXCEPTION( msg.str() );
                return false;
            }
        }       
    }        
    
    bool rez( false );
    switch( Dtype ) {

        case matBool:           
            rez = onParseBool(varName,Dtype, autoVar, node );
            break;
        case matInt:
            rez = onParseInt(varName, Dtype,autoVar, node );
            break;
        case matUInt:
            rez = onParseUInt(varName, Dtype,autoVar, node );
            break;
        case matFloat:
            rez = onParseFloat(varName, Dtype,autoVar, node );
            break;
        case matDouble:
            rez = onParseDouble(varName, Dtype,autoVar, node );
            break;
        case matbVec2:
            rez = onParsebVec2(varName, Dtype,autoVar, node );
            break;
        case matbVec3:
            rez = onParsebVec3(varName, Dtype,autoVar, node );
            break;
        case matbVec4:
            rez = onParsebVec4(varName, Dtype,autoVar, node );
            break;
        case matiVec2:
            rez = onParseiVec2(varName, Dtype,autoVar, node );
            break;
        case matiVec3:
            rez = onParseiVec3(varName,Dtype, autoVar, node );
            break;
        case matiVec4:
            rez = onParseiVec4(varName,Dtype, autoVar, node );
            break;
        case matuVec2:
            rez = onParseuVec2(varName, Dtype,autoVar, node );
            break;
        case matuVec3:
            rez = onParseuVec3(varName, Dtype,autoVar, node );
            break;
        case matuVec4:
            rez = onParseuVec4(varName, Dtype,autoVar, node );
            break;
        case matfVec2:
            rez = onParsefVec2(varName, Dtype,autoVar, node );
            break;
        case matfVec3:
            rez = onParsefVec3(varName, Dtype,autoVar, node );
            break;
        case matfVec4:
            rez = onParsefVec4(varName, Dtype,autoVar, node );
            break;
        case matdVec2:
            rez = onParsedVec2(varName, Dtype,autoVar, node );
            break;
        case matdVec3:
            rez = onParsedVec3(varName, Dtype,autoVar, node );
            break;
        case matdVec4:
            rez = onParsedVec4(varName, Dtype,autoVar, node );
            break;
        case matfMat2:
            rez = onParsefMat2(varName, Dtype, autoVar, node );
            break;
        case matdMat2:
            rez = onParsedMat2(varName, Dtype,autoVar, node );
            break;
        case matfMat3:
            rez = onParsefMat3(varName, Dtype,autoVar, node );
            break;
        case matdMat3:
            rez = onParsedMat3(varName, Dtype,autoVar, node );
            break;
        case matfMat4:
            rez = onParsefMat4(varName, Dtype,autoVar, node );
            break;
        case matdMat4:
            rez = onParsedMat4(varName, Dtype,autoVar, node );
            break;
        case matTexture:        
            rez = onParseTexture(varName, Dtype, autoVar, node, mTextureCount++ );
            break;
        case matSamplerBuffer:
            rez = onParseSBuffer(varName, Dtype, autoVar, node, mTextureCount++);
            break;
        default :
            break;
    }    
    
    if ( !rez ) {

        Tinta::StringUtil::StrStreamType msg;
        msg << _M("Error while parsing material file: ");
        msg << _M(" file ") << mMatName << _M("\n");
        msg << "Error parsing variable: ";
        msg << toWideChar(varName);
        EXCEPTION( msg.str() );
    }

    return rez;
}

bool tintaMaterialParser::fillShaderData( const mf::tintaTreeConfig &conf, const mf::tintaConfNode *node ){
     
    size_t i = 0;
    
    const mf::tintaConfNode *nChild =  conf.getChild( i, node );
    bool r{ true };
    while( nChild ){
        
        if( !parseName( nChild ) ){           
            break;
        }
        i++;
        nChild =  conf.getChild( i, node );
    }    
    
        
    return r;
}


void tintaMaterialParser::parse() {
    if( mMatName.length() == 0 || ( mBuffer.length() == 0 && !isPathValid(mMatName ) ) ){
        EXCEPTION( _M("wrong material path!") );
    }
    
    if( mBuffer.length() > 0 )
        mConf.parse( mBuffer.c_str(), mBuffer.length() );
    else
        mConf.parse( mMatName );
    
    String e = mConf.getError( );
    if( e.length() > 0 ){

        Tinta::StringUtil::StrStreamType msg;
        msg << _M("Error while parsing material file: ");                
        msg << _M(" file ")<<mMatName<<_M("\n");        
        msg << e <<_M("\n");
        
        EXCEPTION( msg.str() );
    }
    
    mDataName.clear();
    for (auto i = 0; i < matSize; i++){
        mDataName.insert(DataTypeNames_t::value_type( MaterialDataTypeStr[i], (MaterialDataType)i ) );
    }

    mAutoNames.clear();
    for(auto i = 0; i < autoMaxElement;i++ ){
        mAutoNames.insert(DataAutoTypeNames_t::value_type( MaterialAutoTypes[i], (MaterialAutoVar)i ) );
    }   

    const mf::tintaConfNode *nVerex = mConf.getChild(_M("v_shader"));
    const mf::tintaConfNode *nFrag  = mConf.getChild(_M("f_shader"));

    if( !nVerex ){
        Tinta::StringUtil::StrStreamType msg;
        msg << _M("Error while parsing material file: ");
        msg << _M(" file ") << mMatName << _M("\n");
        msg << _M("Material parsing error: v_shader section was not found");

        EXCEPTION(msg.str());
    }        
    if( !nFrag ){
        Tinta::StringUtil::StrStreamType msg;
        msg << _M("Error while parsing material file: ");
        msg << _M(" file ") << mMatName << _M("\n");
        msg << _M("Material parsing error: f_shader section was not found");
    }   
    
    const mf::tintaConfNode * file = nVerex->getChild(_M("file"));
    String shaderData; 
    const mf::tintaConfNode *nameNode = file ? file->getChild(0) : NULL_M;

    if ( nameNode && nameNode->getVal(shaderData) ) {
        mClb->onVertexShaderName(shaderData);
    }
    else {
        file = nVerex->getChild(_M("src"));
        nameNode = file ? file->getChild(0) : NULL_M;

        if ( nameNode && nameNode->getVal(shaderData) ) {

            const mf::tintaConfNode * nameShaderNode = nVerex->getChild(_M("name"));
            String shaderName;

            nameNode = nameShaderNode ? nameShaderNode->getChild(0) : NULL_M;

            if (nameNode) {
                nameNode->getVal(shaderName);
            }

            mClb->onVertexShaderSrc(shaderData, shaderName);
        }
        else {
            Tinta::StringUtil::StrStreamType msg;
            msg << _M("Error while parsing material file: ");
            msg << _M(" file ") << mMatName << _M("\n");
            msg << _M("no \"file\" or \"src\" attribute for Vertex shader.");
            EXCEPTION(msg.str());
        }
    }

    file = nFrag->getChild(_M("file"));

    nameNode = file ? file->getChild(0) : NULL_M;
    if ( nameNode && nameNode->getVal(shaderData) ) {
        mClb->onFragmentShaderName(shaderData);
    }
    else {

        file = nFrag->getChild(_M("src"));
        nameNode = file ? file->getChild(0) : NULL_M;

        if ( nameNode && nameNode->getVal(shaderData) ) {

            const mf::tintaConfNode * nameShaderNode = nFrag->getChild(_M("name"));
            String shaderName;

            nameNode = nameShaderNode ? nameShaderNode->getChild(0) : NULL_M;

            if ( nameNode ) {
                nameNode->getVal(shaderName);
            }

            mClb->onFragmentShaderSrc(shaderData, shaderName);
        }
        else {
            Tinta::StringUtil::StrStreamType msg;
            msg << _M("Error while parsing material file: ");
            msg << _M(" file ") << mMatName << _M("\n");
            msg << _M("No \"file\" or \"src\" attribute for Fragment shader.");
            EXCEPTION(msg.str());
        }
    }   

    bool rez = fillShaderData( mConf, nVerex );
    if ( !rez ) {

        Tinta::StringUtil::StrStreamType msg;
        msg << _M("Vertex shader wrong data for material: ");
        msg << _M(" file ") << mMatName << _M("\n");     
        EXCEPTION(msg.str());
    }
    rez = fillShaderData(mConf, nFrag );

    if ( !rez ) {

        Tinta::StringUtil::StrStreamType msg;
        msg << _M("Fragment shader wrong data for material: ");
        msg << _M(" file ") << mMatName << _M("\n");
        EXCEPTION(msg.str());
    }    
}

void tintaMaterialParser::setCallback( MaterialParseCallback *c ) {
    CoreAssert( c, _M("Callback == NULL") ) ;
    mClb = c;
}


bool tintaMaterialParser::onParseBool(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node){
    bool v(false);
    bool r{ true };
    if ( node && var == autoMaxElement ) {

        const mf::tintaConfNode * nodeV = mConf.getChild(0, node);        
        if( nodeV )
            r = nodeV->getVal(v);
        else r = false;
    }
    
    CoreAssert(mClb, "Callback is NULL");
    if (mClb)        mClb->onBool(varName, var, v);
    return r;
}

bool tintaMaterialParser::onParseFloat(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node ) {
    double v(0.f);
    bool r(true);
    if ( node && var == autoMaxElement){
        const mf::tintaConfNode * nodeV = mConf.getChild(0, node);
        if ( nodeV )
            r = nodeV->getVal(v);
        else r = false;
    }
    CoreAssert(mClb, "Callback is NULL");
    if ( mClb )        mClb->onFloat(varName, var, (float)v);


    return r;
}

bool tintaMaterialParser::onParseInt(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node ){
    double v(0.);   
    bool r(true);
    if (node && var == autoMaxElement){
        const mf::tintaConfNode * nodeV = mConf.getChild(0, node);
        if ( nodeV )
            r = nodeV->getVal(v);
        else r = false;
    }
    CoreAssert(mClb, "Callback is NULL");
    if (mClb)        mClb->onInt(varName,var, (int)v);
    return r;
}

bool tintaMaterialParser::onParseUInt(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node ) {

    double v(0.);    

    bool r(true);
    if (node && var == autoMaxElement){
        const mf::tintaConfNode * nodeV = mConf.getChild(0, node);
        if ( nodeV )
            r = nodeV->getVal(v);
        else r = false;
    }
    CoreAssert(mClb, "Callback is NULL");

    if (mClb)        mClb->onUInt(varName,  var, (unsigned)v);

    return r;
}

bool tintaMaterialParser::onParseDouble(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node ){

    double v(0.);
    bool r(true);
    if ( node && var == autoMaxElement ){
        const mf::tintaConfNode * nodeV = mConf.getChild(0, node);
        if ( nodeV )
            r = nodeV->getVal(v);
        else r = false;
    }
    CoreAssert(mClb, "Callback is NULL");

    if ( mClb ) 
        mClb->onDouble(varName, var, v);
     return r;
}

bool tintaMaterialParser::onParsebVec2(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node ){    
    assert(false);
    return false;
}

bool tintaMaterialParser::onParsebVec3(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}
bool tintaMaterialParser::onParsebVec4(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var,const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParseiVec2(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParseiVec3(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParseiVec4(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParseuVec2(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParseuVec3(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParseuVec4(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParsefVec2(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node){

    bool r{ true };
   
    if( node && node->childQuantity() == 2 && var == autoMaxElement ){
        double v[2];
        const mf::tintaConfNode * nodeV = NULL_M;
        nodeV = mConf.getChild( 0, node );
        nodeV->getVal( v[0] );
        nodeV = mConf.getChild( 1, node );
        nodeV->getVal( v[1] );

        tintaVector2f vr( (float)v[0], (float)v[1] );
             
        CoreAssert(mClb, "Callback is NULL");
        if (mClb)
            mClb->onfVec2(varName,  var, vr );
            
    }    
    else
        if (mClb)         
            mClb->onfVec2(varName,  var, tintaVector2f());
    
    return r;
   
}

bool tintaMaterialParser::onParsefVec3(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node){

    bool r{ true };
  
        if( node && node->childQuantity() == 3 && var == autoMaxElement ){
            double v[3];

            const mf::tintaConfNode * nodeV = mConf.getChild(0, node );
            nodeV->getVal( v[0] );
            nodeV = mConf.getChild(1, node );
            nodeV && nodeV->getVal( v[1] );
            mConf.getChild(2, node );
            nodeV && nodeV->getVal( v[2] );

            tintaVector3f vr( (float)v[0], (float)v[1], (float)v[2] );

            CoreAssert(mClb, "Callback is NULL");

            if (mClb)            mClb->onfVec3(varName, var, vr);
        }   
        else
            if (mClb) mClb->onfVec3(varName,  var, tintaVector3f());
    

    return r;
}

bool tintaMaterialParser::onParsefVec4(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node){

    bool r{ true };
    
    if( node && node->childQuantity() == 4 && var == autoMaxElement ){
        double v[4];
        const mf::tintaConfNode * nodeV = mConf.getChild(0, node );
        nodeV->getVal( v[0] );
        nodeV = mConf.getChild(1, node );
        nodeV->getVal( v[1] );
        nodeV = mConf.getChild(2, node );
        nodeV->getVal( v[2] );
        nodeV = mConf.getChild(3, node );
        nodeV->getVal( v[3] );

        tintaVector4f vr( (float)v[0], (float)v[1], (float)v[2], (float)v[3] );

        CoreAssert(mClb, "Callback is NULL");

        if (mClb)            mClb->onfVec4(varName, var, vr);
    }   
    else
        if (mClb) mClb->onfVec4(varName, var, tintaVector4f());
    

    return r;
}

bool tintaMaterialParser::onParsedVec2(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){

    assert(false);
    return false;
}

bool tintaMaterialParser::onParsedVec3(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParsedVec4(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    assert(false);
    return false;
}

bool tintaMaterialParser::onParsefMat2(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){

    assert(false);
    return false;
}

bool tintaMaterialParser::onParsedMat2(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
     assert(false);
     return false;
}

bool tintaMaterialParser::onParsefMat3(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
    bool r{ true };

    

    if (node && node->childQuantity() == 9 && var == autoMaxElement ){
        double v[9];
        const mf::tintaConfNode * nodeV = mConf.getChild(0, node);
        nodeV->getVal(v[0]);
        nodeV = mConf.getChild(1, node);
        nodeV->getVal(v[1]);
        nodeV = mConf.getChild(2, node);
        nodeV->getVal(v[2]);
        nodeV = mConf.getChild(3, node);
        nodeV->getVal(v[3]);
        nodeV = mConf.getChild(4, node);
        nodeV->getVal(v[4]);
        nodeV = mConf.getChild(5, node);
        nodeV->getVal(v[5]);
        nodeV = mConf.getChild(6, node);
        nodeV->getVal(v[6]);
        nodeV = mConf.getChild(7, node);
        nodeV->getVal(v[7]);
        nodeV = mConf.getChild(8, node);
        nodeV->getVal(v[8]);

        tintaMatrix33 vr((float)v[0], (float)v[1], (float)v[2],
            (float)v[3], (float)v[4], (float)v[5],
            (float)v[6], (float)v[7], (float)v[8]);

        CoreAssert(mClb, "Callback is NULL");

        if (mClb)            mClb->onfMat3(varName, var, vr);
    }
     else 
         if (mClb) mClb->onfMat3(varName, var, tintaMatrix33());
           
    

     return r;
}

bool tintaMaterialParser::onParsedMat3(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){
     assert(false);
     return false;
}

bool tintaMaterialParser::onParsefMat4(const StringBasic  &varName,
    MaterialDataType type,MaterialAutoVar  var, const mf::tintaConfNode *node ){

    bool r{ true };

    if( node && node->childQuantity() == 16 && var == autoMaxElement){
        double v[16];

        const mf::tintaConfNode * nodeV = mConf.getChild(0, node );
        nodeV->getVal( v[0] );
        nodeV = mConf.getChild(1, node );
        nodeV->getVal( v[1] );
        nodeV = mConf.getChild(2, node );
        nodeV->getVal( v[2] );
        nodeV = mConf.getChild(3, node );
        nodeV->getVal( v[3] );
        nodeV = mConf.getChild(4, node );
        nodeV->getVal( v[4] );
        nodeV = mConf.getChild(5, node );
        nodeV->getVal( v[5] );
        nodeV = mConf.getChild(6, node );
        nodeV->getVal( v[6] );
        nodeV = mConf.getChild(7, node );
        nodeV->getVal( v[7] );
        nodeV = mConf.getChild(8, node );
        nodeV->getVal( v[8] );
        nodeV = mConf.getChild(9, node );
        nodeV->getVal( v[9] );
        nodeV = mConf.getChild(10, node );
        nodeV->getVal( v[10] );
        nodeV = mConf.getChild(11, node );        
        nodeV->getVal( v[11] );
        nodeV = mConf.getChild(12, node );        
        nodeV->getVal( v[12] );
        nodeV = mConf.getChild(13, node );        
        nodeV->getVal( v[13] );
        nodeV = mConf.getChild(14, node );        
        nodeV->getVal( v[14] );
        nodeV = mConf.getChild(15, node );        
        nodeV->getVal( v[15] );            

        tintaMatrix44 vr(  (float)v[0], (float)v[1], (float)v[2], (float)v[3],
                            (float)v[4], (float)v[5], (float)v[6], (float)v[7],
                            (float)v[8], (float)v[9], (float)v[10], (float)v[11],
                            (float)v[12], (float)v[13], (float)v[14], (float)v[15]);
                 

        CoreAssert(mClb, "Callback is NULL");

        if (mClb)            mClb->onfMat4(varName, var, vr);
    }  
    else
        if ( mClb ) mClb->onfMat4(varName, var, tintaMatrix44());
    

     return r;
}

bool tintaMaterialParser::onParseTexture( const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node, int count ) {

    if ( node && node->childQuantity() == 1
                                && var == autoMaxElement ) {

        String path;
        const mf::tintaConfNode * nodeV = mConf.getChild( 0, node );
        nodeV->getVal( path );

        tintaRoot  *root = Graphica::tintaRoot::getPtr();
        tintaImage *img = NULL_M;

        if ( path.length() > 0 ) {           
             img = root ? root->getImage( path ) : NULL_M;
        }
        else {            
            // temp image 1x1 black            
            img = root->addDummyImage();
        }

        if ( !img ) {
            Tinta::StringUtil::StrStreamType msg;
            msg << _M("Error while parsing material file: ") << mMatName << _M(" \n");
            msg << _M("Image with path: ") << path << _M(" was not found\n");            
            EXCEPTION(msg.str());            
        }

        CoreAssert( mClb, "Callback is NULL" );
        if (mClb)
            mClb->onTexture(varName, var, img, count);
    }

    return true;
}


bool tintaMaterialParser::onParseSBuffer(const StringBasic  &varName, MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node, int count) {
    if ( node && node->childQuantity() == 1
        && var == autoMaxElement ) {

        String name;
        const mf::tintaConfNode * nodeV = mConf.getChild( 0, node );

        nodeV->getVal( name );

        if ( name.size() == 0 ) {
            Tinta::StringUtil::StrStreamType msg;
            msg << _M("Error while parsing material file: ") << mMatName << _M(" \n");
            msg << _M("Sampler buffer wrong name");
            EXCEPTION(msg.str());
        }

        nodeV = mConf.getChild( 1, node );
               
        tintaRoot  *root = Graphica::tintaRoot::getPtr();

        tintaSamplerBuffer* buff = root->getSBuffer(name);

        if ( !buff) {
            Tinta::StringUtil::StrStreamType msg;
            msg << _M("Error while parsing material file: ") << mMatName << _M(" \n");
            msg << _M("Sampler buffer not created: ") << name;
            EXCEPTION(msg.str());
        }

        CoreAssert( mClb, "Callback is NULL" );
        if ( mClb )
            mClb->onSamplerBuffer( varName, var, buff, count );
    }

    return true;
}

bool tintaMaterialParser::onParsedMat4(const StringBasic  &varName,
    MaterialDataType type, MaterialAutoVar  var, const mf::tintaConfNode *node ){
     assert(false);
      return false;
}

}
