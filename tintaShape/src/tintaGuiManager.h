/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov 
tintapix.com
tintapix@gmail.com  */

#ifndef _TINTA_GUI_MANAGER_H
#define _TINTA_GUI_MANAGER_H

#include <tintaCoord2.h>

namespace Graphica
{

    class tintaGLRenderer;
    class tintaTexture;
    class tintaMaterial;

    /*
    Class for drawing images on any level of the scene
    */
    class imageLayer {
    public:
        /*
            Always show last
        */
        tintaTexture* getFrontTexture();

        /*
            Always show last
        */
        const tintaTexture* getFrontTexture() const;

        /*
            Changes position on back
        */
        void setFront(const String &imageName);

        void addTexture(tintaTexture* texture);

        void addTexture(size_t pos, tintaTexture* texture);

        bool delTexture(tintaTexture* texture);

        const tintaCoord2Rec& getPos() const;

        void setPos(const tintaCoord2Rec& newPos);

    private:
        std::list<tintaTexture*> mTextureObject;

        /*
            GL specific coordinates of the layer
        */
        tintaCoord2Rec           mPos;
    };


    class tintaGuiManager
    {
    public:
        tintaGuiManager();

        tintaGuiManager(tintaGLRenderer * renderer);

        void setRenderer(tintaGLRenderer * renderer);
        
        virtual ~tintaGuiManager();

        /*
            Adds layer with GL {-1,1} specifiec size and position values
        */
        virtual const imageLayer* addImageLayer(const tintaCoord2Rec &pos, const String &imageName);


        /*
            Returns first layer in the list
        */
        virtual imageLayer* getFirstImageLayer();

        /*
            Returns next layer in the list
        */
        virtual imageLayer* getNextImageLayer();

        /*
            Adds layer with GL {-1,1} specifiec size and position values
        */
        virtual const imageLayer* addImageLayer(const tintaCoord2Rec &pos, const tintaImage * img);

        virtual imageLayer* addImageLayer(const tintaCoord2Rec &pos);        

        virtual bool addToLayer(const imageLayer *layer, const String &imageName);

        virtual bool addToLayer(const imageLayer *layer, const tintaImage *img);        

        virtual bool addToLayer(const imageLayer *layer, const String &imageName, size_t pos);

        virtual bool addToLayer(const imageLayer *layer, const tintaImage *img, size_t pos);

        virtual bool delFromLayer(const imageLayer *layer, const String &imageName);

        virtual void setFront(const imageLayer* layer, const String &imageName);

        virtual void moveback(const  String &layerName);

        virtual void movefront(const  String &layerName);

        virtual void swap(const  String &layerName1, const  String &layerName2);        

        virtual bool deleteImageLayer(const  imageLayer* layer, bool bReleaseTexture = false);

        virtual void deleteImageLayers(bool bReleaseTexture = false);

        /*
            Returns first layer in the list
        */
        virtual tintaDrawable* getFirstEffectLayer();

        /*
            Returns next layer in the list
        */
        virtual tintaDrawable* getNextEffectLayer();

        /*
            Adds rectangle layer with effect in front of the camera, position in GL {-1,1} specific coordinates
        */
        virtual tintaDrawable* addEffectLayer( const tintaVector2f &pos, const tintaVector2f &size, float angle, 
                            int  xTessellate,  int yTessellate, const String &name, const String &material );

        /*
        Adds rectangle layer with effect in front of the camera, position in GL {-1,1} specific coordinates
        */
        virtual tintaDrawable* addEffectLayer(const tintaVector2f &pos, const tintaVector2f &size, float angle,
            int  xTessellate, int yTessellate, const String &name, tintaMaterial *material);

        virtual bool deleteEffectLayer(const  tintaDrawable* layer);

        virtual bool setParent( tintaDrawable *child, const String &parent );

        virtual bool setParent( const String &child, const String &parent);

        virtual bool deleteEffectLayer(const  String &name, bool removeChildren = false);

        virtual void deleteEffectLayers();

        tintaDrawable *getEffectLayer(const  String &name);

    protected:

        bool addTextureToLayer(const imageLayer *layer, tintaTexture * texture);

        bool addTextureToLayer(const imageLayer *layer, tintaTexture * texture, size_t pos);

    private:
        tintaGLRenderer *   mRenderer;

        imageLayers_t       mImageLayers;

        imageLayers_t::reverse_iterator       mImageLayerIt;

        effectLayers_t                mEffectLayers;

        effectLayers_t::iterator      mEffectLayerIt;

        
    };
}
#endif

