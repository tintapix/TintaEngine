#include "tintaImgWindow.h"
#include <tintaPredefine.h>
#include <tintaImage.h>
#include <tintaTexSpringMain.h>



namespace Tinta
{
    using namespace Graphica;



   String tintaImgWindow::mConfigName = "window.mfg";

   tintaVector2f convertFromGL(float posX, float posY, float winW, float winH) {

       float valX = posX > 0.f ? 1 + posX : 1.0f - (-posX);
       float valY = posY > 0.f ? 1 + posY : 1.0f - (-posY);
       //rez.my = TintaMath::lInterp(-1.f, 1.f, (winSize.my - pos.my) / winSize.my);
       return{ winW * (valX / 2.f),winH - winH * (valY / 2.f) };
   }

    void tintaImgWindow::create() {

        tintaWinApp::mWinApp = (tintaWinApp*)this;
        Main(0, nullptr);

       
    }

    bool tintaImgWindow::shown() const {
        return tintaWinApp::Visible();
    }

    tintaImgWindow::tintaImgWindow() : tintaGLApp(
        "Image"
        , 0
        , 0
        , 600
        , 600
        , tintaRGBAColor(0.f, 0.f, 0.f, 0.0f)
        , tintaImgWindow::mConfigName) {

        mPos = { { -1.f, -1.f },{ -1.f, 1.f },{ 1.f, 1.f },{ 1.f, -1.f } };    
    }
   
    

    tintaImgWindow::~tintaImgWindow()
    {        
    }


    void tintaImgWindow::updateGLData() {
         
        if ( mActions.size() == 0 )
            return;

        tintaGuiManager *gui = NULL_M;

        if( mpRenderer )            
            gui = mpRenderer->getFrontManager();

        CoreAssert(gui, "gui");

        if ( !gui )
            return;

        TINTA_LOCK_MUTEX(mDataMutex);        

        actionqueue_t::iterator a = mActions.begin();
        
        m_uint32 key = a->mKey;

        WinDataActions action = a->mAction;

        auto i = findImage( key );

        if ( mLayer ) {

            String strKey = toStringKey( key );
           
            switch ( action ) {                 
               
                case ActionRecreate:     
                case ActionUpdate:                

                    gui->delFromLayer( mLayer, strKey );
                    mpRenderer->releaseTexture( strKey );

                case ActionAdd:

                    if ( i != mImages.end() ) {

                        if ( i->mImg ) {

                            tintaTexture * tex = mpRenderer->addTexture( i->mImg, strKey );

                            const tintaTexture *oldFront = mLayer->getFrontTexture();

                            gui->addToLayer( mLayer, strKey );

                            if (action != ActionAdd && oldFront )
                                gui->setFront( mLayer, oldFront->getName() );

                                                                                  
                            if ( action != ActionUpdate && mLayer->getFrontTexture() ) {
                               // mParams = { 0.f, 0.f, 1.f };
                                posImage( mLayer->getFrontTexture()->getImage() );
                            }
                            //std::cout << "texture added " << "\n" << "\n";
                            //    posImage( i->mImg );
                        }
                    }                          
                
                    break;

                case ActionDelete:
                
                    if ( key != ZERO_ID ) {
                 
                        gui->delFromLayer( mLayer, strKey );
                        mpRenderer->releaseTexture( strKey );
                    }
                    else {
                        gui->deleteImageLayer(mLayer, true);
                        mPos = { { -1.f, -1.f },{ -1.f, 1.f },{ 1.f, 1.f },{ 1.f, -1.f } };
                        mLayer = gui->addImageLayer( mPos );
                    }

                    break;

                case ActionShow:
                    //std::cout << "action ActionShow " << "\n";
                    gui->setFront( mLayer, strKey );
                    if ( mLayer->getFrontTexture() )
                        posImage( mLayer->getFrontTexture()->getImage() );
                    break;
            }
           
            mActions.erase(a);
#if DEBUG_MODE
            std::cout << "Actions after erase" << mActions.size() << " key " << key << "\n";
#endif
            
        }       
    }
    void tintaImgWindow::posImage( const tintaImage * img ) {
           
        CoreAssert(img, "img");

        if ( !img )
            return;

        if ( (m_uint32)mWinWidth < img->width() || (m_uint32)mWinHeight < img->height() ) {
            mParams.mz = (float)min(mWinWidth, mWinHeight) / (float)std::max(img->width(), img->height());            
        }

        float wWidth = (float)mWinWidth;
        float wHeight = (float)mWinHeight;
        float iW = (float)img->width();
        float iH = (float)img->height();

        float xBeg = wWidth / 2 - iW / 2.f;
        float yBeg = wHeight / 2 - iH / 2.f;

        /*mPos = { { tintaGLRenderer::convertInGL({ xBeg ,        iH + yBeg },{ wWidth ,wHeight }) },
                 { tintaGLRenderer::convertInGL({ xBeg ,        yBeg },{ wWidth ,wHeight }) },
                 { tintaGLRenderer::convertInGL({ xBeg + iW ,yBeg },{ wWidth ,wHeight }) },
                 { tintaGLRenderer::convertInGL({ xBeg + iW ,iH + yBeg }, { wWidth ,wHeight }) } };*/
    
    }

    void tintaImgWindow::addHandler( const String &type, const String &execute ) {

        if ( type == "mouse_move") {
            mOnMouseMove = execute;
        }          
        else if (type == "key") {
            mOnKey = execute;       
        }
    }

    void tintaImgWindow::delHandlers() {
        mOnKey = "";
        mOnMouseMove = "";
    }
    
    void tintaImgWindow::OnKeyStateChanged(tintaControls::EnInnerKeys key, bool pressed ) {


        /*
            ML_UP = 0xC8,    // UpArrow on arrow keypad
            ML_PGUP = 0xC9,    // PgUp on arrow keypad
            ML_LEFT = 0xCB,    // LeftArrow on arrow keypad
            ML_RIGHT = 0xCD,    // RightArrow on arrow keypad
            ML_END = 0xCF,    // End on arrow keypad
        */
        switch ( key ) {

            case tintaControls::ML_UP:               
                mParams.my -= 0.01f;                
                break;
            case tintaControls::ML_DOWN:                
                mParams.my += 0.01f;
                break;
            case tintaControls::ML_LEFT:
                mParams.mx -= 0.01f;
                break;
            case tintaControls::ML_RIGHT:
                mParams.mx += 0.01f;
                break;
            case tintaControls::ML_F11:
                /*if (mImg) {
                    m_uint8* data = mImg->data();
                    for ( int i = 0; i < 100; i++ )
                        data[i] = 100;
                    mpRenderer->releaseTexture(mTexture);
                    mTexture = mpRenderer->addTexture(mImg, "image_name");
                }*/

                break;
        }
        
        if (mOnKey.size() > 0) {

            if (tintaTexSpringMain::getPtr())
                tintaTexSpringMain::getPtr()->executeCommand(mOnKey, tintaExecutingTask::enLocalTask);
        }        
    }

    void tintaImgWindow::OnMouseStateChanged(int x_new_pos, int y_new_pos) {
      
        if (mOnMouseMove.size() > 0) {
            if (tintaTexSpringMain::getPtr())
                tintaTexSpringMain::getPtr()->executeCommand(mOnMouseMove, tintaExecutingTask::enLocalTask);
        }
    }

    void tintaImgWindow::OnMouseWeelStateChanged(int param) {
      
        if ( param > 0 )
            mParams.mz += 0.1f;
        else {            
            mParams.mz -= 0.1f;
        }

        if (mParams.mz < 0.f)
            mParams.mz = 0.f;

    } 

    void tintaImgWindow::OnResize(int iWidth, int iHeight, bool bFullscreen) {
        tintaWinApp::OnResize(iWidth, iHeight, bFullscreen);
       // if( mImg )
        if ( mLayer && mLayer->getFrontTexture() )
            posImage( mLayer->getFrontTexture()->getImage() );
                
    }

    bool tintaImgWindow::onInitialize() {

        if ( !tintaGLApp::onInitialize() )
        {
            return false;
        }
        
        mKeyKeeper.regKeyCallback(this);
        mKeyKeeper.regMouseCallback(this);
              
        
        tintaGuiManager *gui = NULL_M;
        if( mpRenderer )
            gui = mpRenderer->getFrontManager();

        CoreAssert(gui, "gui");

        if ( !gui )
            return false;
        
        mLayer = gui->addImageLayer(mPos);        

        return true;
    }

    void tintaImgWindow::OnClose() {

    }

    void tintaImgWindow::OnIdle() {

        tintaWinApp::OnIdle();

        if ( mActions.size() > 0 )
            updateGLData();

       
        mpRenderer->clearBuffers(false);
        

        {
            TINTA_LOCK_MUTEX(mDataMutex);
                

            if ( mImages.size() > 0 )

                mpRenderer->drawImageLayer( mLayer, &mPos, mParams );
        }
               
        mpRenderer->displayBackBuffer();  

    }
    String tintaImgWindow::toStringKey(m_uint32 key) {

        StringStream s;
        s << std::hex << key;

        return s.str();
    }

    void tintaImgWindow::show(bool bShow) {
        //mVisible = bShow;
        Show( bShow );
    }

    tintaImgWindow::t_layers_i tintaImgWindow::findImage( m_uint32 key ) {

        auto i = std::find_if( mImages.begin(), mImages.end(), [&](auto val) {return val.mKey == key; } );
        if ( i != mImages.end() )
            return i;

        return mImages.end();
    }

    void tintaImgWindow::updateData( m_uint32 key ) {
        mActions.push_back( { ActionUpdate , key } );
    }

    void tintaImgWindow::fillData( m_uint32 key, const tintaUInt8Image * data ) {
        if (!data)
            return;

        auto image = findImage(key);

        if ( image != mImages.end() ) {
            tintaImage * img = image->mImg;

            if ( img ) {

                m_uint32 size = data->getImageSize();

                for (m_uint32 i = 0; i < size; i++) {

                    color_type pix = data->getPixel(i);

                    m_uint8* pixData = (*img)(i);
                    pixData[0] = pix.getVal(0);
                    pixData[1] = pix.getVal(1);
                    pixData[2] = pix.getVal(2);
                }
            }
        }

    }

    void tintaImgWindow::setData( m_uint32 key, const tintaUInt8Image * data ) {
        
        TINTA_LOCK_MUTEX( mDataMutex );
        fillData(key, data);     
        mActions.push_back({ ActionUpdate , key });
    }

    void tintaImgWindow::addData( m_uint32 key, const tintaUInt8Image *data ) {


        TINTA_LOCK_MUTEX( mDataMutex );
        //cout << "add data";
        if ( !data )
            return;

        bool deleted = removeImage( key );        
            
        tintaImage * img = NEW_T(tintaImage);
        imageFormat format{ IMG_RGB };       
        
        img->setName(toStringKey(key));
        
        if ( img->read( *data, toStringKey(key), format, IMG_UBYTE, SAMPLER_2D, true ) ) {
                     

            ImgAction action;

            if ( deleted )
                action.mAction = ActionRecreate;
            else
                action.mAction = ActionAdd;
            action.mKey = key;

            mImages.push_back({ key , img });

            fillData(key, data);

            mActions.push_back( action );

            return;
        }
        else
            DELETE_T( img, tintaImage );

        if ( deleted ) {     
            mActions.push_back({ ActionDelete , key });
        }        
    }

    void tintaImgWindow::deleteData( m_uint32 key ) {

        TINTA_LOCK_MUTEX(mDataMutex);

        bool deleted = false;

        if ( key == ZERO_ID ) {
            while ( mImages.size() != 0 )
                removeImage( mImages.back().mKey );
            deleted = true;
        }
        else {
            if (removeImage(key)) {
                mActions.push_back({ ActionDelete , key });
                deleted = true;
            }
        }

        if( deleted )
            mActions.push_back({ ActionDelete , key });
    }

    void tintaImgWindow::showImage( m_uint32 key ) {

        TINTA_LOCK_MUTEX(mDataMutex);

        mActions.push_back({ ActionShow , key });
    }

    bool tintaImgWindow::removeImage( m_uint32 key ) {
        
        
        auto image = findImage(key);
        if ( image != mImages.end() ) {

            tintaImage * img = image->mImg;
            if (img) {

                DELETE_T(img, tintaImage);
                image->mImg = nullptr;
                mImages.erase(image);
                return true;
            }
        }
        

        return false;
    }

    void tintaImgWindow::setData(m_uint32 key, m_uint32 index, const tintaPixel24b *color) {
               
            TINTA_LOCK_MUTEX(mDataMutex);

            auto image = findImage( key );

            tintaImage * img = image->mImg;

            if ( img ) {
                m_uint8* data = (*img)(index);

                data[0] = color->getVal(0);
                data[1] = color->getVal(1);
                data[2] = color->getVal(2);

                //std::cout << "action added"<<"\n";
              //  mActions.push_back({ ActionUpdate , key });
            }      
    }

    bool tintaImgWindow::keyState(int key, bool &bValid) const {
        
        bValid = false;

        if ( key < tintaControls::ML_ESCAPE || key >= tintaControls::ML_MAX_KEY )
            return false;

        return mKeyKeeper.isKeyPressed( ( tintaControls::EnInnerKeys) key );
    }

    tintaVector2i tintaImgWindow::mouseWinPos() const {
        return mKeyKeeper.getMousePos();
    }

    tintaVector2i tintaImgWindow::mouseImgPos() const {

        if (TintaMath::isZero(mParams.mz))
            return{ -1,-1 };

        tintaVector2i pos = mKeyKeeper.getMousePos();

        tintaVector2f leftUpPos = mPos.mLU;
        leftUpPos.mx -= mParams.mx;
        leftUpPos.mx *= mParams.mz;
        leftUpPos.my += mParams.my;
        leftUpPos.my *= mParams.mz;

        //cout << mParams.mz <<"\n";

        tintaVector2f rez = convertFromGL(leftUpPos.mx, leftUpPos.my, (float)mWinWidth, (float)mWinHeight );

        rez.mx = (float)pos.mx - rez.mx;
        rez.my = (float)pos.my - rez.my;

        return{ (int)(rez.mx / mParams.mz), (int)(rez.my / mParams.mz) };   
    }

           

    void tintaImgWindow::setData( m_uint32 key, m_uint32 x, m_uint32 y , const tintaPixel24b *color ) {

        auto image = findImage(key);

        tintaImage * img = image->mImg;

        if ( img ) {

            m_uint32 index = y * img->width() + x;

            m_uint8* data = (*img)(index);

            data[0] = color->getVal(0);
            data[1] = color->getVal(1);
            data[2] = color->getVal(2);

            //mActions.push_back( { ActionUpdate , key } );
        }
    }

}