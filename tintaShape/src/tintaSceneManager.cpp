/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaSceneManager.h"
#include "tintaBillboard.h"
#include <Math/tintaCommonMath.h>


namespace Graphica {



    tintaSceneManager::tintaSceneManager():
        mpSceneRoot(NULL_M) {

        mEntCit = mEntities.begin();
        mCamCit = mCameras.begin();      
        mPointsCit = mPointLayers.begin();

        mRend = NULL_M;
        UNIQPTRVIRT(tintaDrawable, root, M_NEW(tintaDrawable)());

        mpSceneRoot = root.get();
        mpSceneRoot->setName(_M("Root"));

        addEntity( std::move( root ) );
        
    }

    tintaSceneManager::tintaSceneManager(tintaGLRenderer *rend) 
        : tintaSceneManager() {
        
        mRend = rend;
    }
    
    tintaSceneManager::~tintaSceneManager(){        
        mCameras.clear();
        mEntities.clear();
    }    


    void tintaSceneManager::setRenderer( tintaGLRenderer *rend ) {
        if (rend == NULL_M)         return;

        mRend = rend;
        for (auto i = mEntities.begin(); i != mEntities.end(); i++)
            i->second->setRenderer(mRend);
    }


    bool tintaSceneManager::setParent(tintaDrawable *child, const String &parent) {

        if (!child)
            return false;

        auto p = mEntities.find( parent );

        tintaVector3f posOld = child->getLocalPosition();

        if ( child && p != mEntities.end() ) {

            // clear old parent data
            if ( child->getParent() )
                child->getParent()->dettachChildren(child->getName());

            if ( p->second->addChildren(child) ) {

                child->setParent( p->second.get() );
                
                return true;
            }
        }
        return false;
    }


    bool tintaSceneManager::setParent(const  String &child, const String &parent) {

        auto ch = mEntities.find(child);

        return setParent(ch->second.get(), parent);
    }

    tintaDrawable* tintaSceneManager::addEntity(UNIQPTRVIRT_T(tintaDrawable) e) {

        if ( mEntities.find( e->getName() ) == mEntities.end() ) {
            
            if (e.get() != mpSceneRoot) {
                mpSceneRoot->addChildren(e.get());
                e->setParent(mpSceneRoot);
            }
            
            if ( !e->getRenderer() )
                e->setRenderer( mRend );
            
            auto rez = mEntities.insert( EntitiesMap_t::value_type( e->getName(), std::move(e) ) );

            return rez.first->second.get();
        }
        else {
            StringStream msg;
#if TINTA_LOCAL ==  TINTA_EN
            if ( e )
                msg << _M("Name: ") << e->getName() << _M(" has dublicate");
            else
                msg << _M("Entity is null");
#elif TINTA_LOCAL == TINTA_RU
            if(e)
                msg << _M("Объект с именем: ") << e->getName() << _M(" уже существует");
            else
                msg << _M("Объект не создан");
#endif
           // EXCEPTION(msg.str());
        }

        return NULL_M;
    }

    tintaDrawable *tintaSceneManager::getEntity(const String &name) {
        EntitiesMap_t::iterator it = mEntities.find(name);
        if (mEntities.find(name) == mEntities.end())
            return NULL_M;

        return it->second.get();
    }
    
    tintaDrawable* tintaSceneManager::getEntity(int index) {

        if ((size_t)index >= mEntities.size())
            return NULL_M;
        EntitiesMap_cit_t it = mEntities.begin();
        for (auto i = 0; i < index; i++) {
            it++;
        }
        return it->second.get();
    }

    tintaGLPoints *tintaSceneManager::getPointsLayer(const String &name)
    {
        auto it = std::find_if(mPointLayers.begin(), mPointLayers.end(), [&](auto &val) { return val.mName == name;  });

        if ( it == mPointLayers.end() )
            return NULL_M;

        return &(*it);

    }

    const tintaGLPoints *tintaSceneManager::getPointsLayer(const String &name)const
    {
        auto it = std::find_if(mPointLayers.begin(), mPointLayers.end(), [&](auto &val) { return val.mName == name;  });

        if (it == mPointLayers.end())
            return NULL_M;

        return &(*it);
    }

    const tintaGLPoints* tintaSceneManager::getPointsLayerFirst() const
    {
        mPointsCit = mPointLayers.begin();

        if ( mPointsCit == mPointLayers.end() )
            return NULL_M;

        return &(*mPointsCit);
    }

    const tintaGLPoints* tintaSceneManager::getPointsLayerNext() const
    {
        if (mPointsCit == mPointLayers.end()
            || ++mPointsCit == mPointLayers.end()) {
            return NULL_M;
        }
        return &(*mPointsCit);
    }

    bool tintaSceneManager::addPointsLayer( const String &name, float size, PrimitiveType type)
    {       

       

        auto it = std::find_if(mPointLayers.begin(), mPointLayers.end(), [&](auto &val) { return val.mName == name;  });

        if (it != mPointLayers.end())
            return false;

        mPointLayers.push_back( { VecPoints_t() , size,type,name, true });

        return true;
    }

    void tintaSceneManager::swapPointsLayers(const String &name1, const String &name2) {

        auto it1 = std::find_if(mPointLayers.begin(), mPointLayers.end(), [&](auto &val) { return val.mName == name1;  });
        auto it2 = std::find_if(mPointLayers.begin(), mPointLayers.end(), [&](auto &val) { return val.mName == name2;  });

        if ( it1 == mPointLayers.end() || it2 == mPointLayers.end() )
            return;

        std::swap(*it1, *it2);
    }

      

    bool tintaSceneManager::delPointsLayer(const String &name) 
    {
        auto it = std::find_if(mPointLayers.begin(), mPointLayers.end(), [&](auto &val) { return val.mName == name;  });

        if (it == mPointLayers.end())
            return false;

        mPointLayers.erase(it);

        return true;
    }

    void tintaSceneManager::delPointsLayers() {        
        mPointLayers.clear();        
    }

    const tintaDrawable* tintaSceneManager::getEntityFirst() const {
        mEntCit = mEntities.begin();

        if ( mEntCit == mEntities.end() )           
            return NULL_M;

        return mEntCit->second.get();
    }

    const tintaDrawable* tintaSceneManager::getEntityNext()const{
        if (mEntCit == mEntities.end()
            || ++mEntCit == mEntities.end()){
            return NULL_M;
        }
        return mEntCit->second.get();
    }


    bool tintaSceneManager::removeCamera( const String &name ){
        CameraMap_t::iterator it = mCameras.find(name);
        if ( mCameras.find(name) != mCameras.end() ){            
            mCameras.erase(it);
            return true;
        }
        return false;
            
    }


    void tintaSceneManager::addCamera(const String &name){
        if ( mCameras.find(name) == mCameras.end() ){

            UNIQPTRVIRT(tintaCamera, cam, M_NEW tintaCamera() );
            mCameras.insert(CameraMap_t::value_type(name, std::move(cam) ));
        }
    }

    bool tintaSceneManager::removeEntity(const String &name, bool removeChildren ){

        EntitiesMap_t::iterator it = mEntities.find(name);

        if ( mEntities.find(name) != mEntities.end() ){  

            if ( removeChildren ) {
                tintaDrawable * pCh =  it->second->getChildren();
                if ( pCh )
                    removeEntity( pCh->getName(), removeChildren);
                //pCh->
            }
            
            tintaDrawable * p = it->second->getParent();
            if ( p ) {
                // upper level update
                p->dettachChildren(it->second->getName());
            }    

            // low level update            
            it->second->dettachChildren();

            mEntities.erase( it++ );
            mEntCit = mEntities.begin();

            return true;

        }
        return false;
    }


    void tintaSceneManager::removeEntities() {
       
        mpSceneRoot->dettachChildren();

        EntitiesMap_t::iterator it = mEntities.begin();
       
        while ( it != mEntities.end() ) {

            if ( it->second.get() != mpSceneRoot ) {

                mEntities.erase( it++ );
            }
            else it++;
        }         
        

        
    }



    tintaCamera *tintaSceneManager::getCamera(const String &name) {
        CameraMap_t::iterator it = mCameras.find(name);
        if (mCameras.find(name) == mCameras.end())
            return NULL_M;

        return it->second.get();
    }

    const tintaCamera *tintaSceneManager::getCamera(const String &name) const {
        CameraMap_t::const_iterator it = mCameras.find(name);
        if (mCameras.find(name) == mCameras.end())
            return NULL_M;

        return it->second.get();
    }

    const tintaCamera* tintaSceneManager::getCameraFirst() const {
        mCamCit = mCameras.cbegin();
        return mCamCit->second.get();
    }

    const tintaCamera* tintaSceneManager::getCameraNext()const{
        if (mCamCit == mCameras.end()
            || ++mCamCit == mCameras.end()){
            return NULL_M;
        }
        return mCamCit->second.get();
    }


    tintaDrawable* tintaSceneManager::getRoot() {
        return mpSceneRoot;
    }

    tintaDrawable* tintaSceneManager::createPlane(float w, float h, int xTessel, int yTessel, tintaSceneManager::PlaneTypes type ) {

        if ( w == 0.f || h == 0.f || xTessel < 2 || yTessel < 2 )
            return NULL_M;

        tintaVertexBuffer *vBuff{ NULL_M };
        tintaIndexBuffer *iBuff{ NULL_M };
        tintaAttributes attrib;

        attrib.setPositionChannels(3);
        attrib.setTCoordChannels(0, 2);
        

        float hw = w / 2;
        float hh = h / 2;

        float stepX = 1.f / (xTessel - 1.f);
        float stepY = 1.f / (yTessel - 1.f);

        tintaTriMesh* pGeometry = NULL_M;
        switch (type) {
        case Billboard:
            pGeometry = M_NEW tintaBillboard();
            break;
        case Sprite:
            pGeometry = M_NEW tintaBillboard( true );
            break;
        case Plane:
        default:
            pGeometry = M_NEW tintaTriMesh();
            break;
        }

        pGeometry->createVBuffer(attrib, xTessel * yTessel);

        vBuff = pGeometry->getVBuffer();
       

        /*
        vBuff->Position3(0) = tintaVector3f(-hw, -hh , 0.0f);
        vBuff->Position3(1) = tintaVector3f(-hw,  hh , 0.0f);
        vBuff->Position3(2) = tintaVector3f(hw,   hh , 0.0f);
        vBuff->Position3(3) = tintaVector3f(hw,  -hh , 0.0f);
        */
      /*  vBuff->TextureCoord2(0, 0) = tintaVector2f(0.0f, 0.0f);
        vBuff->TextureCoord2(0, 1) = tintaVector2f(1.0f, 0.0f);
        vBuff->TextureCoord2(0, 2) = tintaVector2f(1.0f, 1.0f);
        vBuff->TextureCoord2(0, 3) = tintaVector2f(0.0f, 1.0f);
        */


        {
            float uX(0); //U 
            float uY(0); //V 
            int globPos(0);
            for (int y = 0; y < yTessel; y++) {

                uY = y * stepY;

                tintaVector3f yPos = ((2.0f*uY - 1.0f) * hh)*tintaVector3f::yAxis;

                for (int x = 0; x < xTessel; x++) {

                    uX = x * stepX;

                    tintaVector3f xPos = ((2.0f*uX - 1.0f) * hw)*tintaVector3f::xAxis;

                    tintaVector3f rez = xPos + yPos;

                    vBuff->getPosition3(globPos) = rez;

                    if (attrib.getMaxTCoords() > 0) {

                        tintaVector2f tCoord(uX, uY);

                        for (int k = 0; k < attrib.getMaxTCoords(); k++) {

                            if (attrib.hasTCoord(k))
                                vBuff->getTextureCoord2(k, globPos) = tCoord;

                        }
                    }
                    globPos++;
                }

            }
        }
#if 0
 // СW

        float uX(0); //U 
        float uY(0); //V 
        int globPos(0);
        for (int x = 0; x < xTessel; x++){
            uX = x * stepX;
            tintaVector3f xDir = ( ( 2.0f * uX - 1.0f ) * hw )*tintaVector3f::xAxis;
            for ( int y = 0; y < yTessel; y++ ){
                uY = y * stepY;
                tintaVector3f yDir = ( ( 2.0f*uY - 1.0f) * hh)*tintaVector3f::yAxis;
                tintaVector3f rez = xDir + yDir;
                vBuff->Position3(globPos) = rez;
                
                if ( attrib.getMaxTCoords() > 0 ){
                    tintaVector2f tCoord( uX, uY );
                    for ( int k = 0; k < attrib.getMaxTCoords(); k++ ){
                        if ( attrib.hasTCoord(k) ){
                            vBuff->TextureCoord2(k, globPos) = tCoord;
                        }
                    }
                }                
                globPos++;
            }

        }
#endif
        pGeometry->createBoundary(BoxBV);       
        

        /* index[0] = 0;
        index[1] = 1;
        index[2] = 3;
        index[3] = 3;
        index[4] = 1;
        index[5] = 2;
        */


        pGeometry->createIBuffer( 3 * (2 * (xTessel - 1)*(yTessel - 1)) );
        iBuff = pGeometry->getIBuffer();
            

        int* index = iBuff->GetData();
#if 0 // CW             
        for (int y = 0, globPos = 0; y < yTessel - 1; y++){
            for (int x = 0; x < xTessel - 1; x++){
                int v0 = x + xTessel * y;
                int v1 = v0 + 1;
                int v2 = v1 + xTessel;
                int v3 = v0 + xTessel;
                index[globPos++] = v0;
                index[globPos++] = v1;
                index[globPos++] = v2;
                index[globPos++] = v0;
                index[globPos++] = v2;
                index[globPos++] = v3;
            }
        }
#endif
        for ( int x = 0, globPos = 0; x < xTessel - 1; x++ ){
            for ( int y = 0; y < yTessel - 1; y++ ){
                int v0 = y + yTessel * x;
                int v1 = v0 + 1;
                int v2 = v1 + yTessel;
                int v3 = v0 + yTessel;
                index[globPos++] = v0;
                index[globPos++] = v1;
                index[globPos++] = v2;
                index[globPos++] = v0;
                index[globPos++] = v2;
                index[globPos++] = v3;
            }
        }


        pGeometry->setCullMode(tintaDrawable::CULL_NEVER);
        pGeometry->genNormals();
        return pGeometry;
    }

    tintaDrawable* tintaSceneManager::createSphere(int zTessel, int radTessel, float radius, bool inside){
        int iZSm1 = zTessel - 1, iZSm2 = zTessel - 2, iZSm3 = zTessel - 3;
        int iRSp1 = radTessel + 1;
        int iVQuantity = iZSm2*iRSp1 + 2;
        int iTQuantity = 2 * iZSm2*radTessel;

        
        tintaAttributes attrib;

        attrib.setPositionChannels(3);
        attrib.setTCoordChannels(0, 2);

        tintaTriMesh* pkMesh = M_NEW tintaTriMesh();
        pkMesh->createVBuffer(attrib, iVQuantity);
        pkMesh->createIBuffer(3 * iTQuantity);
        tintaVertexBuffer* pVBuff = pkMesh->getVBuffer(); //M_NEW tintaVertexBuffer( mAttr, vq );
        tintaIndexBuffer*  pIBuffer = pkMesh->getIBuffer(); //M_NEW tintaIndexBuffer(3 * iq);

        //tintaVertexBuffer* pVBuff =  WM4_NEW tintaVertexBuffer(m_kAttr, iVQuantity);
        //tintaIndexBuffer* pIBuffer = WM4_NEW tintaIndexBuffer(3 * iTQuantity);

        // generate geometry
        float fInvRS = 1.0f / (float)radTessel;
        float fZFactor = 2.0f / (float)iZSm1;
        int iR, iZ, iZStart, i, iUnit;
        tintaVector2f kTCoord;

       

        std::vector<float> afSin(iRSp1);
        std::vector<float> afCos(iRSp1);

        for ( iR = 0; iR < radTessel; iR++ )
        {
            float fAngle = K2_PI*fInvRS*iR;
            afCos[iR] = TintaMath::cos(fAngle);
            afSin[iR] = TintaMath::sin(fAngle);
        }
        afSin[radTessel] = afSin[0];
        afCos[radTessel] = afCos[0];

        // generate the cylinder itself
        for (iZ = 1, i = 0; iZ < iZSm1; iZ++)
        {
            float fZFraction = -1.0f + fZFactor*iZ;  // in (-1,1)
            float fZ = radius*fZFraction;

            // compute center of slice
            tintaVector3f kSliceCenter(0.0f, 0.0f, fZ);

            // compute radius of slice
            float fSliceRadius = TintaMath::sqrt(TintaMath::abs(radius*radius - fZ*fZ));

            // compute slice vertices with duplication at end point
            tintaVector3f kNormal;
            int iSave = i;
            for (iR = 0; iR < radTessel; iR++)
            {
                float fRadialFraction = iR*fInvRS;  // in [0,1)
                tintaVector3f kRadial(afCos[iR], afSin[iR], 0.0f);
                pVBuff->getPosition3(i) = kSliceCenter + fSliceRadius*kRadial;
                if (attrib.hasNormal())
                {
                    kNormal = pVBuff->getPosition3(i);
                    kNormal.normalize();
                    if (inside)
                    {
                        pVBuff->getNormal3(i) = -kNormal;
                    }
                    else
                    {
                        pVBuff->getNormal3(i) = kNormal;
                    }
                }

                if (attrib.getMaxTCoords() > 0)
                {
                    kTCoord = tintaVector2f(fRadialFraction,
                        0.5f*(fZFraction + 1.0f));
                    for (iUnit = 0; iUnit < attrib.getMaxTCoords(); iUnit++)
                    {
                        if (attrib.hasTCoord(iUnit))
                        {
                            pVBuff->getTextureCoord2(iUnit, i) = kTCoord;
                        }
                    }
                }

                i++;
            }

            pVBuff->getPosition3(i) = pVBuff->getPosition3(iSave);
            if (attrib.hasNormal())
            {
                pVBuff->getNormal3(i) = pVBuff->getNormal3(iSave);
            }

            if (attrib.getMaxTCoords() > 0)
            {
                kTCoord = tintaVector2f(1.0f, 0.5f*(fZFraction + 1.0f));
                for (iUnit = 0; iUnit < attrib.getMaxTCoords(); iUnit++)
                {
                    if ( attrib.hasTCoord(iUnit) )
                    {
                        pVBuff->getTextureCoord2(iUnit, i) = kTCoord;
                    }
                }
            }

            i++;
        }

        // south pole
        pVBuff->getPosition3(i) = -radius*tintaVector3f::zAxis;
        if (attrib.hasNormal())
        {
            if (inside)
            {
                pVBuff->getNormal3(i) = tintaVector3f::zAxis;
            }
            else
            {
                pVBuff->getNormal3(i) = -tintaVector3f::zAxis;
            }
        }

        if (attrib.getMaxTCoords() > 0)
        {
            kTCoord = tintaVector2f(0.5f, 0.5f);
            for (iUnit = 0; iUnit < attrib.getMaxTCoords(); iUnit++)
            {
                if (attrib.hasTCoord(iUnit))
                {
                    pVBuff->getTextureCoord2(iUnit, i) = kTCoord;
                }
            }
        }

        i++;

        // north pole
        pVBuff->getPosition3(i) = radius*tintaVector3f::zAxis;
        if (attrib.hasNormal())
        {
            if (inside)
            {
                pVBuff->getNormal3(i) = -tintaVector3f::zAxis;
            }
            else
            {
                pVBuff->getNormal3(i) = tintaVector3f::zAxis;
            }
        }

        if (attrib.getMaxTCoords() > 0)
        {
            kTCoord = tintaVector2f(0.5f, 1.0f);
            for (iUnit = 0; iUnit < attrib.getMaxTCoords(); iUnit++)
            {
                if (attrib.hasTCoord(iUnit))
                {
                    pVBuff->getTextureCoord2(iUnit, i) = kTCoord;
                }
            }
        }

        i++;
        assert(i == iVQuantity);

        // generate connectivity
        int* aiLocalIndex = pIBuffer->GetData();
        for (iZ = 0, iZStart = 0; iZ < iZSm3; iZ++)
        {
            int i0 = iZStart;
            int i1 = i0 + 1;
            iZStart += iRSp1;
            int i2 = iZStart;
            int i3 = i2 + 1;
            for (i = 0; i < radTessel; i++, aiLocalIndex += 6)
            {
                if (inside)
                {
                    aiLocalIndex[0] = i0++;
                    aiLocalIndex[1] = i2;
                    aiLocalIndex[2] = i1;
                    aiLocalIndex[3] = i1++;
                    aiLocalIndex[4] = i2++;
                    aiLocalIndex[5] = i3++;
                }
                else  // inside view
                {
                    aiLocalIndex[0] = i0++;
                    aiLocalIndex[1] = i1;
                    aiLocalIndex[2] = i2;
                    aiLocalIndex[3] = i1++;
                    aiLocalIndex[4] = i3++;
                    aiLocalIndex[5] = i2++;
                }
            }
        }

        // south pole triangles
        int iVQm2 = iVQuantity - 2;
        for (i = 0; i < radTessel; i++, aiLocalIndex += 3)
        {
            if (inside)
            {
                aiLocalIndex[0] = i;
                aiLocalIndex[1] = i + 1;
                aiLocalIndex[2] = iVQm2;
            }
            else  // inside view
            {
                aiLocalIndex[0] = i;
                aiLocalIndex[1] = iVQm2;
                aiLocalIndex[2] = i + 1;
            }
        }

        // north pole triangles
        int iVQm1 = iVQuantity - 1, iOffset = iZSm3*iRSp1;
        for (i = 0; i < radTessel; i++, aiLocalIndex += 3)
        {
            if (inside)
            {
                aiLocalIndex[0] = i + iOffset;
                aiLocalIndex[1] = iVQm1;
                aiLocalIndex[2] = i + 1 + iOffset;
            }
            else  // inside view
            {
                aiLocalIndex[0] = i + iOffset;
                aiLocalIndex[1] = i + 1 + iOffset;
                aiLocalIndex[2] = iVQm1;
            }
        }

        assert(aiLocalIndex == pIBuffer->GetData() + 3 * iTQuantity);
               
             

        // The duplication of vertices at the seam cause the automatically
        // generated bounding volume to be slightly off center.  Reset the bound
        // to use the true information.
        pkMesh->getBoundary()->setCenter(tintaVector3f::zAxis);
        pkMesh->getBoundary()->setRadius(radius);
        pkMesh->genNormals();
        return pkMesh;
    }

    tintaDrawable* tintaSceneManager::createBox(float xSize, float ySize, float zSize, bool inside) {
        int vq = 8;
        int iq = 12;

        tintaAttributes attrib;

        attrib.setPositionChannels(3);
        attrib.setTCoordChannels(0, 2);

        tintaTriMesh* mesh = M_NEW tintaTriMesh();

        mesh->createVBuffer(attrib, vq);
        mesh->createIBuffer(3 * iq);

        tintaVertexBuffer* pVBuff = mesh->getVBuffer(); //M_NEW tintaVertexBuffer( mAttr, vq );
        tintaIndexBuffer*  pIBuffer = mesh->getIBuffer(); //M_NEW tintaIndexBuffer(3 * iq);

        // generate geometry
        pVBuff->getPosition3(0) = tintaVector3f(-xSize, -ySize, -zSize);
        pVBuff->getPosition3(1) = tintaVector3f(+xSize, -ySize, -zSize);
        pVBuff->getPosition3(2) = tintaVector3f(+xSize, +ySize, -zSize);
        pVBuff->getPosition3(3) = tintaVector3f(-xSize, +ySize, -zSize);
        pVBuff->getPosition3(4) = tintaVector3f(-xSize, -ySize, +zSize);
        pVBuff->getPosition3(5) = tintaVector3f(+xSize, -ySize, +zSize);
        pVBuff->getPosition3(6) = tintaVector3f(+xSize, +ySize, +zSize);
        pVBuff->getPosition3(7) = tintaVector3f(-xSize, +ySize, +zSize);

        if ( attrib.getMaxTCoords() > 0 ){
            for ( int i = 0; i < attrib.getMaxTCoords(); i++ ){
                if ( attrib.hasTCoord(i) ){
                   
                    pVBuff->getTextureCoord2(i, 0) = tintaVector2f(0.25f, 0.75f);
                    pVBuff->getTextureCoord2(i, 1) = tintaVector2f(0.75f, 0.75f);
                    pVBuff->getTextureCoord2(i, 2) = tintaVector2f(0.75f, 0.25f);
                    pVBuff->getTextureCoord2(i, 3) = tintaVector2f(0.25f, 0.25f);

                    pVBuff->getTextureCoord2(i, 4) = tintaVector2f(0.25f, 0.75f);
                    pVBuff->getTextureCoord2(i, 5) = tintaVector2f(0.75f, 0.75f);
                    pVBuff->getTextureCoord2(i, 6) = tintaVector2f(0.75f, 0.25f);
                    pVBuff->getTextureCoord2(i, 7) = tintaVector2f(0.25f, 0.25f);
                }
            }
        }

        int* indices = pIBuffer->GetData();
        indices[0] = 0;  indices[1] = 2;  indices[2] = 1;
        indices[3] = 0;  indices[4] = 3;  indices[5] = 2;
        indices[6] = 0;  indices[7] = 1;  indices[8] = 5;
        indices[9] = 0;  indices[10] = 5;  indices[11] = 4;
        indices[12] = 0;  indices[13] = 4;  indices[14] = 7;
        indices[15] = 0;  indices[16] = 7;  indices[17] = 3;
        indices[18] = 6;  indices[19] = 4;  indices[20] = 5;
        indices[21] = 6;  indices[22] = 7;  indices[23] = 4;
        indices[24] = 6;  indices[25] = 5;  indices[26] = 1;
        indices[27] = 6;  indices[28] = 1;  indices[29] = 2;
        indices[30] = 6;  indices[31] = 2;  indices[32] = 3;
        indices[33] = 6;  indices[34] = 3;  indices[35] = 7;
        mesh->setCullMode(tintaDrawable::CULL_NEVER);

        //if (mInside)
        //    ReverseTriangleOrder(iq, indices);
        if ( inside ){
            for (int i = 0; i < iq; i++)
            {
                int j1 = 3 * i + 1, j2 = j1 + 1;
                int iSave = indices[j1];
                indices[j1] = indices[j2];
                indices[j2] = iSave;
            }
        }
        //TransformData(pVBuff);

        mesh->updateModel();
        mesh->genNormals();
        return mesh;
    }


    tintaDrawable* tintaSceneManager::createMesh(const String &name){       
      
        return tintaTriMesh::createFromMesh(name);
    }

}