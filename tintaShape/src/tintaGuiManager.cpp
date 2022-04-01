/*  Copyright (C) 2011 - 2017 Evdokimov Mikhail
tintapix.com
tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaGuiManager.h"
#include "tintaTexture.h"
#include "tintaSceneManager.h"

namespace Graphica
{


    tintaTexture* imageLayer::getFrontTexture() {
        if (mTextureObject.size() == 0)
            return nullptr;

        return mTextureObject.back();
    }

    const tintaTexture* imageLayer::getFrontTexture() const {
        if (mTextureObject.size() == 0)
            return nullptr;

        return mTextureObject.back();
    }


    void imageLayer::setFront(const String &imageName) {

        auto i = std::find_if(mTextureObject.begin(), mTextureObject.end(), [&](auto *val) {return imageName == val->getName(); });

        // if texture in the list move it to the front
        if (i != mTextureObject.end()) {
            std::swap(*(mTextureObject.rbegin()), *i);
            return;
        }

    }

    void imageLayer::addTexture(tintaTexture* texture) {
        mTextureObject.push_back(texture);
    }

    void imageLayer::addTexture(size_t pos, tintaTexture* texture) {

        if (mTextureObject.size() == 1) {
            addTexture(texture);
            return;
        }

        if (pos <  mTextureObject.size()) {
            size_t i = 0;
            auto it = mTextureObject.begin();
            while (++i < pos)
                it++;

            if (it != mTextureObject.end()) {
                mTextureObject.insert(it, texture);
            }
        }
    }

    bool imageLayer::delTexture(tintaTexture* texture) {

        if (!texture)
            return false;

        auto i = std::find_if(mTextureObject.begin(), mTextureObject.end(), [&](auto *val) {return texture->getName() == val->getName(); });

        if (i == mTextureObject.end())
            return false;

        mTextureObject.erase(i);

        return true;
    }



    const tintaCoord2Rec& imageLayer::getPos() const {
        return mPos;
    }

    void imageLayer::setPos(const tintaCoord2Rec& newPos) {
        mPos = newPos;
    }

    tintaGuiManager::tintaGuiManager()
    {
    }

    tintaGuiManager::tintaGuiManager(tintaGLRenderer * renderer)
        :mRenderer(renderer)
    {
    }

    void tintaGuiManager::setRenderer(tintaGLRenderer * renderer) {
        mRenderer = renderer;
    }

    tintaGuiManager::~tintaGuiManager()
    {
        deleteImageLayers();
        deleteEffectLayers();
    }


    const imageLayer* tintaGuiManager::addImageLayer(const tintaCoord2Rec &pos, const String &imageName) {

        CoreAssert(mRenderer, "mRenderer");

        if ( !mRenderer )
            return NULL_M;

        tintaTexture * texture = mRenderer->addTexture(imageName);

        if ( texture ) {

            imageLayer * layer = NEW_T(imageLayer)();
            layer->addTexture(texture);
            layer->setPos(pos);

            mRenderer->loadTexture( texture );

            mImageLayers.push_back( layer );

            return mImageLayers.back();
        }

        return NULL_M;
    }


    
    imageLayer* tintaGuiManager::getFirstImageLayer() {
        if (mImageLayers.size() == 0)
            return NULL_M;

        mImageLayerIt =  mImageLayers.rbegin();
        return (*mImageLayerIt++);
    }

    
    imageLayer* tintaGuiManager::getNextImageLayer() {
        if ( mImageLayers.size() == 0 || mImageLayerIt == mImageLayers.rend()) 
            return NULL_M;

        return (*mImageLayerIt++);
    }


    const imageLayer* tintaGuiManager::addImageLayer(const tintaCoord2Rec &pos, const tintaImage * img) {

        CoreAssert(mRenderer, "mRenderer");

        if (!mRenderer)
            return NULL_M;

        tintaTexture * texture = mRenderer->addTexture(img);
        if (texture) {
                       

            imageLayer * layer = NEW_T(imageLayer)();
            layer->setPos(pos);
            layer->addTexture(texture);
            mImageLayers.push_back(layer);

            return mImageLayers.back();
        }
        return NULL_M;
    }


    imageLayer*  tintaGuiManager::addImageLayer(const tintaCoord2Rec &pos) {
        imageLayer * layer = NEW_T(imageLayer)();
        layer->setPos(pos);
        mImageLayers.push_back(layer);
        return mImageLayers.back();
    }

    
    bool tintaGuiManager::addTextureToLayer(const imageLayer *layer, tintaTexture * texture) {

        if (layer && texture) {

            auto l = std::find_if(mImageLayers.begin(), mImageLayers.end(), [&](auto *val) {return val == layer; });
            if (l == mImageLayers.end()) {
                return false;
            }
            (*l)->addTexture(texture);

            return true;
        }
        return false;
    }

    bool tintaGuiManager::addTextureToLayer(const imageLayer *layer, tintaTexture * texture, size_t pos) {
        if (layer && texture) {

            auto l = std::find_if(mImageLayers.begin(), mImageLayers.end(), [&](auto *val) {return val == layer; });
            if (l == mImageLayers.end()) {
                return false;
            }

            (*l)->addTexture(pos, texture);

            return true;
        }
        return false;
    }
    bool tintaGuiManager::delFromLayer(const imageLayer *layer, const String &imageName) {

        CoreAssert(mRenderer, "mRenderer");

        if (!mRenderer)
            return NULL_M;

        if (layer && imageName.length() > 0) {

            auto l = std::find_if(mImageLayers.begin(), mImageLayers.end(), [&](auto *val) {return val == layer; });
            if (l == mImageLayers.end()) {
                return false;
            }

            tintaTexture * texture = mRenderer->findTexture(imageName);
            if (!texture)
                return false;

            (*l)->delTexture(texture);

            return true;
        }
        return false;
    }


    bool tintaGuiManager::addToLayer(const imageLayer *layer, const String &imageName) {

        CoreAssert(mRenderer, "mRenderer");

        if (!mRenderer)
            return false;

        CoreAssert(layer, "layer");

        if (!layer)
            return false;

        tintaTexture * texture = mRenderer->addTexture(imageName);

        mRenderer->loadTexture(texture);

        return addTextureToLayer(layer, texture);
    }


    bool tintaGuiManager::addToLayer(const imageLayer *layer, const tintaImage *img) {
       
        CoreAssert(mRenderer, "mRenderer");

        if (!mRenderer)
            return false;

        CoreAssert(layer, "layer");

        if (!layer)
            return false;

        tintaTexture * texture = mRenderer->addTexture(img);

        mRenderer->loadTexture(texture);

        return addTextureToLayer(layer, texture);
    }

    bool tintaGuiManager::addToLayer(const imageLayer *layer, const String &imageName, size_t pos) {

        CoreAssert(mRenderer, "mRenderer");

        if (!mRenderer)
            return false;

        CoreAssert(layer, "layer");

        if (!layer)
            return false;

        tintaTexture * texture = mRenderer->addTexture(imageName);
        mRenderer->loadTexture(texture);

        return addTextureToLayer(layer, texture, pos);
    }


    bool tintaGuiManager::addToLayer(const imageLayer *layer, const tintaImage *img, size_t pos) {

        CoreAssert(mRenderer, "mRenderer");

        if (!mRenderer)
            return false;

        CoreAssert(layer, "layer");

        if (!layer)
            return false;

        tintaTexture * texture = mRenderer->addTexture(img);

        mRenderer->loadTexture(texture);

        return addTextureToLayer(layer, texture, pos);
    }

    void tintaGuiManager::setFront(const imageLayer* layer, const String &imageName) {

        CoreAssert(layer, "Layer null");

        if ( !layer )
            return;

        auto l = std::find_if(mImageLayers.begin(), mImageLayers.end(), [&](auto *val) {return val == layer; });
        if ( l == mImageLayers.end() ) {
            return;
        }
        (*l)->setFront(imageName);
    }

    void tintaGuiManager::moveback(const String &name) {

        auto l = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == name; });
        if ( l == mEffectLayers.end() ) {
            return;
        }
        auto next = l;
        if ( ++next == mEffectLayers.end() )
            return;

        std::swap(*next, *l);
    }

    void tintaGuiManager::movefront(const String &name) {
        auto l = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == name; });
        if (l == mEffectLayers.end() || l == mEffectLayers.begin() ) {
            return;
        }
        auto next = l;
        if (--next == mEffectLayers.end())
            return;

        std::swap(*next, *l);
    }

    void tintaGuiManager::swap(const  String &layerName1, const  String &layerName2) {

        auto l1 = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == layerName1; });
        auto l2 = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == layerName2; });

        if ( l1 == mEffectLayers.end() || l2 == mEffectLayers.end() ) {
            return;
        }
        
        std::swap(*l1, *l2);
    }

    bool tintaGuiManager::deleteImageLayer(const imageLayer* layer, bool bReleaseTexture) {

        CoreAssert(mRenderer, "mRenderer");

        if (!mRenderer)
            return false;

        CoreAssert(layer, "layer == NULL");
        if (!layer)
            return false;
        auto i = std::find_if(mImageLayers.begin(), mImageLayers.end(), [&](auto *val) {return layer == val; });

        if (i != mImageLayers.end()) {

            if ( bReleaseTexture ) {

                tintaTexture* t = (*i)->getFrontTexture();

                while (t) {
                    mRenderer->releaseTexture(t);
                    (*i)->delTexture(t);
                    t = (*i)->getFrontTexture();
                }
            }

            DELETE_T(*i, imageLayer);
            layer = nullptr;
            mImageLayers.erase(i);
        }
        else
            return false;

        return true;
    }

    void tintaGuiManager::deleteImageLayers(bool bReleaseTexture) {

        for ( auto i : mImageLayers ) {
            deleteImageLayer( i, bReleaseTexture );
        }
    }


    /*
        Returns first layer in the list
    */
    tintaDrawable* tintaGuiManager::getFirstEffectLayer() {
        
        if (mEffectLayers.size() == 0)
            return NULL_M;

        mEffectLayerIt = mEffectLayers.begin();
        return (*mEffectLayerIt++);
    }

    /*
        Returns next layer in the list
    */
    tintaDrawable* tintaGuiManager::getNextEffectLayer() {

        if ( mEffectLayers.size() == 0 || mEffectLayerIt == mEffectLayers.end() )
            return NULL_M;

        return (*mEffectLayerIt++);    
    }

    tintaDrawable* tintaGuiManager::addEffectLayer( const tintaVector2f &pos, const tintaVector2f &size, float angle, 
        int  xTessellate, int yTessellate, const String &name, const String &material) {
                
        Graphica::tintaDrawable* ent = Graphica::tintaSceneManager::createPlane(size.mx, size.my, xTessellate, yTessellate);

        auto l = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == name; });

        if ( ent && l == mEffectLayers.end() ) {
            if( material.length() > 0 )
                ent->addMaterial(material);

            ent->moveTo({ pos.mx, pos.my, 0.f  }, true, true);
            ent->setName( name );                   


            mEffectLayers.push_back( ent );
            return ent;
        }

        return NULL_M;
    }

    tintaDrawable* tintaGuiManager::addEffectLayer(const tintaVector2f &pos, const tintaVector2f &size, float angle,
        int  xTessellate, int yTessellate, const String &name,tintaMaterial *material) {

        if (!material)
            return NULL_M;


        Graphica::tintaDrawable* ent = Graphica::tintaSceneManager::createPlane(size.mx, size.my, xTessellate, yTessellate);

        auto l = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == name; });

        if (ent && l == mEffectLayers.end()) {
           
            ent->addMaterial(material);
            ent->moveTo({ pos.mx, pos.my, 0.f },true, true);
            ent->setName(name);
            mEffectLayers.push_back(ent);
            return ent;
        }

        return NULL_M;
    }

    bool tintaGuiManager::setParent( tintaDrawable *child, const String &parent ) {

        
        auto p = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == parent; });

        if ( child && p != mEffectLayers.end() ) {

            if ( (*p)->addChildren(child) ) {
                child->setParent(*p);    

                return true;
            }            
        }
        return false;
    }

    bool tintaGuiManager::setParent( const String &child, const String &parent ) {
        auto p = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == parent; });

        auto ch = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == child; });

        if (ch != mEffectLayers.end() && p != mEffectLayers.end()) {
           
            if ( (*p)->addChildren(*ch) ) {

                (*ch)->setParent(*p);           
                return true;
            }
        }
        return false;
    }

    bool tintaGuiManager::deleteEffectLayer( const  String &name, bool removeChildren ) {

        auto l = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == name; });

        if ( l != mEffectLayers.end() ) {

            if ( removeChildren ) {
                tintaDrawable * pCh = (*l)->getChildren();
                while (pCh) {
                    deleteEffectLayer(pCh->getName(), removeChildren);
                    pCh = (*l)->getChildren();
                }
            }

            auto *p = (*l)->getParent();

            if ( p ) {
                p->dettachChildren( (*l)->getName() );
            }

            (*l)->dettachChildren();

            M_DELETE ( *l );

            mEffectLayers.erase( l );

            mEffectLayerIt = mEffectLayers.begin();

            return true;
        }
        return false;      
    }


    tintaDrawable *tintaGuiManager::getEffectLayer(const  String &name) {

        auto l = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val->getName() == name; });
        if ( l != mEffectLayers.end() ) {
            return (*l);
        }
        return NULL_M;
    }

    bool tintaGuiManager::deleteEffectLayer(const  tintaDrawable* layer) {

        auto l = std::find_if(mEffectLayers.begin(), mEffectLayers.end(), [&](auto *val) {return val == layer; });
        if ( l != mEffectLayers.end() ) {

            auto *p = (*l)->getParent();
            if (p) {
                p->dettachChildren((*l)->getName());
            }

            (*l)->dettachChildren();

            M_DELETE(*l);

            mEffectLayers.erase(l);
            mEffectLayerIt = mEffectLayers.begin();

            return true;
        }
        return false;
    }

    void tintaGuiManager::deleteEffectLayers() {

        for ( auto i : mEffectLayers ) {
            M_DELETE i;
            i = NULL_M;
        }
        mEffectLayers.clear();
    }
}



