/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_SCENE_MANAGER_H_
#define _TINTA_SCENE_MANAGER_H_

#include "tintaCamera.h"

namespace Graphica
{
    class _CoreExport tintaSceneManager
    {

    public:

        enum PlaneTypes {
            Plane, // basic
            Billboard, // z face
            Sprite,    // full face
        };

        tintaSceneManager();

        tintaSceneManager( tintaGLRenderer *rend );

        virtual ~tintaSceneManager();

        /*
            Adds camera in to the scene
            casts exception if the camera already exsists
        */
        void addCamera( const String &name );
        
        tintaDrawable* addEntity(UNIQPTRVIRT_T(tintaDrawable) e);

        tintaDrawable* getEntity(const String &name);

        tintaDrawable* getEntity(int index);     

        tintaGLPoints *getPointsLayer(const String &name);

        const tintaGLPoints *getPointsLayer(const String &name)const;

        const tintaGLPoints* getPointsLayerFirst() const;

        void swapPointsLayers(const String &name1 , const String &name2 );

        const tintaGLPoints* getPointsLayerNext() const;

        bool addPointsLayer(const String &name, float size, PrimitiveType type );        

        bool delPointsLayer(const String &name);

        void delPointsLayers();

        bool setParent( tintaDrawable *child, const String &parent );

        bool setParent(const  String &child, const String &parent);

        const tintaDrawable* getEntityFirst() const;

        const tintaDrawable* getEntityNext() const;

        /*
            Removes camera
        */
        bool removeCamera(const String &name);

        bool removeEntity(const String &name, bool removeChildren = false);

        void removeEntities();

        /*
            Returns camera object or NULL_M
        */
        tintaCamera *getCamera( const String &name );

        const tintaCamera* getCameraFirst() const;

        const tintaCamera* getCameraNext() const;

        /*
            Returns camera object or NULL_M
        */
        const tintaCamera *getCamera(const String &name) const ;

        tintaDrawable*     getRoot();

        static tintaDrawable* createPlane( float w, float h, int xTessel, int yTessel, PlaneTypes type = Plane);

        static tintaDrawable* createBox(float xSize, float ySize, float zSize,  bool inside );

        static tintaDrawable* createSphere(int zTessel, int radTessel, float radius, bool inside);

        static tintaDrawable* createMesh(const String &name);

        void setRenderer(tintaGLRenderer *rend);        

    protected:    
        
        typedef std::map<String, UNIQPTRVIRT_T(tintaDrawable)> EntitiesMap_t;
        
        typedef EntitiesMap_t::const_iterator EntitiesMap_cit_t;

        typedef std::map<String, UNIQPTRVIRT_T(tintaCamera)> CameraMap_t;

        typedef CameraMap_t::const_iterator CameraMap_cit_t;

        CameraMap_t   mCameras;       

        mutable CameraMap_cit_t mCamCit;
        
        EntitiesMap_t mEntities;

        mutable EntitiesMap_cit_t mEntCit;        
        
        tintaDrawable* mpSceneRoot;

        tintaGLRenderer *mRend;

        typedef std::list<tintaGLPoints > pointLayers_t;

        pointLayers_t mPointLayers;

        typedef pointLayers_t::const_iterator pointLayersIt_t;

        mutable pointLayersIt_t mPointsCit;
    };

}
#endif