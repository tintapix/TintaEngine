/*  Copyright (C) 2011 - 2017 Evdokimov Mikhail
tintapix.com
tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaMaterial.h"
#include "tintaRoot.h"
#include "GL/tintaGLRenderer.h"
#include "tintaGraphicConfig.h"
#include "tintaException.h"


namespace Graphica {

tintaMaterial::tintaMaterial(void){}

tintaMaterial::tintaMaterial( const String &path, tintaGLRenderer *renderer)
: mParsed( false ),
  mBinded( false ),
  mMaterialEx( _M("material") ),
  mTimeElapsed( 0 ),
  mRenderer( renderer ), 
  mFullPath ( path ) {
   
    mName = StringUtil::getGetFilenameFromPath(path);
}

tintaMaterial::tintaMaterial(const String &path, const String &name, tintaGLRenderer *renderer)
    : mParsed(false),
    mBinded(false),
    mMaterialEx(_M("material")),
    mTimeElapsed(0),
    mRenderer(renderer), 
    mFullPath (path),
    mName ( name ) {
}


tintaMaterial::tintaMaterial(const char *buff, m_uint32 len, const String &matName, tintaGLRenderer *renderer) 
   :mRenderer(renderer)
    , mName(matName)
{
    if ( buff && len > 0 ) {
        mBuffer = buff;
    }
}


tintaMaterial::~tintaMaterial( void )
{
    mRenderer = NULL_M;
    //unbind();
    clear();
}


void tintaMaterial::setRenderer(tintaGLRenderer *renderer) {
    CoreAssert(renderer, "renderer == NULL");
    mRenderer = renderer;
}

String tintaMaterial::getName()const{
    return mName;
}

void tintaMaterial::setPasses( size_t passes ) {
    mGLSLPrograms.clear();
    mGLSLPrograms.resize(passes);
}

tintaMaterialParser::MaterialDataType   tintaMaterial::getVarType(const String &variable) const {
    
    auto v = mTextures.find(variable);

    if (v != mTextures.end())
        return tintaMaterialParser::matTexture;

    auto var = mVariables.find(variable);

    if ( var != mVariables.end() )
        return var->second->mValType;

    return tintaMaterialParser::matSize;
}


void tintaMaterial::clear() {
    
    mGLSLPrograms.clear();

    data_t_it iv = mVariables.begin();
    for (; iv != mVariables.end(); iv++){
        DELETE_T( (iv->second), MaterialData );
    }

}

// already compiled and linked
bool tintaMaterial::binded() const {
    return mBinded;
}

// try to compile and link shaders
void tintaMaterial::bind() {

    if( mBinded )
        return;

    tintaGLSLProgramit_t i = mGLSLPrograms.begin();
    for( ;i!=mGLSLPrograms.end(); i++ ){        
        if ( !(*i)->compileShaders() || !(*i)->linkShaders() )
            return;
    }

    mBinded = true;   
}

bool tintaMaterial::enable( size_t pass ){


//#define TEST_SAMPLER

    CoreAssert(pass < mGLSLPrograms.size(), _M("wrong pass") );
#if defined     (TEST_SAMPLER)
    if ( mTestBuffer.elements() == 0 ) {
        mTestBuffer.create(2);    
        std::srand(std::time(nullptr));
        //assert(rez);
    }   

     // use current time as seed for random generator
    int random_variable = std::rand();
  //  std::cout << "Random value on [0 " << RAND_MAX << "]: "
   //     << random_variable << '\n';

    //mTestBuffer.setValue(0, { float(random_variable) / float(RAND_MAX), float(random_variable) / float(RAND_MAX), float(random_variable) / float(RAND_MAX) , 1.f });
    //mTestBuffer.setValue(1, { float(random_variable) / float(RAND_MAX), float(random_variable) / float(RAND_MAX), float(random_variable) / float(RAND_MAX) , 1.f });

    mTestBuffer.setValue(0, { 0.9f, 0.7f,0.f , 1.f });
    mTestBuffer.setValue(1, { 0.6f, 0.7f,0.f , 1.f });
   
    bool rez = mGLSLPrograms.at(pass)->enable() && assignVariables() && assignTextures();

    rez = rez &&  mGLSLPrograms[0]->setUniform1i(_M("testBuffer"), 2);
    
    mRenderer->enableSBuffer(&mTestBuffer, 2 );    

    
    return true;
#else
    return mGLSLPrograms.at( pass )->enable() && assignVariables() && assignTextures() && assignBuffers();
#endif
}

bool tintaMaterial::disable( size_t pass ){
    CoreAssert(pass < mGLSLPrograms.size(), _M("wrong pass") );
        
    if( mRenderer )
        mRenderer->setStateAlpha(false);
    return mGLSLPrograms.at(pass)->disable();
}

const tintaGLSLProgram* tintaMaterial::getProgram(size_t pass) const {
    CoreAssert(pass < mGLSLPrograms.size(), _M("wrong pass") );
    return mGLSLPrograms.at(pass).get();
}

// dettach shaders
void tintaMaterial::unbind(){


    tintaGLSLProgramit_t i = mGLSLPrograms.begin();
    for( ;i!=mGLSLPrograms.end(); i++ ){               
        (*i)->unLinkShaders(true);
    }
    
    mBinded = false;    
}


bool   tintaMaterial::parse() {

    
    mParsed = true;

    tintaMaterialParser *parser = NULL_M;

    if (mBuffer.length() == 0) {
        parser = NEW_T(tintaMaterialParser)(mFullPath);
    }
    else
        parser = NEW_T(tintaMaterialParser)( mBuffer.c_str(), mBuffer.length(), mName );
    
    UNIQPTRDEF(tintaMaterialParser, parserPtr, parser);
    
    parser->setCallback(this);

    try {
        parserPtr->parse();
    }
    catch (const tintaException &ex) {
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(ex.getDescr(), msg_error);
        else
            stream_out << ex.getDescr() << _M("\n");
        mParsed = false;
    }
    size_t passes = 1;

    if ( mParsed ) {

        UNIQPTRDEF(tintaGLSLProgram, prog, NEW_T(tintaGLSLProgram)());

        try {           
            mParsed = prog->addShaders( mFragmentShaderSrc.c_str(), mFragmentShaderSrc.length(), mFragmentShaderName,
                                mVertexShaderSrc.c_str(), mVertexShaderSrc.length(), mVertexShaderName);
        }
        catch (const tintaException &ex) {
            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(ex.getDescr(), msg_error);
            else
                stream_out << ex.getDescr() << _M("\n");
            throw;
        }
        mGLSLPrograms.resize(passes);
        mGLSLPrograms[0].swap(prog);
    }
      

    return mParsed;
}



const tintaTexture* tintaMaterial::getTextureByVariable(const String &variableName) const {

    auto v = mTextures.find(variableName);

    if (v != mTextures.end()) {
        return v->second.mTexture;
    }
    return NULL_M;
}

int tintaMaterial::getTextureUnit(const String &variableName) const {
    auto v = mTextures.find(variableName);

    if (v != mTextures.end()) {
        return v->second.mUnit;
    }
    return -1;
}

bool tintaMaterial::assignTexture(const String &name, tintaTexture* texture, int index) {

    if (!texture || !mRenderer)
        return false;

    bool rez = true;

    CoreAssert(texture->getImage(), "Texture without image");
       
    if (texture->getImage()->getFormat() == IMG_RGBA)
        mRenderer->setStateAlpha(true);

    
    rez = rez && mGLSLPrograms[0]->setUniform1i(name.c_str(), index);

    mRenderer->enableTexture(texture, index);

    return rez;
}

bool tintaMaterial::assignBuffer(const String &name, tintaSamplerBuffer* buff, int index, SBufferDraw type) {

    if ( !buff || !mRenderer )
        return false;

    

    bool rez = mGLSLPrograms[0]->setUniform1i(name.c_str(), index);

    mRenderer->enableSBuffer(buff, index, type);

    return rez;
}

bool tintaMaterial::removeTexture( const String &name ) {
    bool rez = false;
    for ( auto &i : mTextures ) {
        if ( i.second.mTexture && i.second.mTexture->getName() == name ) {
            
            tintaTexture* dummy =  mRenderer->findTexture(tintaRoot::mDummyName);
            if( !dummy )
                dummy = mRenderer->addTexture( tintaRoot::getPtr()->addDummyImage(), tintaRoot::mDummyName);

            i.second.mTexture = dummy;
            rez = true;
        }
    }

    return rez;
}


bool tintaMaterial::removeSBuffer(const String &name) {
    return mSBuffers.erase(name) > 0 ;
}

bool tintaMaterial::assignTextures() {
    bool rez(true);
    {
        textureData_t::iterator itTex = mTextures.begin();
        for (; itTex != mTextures.end(); itTex++) {   
           // if(itTex->second.mTexture->isManual)
           rez = rez && assignTexture( itTex->first,itTex->second.mTexture, itTex->second.mUnit);
        }
    }
    return rez;
}


bool  tintaMaterial::assignBuffers() {
    bool rez(true);
    {
        sBuffersData_t::iterator itBuff = mSBuffers.begin();
        for (; itBuff != mSBuffers.end(); itBuff++) {
            
            rez = rez && assignBuffer( itBuff->first, itBuff->second.mBuffer, itBuff->second.mUnit, itBuff->second.mBuffer->getType() );
        }
    }
    return rez;
}


bool tintaMaterial::assignVariable(const String &name, const  MaterialData *matData) {

    bool rez = true;

    auto data = matData->mValBuff.GetBuffer();

    switch (matData->mValType) {

    case tintaMaterialParser::matInt:
    {
        int v{ 0 };
        ReadFromBuffer(data, 0, v);
        rez = rez && mGLSLPrograms[0]->setUniform1i(name.c_str(), v);
    }
    break;
    case tintaMaterialParser::matUInt:

    {   m_uint32 v{ 0 };
    ReadFromBuffer(data, 0, v);
    rez = rez && mGLSLPrograms[0]->setUniform1ui(name.c_str(), v);
    }
    break;
    case tintaMaterialParser::matFloat:
    {
        float v{ 0.f };
        ReadFromBuffer(data, 0, v);
        rez = rez && mGLSLPrograms[0]->setUniform1f(name.c_str(), v);
    }
    break;
    case tintaMaterialParser::matDouble:

    {
        double v{ 0. };
        ReadFromBuffer(data, 0, v);
        rez = rez && mGLSLPrograms[0]->setUniform1d(name.c_str(), v);
    }
    break;
    case tintaMaterialParser::matfVec2:

        /*tintaVector2f r;
        ReadFromBuffer(it->second->mValBuff.GetBuffer(), 0, r);
        float v[2];
        v[0] = r.mx;
        v[1] = r.my;*/

        rez = rez && mGLSLPrograms[0]->setUniform2fv(name.c_str(), 1, (float*)data);
        break;
    case tintaMaterialParser::matfVec3:

        /* tintaVector3f r;
        ReadFromBuffer(it->second->mValBuff.GetBuffer(), 0, r);
        float v[3];

        v[0] = r.mx;
        v[1] = r.my;
        v[2] = r.mz;*/
        rez = rez && mGLSLPrograms[0]->setUniform3fv(name.c_str(), 1, (float*)data);
        break;
    case tintaMaterialParser::matfVec4:

        /* tintaVector4f r;
        ReadFromBuffer(it->second->mValBuff.GetBuffer(), 0, r);
        float v[4];
        v[0] = r.mx;
        v[1] = r.my;
        v[2] = r.mz;
        v[3] = r.mw;*/
        rez = rez && mGLSLPrograms[0]->setUniform4fv(name.c_str(), 1, (float*)data);
        break;
    case tintaMaterialParser::matfMat3:
        /*tintaMatrix33 r;
        ReadFromBuffer(it->second->mValBuff.GetBuffer(), 0, r);
        float v[9];
        size_t size = sizeof(v);
        mlMemcpy(v, size, (const float*)r, size);*/
        rez = rez && mGLSLPrograms[0]->setUniformMatrix3fv(name.c_str(), 1, GL_FALSE, (float*)data);
        break;

    case tintaMaterialParser::matfMat4:

        rez = rez && mGLSLPrograms[0]->setUniformMatrix4fv(name.c_str(), 1, GL_FALSE, (float*)data);
        break;
        /*case tintaMaterialParser::matTexture:
        {
        m_int32 i = 0;
        ReadFromBuffer(it->second->mValBuff.GetBuffer(), 0, i);

        rez = rez && mGLSLPrograms[0]->setUniform1i(it->first.c_str(), i);

        }
        break;*/
    default:
        break;

    }

    return rez;
}



bool tintaMaterial::assignVariables() {
       
    bool rez = true;

    data_t::iterator it =  mVariables.begin();    

    for( ;it != mVariables.end(); it++ ){
        
        if ( it->second->mAutoType != tintaMaterialParser::autoMaxElement )
            rez = rez && updateAuto( it->first, it->second );

       rez = rez && assignVariable( it->first, it->second );  

    }
    return rez;
}

bool tintaMaterial::updateAuto( const StringBasic &name,MaterialData* data) {

    CoreAssert( data , _M("NULL data") );

    if ( !mRenderer )
        return false;

    tintaGLRenderer *r = mRenderer;

    bool rez(true);

    switch( data->mAutoType ){
        case tintaMaterialParser::autoCameraView:
        {
            tintaMatrix44 tintaCamera = r->getWorldMatrix() * r->getViewMatrix() * r->getProjMatrix();
            if ( data->mValBuff.GetSize() > 0 )
                WriteToBuffer(data->mValBuff.GetBufferEx(), 0, tintaCamera);

            break;
        }
        case tintaMaterialParser::autoWorldMatrix:
        {
            tintaMatrix44 m = r->getWorldMatrix();
            if (data->mValBuff.GetSize() > 0)
                WriteToBuffer(data->mValBuff.GetBufferEx(), 0, m);

            break;
        }   
        case tintaMaterialParser::autoViewMatrix:
        {
            tintaMatrix44 m = r->getViewMatrix();
            if (data->mValBuff.GetSize() > 0)
                WriteToBuffer(data->mValBuff.GetBufferEx(), 0, m);

            break;
        }
        case tintaMaterialParser::autoProjectMatrix:
        {
            tintaMatrix44 m = r->getProjMatrix();
            if (data->mValBuff.GetSize() > 0)
                WriteToBuffer(data->mValBuff.GetBufferEx(), 0, m);

            break;
        }
	    case tintaMaterialParser::autoTimeElapsed:
	    {							
           
            // main timer elapsed
            float v = (float)r->getTimeElapsed();
            
		    if ( data->mValBuff.GetSize() > 0 )
                WriteToBuffer( data->mValBuff.GetBufferEx(), 0, v );
            break;
	    }
		    
        case tintaMaterialParser::autoCameraPosition:
        {
            // world camera position            
            const tintaCamera * cam = r->getCamera();
            const tintaVector3f &pos = cam->getPosition();
            if (data->mValBuff.GetSize() > 0)
                WriteToBuffer(data->mValBuff.GetBufferEx(), 0, pos );
            break;
        }        
        case tintaMaterialParser::autoRandReal:
        {
            // random real [0, 1]
            float val = tintaRoot::getPtr()->getRandGen()->randomFloat();
            if (data->mValBuff.GetSize() > 0)
                WriteToBuffer(data->mValBuff.GetBufferEx(), 0, val);
            break;
        }
        case tintaMaterialParser::autoCenterPosition:
        {
            // world position
            const tintaGeometry * geom = r->getGeometry();
            const tintaVector3f &pos = geom->getPosition();
            if (data->mValBuff.GetSize() > 0)
                WriteToBuffer(data->mValBuff.GetBufferEx(), 0, pos);
            break;
        }  
        case tintaMaterialParser::autoRandInt:
        {
            // random integer
            int val = tintaRoot::getPtr()->getRandGen()->randomInt();           
            if ( data->mValBuff.GetSize() > 0 )
                WriteToBuffer( data->mValBuff.GetBufferEx(), 0, val );
            break;
        }
        default:
                rez = false;
                break;    
    }
    return rez;

    
}



//  void tintaMaterial::onAuto( const String  &varName, 
//                                      tintaMaterialParser::MaterialVarType  var, 
//                                      tintaMaterialParser::MaterialAutoVar  autoVar ){
//          //TODO Smart pointer
//  //         MaterialData *m = NEW_T (MaterialData)(  var, tintaMaterialParser::varAuto );
//  //         m->mValBuff.AllocateBuffer( sizeof() );
//  //         mVariables.insert( data_t::value_type( varName, m ) );     
//  }

void tintaMaterial::onFloat(const StringBasic  &varName,
    tintaMaterialParser::MaterialAutoVar  varType, float v, bool update){
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matFloat, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matFloat, varType, v)));

}

void tintaMaterial::onDouble(const StringBasic  &varName,
    tintaMaterialParser::MaterialAutoVar  varType, double v, bool update){
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matDouble, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matDouble, varType, v)));
}

void tintaMaterial::onUInt(const StringBasic  &varName,
       tintaMaterialParser::MaterialAutoVar  varType, m_uint32  v, bool update){
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matUInt, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matUInt, varType, v)));
}

void tintaMaterial::onInt(const StringBasic  &varName,
     tintaMaterialParser::MaterialAutoVar  varType, int  v, bool update){
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matInt, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matInt, varType, v)));
}

void tintaMaterial::onBool(const StringBasic  &varName,
    tintaMaterialParser::MaterialAutoVar  varType, bool  v, bool update){
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matBool, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matBool, varType, v)));
}   
void tintaMaterial::onfVec2(const StringBasic  &varName,
    tintaMaterialParser::MaterialAutoVar  varType, const tintaVector2f &v, bool update) {
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matfVec2, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matfVec2, varType, v)));
}

void tintaMaterial::onfVec3(const StringBasic  &varName,
    tintaMaterialParser::MaterialAutoVar  varType, const tintaVector3f &v, bool update){
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matfVec3, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matfVec3, varType, v)));
}

void tintaMaterial::onfVec4(const StringBasic  &varName,
   tintaMaterialParser::MaterialAutoVar  varType, const tintaVector4f &v, bool update){

    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matfVec4, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matfVec4, varType, v)));
}

void tintaMaterial::onfMat3(const StringBasic  &varName,
   tintaMaterialParser::MaterialAutoVar  varType, const tintaMatrix33 &v, bool update){
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matfMat3, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matfMat3, varType, v)));
}

void tintaMaterial::onfMat4(const StringBasic  &varName,
   tintaMaterialParser::MaterialAutoVar  varType, const tintaMatrix44 &v, bool update){
    if (update) {
        auto toUpdate = mVariables.find(varName);
        toUpdate->second = allocateData(tintaMaterialParser::matfMat4, varType, v);
    }
    else
        mVariables.emplace(data_t::value_type(varName, allocateData(tintaMaterialParser::matfMat4, varType, v)));
}

void tintaMaterial::onTexture( const StringBasic  &varName,
    tintaMaterialParser::MaterialAutoVar  var, const tintaImage * img, int index, bool update) {
    
    if ( img && mRenderer ) {

        tintaTexture *texture = mRenderer->addTexture( img );       

        // if material texture parameter changes
        if ( update ) {  
            auto toUpdate = mTextures.find(varName);
            toUpdate->second = { texture, toUpdate->second.mUnit };
        }
        else
            mTextures.emplace(textureData_t::value_type(varName, { texture, index }));
    }
}

void tintaMaterial::onSamplerBuffer(const StringBasic  &varName,
    tintaMaterialParser::MaterialAutoVar  var, tintaSamplerBuffer* sb, int index, bool update ) {

    if ( sb ) {

        // if material sampler buffer parameter changes
        if (update) {
            auto toUpdate = mSBuffers.find(varName);
            toUpdate->second = { sb, toUpdate->second.mUnit };
        }
        else 
            mSBuffers.emplace(sBuffersData_t::value_type(varName, { sb, index }) );
    }
}


void tintaMaterial::onVertexShaderName(const String &name){

    String vFullPath;
    tintaRoot::getPtr()->getConfigurator()->getShaderPath(vFullPath, name);
    
    StringBasic srs;
    if (!readUTF8Text(vFullPath, srs) || name.length() == 0) {

        StringStream s;
        s << _M("wrong path: ") << vFullPath << _M(" for shader ") << name;
        EXCEPTION(s.str());

    }
    mVertexShaderName = name;
    mVertexShaderSrc = srs;
}

void tintaMaterial::onVertexShaderSrc(const String &buffer, const String &name) {
    mVertexShaderSrc = buffer;
    mVertexShaderName = name;
}

void tintaMaterial::onFragmentShaderName(const String &name){
    String fFullPath;
    tintaRoot::getPtr()->getConfigurator()->getShaderPath(fFullPath, name);

    StringBasic srs;
    if (!readUTF8Text(fFullPath, srs) || name.length() == 0) {

        StringStream s;
        s << _M("wrong path: ") << fFullPath << _M(" for shader ") << name;
        EXCEPTION(s.str());

    }
    mFragmentShaderName = name;
    mFragmentShaderSrc = srs;
}

void tintaMaterial::onFragmentShaderSrc(const String &buffer, const String &name) {
    mFragmentShaderSrc = buffer;
    mFragmentShaderName = name;
}

void tintaMaterial::setName( const String &name ){
    mName = name;
}


}
