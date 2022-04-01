/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaRoot.h"
#include "tintaDrawable.h"
#include "tintaCuller.h" 
#include <tintaException.h>
#include "tintaMaterial.h"
#include <tintaBufferIO.h>

namespace Graphica {




    tintaDrawable::tintaDrawable( tintaGLRenderer *rend ) {
       
        mCulling = CULL_NEVER;
        mpParent = 0;
        mRend = rend;
        mBound = M_NEW tintaSphereBV();
    }

    /*tintaDrawable::tintaDrawable(tintaGLRenderer *rend)
        : tintaDrawable() {
        mRend = rend;
    }*/

    tintaDrawable::~tintaDrawable() {

        for ( auto m = mMaterials.begin();
            m != mMaterials.end(); m++ ) {
            
            auto & vars = m->matVariables;
            for ( auto v = vars.begin(); v != vars.end(); v++ ) {
                DELETE_T(v->data, MaterialData );
            }            
        }
    }


    void tintaDrawable::setCullMode(tintaDrawable::CullingMode mode) {
        mCulling = mode;
    }


    void tintaDrawable::updateBound() {
        // updateWorldBound();
        boundToRoot();
    }

    void tintaDrawable::updateWorldData() {

    }

    void tintaDrawable::boundToRoot() {
        if (mpParent) {
          //  mpParent->updateWorldBound();
          //  mpParent->boundToRoot();
        }
    }

    const tintaGLRenderer* tintaDrawable::getRenderer() const {
        return mRend;
    }

    void tintaDrawable::setRenderer(tintaGLRenderer     *rend) {
        mRend = rend;
    }

    const String &tintaDrawable::getName() const {
        return mName;
    }
    void tintaDrawable::setName(const String &name) {
        mName = name;
    }
       

    void tintaDrawable::moveTo(const tintaVector3f& pos, bool bUpdateChildren, bool updateLocal) {
        
        tintaVector3f oldPos = getPosition();
        tintaSpatial::moveTo(pos, bUpdateChildren, updateLocal);

        ///std::cout << getName() << " Local: " << mLocalPosition.mx << " " << mLocalPosition.my << " " << mLocalPosition.mz << "\n";
        //std::cout << getName() << " World: " << getPosition().mx << " " << getPosition().my << " " << getPosition().mz << "\n";

        
        if ( !bUpdateChildren ) 
            return;
       
        for (auto i : mChildren) {
            i->moveOn( getPosition() - oldPos, true, false );
        }
    }

    /*void tintaDrawable::shift(const tintaVector3f& shiftVal) {

        mPosition += shiftVal;

        for (auto i : mChildren) {
            i->shift(shiftVal);
        }
    }*/


    void tintaDrawable::moveOn(const tintaVector3f& addPos, bool bUpdateChildren, bool updateLocal) {
        tintaSpatial::moveOn(addPos, bUpdateChildren, updateLocal);

        //std::cout << getName() << " Local: " << mLocalPosition.mx << " " << mLocalPosition.my << " " << mLocalPosition.mz << "\n";
        //std::cout << getName() << " World: " << getPosition().mx << " " << getPosition().my << " " << getPosition().mz << "\n";

        if (!bUpdateChildren)
            return;

        for (auto i : mChildren) {
            i->moveOn(addPos, true, false );
        }
    }
    void tintaDrawable::moveOnOrient(const tintaVector3f& addPos, bool bUpdateChildren, bool updateLocal) {
        tintaSpatial::moveOnOrient( addPos , bUpdateChildren, updateLocal);

        //std::cout << getName() << " Local: "  << mLocalPosition.mx << " " << mLocalPosition.my << " " << mLocalPosition.mz<<"\n";
        //std::cout << getName() << " World: "  << getPosition().mx << " " << getPosition().my << " " << getPosition().mz << "\n";
        if (!bUpdateChildren)
            return;
        
        for (auto i : mChildren ) {
            i->moveOnOrient(addPos, true, false);
        }
    }

    void tintaDrawable::move(MoveAxies ax, float val, bool bUpdateChildren, bool updateLocal) {
        tintaSpatial::move(ax,val, bUpdateChildren , updateLocal );

        //std::cout << getName() << " Local: " << mLocalPosition.mx << " " << mLocalPosition.my << " " << mLocalPosition.mz << "\n";
        //std::cout << getName() << " World: " << getPosition().mx << " " << getPosition().my << " " << getPosition().mz << "\n";

        if ( !bUpdateChildren )
            return;

        for (auto i : mChildren) {
            i->move(ax, val, true, false);
        }
    }


    void tintaDrawable::roll(float angle) {
        tintaSpatial::roll(angle);

        if (mChildren.size() == 0)
            return;

        for (auto i : mChildren) {
            i->roll(angle);
        }
    }

    void tintaDrawable::pitch(float angle) {
        tintaSpatial::pitch(angle);
        if (mChildren.size() == 0)
            return;

        for (auto i : mChildren) {
            i->pitch(angle);
        }
    }

    void tintaDrawable::yaw(float angle) {
        tintaSpatial::yaw(angle);
        if (mChildren.size() == 0)
            return;

        for (auto i : mChildren) {
            i->yaw(angle);
        }
    }

    void tintaDrawable::rotate(const tintaVector3f& axis, float angle, bool bUpdateChildren, bool updateLocal) {
        tintaSpatial::rotate(axis,angle, bUpdateChildren, updateLocal);

        //std::cout << getName() << " Local: " << mLocalPosition.mx << " " << mLocalPosition.my << " " << mLocalPosition.mz << "\n";
        //std::cout << getName() << " World: " << getPosition().mx << " " << getPosition().my << " " << getPosition().mz << "\n";
        if (!bUpdateChildren)
            return;
        for (auto i : mChildren) {
            i->rotate(axis, angle, true, false);
        }
    }


    void tintaDrawable::mulScale(const tintaVector3f& scale, bool bUpdateChildren  ) {
        
        tintaSpatial::mulScale( scale, bUpdateChildren );

        if ( !bUpdateChildren )
            return;
        
        for ( auto i : mChildren ) {          
               i->mulScale( scale, true);
        }
    }

    void tintaDrawable::setScale(const tintaVector3f& scale, bool bUpdateChildren) {

        tintaSpatial::setScale(scale, bUpdateChildren);

        if (!bUpdateChildren)
            return;

        for (auto i : mChildren) {
            i->setScale(scale, true);
        }
    }

    void tintaDrawable::rotate(const tintaQuatern& q, bool bUpdateChildren, bool updateLocal) {
        tintaSpatial::rotate(q,false , updateLocal);
                
        if (!bUpdateChildren)
            return;

        for (auto i : mChildren) {
            i->rotate(q, true, false);            
        }
    }

    void   tintaDrawable::setOrientation(const tintaQuatern &orient, bool bUpdateChildren ) {
        tintaSpatial::setOrientation(orient, bUpdateChildren);

        //std::cout << getName() << " Local: " << mLocalPosition.mx << " " << mLocalPosition.my << " " << mLocalPosition.mz << "\n";
        //std::cout << getName() << " World: " << getPosition().mx << " " << getPosition().my << " " << getPosition().mz << "\n";

        if (!bUpdateChildren )
            return;

        for (auto i : mChildren) {
            i->setOrientation(orient, true);
        }
    }

    void  tintaDrawable::updateOrientation(const tintaQuatern &orient, bool bUpdateChildren, bool updateLocal) {
         tintaSpatial::updateOrientation(orient, bUpdateChildren, updateLocal);       

        if ( !bUpdateChildren )
            return;

        for ( auto i : mChildren ) {
            i->updateOrientation( orient, true  );
        }
    }

    void    tintaDrawable::updateChildData() {

        if ( mpParent ) { 

            mScale *= mpParent->mScale;            
          
            setPosition( mpParent->getPosition() + mpParent->mScale * ( mpParent->getOrientation()  * mLocalPosition));

        }
    }

    void tintaDrawable::rotate(float angleX, float angleY, float angleZ, bool bUpdateChildren, bool updateLocal) {
       // tintaQuatern qDif = ;
        tintaSpatial::rotate(angleX, angleY, angleZ, bUpdateChildren, updateLocal);

        //qDif =  qDif - mOrientation;

        if ( !bUpdateChildren)
            return;       

        for ( auto i : mChildren ) {

            i->rotate(angleX, angleY, angleZ, true, false);
        }
       // cout << getOrientation().mz<<"\n";
    }        

    void tintaDrawable::addMaterial(const String &name, tintaMaterial *mat) {
        
        tintaMatvec_t::iterator i = mMaterials.begin();
        for (; i != mMaterials.end(); i++) {
            if (i->material->getName() == name) {
                //mMaterials.erase(i);
                break;
            }
        }        
        if ( i != mMaterials.end() ) {

            activateMaterial(i->material->getName());
            return;
        }

        tintaMaterial *m = mat;
        if( !m )
            m = tintaRoot::getPtr()->findMaterial(name);

        if ( !m ) {
            m = tintaRoot::getPtr()->addMaterial( name, name );
            if ( !m ) {
                StringStream s;
                s << _M("Material ") << name;
                s << _M(" was not found ");
                EXCEPTION(s.str());
            }
        }
        
        m->bind();

        if ( !m->binded() ) {
            StringStream s;
            s << _M("Material ") << name;
            s << _M(" was not binded ");
            EXCEPTION(s.str());
        }
        mMaterials.push_front({ m });
        
    }

    void tintaDrawable::addMaterial(tintaMaterial *mat) {       
        addMaterial( mat->getName() );
    }

    bool tintaDrawable::isVisible() const {        
        
        return mVisible;
    }

    void tintaDrawable::setVisible( bool visible ) {
        mVisible = visible;

        if ( mChildren.size() == 0 )
            return;        

        for ( auto i : mChildren ) {                           
            i->setVisible( visible );            
        }
    }

    bool tintaDrawable::addChildren(tintaDrawable * child) {
                
        if ( !child )
            return false;

        if (std::find_if(mChildren.begin(), mChildren.end(), [&](auto *val) {return child->getName() == val->getName(); }) != mChildren.end())
            return false;

        mChildren.push_back(child);

        return true;
    }

    tintaDrawable* tintaDrawable::getChildren(const String &name, bool deepSearch) {
        
        tintaDrawable* rez = NULL_M;

        for ( auto i : mChildren ) {

            //std::cout << "process :" << i->getName() << "\n";
            if ( i->getName() == name )
                rez = i;

            if ( !rez && deepSearch ) {
                rez = i->getChildren(name, true);
            }

            if (rez)
                break;
        }

        return rez;
    }


    drawablelist_t& tintaDrawable::getChildrenList() {
        return mChildren;
    }

    const drawablelist_t& tintaDrawable::getChildrenList() const {
        return mChildren;
    }

   tintaDrawable* tintaDrawable::getChildren(const String &nextAfter ) {

       tintaDrawable* rez = NULL_M;
       if ( nextAfter.length() > 0 ) {

           bool found = false;
           for (auto i : mChildren) {
             
               if ( found ) {
                   rez = i;
                   break;
               }
               if ( i->getName() == nextAfter ) {
                   found = true;
               }               
           }
       }
       else if ( mChildren.size() > 0 ) {
           rez = *( mChildren.begin() );
       }

       return rez;
    }

    bool tintaDrawable::dettachChildren( const String &name ) {

        auto ch = std::find_if(mChildren.begin(), mChildren.end(), [&](auto *val) {return name == val->getName(); });

        if ( ch != mChildren.end() ) {
            mChildren.erase( ch );
            return true;
        }
        return false;
    }

    void tintaDrawable::dettachChildren() {
        for ( auto i : mChildren )
            i->setParent( NULL_M );

        mChildren.clear();
    }

    void tintaDrawable::removeMaterial( const String &name ) {
        tintaMatvec_t::iterator i = mMaterials.begin();
        for (; i != mMaterials.end(); i++) {
            if (i->material->getName() == name ) {
                mMaterials.erase(i);
                break;
            }
        }
    }


    void tintaDrawable::activateMaterial( const String &name ){
        if ( mMaterials.size() == 1 )
            return;

        tintaMatvec_t::iterator i = mMaterials.begin();
        for (; i != mMaterials.end(); i++) {
            if (i->material->getName() == name ) {
                std::swap((*i), mMaterials.front());
                break;
            }        
        }
    }

    void tintaDrawable::removeMaterials(){
        mMaterials.clear();
    }
        
   /* bool tintaDrawable::addSetMaterialVariable( const String &nameMaterial, const String &nameVar, MaterialDataPtr_t matData ) {

        auto l = std::find_if( mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == nameMaterial; } );

        if ( l != mMaterials.end() ) {

            auto &variables = l->matVariables;

            auto v = std::find_if( variables.begin(), variables.end(), [&](auto &val) { return val.name == nameVar; });
            if ( v != variables.end() ) {                
                //variables.erase( v );
                (*v) = { nameVar , std::move(matData) };
            }
            variables.push_back( { nameVar , std::move( matData ) } );

            return true;
        }
        return false;
    }*/



    bool tintaDrawable::onMatVarFloat(const String &matName, const String  &varName, float value) {

        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if( variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; } );

            if ( v != variables.end() ) {

                CoreAssert( v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarFloat Buffer size" );                
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back( { varName, tintaMaterial::allocateData(tintaMaterialParser::matFloat, tintaMaterialParser::autoMaxElement, value) });
            

            return true;
        }
        return false;
    }

    bool tintaDrawable::onMatVarDouble(const String &matName, const StringBasic  &varName, double value) {
        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if (v != variables.end()) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarDouble Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matDouble, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

    bool tintaDrawable::onMatVarUInt(const String &matName, const StringBasic  &varName, m_uint32  value) {
        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if (v != variables.end()) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarUInt Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matUInt, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

    bool tintaDrawable::onMatVarInt(const String &matName, const StringBasic  &varName, int  value) {
        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if (v != variables.end()) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarInt Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matInt, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

    bool tintaDrawable::onMatVarBool(const String &matName, const StringBasic  &varName, bool  value) {
        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if ( v != variables.end() ) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarBool Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matBool, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

    

    bool tintaDrawable::onMatVarfVec2(const String &matName, const StringBasic  &varName, const tintaVector2f &value) {
        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if (v != variables.end()) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarfVec2 Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matfVec2, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

    bool tintaDrawable::onMatVarfVec3(const String &matName, const StringBasic  &varName, const tintaVector3f &value) {
        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if (v != variables.end()) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarfVec3 Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matfVec3, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

    bool tintaDrawable::onMatVarfVec4(const String &matName, const StringBasic  &varName, const tintaVector4f &value) {
        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if (v != variables.end()) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarfVec4 Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matfVec4, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

    bool tintaDrawable::onMatVarfMat3(const String &matName, const StringBasic  &varName, const tintaMatrix33 &value) {
        auto l = std::find_if(mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if (v != variables.end()) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarfMat3 Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matfMat3, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

    bool tintaDrawable::onMatVarfMat4(const String &matName, const StringBasic  &varName, const tintaMatrix44 &value) {
        auto l = std::find_if( mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == matName; });
        if (l != mMaterials.end()) {

            auto &variables = l->matVariables;

            auto v = std::find_if(variables.begin(), variables.end(), [&](auto &val) { return val.name == varName; });

            if ( v != variables.end() ) {

                CoreAssert(v->data->mValBuff.GetBuffer(), "tintaDrawable::onMatVarfMat4 Buffer size");
                Tinta::WriteToBuffer(v->data->mValBuff.GetBufferEx(), 0, value);  //) = { nameVar , std::move(matData) };
            }
            else
                variables.push_back({ varName, tintaMaterial::allocateData(tintaMaterialParser::matfMat3, tintaMaterialParser::autoMaxElement, value) });


            return true;
        }
        return false;
    }

       

    bool tintaDrawable::onMatVarTexture( const String &nameMaterial, const String &nameVar, tintaTexture *texture, int index ) {
        auto l = std::find_if( mMaterials.begin(), mMaterials.end(), [&](auto &val) { return val.material->getName() == nameMaterial; } );

        if (l != mMaterials.end()) {

            auto &variables = l->matManualTextures;

            int unit =  l->material->getTextureUnit(nameVar);

            if ( unit == -1 ) // no such variable
                return false; 

            auto v = std::find_if( variables.begin(), variables.end(), [&](auto &val) { return val.name == nameVar; } );
            if ( v != variables.end() ) {
                (*v) = { nameVar , texture, unit };
            } 
            else
                variables.push_back( { nameVar , texture, unit } );

            return true;
        }
        return false;
    }

    bool tintaDrawable::enableMaterial() {

        if ( mMaterials.size() == 0 )
            return false;

        bool rez  = mMaterials.front().material->enable( 0 );
        auto m = mMaterials.front();
        
        for (auto t : m.matManualTextures) {
            m.material->assignTexture(t.name, t.texture, t.index );
        }
                        
        for ( const auto &t : m.matVariables ) {        
            m.material->assignVariable( t.name, t.data );
        }
        
        return rez;
    }

    bool tintaDrawable::disableMaterial( ) {

        CoreAssert(mMaterials.size() > 0, _M(" mMaterials.size == 0 "));
        return mMaterials.front().material->disable( 0 );
    }

    const tintaMaterial * tintaDrawable::getMaterial( ) const {
        if ( mMaterials.size() == 0 )
            return NULL_M;

        return mMaterials.front().material;
    }

    tintaMaterial * tintaDrawable::getMaterial() {
        if ( mMaterials.size() == 0 )
            return NULL_M;

        return mMaterials.front().material;
    }



    tintaMaterial *tintaDrawable::findMaterial(const String &name) {

        auto i = mMaterials.begin();
        for ( ; i != mMaterials.end(); i++ ) {
            if (i->material->getName() == name) {                
                break;
            }
        }
        if ( i == mMaterials.end() )
            return NULL_M;

        return i->material;
    }

 	void tintaDrawable::onGetVisibleSet ( tintaCuller& culler, bool bNoCull ){
 		if (mCulling == CULL_ALWAYS){
 			return;
 		}
 
 		if (mCulling == CULL_NEVER){
 			bNoCull = true;
 		}
 
 		m_uint32 uiSavePlaneState = culler.GetPlaneState();
 		if ( bNoCull || culler.IsVisible( mBound ) ){
 			getVisibleSet(culler,bNoCull);
 		}
        culler.setPlaneState(uiSavePlaneState);
 	}
  

   

    void tintaDrawable::setParent( tintaDrawable* pkParent ){

        /*if ( mpParent ) {
            mpParent->dettachChildren(getName());
        }*/
        mpParent = pkParent;

        if ( !mpParent ) {
            setPosition(mLocalPosition);
            setOrientation(mLocalOrientation);
            return;
        }
             
        rotate(mpParent->getOrientation(), true, false);
        //mLocalOrientation = getOrientation();
        //setOrientation(mLocalOrientation *  mpParent->getOrientation()) ;
        updateChildData();
               
        //mScale = mpParent->mScale * mLocalScale;
    }

   
    tintaDrawable* tintaDrawable::getParent(){
        return mpParent;
    }

    const tintaDrawable* tintaDrawable::getParent() const {
        return mpParent;
    }

   

    void tintaDrawable::getVisibleSet( tintaCuller& culler, bool bNoCull ){
        for ( tintaDrawable* child : mChildren ) {
            if (child){
                child->onGetVisibleSet(culler, bNoCull);
            }
        }
    }
	

}


