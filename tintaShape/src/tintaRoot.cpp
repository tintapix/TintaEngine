/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaPredefine.h"
#include "tintaRoot.h"
#include <OCL/tintaGPUExt.h>
#include <OCL/tintaBox2dProgram.h>
#include <OCL/tintaClObjectContainer.h>

#include "tintaSceneManager.h"
#include "tintaDrawable.h"
#include "tintaGraphicConfig.h"

#include "tintaMaterial.h"
#include "tintaAction.h"
#include "tintaRGBAColor.h"
#include "tintaGuiManager.h"
#include <utf8.h>

#include <GL/tintaGLApp.h>

#include <tintaMemoryAllocator.h>

#if TINTA_LOCAL  ==  TINTA_RU
    #include "tintaShapeLocalRu.h"
#elif TINTA_LOCAL  ==  TINTA_EN
    #include "tintaShapeLocalEn.h"
#endif

#include <tintaCommFunc.h>



template<> Tinta::tintaLogger* Tinta::Singleton<Tinta::tintaLogger>::mPtr = 0;
template<> Graphica::tintaRoot* Tinta::Singleton<Graphica::tintaRoot>::mPtr = 0;

const Tinta::String Graphica::tintaRoot::mDummyName = _M("0x00000000");

Graphica::tintaVector2f coordToGL(const Graphica::tintaVector2f &pos, const Graphica::tintaVector2f &winSize, bool noRatio = false) {

    
    Graphica::tintaVector2f rez;

    
    if ( !noRatio ) {

        Graphica::tintaVector2f ar = Graphica::tintaRoot::getPtr()->getRenderer()->getAspectRatio();

        rez.mx = TintaMath::lInterp(-ar.mx, ar.mx, (pos.mx + winSize.mx / 2.f) / winSize.mx);
        rez.my = TintaMath::lInterp(-ar.my, ar.my, (pos.my + winSize.my / 2.f) / winSize.my);
    }
    else {        
        // in square by min size
        rez.mx = TintaMath::lInterp(-1.f, 1.f, (pos.mx + winSize.mx / 2.f) / winSize.mx);
        rez.my = TintaMath::lInterp(-1.f, 1.f, (pos.my + winSize.my / 2.f) / winSize.my);
    }

    return rez;
}

Graphica::tintaVector2f fromGL(const Graphica::tintaVector2f &pos, const Graphica::tintaVector2f &winSize, bool noRatio = false) {

    

    if ( !noRatio ) {

        Graphica::tintaVector2f ar = Graphica::tintaRoot::getPtr()->getRenderer()->getAspectRatio();

        float valX = pos.mx > 0.f ? ar.mx + pos.mx : ar.mx - (-pos.mx);
        float valY = pos.my > 0.f ? ar.my + pos.my : ar.my - (-pos.my);

        return{ winSize.mx * (valX / (2.f * ar.mx)) - winSize.mx / 2.f ,winSize.my * (valY / (2.f * ar.my)) - winSize.my / 2.f };
    }
    else {
        
        // in square by min size
        float valX = pos.mx > 0.f ? 1.f + pos.mx : 1.f - (-pos.mx);
        float valY = pos.my > 0.f ? 1.f + pos.my : 1.f - (-pos.my);

        return{ winSize.mx * (valX / (2.f * 1.f)) - winSize.mx / 2.f , winSize.my * (valY / (2.f * 1.f)) - winSize.my / 2.f };
    }
}


Graphica::tintaVector2f sizeToGL(const Graphica::tintaVector2f &size, const Graphica::tintaVector2f &winSize, bool noRatio = false) {

    
    Graphica::tintaVector2f rez;
    if ( !noRatio ) {

        Graphica::tintaVector2f ar = Graphica::tintaRoot::getPtr()->getRenderer()->getAspectRatio();
        rez.mx = TintaMath::lInterp(0.f, 2.f * ar.mx, size.mx / winSize.mx);
        rez.my = TintaMath::lInterp(0.f, 2.f * ar.my, size.my / winSize.my);
    }
    else {        
        rez.mx = TintaMath::lInterp(0.f, 2.f , size.mx / winSize.mx);
        rez.my = TintaMath::lInterp(0.f, 2.f , size.my / winSize.my);
    }

    return rez;
}

Graphica::tintaVector2f sizeFromGL(const Graphica::tintaVector2f &pos, const Graphica::tintaVector2f &winSize, bool noRatio = false) {

    
    if (!noRatio) {
        Graphica::tintaVector2f ar = Graphica::tintaRoot::getPtr()->getRenderer()->getAspectRatio();
        return{ (pos.mx * winSize.mx) / (ar.mx * 2.f),  (pos.my * winSize.my) / (ar.my * 2.f) };
    }
    else
        return{ ( pos.mx * winSize.mx ) / (2.f),  (pos.my * winSize.my) / (2.f) };
}

namespace tintaPoints {

    using namespace Graphica;

    int addlayer(SCRIPT_STATE *L) {


        String name = GET_VAL_STRING(L, 1);


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        float size = GET_VAL_FLOAT(L, 2);

        if (size <= 0.f) {
            StringStream msg;
            msg << _M("Wrong size: ");
            msg << size;
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
            TROW_ERR_FUNC(L, msg.str().c_str());
        }


        String typeName = _M("point");
        Graphica::PrimitiveType type = Graphica::PRIMITIVE_POINT;

        if (IS_VAL_STRING(L, 3)) {
            typeName = GET_VAL_STRING(L, 3);
            if (typeName == _M("point")) {
                type = Graphica::PRIMITIVE_POINT;
            }
            else if (typeName == _M("line")) {
                type = Graphica::PRIMITIVE_LINE;
            }
            else if (typeName == _M("linestrip")) {
                type = Graphica::PRIMITIVE_LINE_STRIP;
            }
            else if (typeName == _M("polygon")) {
                type = Graphica::PRIMITIVE_POLY;
            }
            else {
                StringStream msg;
                msg << _M("Wrong primitive type: ");
                msg << typeName;
                msg << _M(" Should be: \"point\",\"line\",\"polygon\" ");
                msg << type;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }

        if (r) {
            if ( !r->getSceneManager()->addPointsLayer(name, size, type) ) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }


        return 0;
    }


    int dellayer(SCRIPT_STATE *L) {


        String name = GET_VAL_STRING(L, 1);


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (r) {
            r->getSceneManager()->delPointsLayer(name);
        }
        return 0;
    }

    int visible(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( r ) {                     

            tintaGLPoints * pl = r->getSceneManager()->getPointsLayer(name);
            if ( !pl ) {

                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());

            }
            
            PUSH_BOOL_ARG(L, pl->mVisible );
            
            return 1;
        }      
        return 0;
    }

    int setvisible(SCRIPT_STATE *L) {


        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (r) {

            tintaGLPoints * pl = r->getSceneManager()->getPointsLayer(name);
            if (!pl) {

                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());

            }
            pl->mVisible = GET_VAL_BOOL(L, 2);

        }
        return 0;
    }

    int dellayers(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( r ) {
            r->getSceneManager()->delPointsLayers();
        }
        return 0;
    }


    int movelayer(SCRIPT_STATE *L) {


        String name = GET_VAL_STRING(L, 1);


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);

            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            tintaVector2f shift(GET_VAL_FLOAT(L, 2), GET_VAL_FLOAT(L, 3));

            tintaVector2i s = Graphica::tintaRoot::getPtr()->getRenderer()->getWindowSize();

            tintaVector2f shiftGL = sizeToGL({ (float)shift.mx, (float)shift.my }, { (float)s.mx, (float)s.my }, true );
           
            for (auto &it : p->mPoints) {
                it.mPoint += shiftGL;
                it.mPointWin += shift;
            }

        }
        return 0;
    }

    int swap(SCRIPT_STATE *L) {


        String name1 = GET_VAL_STRING(L, 1);
        String name2 = GET_VAL_STRING(L, 2);


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {           

            if (!r->getSceneManager()->getPointsLayer(name1)) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name1;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }            

            if (!r->getSceneManager()->getPointsLayer(name2) ) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name2;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            r->getSceneManager()->swapPointsLayers( name1, name2 );

        }
        return 0;
    }

    int filllayer(SCRIPT_STATE *L) {


        String name = GET_VAL_STRING(L, 1);


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);

            if ( !p ) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }            

            m_uint32 size = GET_VAL_UINT(L, 2);           
            if ( size <= 0  ) {
                StringStream msg;
                msg << _M("Points quantity: ");
                msg << size;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

           tintaVector4f color( GET_VAL_FLOAT(L, 3), GET_VAL_FLOAT(L, 4), GET_VAL_FLOAT(L, 5), GET_VAL_FLOAT(L, 6) );

           p->mPoints.resize( size );
           Graphica::tintaGLPoint data;
           data.mColor = color;
           data.mPoint = tintaVector2f(0.f, 0.f);
           data.mPointWin = tintaVector2f(0.f,0.f);
           
           std::fill( p->mPoints.begin(), p->mPoints.end(), data );      

        }
        return 0;
    }


    int arraypos(SCRIPT_STATE *L) {


        String name = GET_VAL_STRING(L, 1);


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);

            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            //tintaVector2i shift(GET_VAL_INT(L, 2), GET_VAL_INT(L, 3));

            tintaVector2i s = Graphica::tintaRoot::getPtr()->getRenderer()->getWindowSize();

            //tintaVector2f shiftGL = sizeToGL({ (float)shift.mx, (float)shift.my }, { (float)s.mx, (float)s.my });    

            tintaBoxContainer* boxes = r->getBoxes();

            CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 2));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong array id: " << GET_VAL_UINT(L, 2);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            tintaArray2dFloat_t *arrf = box->getFloatBox();
            tintaArray2dInt_t   *arri = NULL_M;

            if (!arrf)
                arri = box->getIntBox();


            if (!arrf && !arri) {

                StringStreamBasic msg;
                msg << "Wrong array type(should be int or float): " << GET_VAL_UINT(L, 2);

                TROW_ERR_FUNC(L, msg.str().c_str());

            }

            if (arrf) {

                size_t size = arrf->getSize();

                // only 2 component coordinates
                if (size % 2 != 0) {
                    size -= 1;
                }

                size = min(size / 2, p->mPoints.size());
                size_t index = 0;
                for (size_t i = 0; i < size - 1; i = +2, index++) {

                    tintaVector2f pos = { arrf->getValReal(i), arrf->getValReal(i + 1) };
                    p->mPoints.at(index).mPointWin = pos;

                    tintaVector2f posGl = coordToGL({ (float)pos.mx, (float)pos.my }, { (float)s.mx, (float)s.my }, true);
                    p->mPoints.at(index).mPoint = posGl;
                }
            }
            else if (arri) {

                size_t size = arri->getSize();

                // only 2 component coordinates
                if (size % 2 != 0) {
                    size -= 1;
                }

                size = min(size / 2, p->mPoints.size());
                size_t index = 0;
                for (size_t i = 0; index < size; i += 2, index++) {

                    tintaVector2f pos = { (float)arri->getValReal(i), (float)arri->getValReal(i + 1) };
                    p->mPoints.at(index).mPointWin = pos;

                    tintaVector2f posGl = coordToGL({ (float)pos.mx, (float)pos.my }, { (float)s.mx, (float)s.my }, true);
                    p->mPoints.at(index).mPoint = posGl;
                }
            }

        }
        return 0;
    }


    int points(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);

        if (!p) {
            StringStream msg;
            msg << _M("Wrong layer name: ");
            msg << name;
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        PUSH_UINT_ARG(L, (m_uint32)p->mPoints.size());
        return 1;
    }





    int add(SCRIPT_STATE *L) {


        int argc = GET_QUANTITY(L);

        const int arg_count = 7;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: add(\"name\", x, y, r,g,b ) !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);

            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            tintaVector2f pos(GET_VAL_FLOAT(L, 2), GET_VAL_FLOAT(L, 3));
            tintaVector2i s = Graphica::tintaRoot::getPtr()->getRenderer()->getWindowSize();
            tintaVector2f posGl = coordToGL({ pos.mx, pos.my }, { (float)s.mx, (float)s.my }, true);

            tintaVector4f color(GET_VAL_FLOAT(L, 4), GET_VAL_FLOAT(L, 5), GET_VAL_FLOAT(L, 6), GET_VAL_FLOAT(L, 7));
            p->mPoints.push_back( { posGl,pos,color } );

        }
        return 0;
    }

    int move(SCRIPT_STATE *L) {

        int argc = GET_QUANTITY(L);
        const int arg_count = 4;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: move(\"name\",index, x,y, ) !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);
            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            m_uint32 index = GET_VAL_UINT(L, 2);

            if (index >= p->mPoints.size()) {
                StringStream msg;
                msg << _M("Wrong point index: ");
                msg << index;
                msg << _M("Quantity is : ") << p->mPoints.size();

                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            tintaVector2f pos(GET_VAL_FLOAT(L, 3), GET_VAL_FLOAT(L, 4));
            tintaVector2i s = Graphica::tintaRoot::getPtr()->getRenderer()->getWindowSize();

            tintaVector2f posGl = coordToGL({ (float)pos.mx, (float)pos.my }, { (float)s.mx, (float)s.my }, true);
           
            p->mPoints.at(index).mPoint = posGl;
            p->mPoints.at(index).mPointWin = pos;
            
        }
        return 0;
    }

   
    int setcolor(SCRIPT_STATE *L) {
        int argc = GET_QUANTITY(L);
        const int arg_count = 5;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: setcolor(\"name\",index, r,g,b ) !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);
            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            m_uint32 index = GET_VAL_UINT(L, 2);

            if (index >= p->mPoints.size()) {
                StringStream msg;
                msg << _M("Wrong point index: ");
                msg << index;
                msg << _M("Quantity is : ") << p->mPoints.size();

                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            tintaVector4f color(GET_VAL_FLOAT(L, 3), GET_VAL_FLOAT(L, 4), GET_VAL_FLOAT(L, 5), GET_VAL_FLOAT(L, 6));


            //Graphica::ListPoints_t::iterator it = p->mPoints.begin();

            //for (m_uint32 i = 0; i < p->mPoints.size(); i++, it++) {
            //     if (i == index) {
            p->mPoints.at(index).mColor = color;
            //        break;
            //    }
            //}
        }
        return 0;
    }

    int getcolor(SCRIPT_STATE *L) {
        int argc = GET_QUANTITY(L);
        const int arg_count = 2;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: getcolor(\"name\",index ) !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);
            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            m_uint32 index = GET_VAL_UINT(L, 2);

            if (index >= p->mPoints.size()) {
                StringStream msg;
                msg << _M("Wrong point index: ");
                msg << index;
                msg << _M("Quantity is : ") << p->mPoints.size();

                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }


            //Graphica::ListPoints_t::iterator it = p->mPoints.begin();
            //tintaVector3f color;

            // for ( m_uint32 i = 0; i < p->mPoints.size(); i++, it++ ) {
            //   if ( i == index ) {
            //        color = it->mColor;
            //         break;
            //     }
            // }
            PUSH_FLOAT_ARG(L, p->mPoints.at(index).mColor.mx);
            PUSH_FLOAT_ARG(L, p->mPoints.at(index).mColor.my);
            PUSH_FLOAT_ARG(L, p->mPoints.at(index).mColor.mz);
            PUSH_FLOAT_ARG(L, p->mPoints.at(index).mColor.mw);

            return 4;
        }
        return 0;
    }

    int getsize(SCRIPT_STATE *L) {
        int argc = GET_QUANTITY(L);
        const int arg_count = 1;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: getsize(\"name\") !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);
            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            PUSH_FLOAT_ARG(L, p->mPointSize);

            return 1;
        }
        return 0;
    }

    int getpos(SCRIPT_STATE *L) {
        int argc = GET_QUANTITY(L);
        const int arg_count = 2;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: getpos(\"name\",index ) !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);
            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            m_uint32 index = GET_VAL_UINT(L, 2);

            if (index >= p->mPoints.size()) {
                StringStream msg;
                msg << _M("Wrong point index: ");
                msg << index;
                msg << _M("Quantity is : ") << p->mPoints.size();

                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }


            //Graphica::ListPoints_t::iterator it = p->mPoints.begin();
            //tintaVector2i pos;

            // for (m_uint32 i = 0; i < p->mPoints.size(); i++, it++) {
            //    if (i == index) {
            //        pos = it->mPointWin;
            //       break;
            //   }
            // }           

            PUSH_FLOAT_ARG(L, p->mPoints.at(index).mPointWin.mx);
            PUSH_FLOAT_ARG(L, p->mPoints.at(index).mPointWin.my);

            return 2;
        }
        return 0;
    }

    int setsize(SCRIPT_STATE *L) {
        int argc = GET_QUANTITY(L);
        const int arg_count = 2;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: setsize(\"name\", size ) !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);
            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            float size(GET_VAL_FLOAT(L, 2));

            p->mPointSize = size;
        }
        return 0;
    }

    int del(SCRIPT_STATE *L) {


        int argc = GET_QUANTITY(L);

        const int arg_count = 2;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: del(\"name\", index ) !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);

            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }
            m_uint32 index = GET_VAL_UINT(L, 2);

            if (index >= p->mPoints.size()) {
                StringStream msg;
                msg << _M("Wrong point index: ");
                msg << index;
                msg << _M("Quantity is : ") << p->mPoints.size();

                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }


            //Graphica::ListPoints_t::iterator it = p->mPoints.begin();
            //Graphica::tintaGLPoint pLast = p->mPoints.at( p->mPoints.size() );

            std::swap(p->mPoints.at(index), p->mPoints.at(p->mPoints.size() - 1));
            p->mPoints.pop_back();

            //for ( m_uint32 i = 0; i < p->mPoints.size(); i++, it++ ) {
            //  if ( i == index ) {
            //      p->mPoints.erase( it );
            //      break;
            //  }
            //}            
        }
        return 0;
    }

    int find(SCRIPT_STATE *L) {


        int argc = GET_QUANTITY(L);

        const int arg_count = 3;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: find(\"name\", x,y, ) !");
        }

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {

            Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);
            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }



            tintaVector2f pos(GET_VAL_FLOAT(L, 2), GET_VAL_FLOAT(L, 3));
            //tintaVector2i s = Graphica::tintaRoot::getPtr()->getRenderer()->getWindowSize();

            bool rez = false;

            m_uint32 i = 0;
            m_uint32 index = 0;            

            for (auto it : p->mPoints) {

                float halfSize = p->mPointSize / 2;

                float xMin = it.mPointWin.mx - halfSize;
                float yMin = it.mPointWin.my - halfSize;

                float xMax = it.mPointWin.mx + halfSize;
                float yMax = it.mPointWin.my + halfSize;

                if (pos.mx <= xMax
                    && pos.mx >= xMin
                    && pos.my <= yMax
                    && pos.my >= yMin) {
                    index = i;
                    rez = true;

                    break;
                }
                i++;
            }

            if (rez) {
                PUSH_UINT_ARG(L, index); // return id
                return 1;
            }
        }
        return 0;
    }

    int findlayer(SCRIPT_STATE *L) {


        int argc = GET_QUANTITY(L);

        const int arg_count = 2;

        if (argc != arg_count) {
            TROW_ERR_FUNC(L, "Wrong arguments quantity: find(\"name\", x,y, ) !");
        }

        //String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if ( r ) {


            /*Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayer(name);
            if (!p) {
                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }*/

            const Graphica::tintaGLPoints* p = r->getSceneManager()->getPointsLayerFirst();

            tintaVector2f pos(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2));
            //tintaVector2i s = Graphica::tintaRoot::getPtr()->getRenderer()->getWindowSize();

            
            String name;
            for ( ; p; p = r->getSceneManager()->getPointsLayerNext() ) {                                          

                bool found = false;
                for (auto it : p->mPoints) {

                    float halfSize = p->mPointSize / 2;

                    float xMin = it.mPointWin.mx - halfSize;
                    float yMin = it.mPointWin.my - halfSize;

                    float xMax = it.mPointWin.mx + halfSize;
                    float yMax = it.mPointWin.my + halfSize;

                    if ( pos.mx <= xMax
                        && pos.mx >= xMin
                        && pos.my <= yMax
                        && pos.my >= yMin ) {
                        found = true;
                        break;
                    }                    
                }

                if ( found ) {
                    name = p->mName;
                    break;
                }
            }

            if ( name.length() > 0  ) {

                PUSH_STRING_ARG( L, name.c_str() ); // return id

                return 1;
            }
        }
        return 0;
    }
}

namespace tintaArrays {
    

    using namespace Graphica;



    /*
    @name array.delall
    @description Deletes all arrays
    @param Array id
    */
    int delall(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        boxes->clear();

        return 0;
    }


    int del(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        boxes->delObj(GET_VAL_UINT(L, 1));

        return 0;
    }


    /*
        @name array.addf
        @description Adds float vector array
        @return Array id
    */
    int addf(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        m_uint32 id = boxes->addObj( tintaFloatValVector::str_floats_vector );

        if (id == Graphica::ZERO_ID) {

            Graphica::StringStreamBasic msg;
            msg << "Box creating error";

            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), Graphica::msg_error, false);

            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        PUSH_INT_ARG(L, id); // return id

        return 1;
    }
     

    


    /*
        @name array.getf
        @description Returns array values by index
        @param Array id
        @param Index
        @return float value
    */
    int getf(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if (!box) {

            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);

            TROW_ERR_FUNC(L, msg.str().c_str());          
        }


        m_float32 val = 0.f;
        size_t index = (size_t)GET_VAL_INT(L, 2);        

        tintaArray2dFloat_t *arr = box->getFloatBox();
        if (!arr) {

            StringStreamBasic msg;
            msg << "Wrong operation for this array type";

            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        if (index < arr->getSize()) {
            val = arr->getValReal(index);
        }
        else {
            StringStreamBasic msg;
            msg << "Out of boundary for index: ";
            msg << index;
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        PUSH_FLOAT_ARG( L, val); //	
        return 1;
    }


    /*
        @name array.pop
        @description Removes last value
        @param Array id        
    */
    int popf(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if ( !box ) {
            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        tintaArray2dFloat_t *arr = box->getFloatBox();
        if (!arr) {

            StringStreamBasic msg;
            msg << "Wrong operation for this array type";

            TROW_ERR_FUNC(L, msg.str().c_str());

        }
        arr->popBack();

        return 0;
    }

    /*
        @name array.push
        @description Adds value in tail
        @param Array id
        @param Value
    */
    int pushf(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if ( !box ) {

            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);

            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        float val = GET_VAL_FLOAT(L, 2);
        tintaArray2dFloat_t *arr = box->getFloatBox();
        if (!arr) {

            StringStreamBasic msg;
            msg << "Wrong operation for this array type";

            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        arr->pushBack( val );

        return 0;
    }
       

    /*
    @name array.set
    @description Sets value in position
    @param Array id
    @param Index
    @param Value
    */
    int setf(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if (!box) {

            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);

            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        m_uint32 index = GET_VAL_UINT(L, 2);
        
        tintaArray2dFloat_t *arr = box->getFloatBox();

        if (!arr) {

            StringStreamBasic msg;
            msg << "Wrong operation for this array type";

            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        float val = GET_VAL_FLOAT(L, 3);
          
        if (index < arr->getSize()) {
            arr->setFast(index, val);
        }
        else {
            StringStreamBasic msg;
            msg << "Out of boundary for index: ";
            msg << index;
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        return 0;
    }


    /*
    @name array.size
    @description Array size
    @param Array id
    @return Array size
    */
    int size( SCRIPT_STATE *L ) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if (!box) {

            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);

            TROW_ERR_FUNC(L, msg.str().c_str());
        }       
           
        PUSH_UINT_ARG(L, box->getSize()); //	     

        return 1;
    }


    /*
    @name array.resize
    @description Array resize   
    */
    int resize(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if (!box) {

            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);

            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        if( !IS_VAL_INTEGER(L, 2) || !box->create( GET_VAL_UINT(L, 2) ) ) {
            StringStreamBasic msg;
            msg << "Wrong array size";
            TROW_ERR_FUNC( L, msg.str().c_str() );
        }

        return 0;
    }


    /*
    @name array.addi
    @description Adds float vector array
    @return Array id
    */
    int addi(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        m_uint32 id = boxes->addObj(tintaIntValVector::str_int32_vector);

        if (id == Graphica::ZERO_ID) {

            Graphica::StringStreamBasic msg;
            msg << "array creating error";

            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), Graphica::msg_error, false);

            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        PUSH_INT_ARG(L, id); // return id

        return 1;
    }


    




    /*
    @name array.geti
    @description Returns array values by index
    @param Array id
    @param Index
    @return float value
    */
    int geti(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if (!box) {

            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);

            TROW_ERR_FUNC(L, msg.str().c_str());


        }


        int val = 0;
        size_t index = (size_t)GET_VAL_INT(L, 2);

        tintaArray2dInt_t *arr = box->getIntBox();

        if (!arr) {

            StringStreamBasic msg;
            msg << "Wrong operation for this array type";

            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        if (index < arr->getSize()) {
            val = arr->getValReal(index);
        }
        else {
            StringStreamBasic msg;
            msg << "Out of boundary for index: ";
            msg << index;
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        PUSH_INT_ARG(L, val); //	
        return 1;
    }


    /*
    @name array.popi
    @description Removes last value
    @param Array id
    */
    int popi(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if (!box) {
            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        tintaArray2dInt_t *arr = box->getIntBox();
        if (!arr) {

            StringStreamBasic msg;
            msg << "Wrong operation for this array type";

            TROW_ERR_FUNC(L, msg.str().c_str());

        }
        arr->popBack();

        return 0;
    }

    /*
    @name array.pushi
    @description Adds value in tail
    @param Array id
    @param Value
    */
    int pushi(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

        if ( !box ) {

            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT(L, 1);

            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        int val = GET_VAL_INT(L, 2);
        tintaArray2dInt_t *arr = box->getIntBox();
        if (!arr) {         

            StringStreamBasic msg;
            msg << "Wrong operation for this array type";

            TROW_ERR_FUNC(L, msg.str().c_str());
        
        }
        arr->pushBack(val);

        return 0;
    }


    /*
    @name array.seti
    @description Sets value in position
    @param Array id
    @param Index
    @param Value
    */
    int seti(SCRIPT_STATE *L) {

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();

        CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");

        tintaArrayBox *box = boxes->getObjEx( GET_VAL_UINT(L, 1) );

        if ( !box ) {

            StringStreamBasic msg;
            msg << "Wrong array id: " << GET_VAL_UINT( L, 1 );

            TROW_ERR_FUNC( L, msg.str().c_str() );
        }

        m_uint32 index = GET_VAL_UINT( L, 2 );

        tintaArray2dInt_t *arr = box->getIntBox();

        int val = GET_VAL_INT(L, 3);

        if (index < arr->getSize()) {
            arr->setFast(index, val);
        }
        else {
            StringStreamBasic msg;
            msg << "Out of boundary for index: ";
            msg << index;
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        return 0;
    }
      


    static const luaL_Reg arrays[] = {
        { ARRAY_delall, tintaArrays::delall },
        { ARRAY_size, tintaArrays::size },
        { ARRAY_resize, tintaArrays::resize },
        { ARRAY_addf, tintaArrays::addf },
        { ARRAY_del, tintaArrays::del },        
        { ARRAY_getvalf, tintaArrays::getf },
        { ARRAY_popf, tintaArrays::popf },
        { ARRAY_pushf, tintaArrays::pushf },
        { ARRAY_setf, tintaArrays::setf },
        { ARRAY_addi, tintaArrays::addi },        
        { ARRAY_getvali, tintaArrays::geti },
        { ARRAY_popi, tintaArrays::popi },
        { ARRAY_pushi, tintaArrays::pushi },
        { ARRAY_seti, tintaArrays::seti },
        { NULL, NULL } };

    int luaopen_util(lua_State *L) {
        luaL_newlib(L, arrays);
        return 1;
    }

    static const luaL_Reg loadedlibsArray[] = {
        { "_G", luaopen_base },
        { "array", luaopen_util },
        { NULL, NULL } };

    void registerLuaArray(SCRIPT_STATE *L) {

        //luaL_newlib(L, gpu);

        const luaL_Reg *lib;
        /* "require" functions from 'loadedlibs' and set results to global table */
        for (lib = loadedlibsArray; lib->func; lib++) {
            luaL_requiref(L, lib->name, lib->func, 1);
            lua_pop(L, 1);  /* remove lib */
        }
    }
}
// OCL functions
namespace tintaOCLFunctions {
    
    using namespace Graphica;
    
    /*
    Returns GPU description string
    */
    int platdescrip(SCRIPT_STATE *L) {        

        if( !Graphica::tintaRoot::getPtr() || !Graphica::tintaRoot::getPtr()->oclEnabled() )
            return 0;
               

        Tinta::tintaGPUExt *ext = Tinta::tintaGPUExt::getPtr();

        Tinta::String result = ext->getPlatformInfo();
        String err = ext->getError();
        if (err.length() > 0) {
            TROW_ERR_FUNC(L, err.c_str());
        }

        PUSH_STRING_ARG(L, result.c_str()); //	

        return 1;
    }

    /*
    Returns GPU devices description string
    param device id
    param GPUDevAll = 60		// all data
    */
    int devdescrip(SCRIPT_STATE *L) {
        if (!Graphica::tintaRoot::getPtr() || !Graphica::tintaRoot::getPtr()->oclEnabled())
        return 0;

        m_uint32 platId = GET_VAL_UINT(L, 1);

       

        Tinta::tintaGPUExt *ext = Tinta::tintaGPUExt::getPtr();

        
        Tinta::String result;
        if (IS_VAL_INTEGER(L, 2)) {

            m_uint32 deviceId = GET_VAL_UINT(L, 2);
            result = ext->getDeviceInfo(platId, &deviceId);
        }
        else
            result = ext->getDeviceInfo(platId, NULL_M);


        String err = ext->getError();
        if (err.length() > 0) {
            TROW_ERR_FUNC(L, err.c_str());
        }

        PUSH_STRING_ARG(L, result.c_str()); //	

        return 1;
    }

    /*
    Returns GPU platforms quantity
    */
    int platforms(SCRIPT_STATE *L) {
        if ( !Graphica::tintaRoot::getPtr() || !Graphica::tintaRoot::getPtr()->oclEnabled() )
            return 0;

        Tinta::tintaGPUExt *ext = Tinta::tintaGPUExt::getPtr();

        m_uint32 result = ext->getPlatformsIDs();
        String err = ext->getError();
        if (err.length() > 0) {
            TROW_ERR_FUNC(L, err.c_str());
        }
        PUSH_UINT_ARG(L, result); //	

        return 1;
    }

    /*
    Returns true if GPU functions enabled
    */
    int enabled(SCRIPT_STATE *L) {
        if ( Graphica::tintaRoot::getPtr() 
                && Graphica::tintaRoot::getPtr()->oclEnabled() ){
#ifdef USING_GPUCL
            PUSH_BOOL_ARG(L, true); //	
            return 1;
#endif
        }
        PUSH_BOOL_ARG(L, false); //	
        return 1;
    }

    int clear( SCRIPT_STATE *L ) {
        if (Graphica::tintaRoot::getPtr()
            && Graphica::tintaRoot::getPtr()->oclEnabled()) {

            Tinta::String program = GET_VAL_STRING_EX(L, 1);

            if ( program.length() != 0 ) {

                Tinta::tintaIClBase* clProg = Graphica::tintaRoot::getPtr()->getOCLProgram( program );
                if (clProg)
                    clProg->clearData();
                else {
                    Tinta::StringStream msg;
                    msg << _M("Wrong program name: ") << program;                    
                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
            }
            else {
                Tinta::StringStream msg;
                msg << _M("Wrong program name");
                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }

        return 0;
    }

    /*
    @name ocl.exist
    @description Determines whether the program exists or not
    @param program name or path
    @return true if program exists
    */
    int exist(SCRIPT_STATE *L) {
        if (Graphica::tintaRoot::getPtr()
            && Graphica::tintaRoot::getPtr()->oclEnabled()) {

            Tinta::String program = GET_VAL_STRING_EX(L, 1);

            Tinta::tintaIClBase* clProg = Graphica::tintaRoot::getPtr()->getOCLProgram(program);
            if ( clProg ) {
                PUSH_BOOL_ARG(L, true);
                return 1;
            }
        }
        return 0;
    }

    int create( SCRIPT_STATE *L ) {

        if (Graphica::tintaRoot::getPtr()
            && Graphica::tintaRoot::getPtr()->oclEnabled()) {

            Tinta::String program = GET_VAL_STRING_EX(L, 1);

            Tinta::String kernel = GET_VAL_STRING_EX(L, 2);           

            if (program.length() == 0) {
                TROW_ERR_FUNC(L, _M("Wrong program name"));
                return 0;
            }

            if (kernel.length() == 0) {
                TROW_ERR_FUNC(L, _M("Wrong kernel name"));
                return 0;
            }


            if ( !Graphica::tintaRoot::getPtr()->registerCl( program, kernel) ) {
                TROW_ERR_FUNC(L, _M("Error while register gpu program "));
            }

        }
        return 0;
    }

    int remove( SCRIPT_STATE *L ) {

        if ( Graphica::tintaRoot::getPtr()
            && Graphica::tintaRoot::getPtr()->oclEnabled() ) {

            Tinta::String program = GET_VAL_STRING_EX(L, 1);

            Tinta::String kernel = GET_VAL_STRING_EX(L, 2);

            if (program.length() == 0) {
                TROW_ERR_FUNC(L, _M("Wrong program name"));
                return 0;
            }
            if ( !Graphica::tintaRoot::getPtr()->getOCLProgram(program) ) {
                TROW_ERR_FUNC(L, _M("Wrong program name"));
            }

            Graphica::tintaRoot::getPtr()->deleteOCL(program);

        }
        return 0;
    }



    int setarray(SCRIPT_STATE *L) {

        if (!Graphica::tintaRoot::getPtr()
            || !Graphica::tintaRoot::getPtr()->oclEnabled())
            return 0;

     
        Tinta::String progName = GET_VAL_STRING_EX(L, 1);
        Tinta::tintaIClBase* prog = Graphica::tintaRoot::getPtr()->getOCLProgram(
            progName);

        if (!prog) {
            Tinta::StringStream msg;
            msg << _M("Wrong program name: ") << progName;
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        tintaBoxContainer* boxes = tintaRoot::getPtr()->getBoxes();
        m_uint32 idBox = GET_VAL_UINT(L, 2);
        m_uint32 memMask = GET_VAL_UINT(L, 3);
        tintaArrayBox *box = boxes->getObjEx(idBox);
        if ( !box ) {
            StringStream msg;
            msg << _M("Wrong array name: ") << box;
            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        void * ptrData = box->GetMemPtr();

        if ( !ptrData || !prog->setDataIn(ptrData, GpuArg_t::enArray, box->GetDataSize(), memMask ) ) {
            StringStream msg;
            msg << _M("Wrong array data or value: ") << box;
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        return 0;
    }

   
    int setvalue(SCRIPT_STATE *L) {

        if (!Graphica::tintaRoot::getPtr()
            || !Graphica::tintaRoot::getPtr()->oclEnabled())
            return 0;


        Tinta::String progName = GET_VAL_STRING_EX(L, 1);
        Tinta::tintaIClBase* prog = Graphica::tintaRoot::getPtr()->getOCLProgram(
            progName);

        if (!prog) {
            Tinta::StringStream msg;
            msg << _M("Wrong program name: ") << progName;
            TROW_ERR_FUNC(L, msg.str().c_str());

        }


        if (IS_VAL_INTEGER(L, 2)) {
            int   iVal = GET_VAL_INT(L, 2);
            if (!prog->setDataIn(
                (void*)&iVal, Tinta::GpuArg_t::enValue, sizeof(iVal), Tinta::enRead)) {
                Tinta::StringStream msg;
                msg << _M("Wrong value: ") << iVal;
                TROW_ERR_FUNC(L, msg.str().c_str());

            }
        }
        else if (IS_VAL_REAL(L, 2)) {
            float fVal = GET_VAL_FLOAT(L, 2);
            if (!prog->setDataIn(
                (void*)&fVal, Tinta::GpuArg_t::enValue, sizeof(fVal), Tinta::enRead)) {
                Tinta::StringStream msg;
                msg << _M("Wrong value: ") << fVal;
                TROW_ERR_FUNC(L, msg.str().c_str());

            }
        }

        return 0;
    }
    int execute(SCRIPT_STATE *L) {

        if ( !Graphica::tintaRoot::getPtr()
            || !Graphica::tintaRoot::getPtr()->oclEnabled() )
            return 0;

        Tinta::String progName = GET_VAL_STRING_EX(L, 1);
        Tinta::tintaIClBase* prog = Graphica::tintaRoot::getPtr()->getOCLProgram(
            progName);

        if ( !prog ) {
            Tinta::StringStream msg;
            msg << _M("Wrong program name: ") << progName;
            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        if ( !IS_VAL_STRING( L, 2 ) )
        {
            Tinta::StringStreamBasic msg;
            msg << "GPU function parameter #2 must be empty string or script to execute after finishing";

            prog->clearData();
            TROW_ERR_FUNC(L, msg.str().c_str());
            return 0;
        }
        Tinta::String onFinish = GET_VAL_STRING_EX(L, 2);

        m_uint32 width = GET_VAL_UINT(L, 3);
        m_uint32 height = 0;
        if (IS_VAL_INTEGER(L, 4))
            height = GET_VAL_UINT(L, 4);

        m_uint32 depth = 0;
        if (IS_VAL_INTEGER(L, 5))
            depth = GET_VAL_UINT(L, 5);

        m_uint32 paramRead = GET_VAL_UINT(L, 6);        

        if (height > 0) { // two dimensions
            m_uint32 globalsize[2] = { std::max(width, 1U), std::max(height, 1U) };

            m_uint32 readpos[1] = { paramRead };

            m_uint32 groupSize[2] = { 0, 0 };

            if (IS_VAL_INTEGER(L, 7) && IS_VAL_INTEGER(L, 8)) {

                groupSize[0] = GET_VAL_UINT(L, 7);
                groupSize[1] = GET_VAL_UINT(L, 8);
            }

            //int maxLen = 0;

            try {
                prog->execute( globalsize, sizeof(globalsize) / sizeof(size_t),
                    readpos, sizeof(readpos) / sizeof(size_t), (groupSize[0] == 0 || groupSize[1] == 0) ? NULL : groupSize);
                Graphica::tintaRoot* root = Graphica::tintaRoot::getPtr();
                root->getCommands()->push(onFinish);
            }
            catch (const Tinta::tintaException &e) {
                Tinta::StringStreamBasic msg;
                msg << "GPU parameters error for program: " << progName;
                msg << " (Exception) " << e.what();
                prog->clearData();
                TROW_ERR_FUNC(L, msg.str().c_str());

            }
        }
        else if (depth > 0) { // three dimensions
            m_uint32 globalsize[3] = { std::max(width, 1U), std::max(height, 1U),std::max(depth, 1U) };

            m_uint32 readpos[1] = { paramRead };

            m_uint32 groupSize[3] = { 0, 0, 0 };

            if (IS_VAL_INTEGER(L, 7) && IS_VAL_INTEGER(L, 8) && IS_VAL_INTEGER(L, 9)) {

                groupSize[0] = GET_VAL_UINT(L, 7);
                groupSize[1] = GET_VAL_UINT(L, 8);
                groupSize[2] = GET_VAL_UINT(L, 9);
            }

            //int maxLen = 0;

            try {
                prog->execute(globalsize, sizeof(globalsize) / sizeof(size_t),
                    readpos, sizeof(readpos) / sizeof(size_t), (groupSize[0] == 0 || groupSize[1] == 0 || groupSize[2] == 0) ? NULL : groupSize);
                Graphica::tintaRoot* root = Graphica::tintaRoot::getPtr();
                root->getCommands()->push(onFinish);
            }
            catch (const Tinta::tintaException &e) {
                Tinta::StringStreamBasic msg;
                msg << "GPU parameters error for program: " << progName;
                msg << " (Exception) " << e.what();
                prog->clearData();
                TROW_ERR_FUNC(L, msg.str().c_str());

            }
        }
        else { // one dimension
            size_t globalsize[1] = { (size_t)(TintaMath::max(width, 1) * TintaMath::max(height, 1)) };

            size_t readpos[1] = { paramRead };

            m_uint32 groupSize[1] = { 0 };

            if (IS_VAL_INTEGER(L, 7)) {

                groupSize[0] = GET_VAL_UINT(L, 7);
            }            

            try {
                prog->execute(globalsize, sizeof(globalsize) / sizeof(size_t), readpos, sizeof(readpos) / sizeof(size_t), groupSize[0] == 0 ? NULL : groupSize);

                Graphica::tintaRoot* root = Graphica::tintaRoot::getPtr();
                root->getCommands()->push(onFinish);
            }
            catch (const Tinta::tintaException &e) {
                Tinta::StringStreamBasic msg;
                msg << "GPU parameters error for program: " << progName;
                msg << " (Exception) " << e.what();
                prog->clearData();
                TROW_ERR_FUNC(L, msg.str().c_str());

            }


        }

        return 0;
    }

    static const luaL_Reg ocl[] = {
        { OCL_platdescrip, tintaOCLFunctions::platdescrip },
        { OCL_devdescrip, tintaOCLFunctions::devdescrip },
        { OCL_platforms, tintaOCLFunctions::platforms },
        { OCL_enabled, tintaOCLFunctions::enabled },
        { OCL_clear, tintaOCLFunctions::clear },
        { OCL_create, tintaOCLFunctions::create },
        { OCL_exist, tintaOCLFunctions::exist },
        { OCL_setarray, tintaOCLFunctions::setarray },
        { OCL_remove, tintaOCLFunctions::remove },
        { OCL_setvalue, tintaOCLFunctions::setvalue },
        { OCL_execute, tintaOCLFunctions::execute },
        { NULL, NULL } };

    int luaopen_util(lua_State *L) {
        luaL_newlib(L, ocl);
        return 1;
    }

    static const luaL_Reg loadedlibsOclu[] = {
        { "_G", luaopen_base },
        { "ocl", luaopen_util },
        { NULL, NULL } };

    void registerLuaOcl(SCRIPT_STATE *L) {

        //luaL_newlib(L, gpu);

        const luaL_Reg *lib;
        /* "require" functions from 'loadedlibs' and set results to global table */
        for (lib = loadedlibsOclu; lib->func; lib++) {
            luaL_requiref(L, lib->name, lib->func, 1);
            lua_pop(L, 1);  /* remove lib */
        }
    }

}

namespace GraphicFunctions {
    
    using namespace Tinta;
       
    Graphica::SceneType getType( const String &textType ) {
        textType[Graphica::Back];
        if (textType == "back")
            return Graphica::Back;
        if (textType == "scene")
            return Graphica::Scene;
        if (textType == "front")
            return Graphica::Front;
        if (textType == "camera")
            return Graphica::Camera;

        //Camera

        return Graphica::errorType;
    }

    int test(SCRIPT_STATE *L) {


        Graphica::tintaVector2f pos(10.f, 10.f);
        Graphica::tintaVector2f winSize(1000.f, 500.f);

        Graphica::tintaVector2f posGl = sizeToGL(pos, winSize, true);

        pos = sizeFromGL(posGl, winSize, true);


        return 0;
    }

    bool fillMaterial( SCRIPT_STATE *L, int indexFirst, Graphica::tintaDrawable* object, Graphica::tintaMaterial *m , const String &varName,  Graphica::tintaMaterialParser::MaterialDataType type ) {       
       
        CoreAssert( object && m, "material or object NULL");

        bool rez = true;
        switch ( type ) {

        case Graphica::tintaMaterialParser::matBool:
            if (IS_VAL_BOOL(L, indexFirst)) {
                //m->onBool(varName, Graphica::tintaMaterialParser::autoMaxElement, GET_VAL_BOOL(L, indexFirst), true);
                object->onMatVarBool(m->getName(), varName, GET_VAL_BOOL(L, indexFirst));              
            }
            break;
        case Graphica::tintaMaterialParser::matInt:
            if (IS_VAL_INTEGER(L, indexFirst)) {
               // m->onInt(varName, Graphica::tintaMaterialParser::autoMaxElement, GET_VAL_INT(L, indexFirst), true);
                object->onMatVarInt(m->getName(), varName, GET_VAL_INT(L, indexFirst));
                
            }
            break;
        case Graphica::tintaMaterialParser::matUInt:
            if (IS_VAL_INTEGER(L, indexFirst)) {
               // m->onUInt(varName, Graphica::tintaMaterialParser::autoMaxElement, GET_VAL_UINT(L, indexFirst), true);
                object->onMatVarUInt(m->getName(), varName, GET_VAL_UINT(L, indexFirst));
            }
            break;
        case Graphica::tintaMaterialParser::matFloat:
            if ( IS_VAL_REAL(L, indexFirst) ) {
       
                object->onMatVarFloat(m->getName(), varName, GET_VAL_FLOAT(L, indexFirst));
            }
            break;
        case Graphica::tintaMaterialParser::matDouble:
            if (IS_VAL_REAL(L, indexFirst)) {               
                object->onMatVarDouble(m->getName(), varName, GET_VAL_DOUBLE(L, indexFirst));
            }
            break;
        case Graphica::tintaMaterialParser::matfVec2:
            if (IS_VAL_REAL(L, indexFirst) && IS_VAL_REAL(L, indexFirst + 1)) {
               // m->onfVec2(varName, Graphica::tintaMaterialParser::autoMaxElement,
               //     { GET_VAL_FLOAT(L, indexFirst), GET_VAL_FLOAT(L, indexFirst + 1) }, true);
                object->onMatVarfVec2(m->getName(), varName, { GET_VAL_FLOAT(L, indexFirst), GET_VAL_FLOAT(L, indexFirst + 1) });
            }
            break;
        case Graphica::tintaMaterialParser::matfVec3:
            if (IS_VAL_REAL(L, indexFirst) && IS_VAL_REAL(L, indexFirst + 1) && IS_VAL_REAL(L, indexFirst + 2)) {
               // m->onfVec3(varName, Graphica::tintaMaterialParser::autoMaxElement,
               // { GET_VAL_FLOAT(L, indexFirst), GET_VAL_FLOAT(L, indexFirst + 1) , GET_VAL_FLOAT(L, indexFirst + 2) }, true);
                
                object->onMatVarfVec3(m->getName(), varName, { GET_VAL_FLOAT(L, indexFirst), GET_VAL_FLOAT(L, indexFirst + 1) , GET_VAL_FLOAT(L, indexFirst + 2) } );
            }
            break;
        case Graphica::tintaMaterialParser::matfVec4:
            if (IS_VAL_REAL(L, indexFirst)
                && IS_VAL_REAL(L, indexFirst + 1)
                && IS_VAL_REAL(L, indexFirst + 2)
                && IS_VAL_REAL(L, indexFirst + 3)) {
                //m->onfVec4(varName, Graphica::tintaMaterialParser::autoMaxElement,
               ////     { GET_VAL_FLOAT(L, indexFirst), GET_VAL_FLOAT(L, indexFirst + 1) ,
               //                     GET_VAL_FLOAT(L, indexFirst + 2), GET_VAL_FLOAT(L, indexFirst + 3) }, true);
                object->onMatVarfVec4(m->getName(), varName, { GET_VAL_FLOAT(L, indexFirst), GET_VAL_FLOAT(L, indexFirst + 1) ,
                                         GET_VAL_FLOAT(L, indexFirst + 2), GET_VAL_FLOAT(L, indexFirst + 3) });
            }
            break;
        case Graphica::tintaMaterialParser::matfMat3:
            if (IS_VAL_REAL(L, indexFirst)
                && IS_VAL_REAL(L, indexFirst + 1)
                && IS_VAL_REAL(L, indexFirst + 2)
                && IS_VAL_REAL(L, indexFirst + 3)
                && IS_VAL_REAL(L, indexFirst + 4)
                && IS_VAL_REAL(L, indexFirst + 5)
                && IS_VAL_REAL(L, indexFirst + 6)
                && IS_VAL_REAL(L, indexFirst + 7)
                && IS_VAL_REAL(L, indexFirst + 8)
               ) {

                tintaMatrix33 vr(GET_VAL_FLOAT(L, indexFirst)
                    , GET_VAL_FLOAT(L, indexFirst + 1)
                    , GET_VAL_FLOAT(L, indexFirst + 2)
                    , GET_VAL_FLOAT(L, indexFirst + 3)
                    , GET_VAL_FLOAT(L, indexFirst + 4)
                    , GET_VAL_FLOAT(L, indexFirst + 5)
                    , GET_VAL_FLOAT(L, indexFirst + 6)
                    , GET_VAL_FLOAT(L, indexFirst + 7)
                    , GET_VAL_FLOAT(L, indexFirst + 8)
                    );

                //m->onfMat3(varName, Graphica::tintaMaterialParser::autoMaxElement, vr);
                object->onMatVarfMat3(m->getName(), varName, vr );               
            }
            break;
        case Graphica::tintaMaterialParser::matfMat4:
            if (IS_VAL_REAL(L, indexFirst)
                && IS_VAL_REAL(L, indexFirst + 1)
                && IS_VAL_REAL(L, indexFirst + 2)
                && IS_VAL_REAL(L, indexFirst + 3)
                && IS_VAL_REAL(L, indexFirst + 4)
                && IS_VAL_REAL(L, indexFirst + 5)
                && IS_VAL_REAL(L, indexFirst + 6)
                && IS_VAL_REAL(L, indexFirst + 7)
                && IS_VAL_REAL(L, indexFirst + 8)
                && IS_VAL_REAL(L, indexFirst + 9)
                && IS_VAL_REAL(L, indexFirst + 10)
                && IS_VAL_REAL(L, indexFirst + 11)
                && IS_VAL_REAL(L, indexFirst + 12)
                && IS_VAL_REAL(L, indexFirst + 13)
                && IS_VAL_REAL(L, indexFirst + 14)
                && IS_VAL_REAL(L, indexFirst + 15)) {

                tintaMatrix44 vr(GET_VAL_FLOAT(L, indexFirst)
                    , GET_VAL_FLOAT(L, indexFirst + 1)
                    , GET_VAL_FLOAT(L, indexFirst + 2)
                    , GET_VAL_FLOAT(L, indexFirst + 3)
                    , GET_VAL_FLOAT(L, indexFirst + 4)
                    , GET_VAL_FLOAT(L, indexFirst + 5)
                    , GET_VAL_FLOAT(L, indexFirst + 6)
                    , GET_VAL_FLOAT(L, indexFirst + 7)
                    , GET_VAL_FLOAT(L, indexFirst + 8)
                    , GET_VAL_FLOAT(L, indexFirst + 9)
                    , GET_VAL_FLOAT(L, indexFirst + 10)
                    , GET_VAL_FLOAT(L, indexFirst + 11)
                    , GET_VAL_FLOAT(L, indexFirst + 12)
                    , GET_VAL_FLOAT(L, indexFirst + 13)
                    , GET_VAL_FLOAT(L, indexFirst + 14)
                    , GET_VAL_FLOAT(L, indexFirst + 15));

                object->onMatVarfMat4(m->getName(), varName, vr);
                
            }

            break;
        case Graphica::tintaMaterialParser::matTexture:

            if ( IS_VAL_STRING(L, indexFirst) ) {

                Graphica::tintaImage * img = Graphica::tintaRoot::getPtr()->getImage(GET_VAL_STRING(L, indexFirst));
                if ( img ) {
                    Graphica::tintaTexture *texture = Graphica::tintaRoot::getPtr()->getRenderer()->addTexture(img, img->getName() );
                    object->onMatVarTexture(m->getName(), varName, texture, 0);                                     
                }
            }
            break;
        default:
            rez = false;            
        }

        return rez;
    }


    

    /*
        @name d3.getpos
        @description Returns entity center coordinates
        @param type "back","scene","front"
        @param name
        @return Real x
        @return Real y
        @return Real z
    */
    int getpos(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);             

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (type == Graphica::Camera) {

            if (r) {

                if(name.length() == 0 )
                    name = "Cam1";

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);

                if (cam) {
                    const tintaVector3f &pos = cam->getPosition();
                    PUSH_FLOAT_ARG(L, pos.mx); //	
                    PUSH_FLOAT_ARG(L, pos.my); //	
                    PUSH_FLOAT_ARG(L, pos.mz); //	

                    return 3;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                    
                    TROW_ERR_FUNC(L, msg.str().c_str());                   
                   
                }
            }
        }
        else if ( type == Graphica::Scene ) {           

            if ( r ) {               

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);

                if ( e ) {
                    const tintaVector3f &pos = e->getPosition();

                    PUSH_FLOAT_ARG(L, pos.mx); //	
                    PUSH_FLOAT_ARG(L, pos.my); //	
                    PUSH_FLOAT_ARG(L, pos.mz); //
                    

                    return 3;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }
        }
        else if ( type == Graphica::Back || type == Graphica::Front ) {

           
            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if( type == Graphica::Back )
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer( name );

                if ( e ) {

                    const tintaVector3f &pos = e->getPosition();

                    PUSH_FLOAT_ARG(L, pos.mx); //	
                    PUSH_FLOAT_ARG(L, pos.my); //	
                    PUSH_FLOAT_ARG(L, pos.mz); //

                    return 3;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                    return 0;
                }
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        return 0;
    }


    int getparent(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {

            if (r) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);

                if (e) {
                    Graphica::tintaDrawable *p = e->getParent();
                    if ( p ) {
                        PUSH_STRING_ARG(L, p->getName().c_str());
                        return 1;
                    }
                    return 0;                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }
        }

        if (type == Graphica::Back || type == Graphica::Front) {


            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {

                    Graphica::tintaDrawable *p = e->getParent();
                    if (p) {
                        PUSH_STRING_ARG(L, p->getName().c_str());
                        return 1;
                    }
                    return 0;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        //PUSH_INT_ARG(L, cores); //			
        return 0;
    }


    
    int visible(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {

            if (r) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);

                if (e) {                                 
                   
                    PUSH_BOOL_ARG(L, e->isVisible());                   
                    return 1;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }
        }
        if ( type == Graphica::Back || type == Graphica::Front ) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;


            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {

                    std::cout << e->getName() << " Local: " << e->getLocalOrientation().mx << " " << e->getLocalOrientation().my << " " << e->getLocalOrientation().mz << "\n";
                    std::cout << e->getName() << " World: " << e->getOrientation().mx << " " << e->getOrientation().my << " " << e->getOrientation().mz << "\n";

                    PUSH_BOOL_ARG(L, e->isVisible() );
                    return 1;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        //PUSH_INT_ARG(L, cores); //			
        return 0;
    }


    int setvisible(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        bool val = GET_VAL_BOOL(L, 3);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {

            if (r) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);

                if (e) {
                    e->setVisible(val);
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
                return 0;
            }
        }
        if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;


            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {
                    //std::cout << e->getName() << " Local: " << e->getLocalOrientation().mx << " " << e->getLocalOrientation().my << " " << e->getLocalOrientation().mz << "\n";
                   // std::cout << e->getName() << " World: " << e->getOrientation().mx << " " << e->getOrientation().my << " " << e->getOrientation().mz << "\n";

                    e->setVisible(val);                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        return 0;
    }


    int mousepos(SCRIPT_STATE *L) {

        Graphica::tintaApp* app = Graphica::tintaWinApp::mWinApp;

        if (app) {
            tintaControls* state = app->GetKeyStates();
            CoreAssert(state, "state");

            tintaVector2i coord = state->getMousePos();
                        
            PUSH_INT_ARG(L, coord.mx);
            PUSH_INT_ARG(L, coord.my);
            return 2;
        }
        return 0;
    }

    int setcursor( SCRIPT_STATE *L ) {

        Graphica::tintaApp* app = Graphica::tintaWinApp::mWinApp;

        String path;
        if( IS_VAL_STRING(L, 1) )
            path = GET_VAL_STRING( L, 1 );

        if ( app ) {
            if ( !app->setCursor( path ) ) {
                StringStream msg;
                msg << _M( "Wrong cursor path: " );
                msg << path;

                if ( Tinta::tintaLogger::getPtr() )
                    Tinta::tintaLogger::getPtr()->logMsg( msg.str(), msg_error, false );

                TROW_ERR_FUNC( L, msg.str().c_str() );
            }
        }
        return 0;
    }

   
    int crosspoint( SCRIPT_STATE *L ) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float x = GET_VAL_FLOAT(L, 3);
        float y = GET_VAL_FLOAT(L, 4);
        float z = GET_VAL_FLOAT(L, 5);


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {

            if (r) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);

                if (e) {

                    const Graphica::tintaBoundingVolume *vol = e->getBoundary();
                    if (vol) {

                        PUSH_BOOL_ARG(L, vol->contains({ x,y,z }));
                        return 1;
                    }
                    return 0;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
            }
        }

        if ( type == Graphica::Back || type == Graphica::Front ) {


            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if ( rend ) {

                Graphica::tintaGuiManager * gui = NULL_M;
                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {
                    const Graphica::tintaBoundingVolume *vol = e->getBoundary();
                    if ( vol ) {

                        PUSH_BOOL_ARG( L, vol->contains( {x,y,z} ) );
                        return 1;
                    }
                    return 0;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        return 0;
    }


    int setmaterial(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);
        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        String material = GET_VAL_STRING_EX(L, 3);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if ( type == Graphica::Scene ) {
            if ( r ) {
                
                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if ( e ) {                    

                    e->addMaterial( material );                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());                    
                }
                return 0;
            }
        }

        if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if ( rend ) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {
                    e->addMaterial(material);
                    return 0;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        return 0;

    }    


    int setvariable(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);
        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        String material = GET_VAL_STRING_EX(L, 3);

        String paramName = GET_VAL_STRING_EX(L, 4);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if ( type == Graphica::Scene ) {

            if ( r ) {

                //r->setSceneManager
                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if ( e ) {

                    Graphica::tintaMaterial * m = e->findMaterial(material);
                    if ( m == NULL_M ) {
                        StringStream msg;
                        msg << _M("Wrong material name: ");
                        msg << material;
                        if (Tinta::tintaLogger::getPtr())
                            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                    else {
                        Graphica::tintaMaterialParser::MaterialDataType varType = m->getVarType(paramName);
                       
                        if ( varType == Graphica::tintaMaterialParser::matSize ) {
                            StringStream msg;
                            msg << _M("Wrong variable or data name: ");
                            msg << paramName;
                            if (Tinta::tintaLogger::getPtr())
                                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                            TROW_ERR_FUNC(L, msg.str().c_str());
                        }
                        else {
                            fillMaterial(L, 5, e, m, paramName, varType);
                        }
                    }

                    return 0;                        
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                    return 0;
                }
            }
        }

        if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {
                    Graphica::tintaMaterial *m = e->findMaterial(material);
                    if (m == NULL_M) {
                        StringStream msg;
                        msg << _M("Wrong material name: ");
                        msg << material;
                        if (Tinta::tintaLogger::getPtr())
                            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                    else {
                        Graphica::tintaMaterialParser::MaterialDataType varType = m->getVarType(paramName);

                        if (varType == Graphica::tintaMaterialParser::matSize) {
                            StringStream msg;
                            msg << _M("Wrong variable or data name: ");
                            msg << paramName;
                            if (Tinta::tintaLogger::getPtr())
                                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                            TROW_ERR_FUNC(L, msg.str().c_str());
                        }
                        else {
                            fillMaterial(L, 5, e, m, paramName, varType);
                        }
                    }
                    return 0;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                    return 0;
                }
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        //PUSH_INT_ARG(L, cores); //			
        return 0;

    }

    
    int getorient(SCRIPT_STATE *L) {
        
        String typeStr = GET_VAL_STRING_EX(L, 1);
        Graphica::SceneType type = getType(typeStr);
        
        String name = GET_VAL_STRING_EX(L, 2);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (type == Graphica::Camera) {

            if (r) {
                //r->setSceneManager

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if ( cam ) {

                    const tintaQuatern &q = cam->getOrientation();

                    PUSH_FLOAT_ARG(L, q.mx); //	
                    PUSH_FLOAT_ARG(L, q.my); //	
                    PUSH_FLOAT_ARG(L, q.mz); //	
                    PUSH_FLOAT_ARG(L, q.mw); //	

                    return 4;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                    return 0;
                }
            }
        }
        else if ( type == Graphica::Scene ) {
            
            if (r ) {
                //r->setSceneManager

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if ( e ) {

                    const tintaQuatern &q = e->getOrientation();

                    PUSH_FLOAT_ARG(L, q.mx); //	
                    PUSH_FLOAT_ARG(L, q.my); //	
                    PUSH_FLOAT_ARG(L, q.mz); //	
                    PUSH_FLOAT_ARG(L, q.mw); //	

                    return 4;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }
        }
        else if ( type == Graphica::Back || type == Graphica::Front ) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {

                    const tintaQuatern &q = e->getOrientation();

                    PUSH_FLOAT_ARG(L, q.mx); //	
                    PUSH_FLOAT_ARG(L, q.my); //	
                    PUSH_FLOAT_ARG(L, q.mz); //	
                    PUSH_FLOAT_ARG(L, q.mw); //	

                    return 4;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                    return 0;
                }
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        return 0;
    }

    int setorient( SCRIPT_STATE *L ) {


        String typeStr = GET_VAL_STRING_EX(L, 1);
        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float x = GET_VAL_FLOAT(L, 3);
        float y = GET_VAL_FLOAT(L, 4);
        float z = GET_VAL_FLOAT(L, 5);
        float w = GET_VAL_FLOAT(L, 6);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Camera) {
            if (r) {

                name = "Cam1";

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam) {
                    cam->updateOrientation(tintaQuatern(w, x, y, z));
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }
            return 0;
        }
        else if ( type == Graphica::Scene ) {
            
            if ( r ) {


                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e) {
                    e->updateOrientation(tintaQuatern(w, x, y, z));                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }
            return 0;
        }

        if ( type == Graphica::Back || type == Graphica::Front ) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {
                    e->updateOrientation( tintaQuatern(w, x, y, z), false, true );                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
            }
            return 0;
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        return 0;
    }


    int addtimer(SCRIPT_STATE *L) {
               
        m_uint32 period = GET_VAL_UINT(L, 1);
        String execute;

        if( IS_VAL_STRING(L,2) )
            execute = GET_VAL_STRING(L, 2);

        if (period == 0) {

            StringStream msg;
            msg << _M("Wrong timer period: ");
            msg << period;
            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

            TROW_ERR_FUNC(L, msg.str().c_str());
            return 0;
        }      

        Graphica::tintaApp* api = Graphica::tintaWinApp::mWinApp;

        if ( api ) {

            m_uint32 id = api->addOnTimerBuff( period, execute );
            PUSH_UINT_ARG(L, id);

            return 1;
        }
        return 0;
    }

    int deltimer(SCRIPT_STATE *L) {

        m_uint32 id = GET_VAL_UINT(L, 1);

        //tintaApp *app = M_NEW DemoLuaApp();
        Graphica::tintaApp* api = Graphica::tintaWinApp::mWinApp;

        if ( api ) {

            if ( !api->delTimer(id)) {
                StringStream msg;
                msg << _M("Wrong timer id: ");
                msg << id;
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }

        return 0;
    }

    int settimer(SCRIPT_STATE *L) {

        m_uint32 id = GET_VAL_UINT(L, 1);
        String data;
        if( IS_VAL_STRING(L, 2) )
            data  = GET_VAL_STRING(L, 2);

        m_uint32 period = 0; 
        if( IS_VAL_INTEGER(L, 3) )
            period = GET_VAL_UINT(L, 3);
        

        //tintaApp *app = M_NEW DemoLuaApp();
        Graphica::tintaApp* api = Graphica::tintaWinApp::mWinApp;

        if (api) {

            if (data.length() > 0) {
                if (!api->setTimer(id, data)) {
                    StringStream msg;
                    msg << _M("Wrong timer id: ");
                    msg << id;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }
            if (period > 0) {
                if ( !api->setTimer(id, period) ) {
                    StringStream msg;
                    msg << _M("Wrong timer id: ");
                    msg << id;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }
        
            
            
        }

        return 0;
    }

    int exists(SCRIPT_STATE *L) {

        String typeName = GET_VAL_STRING(L, 1);

        Graphica::SceneType type = getType( typeName );

        String name = GET_VAL_STRING(L, 2);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {

            if (r) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e) {
                    PUSH_BOOL_ARG(L, true);
                }
                else {
                    PUSH_BOOL_ARG(L, false);
                }

                return 1;
            }            
        }

        if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {

                    PUSH_BOOL_ARG(L, true);
                }
                else {
                    PUSH_BOOL_ARG(L, false);
                }

                return 1;
            }            
        }
        return 0;
    }



    int setkey( SCRIPT_STATE *L ) {

        tintaControls::EnInnerKeys key = (tintaControls::EnInnerKeys)GET_VAL_UINT(L, 1);

        String execute = GET_VAL_STRING(L, 2);

        bool pushDown = true; 
        if( IS_VAL_BOOL(L, 3) )
            pushDown = GET_VAL_BOOL(L, 3);
              
        Graphica::tintaApp* api = Graphica::tintaWinApp::mWinApp;

        if ( api ) {
            tintaControls *keys = api->GetKeyStates();            
            keys->bindCommand(key, execute, pushDown ? tintaControls::downKey : tintaControls::upKey);       
        }

        return 0;
    }

    int keypressed(SCRIPT_STATE *L) {

        tintaControls::EnInnerKeys key = (tintaControls::EnInnerKeys)GET_VAL_UINT(L, 1);        

        Graphica::tintaApp* api = Graphica::tintaWinApp::mWinApp;

        if ( api ) {            

            tintaControls *keys = api->GetKeyStates();

            PUSH_BOOL_ARG(L, keys->isKeyPressed(key) ); //	

            return 1;
        }

        return 0;        
    }
    
    int deltimers(SCRIPT_STATE *L) {

        //tintaApp *app = M_NEW DemoLuaApp();

        Graphica::tintaApp* api = Graphica::tintaWinApp::mWinApp;
        if ( api ) {
            api->delTimers();
        }

        return 0;
    }


    /*
    @name d3.camlookat
    @description Rotates camera in the entity direction
    @param name camera name by default "Cam1"
    @param name entity name    
    */
    int camlookat(SCRIPT_STATE *L) {

        String name(_M("Cam1"));

        //if (IS_VAL_STRING(L, 1))
        //    name = GET_VAL_STRING_EX(L, 1);

        String entityName = GET_VAL_STRING_EX(L, 2);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (r && r->getSceneManager()) {
            //r->setSceneManager
            Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);

            Graphica::tintaDrawable *ent = r->getSceneManager()->getEntity(entityName);

            if (cam && ent )
                cam->setDirection(ent->getPosition());
            else {
                StringStream msg;
                msg << _M("Wrong camera or entity name: ");
                msg << name;
                msg << _M(" ");
                msg << entityName;
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }
        //PUSH_INT_ARG(L, cores); //			
        return 0;
    }

       


    /*
        @name d3.roll
        @description Rolls entity
        @param name 
        @param Real angle in degree
    */
    int roll(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float angle = GET_VAL_FLOAT(L, 3);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Camera) {

            if (r) {

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam)
                    cam->rotate(tintaVector3f::zAxis, TintaMath::toRad(angle),true, true);
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
            }
            return 0;
        }
        else if (type == Graphica::Scene) {
            
            if ( r ) {
                
                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e)
                    e->rotate(tintaVector3f::zAxis, TintaMath::toRad(angle), true, true);
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
            }
            return 0;
        }
        else if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if ( type == Graphica::Back )
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {
                        e->rotate(tintaVector3f::zAxis, TintaMath::toRad(angle), true, true);
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                
            }
            return 0;
        }
        
        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        //PUSH_INT_ARG(L, cores); //			
        return 0;
    }

	
    /*
        @name d3.yaw
        @description Yaws entity
        @param name
        @param Real angle in degree
    */
    int yaw(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float angle = GET_VAL_FLOAT(L, 3);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Camera) {

            if (r) {
                //r->setSceneManager
                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam)
                    cam->rotate(tintaVector3f::yAxis, TintaMath::toRad(angle), true, true);
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
            }
            return 0;
        }
        else if ( type == Graphica::Scene ) {
        
            if ( r ) {
                //r->setSceneManager
                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e)
                    e->rotate(tintaVector3f::yAxis, TintaMath::toRad(angle), true, true);
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
            }
            return 0;
        }
        else if ( type == Graphica::Back || type == Graphica::Front ) {               

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {
                    e->rotate(tintaVector3f::yAxis, TintaMath::toRad(angle), true, true);
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);

        return 0;
    }

    /*
        @name d3.pitch
        @description Pitches entity
        @param name
        @param Real angle in degree
    */
    int pitch(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float angle = GET_VAL_FLOAT(L, 3);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Camera) {
            if (r) {

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam)
                    cam->rotate(tintaVector3f::xAxis, TintaMath::toRad(angle), true, true);
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
            }
            return 0;
        }
        else if (type == Graphica::Scene) {
            if (r) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e)
                    e->rotate(tintaVector3f::xAxis, TintaMath::toRad(angle), true, true);
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
            }
            return 0;
        }
        else if ( type == Graphica::Back || type == Graphica::Front ) {
            
            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if ( rend ) {
                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {
                    e->rotate(tintaVector3f::xAxis, TintaMath::toRad(angle), true, true);
                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }
            return 0;
        }
        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }



    int winsize(SCRIPT_STATE *L) {
        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

        if ( rend ) {

            tintaVector2i size = rend->getWindowSize();

            PUSH_INT_ARG(L, size.mx); //	
            PUSH_INT_ARG(L, size.my); //	
            return 2;
        }
        return 0;       
    }

    /*
        @name d3.rotate
        @description Rotates entity
        @param name
        @param Real x in degree
        @param Real y in degree
        @param Real z in degree
        @param Real speed  degree/msec or nil
        @param String handler on finish or nil e.g. "remove(\"bullet\")"
    */
    int rotate(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float angleX = GET_VAL_FLOAT(L, 3);
        float angleY = GET_VAL_FLOAT(L, 4);
        float angleZ = GET_VAL_FLOAT(L, 5);

        float speed = 0.f;
        if ( IS_VAL_REAL(L, 6) )
            speed = GET_VAL_FLOAT(L, 6);

        StringBasic handler;
        if (IS_VAL_STRING(L, 7))
            handler = GET_VAL_STRING(L, 7);

        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (type == Graphica::Camera) {
            if (r) {

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam) {
                    if (TintaMath::isZero(speed))
                        cam->rotate(TintaMath::toRad(angleX), TintaMath::toRad(angleY), TintaMath::toRad(angleZ), true);
                    else {
                        cam->addAction(M_NEW Graphica::tintaCameraRotateAction(cam, TintaMath::toRad(angleX), TintaMath::toRad(angleY),
                            TintaMath::toRad(angleZ), speed, handler));
                    }
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
                return 0;
            }
        }
        else if ( type == Graphica::Scene )  {
            if ( r ) {
                
                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if ( e ) {
                    if (TintaMath::isZero(speed))
                        e->rotate(TintaMath::toRad(angleX), TintaMath::toRad(angleY), TintaMath::toRad(angleZ), true);
                    else {
                        e->addAction(M_NEW Graphica::tintaEntityRotateAction(e, TintaMath::toRad(angleX), TintaMath::toRad(angleY),
                            TintaMath::toRad(angleZ), speed, handler));
                    }
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }
        else if ( type == Graphica::Back || type == Graphica::Front ) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if ( rend ) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");
                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {
                    if (TintaMath::isZero(speed))
                        e->rotate(TintaMath::toRad(angleX), TintaMath::toRad(angleY), TintaMath::toRad(angleZ), true, true );
                    else {
                        e->addAction(M_NEW Graphica::tintaEntityRotateAction(e, TintaMath::toRad(angleX), TintaMath::toRad(angleY),
                            TintaMath::toRad(angleZ), speed, handler));
                    }                    
                } 
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }

    /*
    @name d3.rotatecycle
    @description Starts to rotate entity
    @param name
    @param Real x in degree/msec
    @param Real y in degree/msec
    @param Real z in degree/msec     
    */
    int rotatecycle(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float angleX = GET_VAL_FLOAT(L, 3);
        float angleY = GET_VAL_FLOAT(L, 4);
        float angleZ = GET_VAL_FLOAT(L, 5);              

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

       
        if (type == Graphica::Scene) {
           
            if ( r ) {
                //r->setSceneManager
                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e)
                    e->addAction(M_NEW Graphica::tintaEntityRotateInfiniteAction(e, TintaMath::toRad(angleX), TintaMath::toRad(angleY),
                        TintaMath::toRad(angleZ)));

                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }
        }
        else if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);
                if ( e ) {
                    e->addAction(M_NEW Graphica::tintaEntityRotateInfiniteAction(e, TintaMath::toRad(angleX), TintaMath::toRad(angleY),
                        TintaMath::toRad(angleZ)));
                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }


    
    /*
        @name d3.setpos
        @description Moves entity to the position
        @param name
        @param Real x
        @param Real y
        @param Real z
        @param Real speed msec or nil
        @param String handler on finish or nil e.g. "remove(\"bullet\")"
    */
    int moveto(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float x = GET_VAL_FLOAT(L, 3);
        float y = GET_VAL_FLOAT(L, 4);
        float z = GET_VAL_FLOAT(L, 5);

        float speed = 0.f;
        if ( IS_VAL_REAL(L, 6) )
            speed = GET_VAL_FLOAT(L, 6);

        StringBasic handler;
        if ( IS_VAL_STRING(L, 7) )
            handler = GET_VAL_STRING(L, 7);
        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();



        if (type == Graphica::Camera) {

            if (r) {

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam) {

                    if (TintaMath::isZero(speed)) {
                        cam->moveOn(tintaVector3f(x, y, z) - cam->getLocalPosition(), true, true);
                    }
                    else {
                        cam->addAction(M_NEW Graphica::tintaCameraMoveAction(cam,
                            cam->getPosition(), tintaVector3f(x, y, z),
                            speed, handler));
                    }
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
                return 0;
            }
        }
        else if (type == Graphica::Scene) {

            if ( r ) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);


                if (e) {

                    if (TintaMath::isZero(speed)) {
                        e->moveOn( tintaVector3f(x, y, z) - e->getLocalPosition(), true, true );
                    }
                    else {
                        e->addAction(M_NEW Graphica::tintaEntityMoveAction(e,
                            e->getPosition(), tintaVector3f(x, y, z),
                            speed, handler));
                    }
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }
        else if (type == Graphica::Back || type == Graphica::Front) {            

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {

                    if (TintaMath::isZero(speed)) {
                        e->moveTo( tintaVector3f(x, y, z), true, true );
                    }
                    else {
                        e->addAction(M_NEW Graphica::tintaEntityMoveAction(e,
                            e->getLocalPosition(), tintaVector3f(x, y, z),
                            speed, handler));
                    }                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }


    /*
    @name d3.getdestpos
    @description returns XYZ values of point from entity on distance
    @param Entity name
    @param Real angle offset from forward x
    @param Real angle offset from forward y
    @param Real angle offset from forward z
    @param Real Distance    
    @return Real x
    @return Real y
    @return Real z
    */
    int getdestpos(SCRIPT_STATE *L) {
         

        if ( GET_QUANTITY(L) != 5 ) {
            StringStream msg;
            msg << _M("Wrong arguments quantity!");
            if(Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error); 

            return 0;
        }
        
        String name = GET_VAL_STRING_EX(L, 1);

        /*float xAngle = GET_VAL_FLOAT(L, 2);
        float yAngle = GET_VAL_FLOAT(L, 3);
        float zAngle = GET_VAL_FLOAT(L, 4);*/

        float dist = 0.f;       

        dist = GET_VAL_FLOAT(L, 5);       

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if ( r && r->getSceneManager() ) {
            //r->setSceneManager
            Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
            if (e)
                if ( !TintaMath::isZero( dist ) ) {

                    tintaVector3f pos = e->getOrientation() * tintaVector3f(dist, 0.f, 0.f);
                              
                    pos += e->getPosition();

                    PUSH_FLOAT_ARG(L, pos.mx); //	
                    PUSH_FLOAT_ARG(L, pos.my); //	
                    PUSH_FLOAT_ARG(L, pos.mz); //	

                    return 3;
                }
            else {
                StringStream msg;
                msg << _M("Wrong entity name: ");
                msg << name;
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }
        return 0;

    }


    int moveorient( SCRIPT_STATE *L ) {

        
        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float x = GET_VAL_FLOAT(L, 3);
        float y = GET_VAL_FLOAT(L, 4);
        float z = GET_VAL_FLOAT(L, 5);       

        float speed = 0.f;
        if (IS_VAL_REAL(L, 6))
            speed = GET_VAL_FLOAT(L, 6);


        StringBasic handler;
        if ( IS_VAL_STRING(L, 7) )
            handler = GET_VAL_STRING( L, 7 );

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        

        if (type == Graphica::Camera) {
            if ( r ) {

                name = "Cam1";

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam ) {
                    if (TintaMath::isZero(speed)) {
                        cam->moveOnOrient(tintaVector3f(x, y, z));
                    }
                    else {
                        cam->addAction(M_NEW Graphica::tintaCameraMoveAction(cam,
                            cam->getPosition(), cam->getPosition() + cam->getOrientation() * tintaVector3f(x, y, z),
                            speed, handler));
                    }

                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }               
            }
            return 0;
        }
        else if ( type == Graphica::Scene ) {

            if ( r ) {               

                //r->setSceneManager
                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);

                if (e->getParent()) {
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(_M("!!!! moveOnOrient for child objects is prohibited in this version"), msg_info);
                    return 0;
                }


                if ( e && !e->getParent() )
                    if (TintaMath::isZero(speed)) {
                        e->moveOnOrient( tintaVector3f(x, y, z) , true, true);
                    }
                    else {
                        e->addAction(M_NEW Graphica::tintaEntityMoveAction(e,
                            e->getPosition(), e->getPosition() + e->getOrientation() * tintaVector3f(x, y, z),
                            speed, handler));
                    }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name or entity has the parent: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg( msg.str(), msg_error, false );

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }
        else if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ?  r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {

                    if (e ->getParent()) {
                        if (Tinta::tintaLogger::getPtr())
                            Tinta::tintaLogger::getPtr()->logMsg(_M("!!!! moveOnOrient for child objects is prohibited in this version"), msg_info);
                        return 0;
                    }


                    // -y to supress error  move in GUI Manager ent->pitch(K_PI);// TintaMath::toRad(180.f));
                    if (TintaMath::isZero(speed)  ) {
                        e->moveOnOrient( tintaVector3f(x, y, z), true, true);
                    }
                    else {                       
                        if ( !e->getParent() ) {
                            e->addAction(M_NEW Graphica::tintaEntityMoveAction(e,
                                e->getLocalPosition(), e->getLocalPosition() + e->getOrientation() * tintaVector3f(x, y, z),
                                speed, handler));
                        }
                        else {
                            e->addAction(M_NEW Graphica::tintaEntityMoveAction(e,
                                e->getLocalPosition(), e->getLocalPosition() + e->getParent()->getOrientation() * e->getOrientation() * tintaVector3f(x, y, z),
                                speed, handler));
                        }
                       
                    }                    
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                   
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }


    int moveon(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float x = GET_VAL_FLOAT(L, 3);
        float y = GET_VAL_FLOAT(L, 4);
        float z = GET_VAL_FLOAT(L, 5);

        float speed = 0.f;
        if (IS_VAL_REAL(L, 6))
            speed = GET_VAL_FLOAT(L, 6);


        StringBasic handler;
        if (IS_VAL_STRING(L, 7))
            handler = GET_VAL_STRING(L, 7);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (type == Graphica::Camera) {
            if (r) {

                name = "Cam1";

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam)
                    if (TintaMath::isZero(speed)) {
                        cam->moveOn(tintaVector3f(x, y, z), true, true);
                    }
                    else {
                        cam->addAction(M_NEW Graphica::tintaCameraMoveAction(cam,
                            cam->getPosition(), cam->getPosition() + tintaVector3f(x, y, z),
                            speed, handler));
                    }
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
            }
            return 0;
        }
        else if (type == Graphica::Scene) {
            if (r) {
                //r->setSceneManager
                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e )
                    if (TintaMath::isZero(speed)) {
                        e->moveOn(tintaVector3f(x, y, z), true, true);
                    }
                    else {
                        e->addAction(M_NEW Graphica::tintaEntityMoveAction(e,
                            e->getPosition(), e->getPosition() + tintaVector3f(x, y, z),
                            speed, handler));
                    }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
                return 0;
            }
        }

        if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {

                    if (TintaMath::isZero(speed)) {
                        e->moveOn(tintaVector3f(x, y, z), true, true);
                    }
                    else {                     
                        e->addAction(M_NEW Graphica::tintaEntityMoveAction(e,
                            e->getLocalPosition(), e->getLocalPosition() + tintaVector3f(x, y, z),
                            speed, handler));
                    }
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        return 0;
    }

    int mulscale(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float x = GET_VAL_FLOAT(L, 3);
        float y = GET_VAL_FLOAT(L, 4);
        float z = GET_VAL_FLOAT(L, 5);

    
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        
       if (type == Graphica::Scene) {
            if ( r ) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e) {
                    e->mulScale({ x,y,z }, true);
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }

        if (type == Graphica::Back || type == Graphica::Front) {


            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if ( rend ) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( e ) {
                    e->mulScale({ x,y,z }, true );
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }

    int scale(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        float x = GET_VAL_FLOAT(L, 3);
        float y = GET_VAL_FLOAT(L, 4);
        float z = GET_VAL_FLOAT(L, 5);


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {
            if (r) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e) {
                    e->setScale({ x,y,z }, true);
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
                return 0;
            }
        }

        if (type == Graphica::Back || type == Graphica::Front) {


            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {
                    e->setScale({ x,y,z }, true);
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());
        return 0;
    }

    int getscale(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {
            if (r) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e) {
                    //e->setScale({ x,y,z }, true);
                    tintaVector3f scale = e->getScale();

                    PUSH_FLOAT_ARG(L, scale.mx);
                    PUSH_FLOAT_ARG(L, scale.my);
                    PUSH_FLOAT_ARG(L, scale.mz);

                    return 3;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
                return 0;
            }
        }

        if (type == Graphica::Back || type == Graphica::Front) {


            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {
                    tintaVector3f scale = e->getScale();

                    PUSH_FLOAT_ARG(L, scale.mx);
                    PUSH_FLOAT_ARG(L, scale.my);                    

                    return 2;
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }




    /*
        @name d3.status
        @description Prints scene status        
    */
    int status(SCRIPT_STATE *L){


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( r && r->getSceneManager() ){
            //Tinta::tintaLogger::getPtr()->logMsg(_M("Entities: \n"), msg_info);
            StringStream msg;
            msg << _M("\nEntities:\n");
            const Graphica::tintaDrawable *s = r->getSceneManager()->getEntityFirst();
            while (s){                
                msg << _M("name: ");
                msg << s->getName();
                msg << _M(" position: ");
                msg << s->getPosition().mx;
                msg << _M(" ")<< s->getPosition().my;
                msg << _M(" ")<< s->getPosition().mz;
                msg << _M("\n");
                s = r->getSceneManager()->getEntityNext();
            }            
            msg << _M("Cameras:");
            const Graphica::tintaCamera *c = r->getSceneManager()->getCameraFirst();
            while (s){

                
                //msg << c->mName;
                msg << _M(" position: ");
                msg << c->getPosition().mx;
                msg << _M(" ") << c->getPosition().my;
                msg << _M(" ") << c->getPosition().mz;
                msg << _M("\n");             

                c = r->getSceneManager()->getCameraNext();
            }
            if( Tinta::tintaLogger::getPtr() )
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_info);

            TROW_ERR_FUNC(L, msg.str().c_str());
            
        }

        return 0;
    }

    /*
        @name d3.genname
        @param String prefix or nil
        @description Generates uniqal name
    */
    int genname(SCRIPT_STATE *L){

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( r ){
            NameGenerator * gen = r->getNamegen();

            StringBasic prefix;
            if ( IS_VAL_STRING(L, 1) )
                prefix = GET_VAL_STRING(L, 1);

            PUSH_STRING_ARG( L, gen->generateBasic(prefix).c_str() );
            return 1;
        }
        return 0;
    }


    int deltexture(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        Graphica::tintaGLRenderer *rend = r ? r->getRenderer() : NULL_M;

        bool rez = false;
        if ( rend ) {                       
            rez = rend->releaseTexture(name, true);
        }

        PUSH_BOOL_ARG(L, rez);

        return 1;
    }


    int loadtexture(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        Graphica::tintaGLRenderer *rend = r ? r->getRenderer() : NULL_M;

        if ( rend ) {
            if ( !rend->addTexture( name ) ) {

                StringStream msg;
                msg << _M("Wrong texture name: ") << name;

                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_info);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }
        return 0;
    }

    int addimage(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        
        if (r) {

            r->delImage(name);

            if ( !r->getImage( name ) ) {

                StringStream msg;
                msg << _M("Wrong image name: ") << name;

                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_info);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }            
        }
        return 0;
    }


    int addmaterial(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        String path = name;

        if( IS_VAL_STRING(L, 2))
            path = GET_VAL_STRING(L, 2);

            

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r) {
            if ( r->addMaterial( path, name ) == NULL_M ) {

                StringStream msg;
                msg << _M("Material ") << name << _M(" was not found");

                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_info);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }
        return 0;
    }

    int delimage(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        Graphica::tintaGLRenderer *rend = r ? r->getRenderer() : NULL_M;

        bool rez = false;

        if ( rend ) {
            if( rend->releaseTexture(name, false) )
                rez = r->delImage( name );            
        }

        PUSH_BOOL_ARG(L, rez);

        return 1;
    }

    int setchannel(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        Graphica::tintaGLRenderer *rend = r ? r->getRenderer() : NULL_M;

        if (rend) {

            m_uint32 x = GET_VAL_UINT(L, 2);
            m_uint32 y = GET_VAL_UINT(L, 3);

            m_uint32 channel = GET_VAL_UINT(L, 4);
            float value = GET_VAL_FLOAT(L, 5);

            rend->releaseTexture(name, true);

            Graphica::tintaImage *img = r->getImage(name);
            if ( !img ) {

                StringStream msg;
                msg << _M("Wrong image name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            Graphica::imageType type = img->getType();
            Graphica::imageFormat format = img->getFormat();            
            m_uint32 channelOffset = img->getChannelSize();

            m_uint32 w = img->width();
            m_uint32 h = img->height();

            if ( x >= w || y >= h ) {
                StringStream msg;
                msg << _M("Wrong color value coordinate");
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }


            if ( channel >= img->channels() ) {
                StringStream msg;
                msg << _M("Wrong channel value for this image format");
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            m_uint32 index = y * w + x;
            m_uint8* pos = img->data( index );        

            switch ( type ) {

            case  Graphica::IMG_UBYTE:
            case  Graphica::IMG_UINT:
                if ( format == Graphica::IMG_LUMINANCE ) {
                    break;                    
                }
                else {
                    m_uint8 val = (m_uint32)floatToByte(value);
                    memcpy(pos + (channel * channelOffset), &val, channelOffset);
                }

                break;
            case  Graphica::IMG_FLOAT:
                //m_uint8 val = (m_uint32)floatToByte(value);
                memcpy(pos + (channel * channelOffset), &value, channelOffset);

                break;
            case  Graphica::IMG_UNSIGNED_INT_8_8_8_8:                
                break;
            default:
                break;
            };
        }


        return 0;
    }

    int getchannel(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        Graphica::tintaGLRenderer *rend = r ? r->getRenderer() : NULL_M;

        if ( rend ) {

            m_uint32 x = GET_VAL_UINT(L, 2);
            m_uint32 y = GET_VAL_UINT(L, 3);

            m_uint32 channel = GET_VAL_UINT(L, 4);            

            rend->releaseTexture(name, true);

            Graphica::tintaImage *img = r->getImage(name);
            if (!img) {

                StringStream msg;
                msg << _M("Wrong image name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            Graphica::imageType type = img->getType();
            Graphica::imageFormat format = img->getFormat();
            m_uint32 channelOffset = img->getChannelSize();

            m_uint32 w = img->width();
            m_uint32 h = img->height();

            if (x >= w || y >= h) {
                StringStream msg;
                msg << _M("Wrong color value coordinate");
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }


            if (channel >= img->channels()) {
                StringStream msg;
                msg << _M("Wrong channel value for this image format");
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            m_uint32 index = y * w + x;
            m_uint8* pos = img->data(index);
            float value = 0.f;
            switch (type) {

            case  Graphica::IMG_UBYTE:
            case  Graphica::IMG_UINT:
                if (format == Graphica::IMG_LUMINANCE) {
                    break;
                }
                else {
                    m_uint8 val = 0; // (m_uint32)floatToByte(value);
                    memcpy(&val, pos + (channel * channelOffset), channelOffset);
                    value = byteToFloat(val);
                }

                break;
            case  Graphica::IMG_FLOAT:
                //m_uint8 val = (m_uint32)floatToByte(value);
                
                memcpy( &value, pos + (channel * channelOffset), channelOffset);

                break;
            case  Graphica::IMG_UNSIGNED_INT_8_8_8_8:
                break;
            default:
                break;
            };

            PUSH_FLOAT_ARG(L, value);
            return 1;
        }


        return 0;
    }

    int imagesize(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if ( r ) {
            const Graphica::tintaImage* img = r->getImage(name);

            if ( !img ) {
                StringStream msg;
                msg << _M("Wrong image name: ");
                msg << name;
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }
            
            PUSH_UINT_ARG(L, img->width());
            PUSH_UINT_ARG(L, img->height());

            return 2;
        }




        return 0;
    }
    
   
    /*
    @name d3.wareframemode
    @description Sets wareframemode
    @param true - to switch on the wareframe mode
    */
    int wareframemode(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (r) {
            Graphica::tintaGLRenderer *rend = r->getRenderer();
            rend->setWareFrame(GET_VAL_BOOL(L, 1));
        }

        return 0;
    }

    /*
        @name d3.addplane
        @description Adds plane entity
        @param name
        @param Real w
        @param Real h
        @param Int w tesselation size
        @param Int h tesselation size
        @param material name
    */
    int addplane(SCRIPT_STATE *L){


        String name = GET_VAL_STRING_EX(L, 1);
        float w = GET_VAL_FLOAT(L, 2);
        float h = GET_VAL_FLOAT(L, 3);  
        int xTessel = GET_VAL_INT(L, 4);
        int yTessel = GET_VAL_INT(L, 5);
        
        String material = GET_VAL_STRING_EX(L, 6);   

        String nameParent = GET_VAL_STRING_EX(L, 7);

        if (xTessel == 0 || yTessel == 0) {
            StringStream msg;
            msg << _M("Wrong tesselation values for entity : ");
            msg << name;
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
            TROW_ERR_FUNC(L, msg.str().c_str());
            
        }

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( r ) {
            //r->setSceneManager
            //ent  = r->getSceneManager()->createPlane(w, h, xTessel, yTessel);

            UNIQPTRVIRT(Graphica::tintaDrawable, ent, Graphica::tintaSceneManager::createPlane(w, h, xTessel, yTessel));

            if (ent) {
                if(material.size() > 0 )
                    ent->addMaterial(material);
                ent->setName(name);
              
                auto newEnt = r->getSceneManager()->addEntity(std::move(ent));

                if ( !newEnt ) {

                    StringStream msg;
                    msg << _M("Entity with name: ");
                    msg << name;
                    msg << _M(" is already exists");
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                if (nameParent.length() > 0 && newEnt) {
                    if (!r->getSceneManager()->setParent(newEnt, nameParent)) {
                        StringStream msg;
                        msg << _M("Parent: ");
                        msg << nameParent;
                        msg << _M(" was not added ");
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                    }
                }
            }
            //e->mTraformWorld.SetTranslate(e->mTraformWorld.mPosition + tintaVector3f(x, y, z));
        }
        
        return 0;
    }


    /*
    @name d3.addbillboard
    @description Adds billboard entity. Billboard always faces camera
    @param name
    @param Real w
    @param Real h
    @param Int w tesselation size
    @param Int h tesselation size
    @param material name
    @param Boolean optionally if true fully faces to camera if false or nil faces along the z axis
    */
    int addbillboard(SCRIPT_STATE *L) {


        String name = GET_VAL_STRING_EX(L, 1);
        float w = GET_VAL_FLOAT(L, 2);
        float h = GET_VAL_FLOAT(L, 3);
        int xTessel = GET_VAL_INT(L, 4);
        int yTessel = GET_VAL_INT(L, 5);


        if (xTessel == 0 || yTessel == 0) {
            StringStream msg;
            msg << _M("Wrong tesselation values for entity : ");
            msg << name;
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
            TROW_ERR_FUNC(L, msg.str().c_str());
            return 0;
        }

        String material = GET_VAL_STRING_EX(L, 6);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (r) {
            //r->setSceneManager
            //ent  = r->getSceneManager()->createPlane(w, h, xTessel, yTessel);

            // full face
            if ( IS_VAL_BOOL(L, 7) && GET_VAL_BOOL(L, 7) == true ) {
                UNIQPTRVIRT(Graphica::tintaDrawable, ent, Graphica::tintaSceneManager::createPlane(w, h, xTessel,
                    yTessel, Graphica::tintaSceneManager::Sprite));

                if (ent) {
                    if (material.size() > 0)
                        ent->addMaterial(material);
                    ent->setName(name);
                    if ( !r->getSceneManager()->addEntity( std::move(ent) ) ) {
                        StringStream msg;
                        msg << _M("Entity with name: ");
                        msg << name;
                        msg << _M(" is already exists");
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
            }
            else {
                UNIQPTRVIRT(Graphica::tintaDrawable, ent, r->getSceneManager()->createPlane(w, h, xTessel,
                    yTessel, Graphica::tintaSceneManager::Billboard));

                if (ent) {
                    if (material.size() > 0)
                        ent->addMaterial(material);
                    ent->setName(name);
                    if ( !r->getSceneManager()->addEntity(std::move(ent)) ) {

                        StringStream msg;
                        msg << _M("Entity with name: ");
                        msg << name;
                        msg << _M(" is already exists");
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
            }
            //e->mTraformWorld.SetTranslate(e->mTraformWorld.mPosition + tintaVector3f(x, y, z));
        }

        return 0;
    }


    /*
        @name d3.addbox
        @description Adds box entity
        @param name
        @param Real w
        @param Real h
        @param Real depth
        @param Boolean inside flag, if true - inside box
        @param String optionally parent or nil
        @param material name
    */
    int addbox(SCRIPT_STATE *L){


        String name = GET_VAL_STRING_EX(L, 1);
        float xSize = GET_VAL_FLOAT(L, 2);
        float ySize = GET_VAL_FLOAT(L, 3);
        float zSize = GET_VAL_FLOAT(L, 4);        
        bool inside = GET_VAL_BOOL(L, 5);

        String material = GET_VAL_STRING_EX(L, 6);


        String nameParent = GET_VAL_STRING_EX(L, 7);
       
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( r ) {
            //r->setSceneManager
            
            UNIQPTRVIRT( Graphica::tintaDrawable, ent, Graphica::tintaSceneManager::createBox(xSize, ySize, zSize, inside) );
            if (ent) {

                if (material.size() > 0)
                    ent->addMaterial(material);
                ent->setName(name);

                auto newEnt = r->getSceneManager()->addEntity( std::move( ent ) );
                if (!newEnt) {
                    StringStream msg;
                    msg << _M("Entity with name: ");
                    msg << name;
                    msg << _M(" is already exists");
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                if ( nameParent.length() > 0 && newEnt ) {
                    if ( !r->getSceneManager()->setParent(newEnt, nameParent) ) {
                        StringStream msg;
                        msg << _M("Parent: ");
                        msg << nameParent;
                        msg << _M(" was not added ");
                        if (Tinta::tintaLogger::getPtr())
                            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
            }
            //e->mTraformWorld.SetTranslate(e->mTraformWorld.mPosition + tintaVector3f(x, y, z));
        }
            

        return 0;
    }

    int setparent(SCRIPT_STATE *L) {

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String child = GET_VAL_STRING_EX(L, 2);
        String parent = GET_VAL_STRING_EX(L, 3);        
             
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {
            
            if ( !r || !r->getSceneManager()->setParent( child, parent ) ) {               
                StringStream msg;
                msg << _M("Parent: ");
                msg << parent;
                msg << _M(" was not added to the child: ");
                msg << child;
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }
            return 0;
        }
        else if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if ( rend ) {
                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                if ( !gui->setParent(child, parent ) ) {

                    StringStream msg;
                    msg << _M("Parent: ");
                    msg << parent;
                    msg << _M(" was not added to the child: ");
                    msg << child;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }            
        }

        return 0;
    }



    int aspectratio( SCRIPT_STATE *L ) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

        if ( rend ) {
            tintaVector2f ar = rend->getAspectRatio();

            PUSH_FLOAT_ARG(L, ar.mx);
            PUSH_FLOAT_ARG(L, ar.my);

            return 2;
        }

        return 0;
    }

    /*
        @name d3.addsphere
        @description Adds plane entity
        @param name
        @param Real radius
        @param Int z tesselation
        @param Int radius tesselation        
        @param Boolean inside flag, if true - inside box
        @param material name
    */
    int addsphere(SCRIPT_STATE *L){


        String name = GET_VAL_STRING_EX(L, 1);
        
        float radius = GET_VAL_FLOAT(L, 2);
        int zTessel = GET_VAL_INT(L, 3);
        int radTessel = GET_VAL_INT(L, 4);

        bool inside = GET_VAL_BOOL(L, 5);

        String material = GET_VAL_STRING_EX(L, 6);
        String nameParent = GET_VAL_STRING_EX(L, 7);

        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (r ) {
            
            UNIQPTRVIRT(Graphica::tintaDrawable, ent, Graphica::tintaSceneManager::createSphere(zTessel, radTessel, radius, inside));
            if (ent) {
                if (material.size() > 0)
                    ent->addMaterial(material);
                ent->setName(name);

                auto newEnt = r->getSceneManager()->addEntity(std::move(ent));
                if (!newEnt) {
                    StringStream msg;
                    msg << _M("Entity with name: ");
                    msg << name;
                    msg << _M(" is already exists");
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                if (nameParent.length() > 0 && newEnt) {
                    if (!r->getSceneManager()->setParent(newEnt, nameParent)) {
                        StringStream msg;
                        msg << _M("Parent: ");
                        msg << nameParent;
                        msg << _M(" was not added ");
                        if (Tinta::tintaLogger::getPtr())
                            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
            }
        }
        
        return 0;
    }


    /*
        @name d3.addfront
        @description Adds GUI layer to the scene
        @param name
        @param Real position x {-1,1}
        @param Real position y {-1,1}
        @param Real w
        @param Real h
        @param Int w tesselation size
        @param Int h tesselation size
        @param material name
    */
    int addfront(SCRIPT_STATE *L) {
        
        String name = GET_VAL_STRING_EX(L, 1);

        float posX = GET_VAL_FLOAT(L, 2);
        float posY = GET_VAL_FLOAT(L, 3);

        float w = GET_VAL_FLOAT(L, 4);
        float h = GET_VAL_FLOAT(L, 5);
        int xTessel = GET_VAL_INT(L, 6);
        int yTessel = GET_VAL_INT(L, 7);

        String material = GET_VAL_STRING_EX(L, 8);        

        if (TintaMath::isZero(w) || TintaMath::isZero(h)) {
            StringStream msg;
            msg << _M("Wrong width or height for entity : ");
            msg << name;

            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
            TROW_ERR_FUNC(L, msg.str().c_str());

            return 0;
        }

        if (xTessel == 0 || yTessel == 0) {
            StringStream msg;
            msg << _M("Wrong tesselation values for entity : ");
            msg << name;
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
            TROW_ERR_FUNC(L, msg.str().c_str());
            return 0;
        }

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if ( r ) {            

            Graphica::tintaGuiManager * gui = r->getRenderer()->getFrontManager();
            CoreAssert(gui, "gui");

            Graphica::tintaDrawable* added = gui->addEffectLayer( { posX, posY }, { w,h }, 0.f, xTessel, yTessel, name, material );

            String parent = _M("");

            if ( IS_VAL_STRING(L, 9) ) {

                parent = GET_VAL_STRING_EX(L, 9);
                bool rez = gui->setParent(added, parent);

                if ( !rez ) {
                    StringStream msg;
                    msg << _M("Parent: ");
                    msg << parent;
                    msg << _M(" was not added ");
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }
            
        }        

        return 0;
    }


    /*
    @name d3.addback
    @description Adds Background layer to the scene
    @param name
    @param Real position x {-1,1}
    @param Real position y {-1,1}
    @param Real w
    @param Real h
    @param Int w tesselation size
    @param Int h tesselation size
    @param material name
    */
    int addback(SCRIPT_STATE *L) {

        String name = GET_VAL_STRING_EX(L, 1);

        float posX = GET_VAL_FLOAT(L, 2);
        float posY = GET_VAL_FLOAT(L, 3);

        float w = GET_VAL_FLOAT(L, 4);
        float h = GET_VAL_FLOAT(L, 5);
        if ( TintaMath::isZero(w) || TintaMath::isZero(h) ) {
            StringStream msg;
            msg << _M("Wrong width or height for entity : ");
            msg << name;            

            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
            TROW_ERR_FUNC(L, msg.str().c_str());
            
            return 0;
        }

        int xTessel = GET_VAL_INT(L, 6);
        int yTessel = GET_VAL_INT(L, 7);

        if ( xTessel == 0 || yTessel == 0 ) {
            StringStream msg;
            msg << _M("Wrong tesselation values for entity : ");
            msg << name;
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);
            TROW_ERR_FUNC(L, msg.str().c_str());
            return 0;
        }

        String material = GET_VAL_STRING_EX(L, 8);
        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if ( r ) {

            Graphica::tintaGuiManager * gui = r->getRenderer()->getBackManager();
            CoreAssert(gui, "gui");

            Graphica::tintaDrawable* added = gui->addEffectLayer({ posX, posY }, { w,h }, 0.f, xTessel, yTessel, name, material );

            if ( IS_VAL_STRING(L, 9) ) {

                String parent = GET_VAL_STRING_EX(L, 9 );
                bool rez = gui->setParent( added, parent );
                if ( !rez ) {
                    StringStream msg;
                    msg << _M( "Parent: " );
                    msg << parent;
                    msg << _M(" was not added ");
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }
        }

        return 0;
    }

    /*
        @name d3.addmesh
        @description Adds mesh entity
        @param name
        @param mesh name        
        @param material name
    */
    int addmesh(SCRIPT_STATE *L){


        String name = GET_VAL_STRING_EX(L, 1);        
        String meshName = GET_VAL_STRING_EX(L, 2);
        String material = GET_VAL_STRING_EX(L, 3);
        
        String nameParent = GET_VAL_STRING_EX(L, 4);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        
        if ( r ) {

            //r->setSceneManager
            //ent = r->getSceneManager()->createMesh(meshName);
            UNIQPTRVIRT(Graphica::tintaDrawable, ent, Graphica::tintaSceneManager::createMesh(meshName));
            
            if ( ent ) {
                if (material.size() > 0)
                    ent->addMaterial(material);
                ent->setName(name);

                auto newEnt = r->getSceneManager()->addEntity( std::move( ent ) );
                if (!newEnt) {
                    StringStream msg;
                    msg << _M("Entity with name: ");
                    msg << name;
                    msg << _M(" is already exists");
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
                if (nameParent.length() > 0 && newEnt) {
                    if (!r->getSceneManager()->setParent(newEnt, nameParent)) {
                        StringStream msg;
                        msg << _M("Parent: ");
                        msg << nameParent;
                        msg << _M(" was not added ");
                        if (Tinta::tintaLogger::getPtr())
                            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
            }
            else {
                StringStream msg;
                msg << _M("Wrong mesh name: ");
                msg << name;
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                TROW_ERR_FUNC(L, msg.str().c_str());
            }                
        }
        
        return 0;
    }

    /*
        @name d3.setclear
        @description Sets clear color        
        @param Real r
        @param Real g
        @param Real b        
    */
    int setclear(SCRIPT_STATE *L){


        float r = GET_VAL_FLOAT(L, 1);
        float g = GET_VAL_FLOAT(L, 2);
        float b = GET_VAL_FLOAT(L, 3);

        Graphica::tintaRoot* root = Graphica::tintaRoot::getPtr();
        if (root && root->getRenderer()){
            root->getRenderer()->setClearColor( Graphica::tintaRGBAColor(r, g, b, 0.f) );          
        }
        return 0;
    }

    
    /*
        @name d3.e
        @description executes lua command file
        @param name
    */
    int e(SCRIPT_STATE *L){

        String command = GET_VAL_STRING_EX(L, 1);
        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        String full;
        if ( r ) {

            if ( Tinta::isPathValid( command) )
                full = command;
            else {
                full.append(Tinta::getRootPath());
                full.append(_M("/"));
                full.append(command);

                if ( !Tinta::isPathValid(full) ){
                    StringStream msg;
                    msg << _M("Wrong path: ");
                    msg << full;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    return 0;
                }
            }


            tintaScriptContext *context =  r->getContext();
            context->executeFile(full);
        }
        
        return 0;
    }     

    /*
        @name d3.remove
        @description Removes entity
        @param name
    */
    int remove(SCRIPT_STATE *L){

        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX( L, 2 );

        bool andChild = false;
        if( IS_VAL_BOOL(L,3) )
            andChild = GET_VAL_BOOL(L, 3);
        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {
            
            if ( r ) {
                
                if ( !r->getSceneManager()->removeEntity(name, andChild) ) {
                    StringStream msg;
                    msg << _M("Wrong name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }                
            }
            return 0;
        }
        if (type == Graphica::Back || type == Graphica::Front) {


            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                //Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if ( !gui->deleteEffectLayer( name, andChild ) ) {                    
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
            }
            return 0;
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }


    /*
    @name d3.removeall
    @description Removes entity
    @param name
    */
    int removeall(SCRIPT_STATE *L) {
                
        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Scene) {           
            if (r)
                r->getSceneManager()->removeEntities();

            return 0;
        }

        if (type == Graphica::Back || type == Graphica::Front) {

            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                gui->deleteEffectLayers();

                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }


    /*
    @name d3.init
    @description     
    */
    int init(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( r ) {
           // r->createContext();
        }
        return 0;
    }

    /*
        @name d3.stop
        @description Removes all actions from entity or camera
        @param name
    */
    int stop(SCRIPT_STATE *L){



        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        
        String name = GET_VAL_STRING_EX(L, 2);
                
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (type == Graphica::Camera) {

            if (r) {

                Graphica::tintaCamera *cam = r->getSceneManager()->getCamera(name);
                if (cam) {
                    cam->deleteActions();
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong camera name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());


                }
                return 0;
            }
        }
        else if (type == Graphica::Back) {

            if (r ) {

                Graphica::tintaDrawable *e = r->getSceneManager()->getEntity(name);
                if (e) {
                    e->deleteActions();
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong entity name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());

                    
                }
                return 0;
            }
        }
        else if (type == Graphica::Back || type == Graphica::Front) {


            Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

            if (rend) {

                Graphica::tintaGuiManager * gui = NULL_M;

                if (type == Graphica::Back)
                    gui = rend->getBackManager();
                else
                    gui = rend->getFrontManager();

                CoreAssert(gui, "gui");

                Graphica::tintaDrawable *e = gui->getEffectLayer(name);

                if (e) {
                    e->deleteActions();                   
                }
                else {
                    StringStream msg;
                    msg << _M("Wrong layer name: ");
                    msg << name;
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                    TROW_ERR_FUNC(L, msg.str().c_str());
                    
                }
                return 0;
            }
        }

        StringStream msg;
        msg << _M("Wrong type name: ");
        msg << typeStr;
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;      

    }
   
    /*
        @name d3.setresolution
        @description Changes resulution
        @param Int w
        @param Int h
        @param Boolean fullscreen, true - switch on
    */
    int setresolution(SCRIPT_STATE *L){

        int w = GET_VAL_INT(L, 1);
        int h = GET_VAL_INT(L, 2);
        bool fullScr = GET_VAL_BOOL(L, 3);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (r && r->getRenderer()){
            Graphica::tintaGLRenderer *rend = r->getRenderer();
            rend->resizeView(w, h, fullScr);            
        }
        return 0;


    }

    /*
    @name d3.screenshot
    @description Makes screenshot        
    */
    int screenshot( SCRIPT_STATE *L ){

        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( r && r->getRenderer() ){
            Graphica::tintaGLRenderer *rend = r->getRenderer();

            String prefix = "Shot";
            String directory;

            if (IS_VAL_STRING(L, 1))
                prefix = GET_VAL_STRING_EX(L, 1);
            if (IS_VAL_STRING(L, 2))
                directory = GET_VAL_STRING_EX(L, 2);
                        
            rend->makeScreenShot( prefix, directory );
            
        }
        return 0;


    }
    

    int layerfront(SCRIPT_STATE *L) {

                
        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

        if (rend) {

            Graphica::tintaGuiManager * gui = NULL_M;

            if (type == Graphica::Back)
                gui = rend->getBackManager();
            else
                gui = rend->getFrontManager();

            CoreAssert(gui, "gui");
            
            if ( gui->getEffectLayer(name) ) {

                gui->moveback( name );
                return 0;
            }
            
            StringStream msg;
            msg << _M("Wrong layer name: ");
            msg << name;
            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

            TROW_ERR_FUNC(L, msg.str().c_str());         
           
        }

        return 0;
    }

    int layerback(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name = GET_VAL_STRING_EX(L, 2);        

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

        if ( rend ) {

            Graphica::tintaGuiManager * gui = NULL_M;

            if (type == Graphica::Back)
                gui = rend->getBackManager();
            else
                gui = rend->getFrontManager();

            CoreAssert(gui, "gui");

            if ( gui->getEffectLayer(name) ) {

                gui->movefront(name);
                return 0;
            }

            StringStream msg;
            msg << _M("Wrong layer name: ");
            msg << name;
            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

            TROW_ERR_FUNC(L, msg.str().c_str());

        }

        return 0;
    }

    int layerswap(SCRIPT_STATE *L) {


        String typeStr = GET_VAL_STRING_EX(L, 1);

        Graphica::SceneType type = getType(typeStr);

        String name1 = GET_VAL_STRING_EX(L, 2);

        String name2 = GET_VAL_STRING_EX(L, 3);

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        Graphica::tintaGLRenderer * rend = r ? r->getRenderer() : NULL_M;

        if (rend) {

            Graphica::tintaGuiManager * gui = NULL_M;

            if (type == Graphica::Back)
                gui = rend->getBackManager();
            else
                gui = rend->getFrontManager();

            CoreAssert(gui, "gui");

            if ( !gui->getEffectLayer(name1) ) {

                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name1;
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                TROW_ERR_FUNC(L, msg.str().c_str());
                
            }

            if (!gui->getEffectLayer(name2)) {

                StringStream msg;
                msg << _M("Wrong layer name: ");
                msg << name2;
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error, false);

                TROW_ERR_FUNC(L, msg.str().c_str());

            }

            gui->swap(name1, name2);            

        }

        return 0;
    }

    int gettime(SCRIPT_STATE *L) {

        //tintaApp *app = M_NEW DemoLuaApp();
        Graphica::tintaWinApp* api = Graphica::tintaWinApp::mWinApp;

        if ( api ) {        
            PUSH_UINT_ARG(L, api->getTime());
            return 1;
        }
        return 0;
    }

    int manual( SCRIPT_STATE *L ) {

        //tintaApp *app = M_NEW DemoLuaApp();
        Graphica::tintaWinApp* api = Graphica::tintaWinApp::mWinApp;

        if ( api ) {                      

            int timerPeriod = GET_VAL_INT(L, 1);
            api->setManual( timerPeriod );            
        }
        return 0;
    }


    int copyfont(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (!r)
            return 0;

        m_uint32 id = r->cloneFont(GET_VAL_UINT(L, 1));

        if (id == ZERO_ID) {
            StringStreamBasic msg;
            msg << "Wrong source font id: " << GET_VAL_UINT(L, 1);
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        PUSH_UINT_ARG(L, id);

        return 1;
    }

    int addfont(SCRIPT_STATE *L) {


        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if ( !r )
            return 0;

        String name = GET_VAL_STRING(L, 1);

       
        color_type color = { 0,0,0,0 };
        if (IS_VAL_INTEGER(L, 2) && IS_VAL_INTEGER(L, 3) && IS_VAL_INTEGER(L, 4))
            color = { GET_VAL_UBYTE(L, 2), GET_VAL_UBYTE(L, 3), GET_VAL_UBYTE(L, 4) };

        float scaleX = 1.f;
        float scaleY = 1.f;

        if ( IS_VAL_REAL(L, 5) && IS_VAL_REAL(L, 6) ) {
            scaleX = GET_VAL_FLOAT(L, 5);
            scaleY = GET_VAL_FLOAT(L, 6);
        }
            


        m_uint32 added = r->addFont(name, scaleX, scaleY, color);
        if (added != ZERO_ID) {
            PUSH_INT_ARG(L, added);
            return 1;
        }

        StringStreamBasic msg;
        msg << "Error while creating font name/path: " << name;
        TROW_ERR_FUNC(L, msg.str().c_str());

        return 0;
    }

    int selfont(SCRIPT_STATE *L) {
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (!r)
            return 0;

        if (!r->setFont(GET_VAL_UINT(L, 1))) {
            StringStreamBasic msg;
            msg << "Wrong font id: " << GET_VAL_UINT(L, 1);
            TROW_ERR_FUNC(L, msg.str().c_str());
        }
        return 0;
    }


    int delfont(SCRIPT_STATE *L) {
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (!r)
            return 0;

        bool rez = r->deleteFont(GET_VAL_UINT(L, 1));

        PUSH_BOOL_ARG(L, rez);

        return 1;
    }

    int delallfont(SCRIPT_STATE *L) {
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (!r)
            return 0;

        r->deleteFonts();
        return 0;
    }



    int fontcolor(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        if (!r)
            return 0;

        m_uint32 id = 0;
        if (IS_VAL_INTEGER(L, 1))
            id = GET_VAL_UINT(L, 1);

        color_type color = { 0,0,0,0 };
        if (IS_VAL_INTEGER(L, 2) && IS_VAL_INTEGER(L, 3) && IS_VAL_INTEGER(L, 4)) {
            color = { GET_VAL_UBYTE(L, 2), GET_VAL_UBYTE(L, 3), GET_VAL_UBYTE(L, 4) };

            if (!r->setFontParameter(id, 1.f, 1.f, color)) {
                StringStreamBasic msg;
                msg << "Wrong font id: " << GET_VAL_UINT(L, 1);
                TROW_ERR_FUNC(L, msg.str().c_str());
            }
        }
        return 0;
    }

    int rendertext(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (!r)
            return 0;

        String texName;
        if ( !IS_VAL_STRING(L, 1) ) {
            StringStreamBasic msg;
            msg << "Wrong name for new texture: " << GET_VAL_UINT(L, 1);
            TROW_ERR_FUNC(L, msg.str().c_str());
            return 0;
        }

        texName = GET_VAL_STRING(L, 1);

        String text;
        if (IS_VAL_STRING(L, 2))
            text = GET_VAL_STRING(L, 2);

        

        // default
        int xSpace = 15;
        if (IS_VAL_INTEGER(L, 3))
            xSpace = GET_VAL_INT(L, 3);

        int ySpace = xSpace;
        if (IS_VAL_INTEGER(L, 4))
            ySpace = GET_VAL_INT(L, 4);

        
        int textureW = 0;
        if (IS_VAL_INTEGER(L, 5))
            textureW = GET_VAL_INT(L, 5);

        int textureH = 0;
        if (IS_VAL_INTEGER(L, 6))
            textureH = GET_VAL_INT(L, 6);

        bool autoBreak = true;

        // get selected font
        tintaFont *font = r->getFont();

        if ( !font ) {
            TROW_ERR_FUNC(L, "Font not selected!");
        }
                     

        typedef utf8::iterator<std::string::iterator> utf8_string_iterator;
              
        int offsetX = 0;
        int offsetY = 0;

        int offsetXLocal = 0;
        int offsetYLocal = 0;

        int maxVertUpOffset = 0;
        int maxVertDownOffset = 0;

        
        utf8_string_iterator it(text.begin(), text.begin(), text.end());                 
        int maxX = 0;
        for (; it.base() != text.end(); ++it) {

            char buf[7] = {};

            utf8::append(*it, buf);
            String find(buf);

            if (find == _M("\n")) {
                maxX = max(maxX, offsetX);
                offsetX = 0;
                offsetY += offsetYLocal + ySpace;
            }

            if (find == _M(" ")) {
                offsetX += 2 * xSpace;
            }
            else {

                auto symbol = font->getSymbolData(find);

                if ( symbol ) {
                    if ( symbol->vertOffset < 0 )
                        maxVertUpOffset = -(symbol->vertOffset);

                    if (symbol->vertOffset > 0 && maxVertDownOffset < symbol->vertOffset)
                        maxVertDownOffset = symbol->vertOffset;
                        

                    //pImage->mixImage(&(symbol->mImage), offsetX, offsetY + symbol->vertOffset);
                    offsetXLocal = symbol->mImage.getWidth();
                    offsetYLocal = symbol->mImage.getHeight();
                    offsetX += (offsetXLocal + xSpace);
                }
            }

        }

        offsetY += offsetYLocal + ySpace;

        if ( textureW == 0 ) {
            textureW = max(offsetX,maxX);            
        }    

        if ( textureH == 0 ) {
            textureH = offsetY;
        }

        if ( textureW > 0 && textureH > 0 ) {

            utf8_string_iterator it_img(text.begin(), text.begin(), text.end());

            offsetXLocal = 0;
            offsetYLocal = 0;

            tintaUInt8Image imageSymbols( textureW, textureH + maxVertDownOffset + maxVertUpOffset );

            imageSymbols.setChannel(color_type::channel_a,0);
            offsetX = 0;
            offsetY = maxVertUpOffset;
            
            for ( ; it_img.base() != text.end(); ++it_img ) {

                char buf[7] = {};

                utf8::append(*it_img, buf);
                String find(buf);

                if (find == _M("\n")) {
                   
                    offsetX = 0;
                    offsetY += offsetYLocal + ySpace;
                }

                if (find == _M(" ")) {
                    offsetX += 2 * xSpace;
                }
                else {

                    auto symbol = font->getSymbolData(find);

                    if (symbol) {

                        imageSymbols.mixChannel(&(symbol->mImage), offsetX, offsetY + symbol->vertOffset, color_type::channel_a);

                        offsetXLocal = symbol->mImage.getWidth();
                        offsetYLocal = symbol->mImage.getHeight();
                        offsetX += (offsetXLocal + xSpace);
                    }
                }
            }   

           // Tinta::tintaIImgCodec* mImgFile = M_NEW tintaPngImgCodec ();
#if DEBUG_MODE
            UNIQPTRVIRT(Tinta::tintaIImgCodec, mImgFilePtr, M_NEW tintaPngImgCodec() );
            imageSymbols.saveToFile( mImgFilePtr.get(), _M("c:/foto/rez/font_out.png") );
#endif

            Graphica::tintaGLRenderer *rend = r ? r->getRenderer() : NULL_M;

            if ( rend ) {
                rend->releaseTexture( texName, false );
                r->addImage( imageSymbols, texName );
                rend->addTexture(texName);

                PUSH_UINT_ARG(L, imageSymbols.getWidth());
                PUSH_UINT_ARG(L, imageSymbols.getHeight());

                return 2;
            }
        
        }
        return 0;
    }


    int sbadd(SCRIPT_STATE *L) {
        
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (!r)
            return 0;

        String name = GET_VAL_STRING(L, 1);
        if (r->getSBuffer(name)) {            

            StringStreamBasic msg;
            msg << "Sampler buffer with name : " << name << _M(" already exists");
            TROW_ERR_FUNC(L, msg.str().c_str());            
        }

        Graphica::SBufferDraw drawType( Graphica::STATIC_DRAW );

        if ( IS_VAL_BOOL(L, 2) && GET_VAL_BOOL(L, 2) )
            drawType = Graphica::DYNAMIC_DRAW;

        r->addSBuffer(name, drawType);
        
        return 0;
    }

    int sbdel(SCRIPT_STATE *L) {




        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (!r)
            return 0;

        String name = GET_VAL_STRING(L, 1);

        Graphica::tintaSamplerBuffer *sb = r->getSBuffer(name);
        bool rez = false;

        if ( sb ) {

            Graphica::tintaGLRenderer *rend = r ? r->getRenderer() : NULL_M;

            if (rend  &&  rend->releaseSBuffer(sb))
                r->delSBuffer(name);
        }

        PUSH_BOOL_ARG(L, rez);

        return 1;
    }


    int sbresize(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (!r)
            return 0;

        String name = GET_VAL_STRING(L, 1);
        m_uint32 size = GET_VAL_UINT(L, 2);

        bool clear = false;
        if(IS_VAL_BOOL(L, 3))
            clear = GET_VAL_BOOL(L, 3);

        Graphica::tintaSamplerBuffer *sb = r->getSBuffer(name);

        if ( !sb) {

            StringStreamBasic msg;
            msg << "Sampler buffer with name " << name << (" was not found");
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        
        sb->resize(size, clear);        

        return 0;
    }

    int sbsize(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (!r)
            return 0;

        String name = GET_VAL_STRING(L, 1);       

        Graphica::tintaSamplerBuffer *sb = r->getSBuffer(name);

        if (!sb) {

            StringStreamBasic msg;
            msg << "Sampler buffer with name " << name << " was not found";
            TROW_ERR_FUNC(L, msg.str().c_str() );
        }

        PUSH_INT_ARG(L, (int)sb->elements() );
        return 1;
    }


    int sbsetchannel(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (!r)
            return 0;

        String name = GET_VAL_STRING(L, 1);
        m_uint32 index = GET_VAL_UINT(L, 2);
        int channel = GET_VAL_INT(L, 3);

        Graphica::tintaSamplerBuffer *sb = r->getSBuffer(name);

        if (!sb ) {
            StringStreamBasic msg;
            msg << "Sampler buffer with name " << name << " was not found";
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        if (index >= sb->elements() ) {
            StringStreamBasic msg;
            msg << "Out of boundary index:" << index;
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        if ( channel < 0 || channel >= Graphica::tintaSamplerBuffer::mComponents ) {
            StringStreamBasic msg;
            msg << "Channel value must be in range (0,3)";
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        sb->setChannel(index, channel, GET_VAL_FLOAT(L, 4) );

        return 0;
    }

    int sbgetchannel(SCRIPT_STATE *L) {

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        if (!r)
            return 0;

        String name = GET_VAL_STRING(L, 1);
        m_uint32 index = GET_VAL_UINT(L, 2);
        int channel = GET_VAL_INT(L, 3);

        Graphica::tintaSamplerBuffer *sb = r->getSBuffer(name);

        if (!sb) {
            StringStreamBasic msg;
            msg << "Sampler buffer with name " << name << " was not found";
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        if (index >= sb->elements()) {
            StringStreamBasic msg;
            msg << "Out of boundary index:" << index;
            TROW_ERR_FUNC(L, msg.str().c_str());
        }

        if (channel < 0 || channel >= Graphica::tintaSamplerBuffer::mComponents) {
            StringStreamBasic msg;
            msg << "Channel value must be in range (0,3)";
            TROW_ERR_FUNC(L, msg.str().c_str());
        }
       
        PUSH_FLOAT_ARG(L, sb->getChannel(index, channel));

        return 1;
    }


    static const luaL_Reg luaPoint[] = {        
        { POINT_setcolor, tintaPoints::setcolor },
        { POINT_getcolor, tintaPoints::getcolor },
        { POINT_getsize, tintaPoints::getsize },
        { POINT_setsize, tintaPoints::setsize },
        { POINT_points, tintaPoints::points },
        { POINT_add, tintaPoints::add },
        { POINT_del, tintaPoints::del },
        { POINT_find, tintaPoints::find },
        { POINT_addlayer, tintaPoints::addlayer },
        { POINT_findlayer, tintaPoints::findlayer },
        { POINT_filllayer, tintaPoints::filllayer },
        { POINT_swap, tintaPoints::swap },
        { POINT_dellayer, tintaPoints::dellayer },
        { POINT_dellayers, tintaPoints::dellayers },
        { POINT_visible, tintaPoints::visible },
        { POINT_setvisible, tintaPoints::setvisible },
        { POINT_movelayer, tintaPoints::movelayer },
        { POINT_arraypos, tintaPoints::arraypos },
        { POINT_move, tintaPoints::move },
        { POINT_getpos, tintaPoints::getpos },
        { NULL, NULL } };

#define D3_test "test"
    static const luaL_Reg util3d[] = {
        
        { D3_test, GraphicFunctions::test },
        { D3_getpos, GraphicFunctions::getpos },        
        { D3_setorient, GraphicFunctions::setorient },
        { D3_setparent, GraphicFunctions::setparent },
        { D3_setmaterial, GraphicFunctions::setmaterial },
        { D3_crosspoint, GraphicFunctions::crosspoint },
        { D3_visible, GraphicFunctions::visible },
        { D3_setvisible, GraphicFunctions::setvisible },
        { D3_mousepos, GraphicFunctions::mousepos },        
        { D3_setvariable, GraphicFunctions::setvariable },
        { D3_getorient, GraphicFunctions::getorient },
        { D3_getparent, GraphicFunctions::getparent },        
        { D3_roll, GraphicFunctions::roll },
        { D3_yaw, GraphicFunctions::yaw },
        { D3_pitch, GraphicFunctions::pitch },
        { D3_rotate, GraphicFunctions::rotate },
        { D3_rotatecycle, GraphicFunctions::rotatecycle },
        { D3_getdestpos, GraphicFunctions::getdestpos },
        { D3_moveto, GraphicFunctions::moveto },
        { D3_moveorient, GraphicFunctions::moveorient },
        { D3_moveon, GraphicFunctions::moveon },
        { D3_mulscale, GraphicFunctions::mulscale },
        { D3_scale, GraphicFunctions::scale },
        { D3_getscale, GraphicFunctions::getscale },
        { D3_status, GraphicFunctions::status },
        { D3_genname, GraphicFunctions::genname },        
        { D3_wareframemode, GraphicFunctions::wareframemode },
        { D3_delimage, GraphicFunctions::delimage },
        { D3_setchannel, GraphicFunctions::setchannel },
        { D3_getchannel, GraphicFunctions::getchannel },
        { D3_imagesize, GraphicFunctions::imagesize },
        { D3_addmaterial, GraphicFunctions::addmaterial },
        { D3_deltexture, GraphicFunctions::deltexture },
        { D3_loadtexture, GraphicFunctions::loadtexture },
        { D3_addimage, GraphicFunctions::addimage },        
        { D3_addplane, GraphicFunctions::addplane },
        { D3_addbox, GraphicFunctions::addbox },
        { D3_addbillboard, GraphicFunctions::addbillboard },
        { D3_winsize, GraphicFunctions::winsize },
        { D3_addmesh, GraphicFunctions::addmesh },
        { D3_addGUILayer, GraphicFunctions::addfront },
        { D3_addBackLayer, GraphicFunctions::addback },
        { D3_setclear, GraphicFunctions::setclear },        
        { D3_e, GraphicFunctions::e },
        { D3_remove, GraphicFunctions::remove },
        { D3_layerback, GraphicFunctions::layerback },
        { D3_layerswap, GraphicFunctions::layerswap },
        { D3_layerfront, GraphicFunctions::layerfront },
        { D3_removeall, GraphicFunctions::removeall },
        { D3_init, GraphicFunctions::init },        
        { D3_setresolution, GraphicFunctions::setresolution },
        { D3_addsphere, GraphicFunctions::addsphere },
        { D3_aspectratio, GraphicFunctions::aspectratio },
        { D3_screenshot, GraphicFunctions::screenshot },
        { D3_gettime, GraphicFunctions::gettime },
        { D3_manual, GraphicFunctions::manual },
        { D3_addtimer, GraphicFunctions::addtimer },
        { D3_deltimer, GraphicFunctions::deltimer },
        { D3_settimer, GraphicFunctions::settimer },
        { D3_deltimers, GraphicFunctions::deltimers },
        { D3_setkey, GraphicFunctions::setkey },
        { D3_keypressed, GraphicFunctions::keypressed },
        { D3_exists, GraphicFunctions::exists },
        { D3_stop, GraphicFunctions::stop }, 
        { D3_setcursor, GraphicFunctions::setcursor },
        // font
        { D3_copyfont, GraphicFunctions::copyfont },
        { D3_addfont, GraphicFunctions::addfont },
        { D3_selfont, GraphicFunctions::selfont },
        { D3_delfont, GraphicFunctions::delfont },
        { D3_delallfont, GraphicFunctions::delallfont },
        { D3_fontcolor, GraphicFunctions::fontcolor },
        { D3_rendertext, GraphicFunctions::rendertext },
        // sampler buffers
        { D3_sbadd, GraphicFunctions::sbadd },
        { D3_sbdel, GraphicFunctions::sbdel },
        { D3_sbresize, GraphicFunctions::sbresize },
        { D3_sbsize, GraphicFunctions::sbsize },
        { D3_sbsetchannel, GraphicFunctions::sbsetchannel },
        { D3_sbgetchannel, GraphicFunctions::sbgetchannel },
        { NULL, NULL } };


    int luaopen_util3d(lua_State *L) {
        luaL_newlib(L, util3d);
        return 1;
    }

    int luaopen_luaPoint(lua_State *L) {
        luaL_newlib(L, luaPoint);
        return 1;
    }
    static const luaL_Reg loadedlibs[] = {
        { "_G", luaopen_base },
        { "d3", luaopen_util3d },
        { "point", luaopen_luaPoint },
        { NULL, NULL } };


    void register3dLua(SCRIPT_STATE *L) {

        //luaL_newlib(L, util3d);

        const luaL_Reg *lib;
        /* "require" functions from 'loadedlibs' and set results to global table */
        for (lib = loadedlibs; lib->func; lib++) {
            luaL_requiref(L, lib->name, lib->func, 1);
            lua_pop(L, 1);  /* remove lib */
        }
    }

}



namespace Graphica {

    tintaRoot::tintaRoot(void)       
    {

        mRandGen.setRandSeed((int)time(0));
        createContext(); // NEW_T(tintaScriptContext)();
        
        mFontFacroties.push_back( M_NEW tintaImageFontFactory() );

        for (fontFactories_t::iterator it = mFontFacroties.begin(); it != mFontFacroties.end(); it++) {
            bool rez = mFonts.registerFactory(*it);
            CoreAssert(rez, "bool rez = mFonts.registerFactory(*it);");
        }
    }

    tintaRoot::tintaRoot(const String &configName) 
        :tintaRoot()
    {
        mConfigName = configName;
    }
    

    

    void tintaRoot::addFunctions() {

        Tinta::tintaUtilFunc::registerUtilLua(mScrptContext->getStateEx());
        Tinta::tintaUtilFunc::registerImageLua(mScrptContext->getStateEx());
        GraphicFunctions::register3dLua(mScrptContext->getStateEx());
        tintaOCLFunctions::registerLuaOcl(mScrptContext->getStateEx());
        tintaArrays::registerLuaArray(mScrptContext->getStateEx());
    }

    tintaRoot::~tintaRoot(void) {


        if (mConfig) {
            DELETE_T(mConfig, tintaGraphicConfig);
            mConfig = NULL_M;
        }

        if (mScrptContext) {
            DELETE_T(mScrptContext, tintaScriptContext);
            mScrptContext = NULL_M;
        }
       

        for ( images_t::iterator i = mImages.begin(); i!= mImages.end(); i++ )
            DELETE_T( i->second, tintaImage);

        mImages.clear();

        mMaterials.clear();
    }

    tintaMaterial* tintaRoot::findMaterial(const String &name) {
        auto it = mMaterials.find(name);
        if (it == mMaterials.end())
            return NULL_M;
        return it->second.get();
    }



    TintaMath::tintaRandomV2	   *tintaRoot::getRandGen() {
        return &mRandGen;
    }
    NameGenerator *tintaRoot::getNamegen() {
        return &mNameGen;
    }

	bool tintaRoot::initialize(){
		
       // if (!Tinta::tintaLogger::getPtr()) {
           // mLog = M_NEW Tinta::tintaLogger();
           // mLog->createLog(_M("ShapeLog.log"), false);
            //mLog->addConsole(&mCmdOut);
       //    }

		
        
        
		mConfig          = NEW_T (tintaGraphicConfig)();
        //mImgReader       = NEW_T(tintaImageReader)();	
        
        if ( !mConfig->parseConfig(mConfigName) )
            return false;

        
        const auto &meshes = mConfig->getMeshesPaths();

        for (auto i = meshes.begin(); i != meshes.end(); i++){

            String meshDir;
            if ( !isPathValid( *i ) ){
                meshDir.append( Tinta::getRootPath() );
                meshDir.append( _M("/") );
                meshDir.append( *i );
            }
            else
                meshDir = *i;

            StringVector meshPathVec = getFiles( meshDir, getFileProp( false, _M("mmesh"), -1) );
            
            UNIQPTRDEF( tintaMeshParser, meshParser, NEW_T(tintaMeshParser)() );
            for ( auto mesh = meshPathVec.begin(); mesh != meshPathVec.end(); mesh++ ){                
                tintaMeshData data = meshParser->parse( *mesh );                
                mMeshData.insert(t_MeshData::value_type( data.mMeshName, std::move(data) ) );                
            }
            
        }

        if (gpuExt)
            DELETE_T(gpuExt, tintaGPUExt);

        gpuExt = NEW_T(tintaGPUExt)();

        if ( mGPUObjs )
            DELETE_T(mGPUObjs, tintaClObjectContainer);

        mGPUObjs = NEW_T(tintaClObjectContainer)();
        {
            //m_ulong32 p = gpuExt->getPlatformsIDs();
            StringUtil::StrStreamType t;
            String err = gpuExt->getError();
            if (err.length() == 0) {
                t << "OCL GPU enabled";
            }
            Tinta::tintaLogger::getPtr()->logMsg(t.str());
        }

        // only one type - float vector(push/pop operations)
        mValBoxFacVec.push_back(M_NEW tintaFloatVectorFactory());
        mValBoxFacVec.push_back(M_NEW tintaIntVectorFactory());


        for ( compValBoxFactories_t::iterator it = mValBoxFacVec.begin(); it != mValBoxFacVec.end(); it++ ) {
            bool rez = mBoxObjects.registerFactory(*it);
            CoreAssert( rez, "bool rez = mBoxObjects->registerFactory( *it );" );
        }

		return true;
	}

    tintaImage *tintaRoot::addDummyImage() {
        // temp image 1x1 black
        StringStream nameStream;
        nameStream << mDummyName;
        tintaImage *img = getImage(nameStream.str());
        if ( !img ) {

            tintaUInt8Image colored;
            colored.createImage(1, 1);
            colored.setPixel({ 0,0,0,0 });

            img = addImage(colored, nameStream.str());
        }
        return img;
    }
    tintaImage * tintaRoot::addImage( const tintaUInt8Image &image, const String &name ) {

        imageFormat format{ IMG_RGBA };          
         
        tintaImage * texImage = NEW_T(tintaImage)();

        if ( texImage->read( image, name, format, IMG_UBYTE ) ) {
            auto find = mImages.find(name);
            
            if (find != mImages.end())
                mImages.erase(find);

            mImages.emplace(images_t::value_type(name, texImage));
        }        
        else {
            DELETE_T(texImage, tintaImage);
            texImage = nullptr;
        }

        return texImage;
    }

    bool  tintaRoot::delImage(const String &imageName) {
        auto it = mImages.find(imageName);
        if ( it == mImages.end() )
            return false;

        mImages.erase( it );
        return true;
    }

    void  tintaRoot::delTexFromMaterials(const String &textureName) {
       
        for (auto i : mMaterials) {
            i.second->removeTexture(textureName);
        }
    }

    void tintaRoot::deleteOCL(const String &program) {
#ifdef USING_GPUCL
        if (program.length() == 0) {
            gpuPrograms_t::iterator it = mGPUPrograms.begin();
            for (; it != mGPUPrograms.end(); it++) {
                M_DELETE(*it);
            }
            mGPUPrograms.clear();
            if (mGPUObjs) {
                mGPUObjs->clear();
                DELETE_T(mGPUObjs, tintaClObjectContainer);
                mGPUObjs = NULL_M;
            }
        }
        else {

            gpuPrograms_t::iterator i = std::find_if(mGPUPrograms.begin(), mGPUPrograms.end(), [&](tintaIClBase* prog) {
                return prog->getProgramName() == program; });

            if (i != mGPUPrograms.end()) {
                M_DELETE(*i);
                if (mGPUObjs) {
                    mGPUObjs->remove(program);
                }
            }
        }

#endif
    }

	tintaImage * tintaRoot::getImage( const String &imageName ){
		
        auto i = mImages.find(imageName);

        tintaImage * image = NULL_M;

        if (i != mImages.end())
            image = i->second;

        // not added yet
		if( !image ){

            try {  
              
                String ex(StringUtil::getFileExt(imageName));
                StringUtil::toLower(ex);

                imageFormat format{ IMG_RGB };

                if ( ex == _M("png") )
                    format = IMG_RGBA;
                     

                image = NEW_T(tintaImage)();
                if (image->read(imageName, format, IMG_FLOAT)) {

                    mImages.emplace(images_t::value_type(image->getName(), image)); //mImages.insertResource(image->getName(), image);
                }
                else {
                    DELETE_T(image, tintaImage);
                    image = NULL_M;
                }                
            }
            catch ( const tintaException &e ) {
                if (image)
                    DELETE_T(image, tintaImage);
                image = NULL_M;

                StringStream msg;
                msg << _M("Image read error: ");
                msg << imageName;
                msg << _M("Exception: ") << e.what();
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
            }            
            
		}
		return image;        
	}

    bool tintaRoot::registerCl(const String &program, const String &kernel) {
        
#ifdef USING_GPUCL
        if ( isPathValid( program ) && mGPUObjs ) {

            tintaIClBase* clProg = getOCLProgram(program);

            if ( clProg ) {

                return true;                
            }

            try {

                tintaIClBase *obj = M_NEW Box2dProgram(program, program, kernel);

                obj->create();

                mGPUObjs->registerObject(obj);
                mGPUPrograms.push_back(obj);

                return true;
            }
            catch (const Tinta::tintaException &e) {
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(e.getDescr());
            }


        }
#endif
        return false;
    }


    Tinta::tintaIClBase* tintaRoot::getOCLProgram(const String &program) {

        return mGPUObjs ? mGPUObjs->findObject( program ) : NULL_M;
    }

    void tintaRoot::setRenderer(tintaGLRenderer *renderer){
        mRenderer = renderer;    

        // parse materials only after renderer assigned
        const StringVector &m = mConfig->getMaterialPaths();

        for ( auto i = m.begin(); i != m.end(); i++ ) {

            String matDir;
            if (!isPathValid(*i)) {
                matDir.append(Tinta::getRootPath());
                matDir.append(_M("/"));
                matDir.append(*i);
            }
            else
                matDir = *i;

            StringVector matPathVec = getFiles(matDir, getFileProp(false, _M("material"), -1));

            for (auto mat = matPathVec.begin(); mat != matPathVec.end(); mat++) {
                //tintaMaterial * material = NEW_T(tintaMaterial)( *mat);
                if( !addMaterial( *mat ) ){
                    StringStream msg;
#if TINTA_LOCAL == TINTA_RU  
                    msg << _M("Материал : ") << *mat << _M(" не был  добавлен");
#elif TINTA_LOCAL ==  TINTA_EN	
                    msg << _M("Material : ") << *mat << _M(" has not been added");
#endif
                    if ( Tinta::tintaLogger::getPtr() )
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);
                }
            }
        }       
    }

    void tintaRoot::setSceneManager(tintaSceneManager  * sManager) {
        mSceneManager = sManager;
    }

    tintaGLRenderer *tintaRoot::getRenderer() {
        return mRenderer;
    }


    bool  tintaRoot::oclEnabled() const {
#ifdef USING_GPUCL
        return true;
#else
        return false;
#endif
        
    }

    tintaSceneManager *tintaRoot::getSceneManager() {
        return mSceneManager;
    }

	tintaScriptContext* tintaRoot::getContext(){
		return mScrptContext;
	}

    void tintaRoot::createContext() {
        
        if ( mScrptContext ) {
            DELETE_T(mScrptContext, tintaScriptContext);         
        }
        mScrptContext = NEW_T(tintaScriptContext)();
        mScrptContext->createState();

        addFunctions();
     
    }


    bool tintaRoot::addMaterial( SHAREDPTR(tintaMaterial) matPtr ) {
        if(matPtr.get())
            return   mMaterials.insert( t_materials_container::value_type(matPtr->getName(), matPtr) ).second;
        return NULL_M;
    }

    tintaMaterial     *tintaRoot::addMaterial(const String &path, const String &name ) {

        SHAREDPTRDEF(tintaMaterial, material, NEW_T(tintaMaterial)(path));

        if ( name.length() > 0 )
            material->setName( name );

        material->setRenderer(mRenderer);

        if ( material->parse() ) {

            if ( mMaterials.find(name) != mMaterials.end() )
                mMaterials.erase(name);
            
            if ( addMaterial(material) )
              return material.get();
            
        }
        return NULL_M;
    }

	const tintaGraphicConfig* tintaRoot::getConfigurator() const {
		return mConfig;
	}

	
    const tintaMeshData *tintaRoot::findMesh( const String &name ) const {
        t_MeshData::const_iterator i = mMeshData.find(name);
        if ( i == mMeshData.end() )
            return NULL_M;
        return &i->second;

    }

    tintaCommands      *tintaRoot::getCommands() {
        return &mCommands;
    }

    bool tintaRoot::setFont(m_uint32 id) {
        return mFonts.selectObj(id);
    }

    bool tintaRoot::setFontParameter(m_uint32 id, float scaleX, float scaleY, const color_type &color) {

        tintaFont* font = mFonts.getObjEx(id);

        if (!font)
            return false;

        if (scaleX > 0.f || scaleY > 0.f) {
            font->setSize(scaleX, scaleY);
        }

        if (color != color_type{ 0, 0, 0, 0 }) {
            font->setColor(color);
        }

        return true;

    }

    m_uint32 tintaRoot::addFont(const String &name, float sizeX, float sizeY, const color_type &color) {

        m_uint32 added = mFonts.addObj(tintaImageFont::str_font);
        tintaFont* font = mFonts.getObjEx(added);

        if (!font)
            return ZERO_ID;

        if (sizeX > 0 || sizeY > 0) {
            font->setSize(sizeX, sizeY);
        }

        if (color != color_type{ 0, 0, 0, 0 }) {
            font->setColor(color);
        }

        if (!font->create(name)) {
            mFonts.delObj(added);
            return ZERO_ID;
        }

        return added;
    }

    tintaFont* tintaRoot::getFont(m_uint32 id) {
        return mFonts.getObjEx(id);
    }

    m_uint32 tintaRoot::cloneFont(m_uint32 src) {


        tintaFont* srcFont = mFonts.getObjEx(src);

        if (!srcFont)
            return ZERO_ID;

        m_uint32 added = mFonts.addObj(tintaImageFont::str_font);
        tintaFont* font = mFonts.getObjEx(added);

        *font = *srcFont;
        return added;
    }

    tintaFont* tintaRoot::getFont() {
        return mFonts.getObjEx(mFonts.getIndexSel());
    }

    bool tintaRoot::deleteFont(m_uint32 id) {
        return mFonts.delObj(id);
    }

    void tintaRoot::deleteFonts() {
        mFonts.clear();
    }


    tintaSamplerBuffer *tintaRoot::getSBuffer(const String &name) {

        sbuffers_t::iterator itDel = mSBuffers.find(name);

        if( itDel == mSBuffers.end() )
            return NULL_M;

        return itDel->second;
    }

    const tintaSamplerBuffer *tintaRoot::getSBuffer( const String &name ) const {

        sbuffers_t::const_iterator itDel = mSBuffers.find(name);

        if (itDel == mSBuffers.end())
            return NULL_M;

        return itDel->second;
    }

    tintaSamplerBuffer * tintaRoot::addSBuffer(const String &name, SBufferDraw type) {

        tintaSamplerBuffer * rez = getSBuffer( name );
        if (rez)
            return rez;

        rez = NEW_T (tintaSamplerBuffer)( type, 0 );

        mSBuffers.emplace( sbuffers_t::value_type( name, rez ) );

        return rez;
    }

    void tintaRoot::delSBuffer(const String &name) {


        sbuffers_t::iterator itDel = mSBuffers.find(name);
        if (itDel == mSBuffers.end())
            return;

        DELETE_T(itDel->second, tintaSamplerBuffer);

        mSBuffers.erase(itDel);

        for (auto i : mMaterials) {
            i.second->removeSBuffer(name);
        }
    }

    tintaBoxContainer *tintaRoot::getBoxes() {
        return &mBoxObjects;
    }
	
}
