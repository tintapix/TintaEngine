/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaFunctions.h"

#include <Math/tintaVector3.h>
#include <Math/tintaVector4.h>
#include <Math/tintaCatRomSpline.h>
#include "Math/tintaMatrix33.h"
#include <tintaArray2d.h>
#include <OCL/tintaGPUExt.h>
#include <tintaTreeConfig/tintaTreeConfig.h>

// codecs
#include <ImageCodecs/tintaPngImgCodec.h>
#include <ImageCodecs/tintaJpgImgCodec.h>
#include <utf8.h>

#include "tintaCoord2.h"

#include "tintaImgStack.h"
#include "tintaConfigurator.h"
#include "tintaHermSpline.h"
#include "tintaBoxContainer.h"
#include "tintaTexSpringMain.h"
#include "tintaUInt8ImageSeamless.h"
#include "tintaObjCreator.h"
#include "tintaArrayBox.h"
#include "tintaInteractHandlerImpl.h"



namespace  Tinta {

	void clearstate(){
		tintaTexSpringMain* main = Tinta::tintaTexSpringMain::getPtr();
		if (main)
			main->clearContainers();
	}

    // setAborted(false) - resets flag(flag may be assigned outside functions)
#define TROW_ERR_FUNC_CLEAR(L, TEXT) { clearstate(); Tinta::tintaTexSpringMain::getPtr()->setAborted(false); TROW_ERR_FUNC(L, TEXT ); }
#define TROW_ERR_FUNC_CLEAR_EX(L, TEXT) { clearstate(); Tinta::tintaTexSpringMain::getPtr()->setAborted(false); TROW_ERR_FUNC_EX(L, TEXT ); }

    StringBasic createUniqId(){
        StringStreamBasic uid;
        StringBasic app_name = Tinta::tintaTexSpringMain::getPtr()->getAppName();
        m_uint32 appHash = FastHash(app_name.c_str(), app_name.length());
        uid << appHash;
        uid << "_";
        struct tm *pTime;
        time_t ctTime; time(&ctTime);
        pTime = localtime(&ctTime);

        timeb tb;
        ftime(&tb);
        //int nCount = tb.millitm;


        uid << pTime->tm_year + 1900
            << pTime->tm_mon + 1 << pTime->tm_mday
            << pTime->tm_hour << pTime->tm_min
            << pTime->tm_sec << tb.millitm << "_";
        int iRand = Tinta::tintaTexSpringMain::getPtr()->getRandGen()->randomInt();
        uid << iRand;

        return uid.str();
    }
		

    String readData( const String &pathIn , SCRIPT_STATE *L ) {


        String path;
        String buff;

        if ( Tinta::isPathValid(pathIn) )
            path = pathIn;
        else {
            const tintaConfigurator * config = Tinta::tintaConfigurator::getPtr();
            CoreAssert(config, "config==NULL");
            if (!config->getScriptPath(path, pathIn)) {
                StringUtil::StrStreamType msg;
                msg << "No one path was found for: ";
                msg << pathIn << "\n";
                msg << "Root path is: ";
                msg << Tinta::getRootPath();
                TROW_ERR_FUNC_CLEAR_EX(L, msg.str().c_str());
                
            }
        }

        
        readUTF8Text(path, buff);
        
        return buff;
    }

    namespace imageAlgorithm {


        struct ColorPrior {
            int      count;
            color_type color;
        };


        typedef std::vector<ColorPrior> colors_t;
        typedef std::vector<Tinta::coord2dI_t> coordinates_t;


        bool doNext(const color_type *curPixel, const color_type *nearPixel,
            const Tinta::coord2dI_t &cur, const Tinta::coord2dI_t &next, colors_t &around,
            coordinates_t &walked, float factor) {

            coordinates_t::iterator i = std::find_if(walked.begin(), walked.end(), [&](const Tinta::coord2dI_t &v) {return v == next; });

            if (i == walked.end()) {
                float dist = byteToFloat((m_uint8)Tinta::distance(Tinta::tintaVector3i(nearPixel->getVal(color_type::channel_r), nearPixel->getVal(color_type::channel_g),
                    nearPixel->getVal(color_type::channel_b)), Tinta::tintaVector3i(curPixel->getVal(color_type::channel_r),
                        curPixel->getVal(color_type::channel_g), curPixel->getVal(color_type::channel_b))));

                if (dist <= factor) {
                    return true;
                }
                else {

                    colors_t::iterator color = std::find_if(around.begin(), around.end(), [&](const ColorPrior &v) {
                        return v.color == *nearPixel; });

                    if (color == around.end()) {
                        around.push_back({ 1 ,  *nearPixel });
                    }
                    else
                        color->count++;
                }
            }
            return false;
        }

        void pixelWalker(const image_t *src, const Tinta::coord2dI_t &point, colors_t &around,
            coordinates_t &walked, float factor, m_uint32 maxWalk) {

            if (walked.size() > maxWalk)
                return;

            walked.push_back(point);

            color_type curPixel = src->getPixel(point);
            {
                Tinta::coord2dI_t next{ point.mValX - 1, point.mValY };
                color_type left = src->getPixel(next);

                if (src->validPos(next)) {
                    if (doNext(&curPixel, &left, point, next, around, walked, factor)) {
                        pixelWalker(src, next, around, walked, factor, maxWalk);
                    }
                }
            }
            {
                Tinta::coord2dI_t next{ point.mValX - 1, point.mValY - 1 };
                color_type leftUp = src->getPixel(next);

                if (src->validPos(next)) {
                    if (doNext(&curPixel, &leftUp, point, next, around, walked, factor)) {
                        pixelWalker(src, next, around, walked, factor, maxWalk);
                    }
                }
            }

            {

                Tinta::coord2dI_t next{ point.mValX, point.mValY - 1 };
                color_type up = src->getPixel(next);

                if (src->validPos(next)) {
                    if (doNext(&curPixel, &up, point, next, around, walked, factor)) {
                        pixelWalker(src, next, around, walked, factor, maxWalk);
                    }
                }
            }
            {
                Tinta::coord2dI_t next{ point.mValX + 1, point.mValY - 1 };
                color_type rightUp = src->getPixel(next);
                if (src->validPos(next)) {

                    if (doNext(&curPixel, &rightUp, point, next, around, walked, factor)) {
                        pixelWalker(src, next, around, walked, factor, maxWalk);
                    }
                }
            }
            {
                Tinta::coord2dI_t next{ point.mValX + 1, point.mValY };
                color_type right = src->getPixel(next);
                if (src->validPos(next)) {

                    if (doNext(&curPixel, &right, point, next, around, walked, factor)) {
                        pixelWalker(src, next, around, walked, factor, maxWalk);
                    }
                }
            }
            {
                Tinta::coord2dI_t next{ point.mValX + 1, point.mValY + 1 };
                color_type rightDown = src->getPixel(next);
                if (src->validPos(next)) {

                    if (doNext(&curPixel, &rightDown, point, next, around, walked, factor)) {
                        pixelWalker(src, next, around, walked, factor, maxWalk);
                    }
                }
            }
            {
                Tinta::coord2dI_t next{ point.mValX, point.mValY + 1 };
                color_type down = src->getPixel(next);

                if (src->validPos(next)) {

                    if (doNext(&curPixel, &down, point, next, around, walked, factor)) {
                        pixelWalker(src, next, around, walked, factor, maxWalk);
                    }
                }
            }
            {
                Tinta::coord2dI_t next{ point.mValX - 1, point.mValY + 1 };
                color_type leftDown = src->getPixel(next);
                if (src->validPos(next)) {
                    if (doNext(&curPixel, &leftDown, point, next, around, walked, factor)) {
                        pixelWalker(src, next, around, walked, factor, maxWalk);
                    }
                }
            }


        }
    }

    namespace tintaMainFunctions {


        int test(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }



            tintaExecutingTask task(_M("Shit command!"), 100, tintaExecutingTask::enCommonTask);


            size_t size = task.GetBufferSize();

            UNIQPTRDEF(tintaBufferIO, dataBuff, NEW_T(tintaBufferIO)());

            dataBuff->AllocateBuffer(size);


            tintaExecutingTask task2;

            return 1;

#if TESTINTERAVAL
            tintaRandomV2 rand;
            rand.setRandSeed((unsigned)time(0));

            float r = rand.randomInterv(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2));
            PUSH_FLOAT_ARG(L, r);
            return 1;
            //THREAD_SLEEP(100000)
            //MUTEX(mRequestMutex)
            //LOCK_MUTEX_NAMED(mRequestMutex, queueLock);
            //THREAD_WAIT(mRequestCondition, mRequestMutex, queueLock);
            //mRequestCondition.wait(queueLock);
#endif
#if MOLYCONFTEST
            String path = toWideChar(GET_VAL_STRING(L, 1));
            String request = toWideChar(GET_VAL_STRING(L, 2));
            size_t q = 0;
            tintaTreeConfig::tintaTreeConfig conf(path, false);
            if (!conf.getErrors(q))
                conf.parse(); // if not parsed - failed

            String *err = conf.getErrors(q);
            for (size_t i = 0; i < q; i++) {
                Tinta::tintaLogger::getPtr()->logMsg(err[i]);

            }
            if (err)
                return 0;

            path.append(_M("s"));

            if (IS_VAL_REAL(L, 3)) {
                double result;
                if (!conf.getErrors(q)) {
                    if (conf.getValue(request, result)) {
                        PUSH_FLOAT_ARG(L, (float)(result));

                        conf.setValue(request, 100.);
                        String err = conf.getError();
                        if (!conf.getErrors(q))
                            conf.savetoTheFile(path);

                        return 1;
                    }
                }
            }
            else if (IS_VAL_STRING(L, 3)) {
                String result;
                if (!conf.getErrors(q)) {
                    if (conf.getValue(request, result)) {
                        PUSH_STRING_ARG(L, result).c_str());
                        return 1;
                    }
                }
            }
            else if (IS_VAL_BOOL(L, 3)) {
                bool result;
                if (!conf.getErrors(q)) {
                    if (conf.getValue(request, result)) {
                        PUSH_BOOL_ARG(L, result);
                        return 1;
                    }
                }
            }

            err = conf.getErrors(q);
            for (size_t i = 0; i < q; i++)
                Tinta::tintaLogger::getPtr()->logMsg(err[i]);
#endif			




#if GPUTEST
            tintaUInt8Image img;
            img.createImage(10, 12);
            img.setPixel(12, color_type(100, 101, 102, 10));
            tintaBufferIO *dataBuff = NEW_T(tintaBufferIO)();
            size_t sizeData = img.GetBufferSize();
            dataBuff->AllocateBuffer(sizeData);

            img.packData(dataBuff->GetBufferEx(), 0);

            tintaUInt8Image img2;

            img2.unpackData(dataBuff->GetBuffer(), 0);


            return 0;

#endif

        }


#define GET_VAL_UINT_(L, INDEX) ( static_cast<m_uint32> ( (lua_tointeger(L, INDEX)  >= 0) ? lua_tointeger(L, INDEX) : 0 ) )

        int printver(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            String values;
            Tinta::tintaLogger::getPtr()->logMsg(values);
            return 0;

        }

        /*
         Throws execution error with msg
         param -  comma-separated parameters
        */
        int lassert(SCRIPT_STATE *L) {

            bool vExp = GET_VAL_BOOL(L, 1);
            if (!vExp)
            {
                StringBasic msg = GET_VAL_STRING(L, 2);
                if (msg.size() > 0) {
                    TROW_ERR_FUNC_CLEAR(L, msg.c_str());
                }
                else
                    TROW_ERR_FUNC_CLEAR(L, "Assert has occurred!");
            }
            return 0;
        }






        int f(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            if (GET_QUANTITY(L) != 1) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            String com = GET_VAL_STRING_EX(L, 1);

            String path;
            if (Tinta::isPathValid(com))
                path = com;
            else {
                const tintaConfigurator * config = Tinta::tintaConfigurator::getPtr();
                CoreAssert(config, "config==NULL");
                if (!config->getScriptPath(path, com)) {
                    StringUtil::StrStreamType msg;
                    msg << "No one path was found for: ";
                    msg << com << "\n";
                    msg << "Root path is: ";
                    msg << Tinta::getRootPath();
                    //Tinta::tintaLogger::getPtr()->logMsg(msg.str());	
                    TROW_ERR_FUNC_CLEAR_EX(L, msg.str().c_str());

                }
            }

            //String command = StringUtil::tintaConstrFileExec(path);

            String buff;
            bool rez = false;


            rez = readUTF8Text(path, buff);

            if (rez)
                tintaTexSpringMain::getPtr()->executeCommand(buff, tintaExecutingTask::enCommonTask);

            return 0;
        }

        int fd(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            if (GET_QUANTITY(L) != 1) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            String com = GET_VAL_STRING_EX(L, 1);

            String path;
            if (Tinta::isPathValid(com))
                path = com;
            else {
                const tintaConfigurator * config = Tinta::tintaConfigurator::getPtr();
                CoreAssert(config, "config==NULL");
                if (!config->getScriptPath(path, com)) {
                    StringUtil::StrStreamType msg;
                    msg << "No one path was found for: ";
                    msg << com << "\n";
                    msg << "Root path is: ";
                    msg << Tinta::getRootPath();
                    //Tinta::tintaLogger::getPtr()->logMsg(msg.str());	
                    TROW_ERR_FUNC_CLEAR_EX(L, msg.str().c_str());

                }
            }

            //String command = StringUtil::tintaConstrFileExec(path);

            String buff;

            bool rez = readUTF8Text(path, buff);

            if (rez)
                tintaTexSpringMain::getPtr()->executeCommand(buff, tintaExecutingTask::enDownTask);

            return 0;
        }


        int fl(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            if (GET_QUANTITY(L) != 1) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            String com = GET_VAL_STRING_EX(L, 1);

            String path;
            if (Tinta::isPathValid(com))
                path = com;
            else {
                const tintaConfigurator * config = Tinta::tintaConfigurator::getPtr();
                CoreAssert(config, "config==NULL");
                if (!config->getScriptPath(path, com)) {
                    StringUtil::StrStreamType msg;
                    msg << "No one path was found for: ";
                    msg << com << "\n";
                    msg << "Root path is: ";
                    msg << Tinta::getRootPath();
                    //Tinta::tintaLogger::getPtr()->logMsg(msg.str());	
                    TROW_ERR_FUNC_CLEAR_EX(L, msg.str().c_str());

                }
            }

            //String command = StringUtil::tintaConstrFileExec(path);

            String buff;

            bool rez = readUTF8Text(path, buff);

            if (rez)
                tintaTexSpringMain::getPtr()->executeCommand(buff, tintaExecutingTask::enLocalTask);

            return 0;


        }

        int fu(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            if (GET_QUANTITY(L) != 1) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            String com = GET_VAL_STRING_EX(L, 1);

            String path;
            if (Tinta::isPathValid(com))
                path = com;
            else {
                const tintaConfigurator * config = Tinta::tintaConfigurator::getPtr();
                CoreAssert(config, "config==NULL");
                if (!config->getScriptPath(path, com)) {
                    StringUtil::StrStreamType msg;
                    msg << "No one path was found for: ";
                    msg << com << "\n";
                    msg << "Root path is: ";
                    msg << Tinta::getRootPath();
                    //Tinta::tintaLogger::getPtr()->logMsg(msg.str());	
                    TROW_ERR_FUNC_CLEAR_EX(L, msg.str().c_str());

                }
            }

            //String command = StringUtil::tintaConstrFileExec(path);

            String buff;

            bool rez = readUTF8Text(path, buff);

            if (rez)
                tintaTexSpringMain::getPtr()->executeCommand(buff, tintaExecutingTask::enUpperTask);

            return 0;
        }

     
        int getangle(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L); //function name
            const int params = 4;
            if (argc != params) {

                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            float angle = uangleBetween(tintaVector2f(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2)), tintaVector2f(GET_VAL_FLOAT(L, 3), GET_VAL_FLOAT(L, 4)));


            PUSH_DOUBLE_ARG(L, (double)TintaMath::toDeg(angle)); // return value
            return 1;

        }

        int  start(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            m_uint32 period = GET_VAL_UINT(L, 1);

            String command = GET_VAL_STRING_EX(L, 2);            

            if (period > 0 && command.length() > 0)
                Tinta::tintaTexSpringMain::getPtr()->resetTimer(period, command);
            else {
                StringStream s;
                s << _M("Wrond timer parameteres");
                TROW_ERR_FUNC_CLEAR_EX(L, s.str().c_str());
            }


            return 0;


        }

        int  stop(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            Tinta::tintaTexSpringMain::getPtr()->stopTimer();

            return 0;
        }

       

        /*
            Rotates point around other on angle
            param - xCenter
            param - yCenter
            param - xPoint
            param - yPoint
            param - angle in degree
            return xPoint
            return yPoint

        */
        int rotpoint(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L);
            int rag_count = 5; // x center, y center, x point, y point, angle

            if (argc != rag_count) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }
            GeomPoint center(GET_VAL_INT(L, 1), GET_VAL_INT(L, 2));
            GeomPoint point(GET_VAL_INT(L, 3), GET_VAL_INT(L, 4));
            float f_angle = GET_VAL_FLOAT(L, 5);

            GeomPoint resultPoint;
            //f_angle *= -1;
//			Tinta::tintaAlgorithm::getRotPoint(point_center, point_to_rotate, TintaMath::toRad(f_angle), ropoint_t );

            //!!! rotation  CCW
            tintaMatrix33 rot_matrix;
            rot_matrix.rotationZ(f_angle);
            // new coordinates
            Tinta::tintaVector3f resultVec((m_float32)(center.mValX - center.mValX), (m_float32)(center.mValY - center.mValY), 0.f);
            resultPoint.mValX = (int)ceil(rot_matrix.getColumn(0).mx * resultVec.mx + rot_matrix.getColumn(0).my * resultVec.my);
            resultPoint.mValY = (int)ceil(rot_matrix.getColumn(1).mx * resultVec.mx + rot_matrix.getColumn(1).my * resultVec.my);

            // return to old coordinate system
            resultPoint.mValX += center.mValX;
            resultPoint.mValY += center.mValY;

            PUSH_INT_ARG(L, resultPoint.mValX);
            PUSH_INT_ARG(L, resultPoint.mValY);

            return 2;

        }




        int setrandseed(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            TintaMath::tintaRandomV2 *rand_gen = Tinta::tintaTexSpringMain::getPtr()->getRandGen();
            if (rand_gen)
                rand_gen->setRandSeed((unsigned)time(0));

            return 0;

        }

        int randint(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L); //1 - String val - string
            String val;
            //wrong arg value
            if (argc != 2) {

                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
                return 0;
            }

            TintaMath::tintaRandomV2 *rand_gen = Tinta::tintaTexSpringMain::getPtr()->getRandGen();
            int vMin = (int)(GET_VAL_DOUBLE(L, 1));
            int vMax = (int)(GET_VAL_DOUBLE(L, 2));
            if (vMax < vMin) {

                TROW_ERR_FUNC_CLEAR(L, "Random min grater than random max!");

            }

            PUSH_INT_ARG(L, rand_gen->randomInt(vMin, vMax));

            return 1;
        }


        int randintex(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L); //1 - String val - string
            String val;
            //wrong arg value
            if (argc != 2) {

                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            //tintaRandomV2 *rand_gen = Tinta::tintaTexSpringMain::getPtr()->getRandGen();

            tintaRandGen *gen = Tinta::tintaRandGen::getPtr();
            int vMin = (int)(GET_VAL_DOUBLE(L, 1));
            int vMax = (int)(GET_VAL_DOUBLE(L, 2));
            if (vMax < vMin) {

                TROW_ERR_FUNC_CLEAR(L, "Random min grater than random max!");

            }

            PUSH_INT_ARG(L, gen->getNext(vMin, vMax));

            return 1;
        }


        /*
          Generates float random number [0,1]
          return float random number
       */
        int randfloat(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            TintaMath::tintaRandomV2 *rand_gen = Tinta::tintaTexSpringMain::getPtr()->getRandGen();

            PUSH_FLOAT_ARG(L, rand_gen->randomFloat());

            return 1;
        }



        /*
           calculates hash from string
           param script name to execute like file.lua
        */
        int calchash(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L); //1 - String val - string
            StringBasic val;
            //wrong arg value
            if (argc != 1) {

                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            val = GET_VAL_STRING(L, 1);


            m_uint32 rez = FastHash(val.c_str(), val.length());

            PUSH_UINT_ARG(L, rez);
            return 1;
        }






        /*
           Returns app name
       */
        int getname(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            String app_name = Tinta::tintaTexSpringMain::getPtr()->getAppName();


            PUSH_STRING_ARG(L, app_name.c_str());
            return 1;
        }




        int uniqid(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            PUSH_STRING_ARG(L, createUniqId().c_str());
            return 1;

        }


        int getseed(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int uniq_id = StringUtil::hashStrCaseSens(
                Tinta::tintaTexSpringMain::getPtr()->getAppName().c_str());
            unsigned seed_val = uniq_id + (unsigned)time(0);

            PUSH_DOUBLE_ARG(L, seed_val);
            return 1;

        }


        int readfile(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            StringVector get;
            StringBasic type = GET_VAL_STRING(L, 2);
            StringBasic path = GET_VAL_STRING(L, 1);

            if (path.compare("") == 0) {
                StringStreamBasic msg;
                msg << "Wrong path: " << path.c_str();
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }
            bool rez = false;
            String realPath = toWideChar(path);
            StringUtil::toLower(type);



            rez = readUTF8Text(realPath, get);


            if (!rez) {
                TROW_ERR_FUNC_CLEAR(L, "Read file error.");
                return 0;
            }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            m_uint32 id = boxes->addObj(tintaStringValVector::str_strs_vector);

            if (id == ZERO_ID) {
                StringStreamBasic msg;
                msg << "Buffer creating error: " << GET_VAL_STRING(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                //PUSH_INT_ARG( L, ZERO ); // return value
                return 0;
            }


            tintaArrayBox *box = boxes->getObjEx(id);


            bool inOneLine{ false };

            if (IS_VAL_BOOL(L, 3)) {
                inOneLine = GET_VAL_BOOL(L, 3);
            }

            tintaArray2dString_t* arr = box->getStringBox();
            String val;

            if (inOneLine) {
                for (size_t i = 0; i < get.size(); i++) {
                    val.append(_M("\n"));
                    val.append(get[i]);
                }


                arr->pushBack(_M(""));
                String* add = arr->getPtr();
                *add = std::move(val);
            }
            else {
                for (size_t i = 0; i < get.size(); i++)
                    arr->pushBack(get[i]);
            }

            PUSH_UINT_ARG(L, id);
            return 1;
        }

        int writetofile(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");



            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 2));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 2);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dString_t* arr = box->getStringBox();

            if (!arr || box->getType() != tintaArrayBox::enVectorBox) {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            const std::vector< String > &add = arr->getArray();


            StringBasic path = GET_VAL_STRING(L, 1);
            if (path.length() == 0) {
                StringStreamBasic msg;
                msg << "Wrong path: " << path.c_str();
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }
            bool rez = false;
            String realPath = toWideChar(path);

            rez = writeUTF8toFile(realPath, add, true);

            if (!rez) {
                TROW_ERR_FUNC_CLEAR(L, "Write file error.");
                return 0;
            }

            return 0;
        }



        /*
            Returns mode,  true - if server mode
        */
        int isserver(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            PUSH_INT_ARG(L, Tinta::tintaTexSpringMain::getPtr()->isServMode()); //	

            return 1;

        }

        

        /*
        Closes app
        */
        int exit(SCRIPT_STATE *L) {

            tintaTexSpringMain::getPtr()->exit();
            return 0;
        }

        

		  // system functions
		  void tasks(){


              StringStream msg;
              msg << _M("Tasks in queue: ") << Tinta::tintaTexSpringMain::getPtr()->getTaskQueue()->getSize();
              Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msgLevel::msg_info);

		  }

		  void state() {
			  const tintaExecutingUnit *unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit( NULL_M );

			  m_ulong32 id = unit ? unit->getId().mid : 0;

              for( ;unit; unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit( &id ) ){
                      id = unit->getId().mid;
				  	  state( unit );
              }
		  }
        
          

		  void state( const tintaExecutingUnit *unit ){
#ifndef TINTA_NO_INTERACT
			if( !unit || unit->getType() !=  tintaExecutingUnit::enInteractUnit)
				return;

			const tintaAsioClient *client = unit->getInteractClient();

			if( !client )
				return;

			tintaBufferIO buff;
			buff.AllocateBuffer( sizeof( m_int8 ) ); // minimum data


			//tintaBufferIO *buffPacket = tintaInteractHandlerImpl::allocatePacketData(tintaInteractHandlerImpl::enStateRequest, &buff);

            //UNIQPTRDEF(tintaBufferIO, buffPacket, tintaInteractHandlerImpl::allocatePacketData(tintaInteractHandlerImpl::enStateRequest, &buff));

            tintaBufferIOUnqPtr buffPacket = tintaInteractHandlerImpl::allocatePacketDataPtr(tintaInteractHandlerImpl::enStateRequest, &buff);

			tintaChildProc * interact = tintaTexSpringMain::getPtr()->getInteract();
			
			if( interact ){
				tintaAsioInteract* interactBoost = interact->getInteract();
                CoreAssert(interactBoost, "interactBoost == NULL");

				if ( interactBoost ){

					size_t id( 0 );
					client->getId();
					m_ulong32 clientId = client->getId();
					interactBoost->sendData( buffPacket.get(), id , clientId );
				}
			}
#endif
			//DELETE_T(buffPacket, tintaBufferIO);

		  }

          void cls(){
              
          }

          void cleartasks( ) {              
              Tinta::tintaTexSpringMain::getPtr()->getTaskQueue()->clearQueue();
                            
          }
		  // system functions
		  void units(){

			  const tintaExecutingUnit *unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit( );
			  if(!unit)
				  return; 
			  m_ulong32 id = unit->getId().mid;
			  
			  
              Tinta::tintaLogger::getPtr()->logMsg(_M("# Units "), msgLevel::msg_system);
			  for( ;unit != NULL ; unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit( &id ) ){

				  StringStream msg;  

                  String unitState;
                  if (unit->getState() == tintaExecutingUnit::enNoState)
                      unitState = _M("# unknown ");
                  else if (unit->getState() == tintaExecutingUnit::enReady)
                      unitState = _M("# ready ");
                  else if (unit->getState() == tintaExecutingUnit::enBusy)
                      unitState = _M("# busy ");
                  else if (unit->getState() == tintaExecutingUnit::enReserved)
                      unitState = _M("# reserved ");

                  msg << unitState;
				  id = unit->getId().mid;		  
				  
                  

                  msg << _M("name: ") << unit->getId().mname << _M(" id: ") << unit->getId().mid << _M(" prior: ") << std::setprecision(3) << unit->getPrior();
                  msg << _M(" ");			  				 
				  
                  const tintaExecutingTask *t = unit->getSavedTask();
                  msg << _M("tasks: ");
                  if ( t  && t->getCommand().length() > 0 )
                    msg << 1;
                  else
                    msg << 0;
                  Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msgLevel::msg_info);

			  }

		  }

          // system functions
          // totaly restarts unit with tasks deleting 
          void restart() {

              abort();
              cleartasks();
              const tintaExecutingUnit *unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit();
              if ( !unit )
                  return;
              m_ulong32 id = unit->getId().mid;
              
              for (; unit != NULL; unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit(&id)){
                  
                  id = unit->getId().mid;                                   
                  if ( unit->getType() == tintaExecutingUnit::enLocalUnit ){
                      tintaTexSpringMain::getPtr()->getUnitsSet()->resetUnit( unit->getId() );
                  }                     
              }
          }


          void reset() {

              abort();           

              const tintaExecutingUnit *unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit();
              if ( !unit )
                  return;

              m_ulong32 id = unit->getId().mid;

              for ( ; unit != NULL; unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit( &id ) ) {

                  id = unit->getId().mid;
                  if ( unit->getType() == tintaExecutingUnit::enLocalUnit ) {
                      tintaTexSpringMain::getPtr()->getUnitsSet()->resetUnit(unit->getId());
                  }
              }

          }


          // fonsts
          void fonts() {


              if ( !Tinta::tintaTexSpringMain::getPtr() )
                  return;

              const tintaFontContainer *fonts =  Tinta::tintaTexSpringMain::getPtr()->getFonts();
              m_uint32 indexNext = ZERO_ID;

              const tintaFont * font = fonts->getFirstObj( indexNext );

              if ( !font )
                  return;

              StringStream msg;
              msg << _M("Fonts: \n");
              while ( font ) {                  
                  
                  msg << indexNext << _M(" ") << font->getName() << _M("\n");
                  font = fonts->getNextObj(indexNext);                  
              }
              Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msgLevel::msg_system);
          }
		  // system containers
		  void containers() {	

					 

			  tintaBoxContainer		  *boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();

			  typedef Tinta::tintaImgStack::tintaPixImageContainer ImgContainers_t;

			  const ImgContainers_t   *images  = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getContainer();
			
              m_uint32 indexNext = ZERO_ID;
              const tintaUInt8Image * img = images->getFirstObj(indexNext);
			  while( img ){
					StringStream msg;  
					msg<<_M("Image: ");
					msg<<img->getWidth();
					msg<<_M("x");
					msg<<img->getHeight();
					msg<<_M(" id: ");
					msg<<indexNext;	
                    img = images->getNextObj(indexNext);
                    Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msgLevel::msg_system);
			  }
             
              const tintaArrayBox * box = boxes->getFirstObj(indexNext);
			  while(box){
				  StringStream msg;  
				  msg<<_M("Box: ");				  	 			  
				  msg<<_M(" id: ");
				  msg<<indexNext;	

                  box = boxes->getNextObj(indexNext);
				 
                  Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msgLevel::msg_system);
			  }

			  //tintaTexSpringMain::getPtr()->reinitContext();


		  }
          
		  // system functions
		  void status(){
			  units();
			  tasks();
			  containers();
              fonts();
		  }


          // exit command
          void exit() {
              if (Tinta::tintaTexSpringMain::getPtr()->isServMode()) // TODO sends all 
                THREAD_SLEEP(3000);
              raise(SIGINT);
          }

          void abort( ) { //const String &name, m_ulong32 idUnit ){
			
			  // aborting only if local unit in the executing state
			  tintaExecutingUnit *unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit( NULL_M );			  
			  m_ulong32 id = unit ? unit->getId().mid : 0;
			  for( ;unit != NULL ; unit = tintaTexSpringMain::getPtr()->getUnitsSet()->getNextUnit( &id ) ){
                 
                 //StringStream msg;  
				 id = unit->getId().mid;	
                 if ( unit->getState() == tintaExecutingUnit::enBusy 
                     && unit->getType() == tintaExecutingUnit::enLocalUnit)
                          unit->setAbort();					    
			  }			  
		  }

          void cinit(){
                 Tinta::tintaTexSpringMain::getPtr()->reinitContext(Tinta::tintaTexSpringMain::getPtr()->getContext());
          }

                   

		  void ginit(){
			  Tinta::tintaTexSpringMain::getPtr()->initGPU();
		  }
                    
          int e(SCRIPT_STATE *L) {             

              if (Tinta::tintaTexSpringMain::getPtr()->isAborted()){ TROW_ERR_FUNC_CLEAR(L, "Command to abort......");	  }


              if ( !IS_VAL_STRING(L, 1 )){
                  TROW_ERR_FUNC_CLEAR(L, "Wrong buffer data!");
                 
              }

              String com = GET_VAL_STRING_EX(L, 1);             

              if ( IS_VAL_BOOL(L, 2) && GET_VAL_BOOL(L, 2) ) {
                  com = readData( com, L );
              }
              if( com.length() > 0 )
                tintaTexSpringMain::getPtr()->executeCommand(com, tintaExecutingTask::enCommonTask);             

              return 0;
          }

        /*
          int winupdate( SCRIPT_STATE *L ) {

              if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......");  }

              int argc = GET_QUANTITY(L); 
              const int arg_count_min = 1;  

              if ( argc != arg_count_min ) {
                  TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
                 
              }
              
              tintaIImgWindow* win = Tinta::tintaTexSpringMain::getPtr()->getWindow();

              if (win)
                  win->updateData(GET_VAL_UINT(L, 1));

              return 0;
          }

          int winsetimg(SCRIPT_STATE *L) {

              if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......");  }

              int argc = GET_QUANTITY(L);
              const int arg_count_min = 1;

              if (argc != arg_count_min) {
                  TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
                 
              }

              tintaIImgWindow* win = Tinta::tintaTexSpringMain::getPtr()->getWindow();

              if (win)
                  win->showImage(GET_VAL_UINT(L, 1));

              return 0;
          }


          int winhandler(SCRIPT_STATE *L) {
              if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......");  }

              int argc = GET_QUANTITY(L);
              const int arg_count_min = 2;

              if (argc != arg_count_min) {
                  TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
                  
              }

              if (!IS_VAL_STRING(L, 1)) {
                  TROW_ERR_FUNC_CLEAR(L, "Wrong action data!");
                 
              }

              if (!IS_VAL_STRING(L, 2)) {
                  TROW_ERR_FUNC_CLEAR(L, "Wrong buffer data!");
                 
              }


              tintaIImgWindow* win = Tinta::tintaTexSpringMain::getPtr()->getWindow();

              if (win)
                  win->addHandler( GET_VAL_STRING(L, 1), GET_VAL_STRING(L, 2) );

              return 0;
          }


          int windelhandlers(SCRIPT_STATE *L) {

              if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......");  }

              tintaIImgWindow* win = Tinta::tintaTexSpringMain::getPtr()->getWindow();

              if (win)
                  win->delHandlers();
              return 0;
          }

          */

          int el(SCRIPT_STATE *L) {

              if (Tinta::tintaTexSpringMain::getPtr()->isAborted()){ TROW_ERR_FUNC_CLEAR(L, "Command to abort......");	  }


              if (!IS_VAL_STRING(L, 1)){
                  TROW_ERR_FUNC_CLEAR(L, "Wrong buffer data!");
                 
              }

              String com = GET_VAL_STRING_EX(L, 1);

              if (IS_VAL_BOOL(L, 2) && GET_VAL_BOOL(L, 2)) {
                  com = readData(com, L);
              }
              if (com.length() > 0)
                tintaTexSpringMain::getPtr()->executeCommand(com, tintaExecutingTask::enLocalTask);

              return 0;
          }

          
          int ed(SCRIPT_STATE *L) {

              if (Tinta::tintaTexSpringMain::getPtr()->isAborted()){ TROW_ERR_FUNC_CLEAR(L, "Command to abort......");	 }


              if (!IS_VAL_STRING(L, 1)){
                  TROW_ERR_FUNC_CLEAR(L, "Wrong buffer data!");
               
              }

              String com = GET_VAL_STRING_EX(L, 1);

              if (IS_VAL_BOOL(L, 2) && GET_VAL_BOOL(L, 2)) {
                  com = readData(com, L);
              }
              if (com.length() > 0)
                tintaTexSpringMain::getPtr()->executeCommand(com, tintaExecutingTask::enDownTask);

              return 0;
          }

          
          int eu(SCRIPT_STATE *L){

              if (Tinta::tintaTexSpringMain::getPtr()->isAborted()){ TROW_ERR_FUNC_CLEAR(L, "Command to abort......");	 return 0; }


              if (!IS_VAL_STRING(L, 1)){
                  TROW_ERR_FUNC_CLEAR(L, "Wrong buffer data!");
                 
              }

              String com = GET_VAL_STRING_EX(L, 1);

              if (IS_VAL_BOOL(L, 2) && GET_VAL_BOOL(L, 2)) {
                  com = readData(com, L);
              }
              if (com.length() > 0)
                tintaTexSpringMain::getPtr()->executeCommand(com, tintaExecutingTask::enUpperTask);
               
              return 0;
          }

          
		 
         int executeshell(SCRIPT_STATE *L) {
             if (Tinta::tintaTexSpringMain::getPtr()->isAborted()){ TROW_ERR_FUNC_CLEAR(L, "Command to abort......");	 }

#if CORE_PLATFORM == CORE_PLATFORM_WIN32

             String com = GET_VAL_STRING_EX(L, 1);            
             com = Tinta::StringUtil::replaceAll(com, _M("/"), _M("\\"));

             if (!system(com.c_str()) == 0) {
                 TROW_ERR_FUNC_CLEAR(L, "Shell executing error!");
             }


#if 0
             SHELLEXECUTEINFO ExecuteInfo;
             memset(&ExecuteInfo, 0, sizeof(ExecuteInfo));

             ExecuteInfo.cbSize = sizeof(ExecuteInfo);
             ExecuteInfo.fMask = 0;
             ExecuteInfo.hwnd = 0;
             ExecuteInfo.lpVerb = _M("open");                      // Operation to perform
             ExecuteInfo.lpFile = _M("cmd");             // Application name
             ExecuteInfo.lpParameters = com.c_str();           // Additional parameters
             ExecuteInfo.lpDirectory = 0;                           // Default directory
             ExecuteInfo.nShow = SW_HIDE;
             ExecuteInfo.hInstApp = 0;  
             
             if ( ExecuteInfo.hProcess )
                 CloseHandle(ExecuteInfo.hProcess);

             if ( !ShellExecuteEx(&ExecuteInfo) == TRUE ) {

                 StringStreamBasic msg;
                 msg << "Shell command executed with errors. GetLastError(): ";
                 msg << GetLastError();
                 TROW_ERR_FUNC_CLEAR(L, msg.str() );                 
             }            

#endif

             
#else
             StringBasic com = GET_VAL_STRING(L, 1);

             if ( !system(com.c_str() ) == 0){
                 TROW_ERR_FUNC_CLEAR(L, "Shell executing error!");                 
             }
#endif
             return 0;
         }

         
         int regextoken(SCRIPT_STATE *L) {
             if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

#if         	CORE_COMPILER == CORE_COMPILER_MSVC
             String match = toWideChar(GET_VAL_STRING(L, 1));

             try {
                 regex_t regex(match);
             
                 String text = toWideChar(GET_VAL_STRING(L, 2));

                 using iter = std::regex_token_iterator < String::iterator > ;

                 iter rend;
                 std::vector<int> v = { 0 };
                 if ( IS_VAL_STRING(L, 3) ){
                     String tok = GET_VAL_STRING_EX(L, 3);
                     StringVector vstr = StringUtil::split( tok, { _M(",") } );
                     if ( vstr.size() > 0 )
                         v.clear();
                     for ( auto vtok: vstr ){
                         int param = std::stoi( vtok );                     
                         v.push_back( param );
                     }                 
                 }

                 m_uint32 mask = regex_constants::match_default; // def 0
                 if ( IS_VAL_INTEGER(L, 4) )
                     mask = GET_VAL_UINT(L, 4);

                 iter d( text.begin(), text.end(), regex, v, (regex_constants::match_flag_type)mask );

                 if ( d != rend){
                     tintaBoxContainer* boxes =Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
                     m_uint32 id = boxes->addObj(tintaStringValVector::str_strs_vector);

                     if (id == ZERO_ID) {
                         StringStreamBasic msg;
                         msg << "Buffer creating error: " << GET_VAL_STRING(L, 1);
                         TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());                     
                       
                     }

                     tintaArrayBox *box = boxes->getObjEx(id);


                     tintaArray2dString_t* arr = box->getStringBox();

                     while ( d != rend ) {
                         arr->pushBack( *d++ );
                     }

                     PUSH_UINT_ARG(L, id);

                     return 1;
                 }
             }
             catch (const std::exception & e){
                 StringStreamBasic msg;
                 msg << e.what();
                 TROW_ERR_FUNC_CLEAR( L, msg.str().c_str() );
             }
#endif
             return 0;
         }


         int regextokenex(SCRIPT_STATE *L) {
             if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

#if         	CORE_COMPILER == CORE_COMPILER_MSVC

             try {
                 regex_t regex(toWideChar(GET_VAL_STRING(L, 1)));


                 //String text = toWideChar(GET_VAL_STRING(L, 2));

                 m_uint32  id = GET_VAL_UINT(L, 2);

                 m_uint32  posVal = GET_VAL_UINT(L, 3);


                 tintaBoxContainer* boxes =Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
                 CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


                 const tintaArrayBox *box = boxes->getObjEx(id);

                 if (!box) {
                     StringStreamBasic msg;
                     msg << "Wrong box id: " << id;
                     TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                     
                 }

                 const tintaArray2dString_t* arr = box->getStringBox();

                 if (!arr || box->getType() != tintaArrayBox::enVectorBox){
                     StringStreamBasic msg;
                     msg << "Not supported function for this box type.";
                     TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                    
                 }

                 size_t pos = (size_t)posVal;

                 if (pos < arr->getSize()){
                     const String &text = arr->getValReal(pos);


                     using iter = std::regex_token_iterator < String::const_iterator > ;

                     iter rend;
                     std::vector<int> v = { 0 };
                     if (IS_VAL_STRING(L, 4)){
                         String tok = GET_VAL_STRING_EX(L, 4);
                         StringVector vstr = StringUtil::split(tok, { _M(",") });
                         if (vstr.size() > 0)
                             v.clear();
                         for (auto vtok : vstr){
                             int param = std::stoi(vtok);
                             v.push_back(param);
                         }
                     }

                     m_uint32 mask = regex_constants::match_default; // def 0
                     if (IS_VAL_INTEGER(L, 5))
                         mask = GET_VAL_UINT(L, 5);

                     iter d(text.begin(), text.end(), regex, v, (regex_constants::match_flag_type)mask);
                     

                     if (d != rend){
                         //tintaBoxContainer* boxes =Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
                         m_uint32 idRez = boxes->addObj(tintaStringValVector::str_strs_vector);

                         if (idRez == ZERO_ID) {
                             StringStreamBasic msg;
                             msg << "Buffer creating error: " << tintaStringValVector::str_strs_vector;
                             TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                            
                         }

                         tintaArrayBox *boxRez = boxes->getObjEx(idRez);
                         tintaArray2dString_t* arrRez = boxRez->getStringBox();

                         while (d != rend) {
                             arrRez->pushBack(*d++);
                         }

                         PUSH_UINT_ARG(L, idRez);

                         return 1;
                     }
                 }
                 else {
                     StringStreamBasic msg;
                     msg << "Out of boundary for index: ";
                     msg << posVal;
                     TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                 }
             }
             catch (std::exception & e){
                 StringStreamBasic msg;
                 msg << e.what();
                 TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());                 
             }
             
#endif
             return 0;
         }

         int regexmatch(SCRIPT_STATE *L) {
             if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

#if         	CORE_COMPILER == CORE_COMPILER_MSVC
             try {
                 regex_t regex(toWideChar(GET_VAL_STRING(L, 1)));

                 m_uint32  id = GET_VAL_UINT(L, 2);

                 m_uint32  posVal = GET_VAL_UINT(L, 3);

                 tintaBoxContainer* boxes =Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
                 const tintaArrayBox *box = boxes->getObjEx(id);
                 if (!box) {
                     StringStreamBasic msg;
                     msg << "Wrong box id: " << id;
                     TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                    
                 }

                 const tintaArray2dString_t* arr = box->getStringBox();

                 if (!arr || box->getType() != tintaArrayBox::enVectorBox) {
                     StringStreamBasic msg;
                     msg << "Not supported function for this box type.";
                     TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                     
                 }

                 size_t pos = (size_t)posVal;

                 if ( pos < arr->getSize()) {
                     const String &text = arr->getValReal(pos);

                     m_uint32 mask = regex_constants::match_default; // def 0
                     if ( IS_VAL_INTEGER(L, 4) )
                         mask = GET_VAL_UINT(L, 4);

                     int rez = std::regex_match(text, regex, (regex_constants::match_flag_type)mask);

                     PUSH_INT_ARG(L, rez);
                     return 1;
                 }
             }
             catch (...) {
                 if (Tinta::tintaLogger::getPtr())
                     Tinta::tintaLogger::getPtr()->logMsg(_M("Regex error!"));
             }
#endif
             return 0;
         }

         // system functions
         int tasks(SCRIPT_STATE *L) {
             PUSH_UINT_ARG(L, (unsigned)Tinta::tintaTexSpringMain::getPtr()->getTaskQueue()->getSize());
             return 1;
         }


         /*
         _CoreExport int wincursor(SCRIPT_STATE *L) {
             if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

             const tintaIImgWindow* wPtr = tintaTexSpringMain::getPtr()->getWindow();

             if ( wPtr ) {
                 tintaVector2i pos = wPtr->mouseWinPos();
                 PUSH_INT_ARG(L, pos.mx );
                 PUSH_INT_ARG(L, pos.my );
                 return 2;
             }
             return 0;

         }
       

         _CoreExport int cursorimg(SCRIPT_STATE *L) {
             if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

             const tintaIImgWindow* wPtr = tintaTexSpringMain::getPtr()->getWindow();

             if (wPtr) {
                 tintaVector2i pos = wPtr->mouseImgPos();
                 PUSH_INT_ARG(L, pos.mx);
                 PUSH_INT_ARG(L, pos.my);
                 return 2;
             }
             return 0;

         }
         
                  
         _CoreExport int keystate(SCRIPT_STATE *L) {
             if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

             const tintaIImgWindow* wPtr =  tintaTexSpringMain::getPtr()->getWindow();

             if ( wPtr ) {
                 bool valid = false;
                 
                 int  key = GET_VAL_INT(L, 1);

                 bool pressed = wPtr->keyState(key, valid );
                 //cout << key << " code state " << pressed<<"\n";
                 if ( valid ) {                  

                     StringStreamBasic msg;
                     msg << u8"Wrong Key code" << key;
                     TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                    
                 }

                 PUSH_BOOL_ARG(L, pressed );
                 return 1;
             }
             return 0;
         }
         */         

         static const luaL_Reg mainFunc[] = {
             { "lassert", Tinta::tintaMainFunctions::lassert },
             { "f", Tinta::tintaMainFunctions::f },
             { "fd", Tinta::tintaMainFunctions::fd },
             { "fl", Tinta::tintaMainFunctions::fl },
             { "fu", Tinta::tintaMainFunctions::fu },             
             { "getangle", Tinta::tintaMainFunctions::getangle },
             { "start", Tinta::tintaMainFunctions::start },
             { "stop", Tinta::tintaMainFunctions::stop },
             { "rotpoint", Tinta::tintaMainFunctions::rotpoint },
             { "setrandseed", Tinta::tintaMainFunctions::setrandseed },
             { "randint", Tinta::tintaMainFunctions::randint },
             { "randintex", Tinta::tintaMainFunctions::randintex },
             { "randfloat", Tinta::tintaMainFunctions::randfloat },
             { "calchash", Tinta::tintaMainFunctions::calchash },
             { "getname", Tinta::tintaMainFunctions::getname },
             { "uniqid", Tinta::tintaMainFunctions::uniqid },
             { "getseed", Tinta::tintaMainFunctions::getseed },
             { "readfile", Tinta::tintaMainFunctions::readfile },
             { "writetofile", Tinta::tintaMainFunctions::writetofile },
             { "isserver", Tinta::tintaMainFunctions::isserver },
             { "e", Tinta::tintaMainFunctions::e },
             { "el", Tinta::tintaMainFunctions::el },
             { "ed", Tinta::tintaMainFunctions::ed },
             { "eu", Tinta::tintaMainFunctions::eu },
             { "exit", Tinta::tintaMainFunctions::exit },
             { "tasks", Tinta::tintaMainFunctions::tasks },                          
             { "executeshell", Tinta::tintaMainFunctions::executeshell },
             { "regextoken", Tinta::tintaMainFunctions::regextoken },
             { "regextokenex", Tinta::tintaMainFunctions::regextokenex },
             { "regexmatch", Tinta::tintaMainFunctions::regexmatch },
             { NULL, NULL } };

         int luaopen_Main(lua_State *L) {
             luaL_newlib(L, mainFunc);
             return 1;
         }

         static const luaL_Reg font[] = {
             { "add", Tinta::tintaFontFunctions::add },
             { "copy", Tinta::tintaFontFunctions::copy },
             { "select", Tinta::tintaFontFunctions::select },
             { "setcolor", Tinta::tintaFontFunctions::setcolor },
             { "render", Tinta::tintaFontFunctions::render },
             { "erase", Tinta::tintaFontFunctions::erase },
             { "eraseall", Tinta::tintaFontFunctions::eraseall },
             { "render", Tinta::tintaFontFunctions::render },             
             { NULL, NULL } };

         int luaopen_Font(lua_State *L) {
             luaL_newlib(L, font);
             return 1;
         }


		 static const luaL_Reg ocl[] = {
			 { "platdescrip", Tinta::tintaOCLFunctions::platdescrip },
			 { "devdescrip", Tinta::tintaOCLFunctions::devdescrip },
			 { "platforms", Tinta::tintaOCLFunctions::platforms },
             { "programdescrip", Tinta::tintaOCLFunctions::programdescrip },             
			 { "gpuenabled", Tinta::tintaOCLFunctions::enabled },
			 { "clear", Tinta::tintaOCLFunctions::clear },
             { "create", Tinta::tintaOCLFunctions::create },
             { "exist", Tinta::tintaOCLFunctions::exist },			
			 { "setarray", Tinta::tintaOCLFunctions::setarray },			
             { "setimage", Tinta::tintaOCLFunctions::setimage },
			 { "setvalue", Tinta::tintaOCLFunctions::setvalue },
			 { "execute", Tinta::tintaOCLFunctions::execute },
			 { NULL, NULL } };

		 int luaopen_Ocl(lua_State *L) {
			 luaL_newlib(L, ocl);
			 return 1;
		 }
		
         static const luaL_Reg imageLua[] = {            
             { "clearstains", Tinta::TintaImageLua::clearstains },
             { "getcolors", Tinta::TintaImageLua::getcolors },
             { "priorcolor", Tinta::TintaImageLua::priorcolor },
             { "poster", Tinta::TintaImageLua::poster },
             { "setpixelb", Tinta::TintaImageLua::setpixelb },
             { "setpixelf", Tinta::TintaImageLua::setpixelf },
             { "selected", Tinta::TintaImageLua::selected },             
             { "select", Tinta::TintaImageLua::select },             
             { "save", Tinta::TintaImageLua::save },
             { "width", Tinta::TintaImageLua::width },
             { "height", Tinta::TintaImageLua::height },
             { "getpixelb", Tinta::TintaImageLua::getpixelb },
             { "getpixelf", Tinta::TintaImageLua::getpixelf },
             { "getindpixelb", Tinta::TintaImageLua::getindpixelb },
             { "getindpixelf", Tinta::TintaImageLua::getindpixelf },
             { "fillb", Tinta::TintaImageLua::fillb },
             { "fillf", Tinta::TintaImageLua::fillf },
             { "create", Tinta::TintaImageLua::create },
             { "read", Tinta::TintaImageLua::read },
             { "channels", Tinta::TintaImageLua::channels },             
             { "fillchannelf", Tinta::TintaImageLua::fillchannelf },
             { "setchannelf", Tinta::TintaImageLua::setchannelf },
             { "setchannelb", Tinta::TintaImageLua::setchannelb },
             { "mix", Tinta::TintaImageLua::mix },
             { "mixchannel", Tinta::TintaImageLua::mixchannel },
             { "erase", Tinta::TintaImageLua::erase },
             { "eraseall", Tinta::TintaImageLua::eraseall },
             { "fillchannelb", Tinta::TintaImageLua::fillchannelb },             
             { "images", Tinta::TintaImageLua::images },                          
             { NULL, NULL } };

         int luaopen_Image(lua_State *L) {
             luaL_newlib(L, imageLua);
             return 1;
         }


         static const luaL_Reg boxLua[] = {
             { "create", Tinta::TintaBoxLua::create },
             { "erase", Tinta::TintaBoxLua::erase },
             { "eraseall", Tinta::TintaBoxLua::eraseall },
             { "getvalf", Tinta::TintaBoxLua::getvalf },
             { "setvalf", Tinta::TintaBoxLua::setvalf },
             { "setval3f", Tinta::TintaBoxLua::setval3f },
             { "getvali16", Tinta::TintaBoxLua::getvali16 },
             { "setvali16", Tinta::TintaBoxLua::setvali16 },
             { "getvali32", Tinta::TintaBoxLua::getvali32 },
             { "getvalui8", Tinta::TintaBoxLua::getvalui8 },
             { "getval3f", Tinta::TintaBoxLua::getval3f },
             { "getvals", Tinta::TintaBoxLua::getvals },
             { "setvals", Tinta::TintaBoxLua::setvals },
             { "setvali32", Tinta::TintaBoxLua::setvali32 },
             { "setvalui8", Tinta::TintaBoxLua::setvalui8 },            
             { "pushbacki16", Tinta::TintaBoxLua::pushbacki16 },
             { "pushbacks", Tinta::TintaBoxLua::pushbacks },
             { "pushbacki32", Tinta::TintaBoxLua::pushbacki32 },
             { "pushbackui8", Tinta::TintaBoxLua::pushbackui8 },
             { "pushbackf", Tinta::TintaBoxLua::pushbackf },
             { "pushback3f", Tinta::TintaBoxLua::pushback3f },
             { "popbackf", Tinta::TintaBoxLua::popbackf },
             { "popbacks", Tinta::TintaBoxLua::popbacks },
             { "popback3f", Tinta::TintaBoxLua::popback3f },
             { "popbacki32", Tinta::TintaBoxLua::popbacki32 },
             { "popbackui8", Tinta::TintaBoxLua::popbackui8 },             
             { "popbacki16", Tinta::TintaBoxLua::popbacki16 },
             { "elements", Tinta::TintaBoxLua::elements },
             { "clear", Tinta::TintaBoxLua::clear },
             { "boxes", Tinta::TintaBoxLua::boxes },
             { "resize", Tinta::TintaBoxLua::resize },
             { NULL, NULL } };

         int luaopen_Box(lua_State *L) {
             luaL_newlib(L, boxLua);
             return 1;
         }


         static const luaL_Reg loadedTintalibs[] = {
             { "_G", luaopen_base },
             { "image", luaopen_Image },
             { "ocl", luaopen_Ocl },
             { "box", luaopen_Box },
             { "font", luaopen_Font },
             { "main", luaopen_Main },             
             { NULL, NULL } };

         void registerEngineLua(SCRIPT_STATE *L) {

             const luaL_Reg *lib;
             /* "require" functions from 'loadedlibs' and set results to global table */
             for (lib = loadedTintalibs; lib->func; lib++) {
                 luaL_requiref(L, lib->name, lib->func, 1);
                 lua_pop(L, 1);  /* remove lib */
             }
         }

	}

	namespace tintaOCLFunctions {
		/*
		    Returns GPU description string
		*/
		int platdescrip(SCRIPT_STATE *L){
			if (Tinta::tintaTexSpringMain::getPtr()->isAborted()){ TROW_ERR_FUNC(L, "Command to abort......");	 }

            if ( !Tinta::tintaTexSpringMain::getPtr()->gpuEnabled())
                return 0;			

			tintaGPUExt *ext = tintaGPUExt::getPtr();

			String result = ext->getPlatformInfo();
            String err = ext->getError();
            if ( err.length() > 0 ) {                
                TROW_ERR_FUNC(L, err.c_str());
            }

			PUSH_STRING_ARG(L, result.c_str()); //	

			return 1;
		}


		int devdescrip(SCRIPT_STATE *L){
			if (Tinta::tintaTexSpringMain::getPtr()->isAborted()){ TROW_ERR_FUNC(L, "Command to abort......");}

            if (!Tinta::tintaTexSpringMain::getPtr()->gpuEnabled())
                return 0;

			m_uint32 platId = GET_VAL_UINT(L, 1);

			tintaGPUExt *ext = tintaGPUExt::getPtr();

            String result;
            if ( IS_VAL_INTEGER(L, 2 ) ) {

                m_uint32 deviceId = GET_VAL_UINT(L, 2);
                result  = ext->getDeviceInfo(platId, &deviceId);
            }
            else
                result = ext->getDeviceInfo(platId, NULL_M );

            String err = ext->getError();
            if (err.length() > 0) {
                TROW_ERR_FUNC(L, err.c_str());
            }

			PUSH_STRING_ARG(L, result.c_str()); //	

			return 1;
		}


        int programdescrip(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC(L, "Command to abort......"); }

            if (!Tinta::tintaTexSpringMain::getPtr()->gpuEnabled())
                return 0;

            m_uint32 platId = GET_VAL_UINT(L, 1);

            tintaGPUExt *ext = tintaGPUExt::getPtr();

            String result;

            String program = GET_VAL_STRING(L, 2);
            
            tintaIClBase* clProg = Tinta::tintaTexSpringMain::getPtr()->getGPUProgram(program);
            if (!clProg) {
                StringStream msg;
                msg << _M("Wrong program name: ") << program;
                TROW_ERR_FUNC(L, msg.str().c_str());
            }
            void *kernel = clProg->getKernel();
            if (!kernel) {
                StringStream msg;
                msg << _M("Kernel not created");
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            if( kernel )
                result = ext->getDeviceInfo( platId, NULL_M, kernel);

            String err = ext->getError();
            if ( err.length() > 0 ) {
                TROW_ERR_FUNC(L, err.c_str());
            }

            PUSH_STRING_ARG(L, result.c_str()); //	

            return 1;
        }


       

		/*
			Returns GPU platforms quantity
		*/
		int platforms(SCRIPT_STATE *L){
			if (Tinta::tintaTexSpringMain::getPtr()->isAborted()){ TROW_ERR_FUNC(L, "Command to abort......");	 }

            if (!Tinta::tintaTexSpringMain::getPtr()->gpuEnabled())
                return 0;

			tintaGPUExt *ext = tintaGPUExt::getPtr();

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
			if (Tinta::tintaTexSpringMain::getPtr()->gpuEnabled()){
#ifdef USING_GPUCL
				PUSH_BOOL_ARG(L, true); //	
				return 1;
#endif
			}
			PUSH_BOOL_ARG(L, false); //	
			return 1;
		}

		int clear(SCRIPT_STATE *L) {
			if ( Tinta::tintaTexSpringMain::getPtr()->gpuEnabled() ){

				String program = GET_VAL_STRING_EX(L, 1);

                if ( program.length() != 0 ) {
                    tintaIClBase* clProg = Tinta::tintaTexSpringMain::getPtr()->getGPUProgram(program);
                    if (clProg)
                        clProg->clearData();
                    else {
                        StringStream msg;
                        msg << _M("Wrong program name: ") << program;
                        TROW_ERR_FUNC(L, msg.str().c_str());

                    }
                }                
                else {
                    StringStream msg;
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
            if (Tinta::tintaTexSpringMain::getPtr()->gpuEnabled()) {

                String program = GET_VAL_STRING_EX(L, 1);

                
                   tintaIClBase* clProg = Tinta::tintaTexSpringMain::getPtr()->getGPUProgram(program);
                   if (clProg) {
                       PUSH_BOOL_ARG(L, true);
                       return 1;
                   }                                 
            }
            return 0;
        }

        int create( SCRIPT_STATE *L ) {

            if ( Tinta::tintaTexSpringMain::getPtr()->gpuEnabled() ) {

                String program = GET_VAL_STRING_EX(L, 1);

                String kernel = GET_VAL_STRING_EX(L, 2);

                if ( program.length() == 0 ) {
                    TROW_ERR_FUNC(L, _M("Wrong program name"));
                    return 0;
                }

                if ( kernel.length() == 0 ) {
                    TROW_ERR_FUNC(L, _M("Wrong kernel name"));
                    return 0;
                }

               
                try {
                    if ( !Tinta::tintaTexSpringMain::getPtr()->registerCl(program, kernel) ) {
                        StringStream msg;
                        msg << _M("Wrong path for program: ") << program;                        
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                catch( const std::exception &e ){
                    StringStream msg;
                    msg << _M("Error while register gpu program ");                    
                    msg << _M("Exception: ") << e.what();
                    TROW_ERR_FUNC(L, msg.str().c_str());
                }
               
            }
            return 0;
        }
        	
		int setarray(SCRIPT_STATE *L) {
			
            
			String progName = GET_VAL_STRING_EX(L, 1);
			tintaIClBase* prog = Tinta::tintaTexSpringMain::getPtr()->getGPUProgram(
				progName );

			if (!prog){
				StringStream msg;
				msg << _M("Wrong program name: ") << progName;
                TROW_ERR_FUNC(L, msg.str().c_str());
				return 0;
			}

			tintaBoxContainer* boxes =Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
			m_uint32 idBox = GET_VAL_UINT(L, 2);
			m_uint32 memMask = GET_VAL_UINT(L, 3);
			tintaArrayBox *box = boxes->getObjEx(idBox);
			if ( !box ){
				StringStream msg;
				msg << _M("Wrong box name: ") << box;
                TROW_ERR_FUNC(L, msg.str().c_str());
				
			}

			void * ptrData = box->GetMemPtr();

            try {

                if( ptrData )
                    prog->setDataIn(ptrData, GpuArg_t::enArray, box->GetDataSize(), memMask);

            }
            catch (const std::exception &e) {
                StringStream msg;
                msg << _M("Wrong box data or value: ") << box;
                msg << _M("Exception: ") << e.what();
                TROW_ERR_FUNC(L, msg.str().c_str());
            }	

            return 0;
		}


        int setimage(SCRIPT_STATE *L) {


            String progName = GET_VAL_STRING_EX(L, 1);
            tintaIClBase* prog = Tinta::tintaTexSpringMain::getPtr()->getGPUProgram(
                progName);

            if ( !prog ) {
                StringStream msg;
                msg << _M("Wrong program name: ") << progName;
                TROW_ERR_FUNC(L, msg.str().c_str());
                return 0;
            }
           
            m_uint32 imgid = GET_VAL_UINT(L, 2);
            m_uint32 memMask = GET_VAL_UINT(L, 3);

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImageEx( imgid );

            if ( !pImage ) {
                StringStream msg;
                msg << _M("Wrong image name: ") << imgid;
                TROW_ERR_FUNC( L, msg.str().c_str());
            }

            void * ptrData = pImage->getMemPtrEx();
            
            //auto v1 = pImage->getMemSize();
            //auto v2 = sizeof(tintaPixel32b);

            try {
                if ( ptrData )
                    prog->setDataIn(ptrData, GpuArg_t::enArray, pImage->getMemSize(), memMask);  
            }
            catch (const std::exception &e) {
                StringStream msg;
                msg << _M("Wrong image data or value: ") << imgid;
                msg << _M("Exception: ") << e.what();
                TROW_ERR_FUNC(L, msg.str().c_str());
            }

            return 0;
           
        }


				
		int setvalue(SCRIPT_STATE *L) {         

			String progName = GET_VAL_STRING_EX(L, 1);
			tintaIClBase* prog = Tinta::tintaTexSpringMain::getPtr()->getGPUProgram(
				progName);

			if (!prog){
				StringStream msg;
				msg << _M("Wrong program name: ") << progName;
                TROW_ERR_FUNC(L, msg.str().c_str());
				
			}
						
			
            if ( IS_VAL_STRING(L, 3) ) {

                String type = GET_VAL_STRING(L, 3);
                if ( type == "int8" ) {

                    m_int8  chVal = (m_int8)GET_VAL_INT(L, 2);
                    try {
                        prog->setDataIn((void*)&chVal, GpuArg_t::enValue, sizeof(chVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << chVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if ( type == "uint8" ) {

                    m_uint8  btVal = (m_uint8)GET_VAL_UINT(L, 2);

                    try {
                        prog->setDataIn((void*)&btVal, GpuArg_t::enValue, sizeof(btVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << btVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if (type == "bool") {

                    bool  bVal = GET_VAL_BOOL(L, 2);

                    try {
                        prog->setDataIn((void*)&bVal, GpuArg_t::enValue, sizeof(bVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << bVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if (type == "uint16") {

                    m_uint16 ushVal = (m_uint16)GET_VAL_UINT(L, 2);

                    try {
                        prog->setDataIn((void*)&ushVal, GpuArg_t::enValue, sizeof(ushVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << ushVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if (type == "int16") {

                    m_int16 shVal = (m_int16)GET_VAL_INT(L, 2);

                    try {
                        prog->setDataIn((void*)&shVal, GpuArg_t::enValue, sizeof(shVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << shVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if (type == "uint32") {

                    unsigned int uiVal = GET_VAL_UINT(L, 2);

                    try {
                        prog->setDataIn((void*)&uiVal, GpuArg_t::enValue, sizeof(uiVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << uiVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if (type == "int32") {

                    int iVal = GET_VAL_INT(L, 2);

                    try {
                        prog->setDataIn((void*)&iVal, GpuArg_t::enValue, sizeof(iVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << iVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if (type == "int64") {

                    m_int64 iVal = (m_int64)GET_VAL_INT(L, 2);

                    try {
                        prog->setDataIn((void*)&iVal, GpuArg_t::enValue, sizeof(iVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << iVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if (type == "uint64") {

                    m_uint64 iVal = (m_uint64)GET_VAL_UINT(L, 2);

                    try {
                        prog->setDataIn((void*)&iVal, GpuArg_t::enValue, sizeof(iVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << iVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if (type == "float") {

                    float fVal = GET_VAL_FLOAT(L, 2);

                    try {
                        prog->setDataIn((void*)&fVal, GpuArg_t::enValue, sizeof(fVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << fVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }                      

            }
            else {
                if (IS_VAL_INTEGER(L, 2)) {
                    int   iVal = GET_VAL_INT(L, 2);

                    try {
                        prog->setDataIn((void*)&iVal, GpuArg_t::enValue, sizeof(iVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << iVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
                else if ( IS_VAL_REAL(L, 2) ) {

                    float fVal = GET_VAL_FLOAT(L, 2);

                    try {
                        prog->setDataIn((void*)&fVal, GpuArg_t::enValue, sizeof(fVal), enRead);
                    }
                    catch (const std::exception &e) {
                        StringStream msg;
                        msg << _M("Wrong value: ") << fVal;
                        msg << _M("Excption: ") << e.what();
                        TROW_ERR_FUNC(L, msg.str().c_str());
                    }
                }
            }
		
            return 0;
		}
		int execute(SCRIPT_STATE *L){			
			
#ifdef USING_GPUCL
			String progName = GET_VAL_STRING_EX(L, 1);
			tintaIClBase* prog = Tinta::tintaTexSpringMain::getPtr()->getGPUProgram(
				progName);

			if (!prog){
				StringStream msg;
				msg << _M("Wrong program name: ") << progName;
                TROW_ERR_FUNC(L, msg.str().c_str());
				
			}
				
			m_uint32 width = GET_VAL_UINT(L, 2);
            m_uint32 height = 0;
            if(IS_VAL_INTEGER(L, 3))
                height =  GET_VAL_UINT(L, 3);

            m_uint32 depth = 0; 
            if (IS_VAL_INTEGER(L, 4))
                depth = GET_VAL_UINT(L, 4);

			m_uint32 paramRead = GET_VAL_UINT(L, 5);

			if ( height > 0 ){ // two dimensions
                m_uint32 globalsize[2] = { std::max(width, 1U), std::max(height, 1U) };

                m_uint32 readpos[1] = { paramRead };

                m_uint32 groupSize[2] = { 0, 0 };

                if ( IS_VAL_INTEGER(L, 6) && IS_VAL_INTEGER(L, 7) ) {

                    groupSize[0] = GET_VAL_UINT(L, 6);
                    groupSize[1] = GET_VAL_UINT(L, 7);
                }

				//int maxLen = 0;

				try {
					prog->execute(globalsize, sizeof(globalsize) / sizeof(size_t), 
                                        readpos, sizeof(readpos) / sizeof(size_t), (groupSize[0] == 0 || groupSize[1] == 0) ? NULL : groupSize);
				}
				catch ( const tintaException &e ){
					StringStreamBasic msg;
					msg << "GPU parameters error for program: " << progName;
                    msg << "Exception: " << e.what();
                    prog->clearData();
                    TROW_ERR_FUNC(L, msg.str().c_str());

				}
			}
            else if (depth > 0) { // three dimensions
                m_uint32 globalsize[3] = { std::max(width, 1U), std::max(height, 1U),std::max(depth, 1U) };

                m_uint32 readpos[1] = { paramRead };

                m_uint32 groupSize[3] = { 0, 0, 0 };

                if (IS_VAL_INTEGER(L, 6) && IS_VAL_INTEGER(L, 7) && IS_VAL_INTEGER(L, 8)) {

                    groupSize[0] = GET_VAL_UINT(L, 6);
                    groupSize[1] = GET_VAL_UINT(L, 7);
                    groupSize[2] = GET_VAL_UINT(L, 8);
                }

                //int maxLen = 0;

                try {
                    prog->execute(globalsize, sizeof(globalsize) / sizeof(size_t),
                        readpos, sizeof(readpos) / sizeof(size_t), (groupSize[0] == 0 || groupSize[1] == 0 || groupSize[2] == 0) ? NULL : groupSize);
                }
                catch (const tintaException &e) {
                    StringStreamBasic msg;
                    msg << "GPU parameters error for program: " << progName;
                    msg << "Exception: " << e.what();
                    prog->clearData();
                    TROW_ERR_FUNC(L, msg.str().c_str());

                }
            }
			else { // one dimension
				size_t globalsize[1] = { (size_t)(TintaMath::max( width, 1 ) * TintaMath::max (height, 1 )) };

				size_t readpos[1] = { paramRead };

                m_uint32 groupSize[1] = { 0 };

                if ( IS_VAL_INTEGER(L, 6) ) {

                    groupSize[0] = GET_VAL_UINT(L, 6);                    
                }

				//int maxLen = 0;

				try {
					prog->execute(globalsize, sizeof(globalsize) / sizeof(size_t), readpos, sizeof(readpos) / sizeof(size_t), groupSize[0] == 0 ? NULL : groupSize);
				}
				catch ( const tintaException &e ){
					StringStreamBasic msg;
					msg << "GPU parameters error for program: " << progName;
                    msg << "Exception: " << e.what();
                    prog->clearData();
                    TROW_ERR_FUNC(L, msg.str().c_str());

				}
			}          
#endif
			return 0;
		}
        
	}


    namespace TintaImageLua {

        int clearstains(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            m_uint32 quantity = GET_VAL_UINT(L, 1);

            float threshold = GET_VAL_FLOAT(L, 2);

            if (threshold < 0.f || threshold > 1.f) {
                TROW_ERR_FUNC_CLEAR(L, "Threshold value should be in range [0 1]");

            }

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();


            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            image_t imgOut(pImage->getWidth(), pImage->getHeight());

            int w = (int)pImage->getWidth();
            int h = (int)pImage->getHeight();

            imgOut.setChannel(color_type::channel_a, 0);
            //  std::cout << quantity << " " << threshold<< w << " " << h <<"\n";
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {

                    color_type p = imgOut.getPixel({ x,y });
                    if (p.getVal(color_type::channel_a) < 255) {

                        imageAlgorithm::colors_t around;
                        imageAlgorithm::coordinates_t walked;
                        walked.reserve(quantity);

                        pixelWalker(pImage, { x, y }, around, walked, threshold, quantity);
                        if (walked.size() <= quantity) {

                            imageAlgorithm::colors_t::iterator iColor = std::max_element(around.begin(), around.end(), [](const imageAlgorithm::ColorPrior &lv,
                                const imageAlgorithm::ColorPrior &rv) {
                                return lv.count < rv.count;
                            });

                            if (iColor != around.end()) {
                                for (const coord2dI_t& i : walked) {

                                    color_type colorNew = iColor->color;

                                    colorNew.setVal(color_type::channel_a, 255);
                                    imgOut.setPixel(i, colorNew);
                                }
                            }
                        }
                        else {
                            imgOut.setPixel({ x,y }, pImage->getPixel({ x,y }));
                        }
                    }
                }
            }

            size_t c = pImage->getSize();

            for (size_t i = 0; i < c; i++) {
                m_uint8 a = pImage->getChannel(i, 3);
                imgOut.setPixelAlpha(i, a);
            }

            *pImage = imgOut;           

            return 0;
        }



        int getcolors(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            m_uint32 quantity = GET_VAL_UINT(L, 1);
            float threshold = GET_VAL_FLOAT(L, 2);

            if (threshold < 0.f || threshold > 1.f) {
                TROW_ERR_FUNC_CLEAR(L, "Threshold value should be in range [0 1]");

            }



            std::vector<imageAlgorithm::ColorPrior> colors;
            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            size_t q = pImage->getSize();

            std::vector<imageAlgorithm::ColorPrior> colorsFinal;

            for (size_t pos = 0; pos < q; pos++) {

                color_type c = pImage->getPixel(pos);
                std::vector<imageAlgorithm::ColorPrior>::iterator iAdd = std::find_if(colorsFinal.begin(), colorsFinal.end(), [&](auto &val) {

                    float dist = byteToFloat((m_int8)Tinta::distance(Tinta::tintaVector3i(c.getVal(color_type::channel_r), c.getVal(color_type::channel_g), c.getVal(color_type::channel_b)), tintaVector3i{ val.color.getVal(color_type::channel_r), val.color.getVal(color_type::channel_g) , val.color.getVal(color_type::channel_b) }));

                    return dist <= threshold;
                });

                if (iAdd == colorsFinal.end())
                    colorsFinal.push_back({ 1, c, });
            }

            m_uint32 retId = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->addImg("basic");
            pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImageEx(retId);

            if (quantity < colorsFinal.size())
                colorsFinal.resize(quantity);

            if (colorsFinal.size() == 0) {
                TROW_ERR_FUNC_CLEAR(L, "Pallete image has 0 length");


            }

            if (pImage->createImage(colorsFinal.size(), 1)) {

                m_uint32 index = 0;
                for (auto i : colorsFinal) {
                    pImage->setPixel(index, { i.color });
                    index++;
                }
                PUSH_UINT_ARG(L, retId);
                return 1;
            }

            return 0;

        }




        int priorcolor(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int threshold = GET_VAL_INT(L, 1);

            float colorFactor = GET_VAL_FLOAT(L, 2);
            if (colorFactor < 0.f || colorFactor > 1.f) {
                TROW_ERR_FUNC_CLEAR(L, "Coefficient of equivalence should be between 0.0 and 1.0");

            }

            int passes = 1;
            if (IS_VAL_INTEGER(L, 3)) {
                passes = GET_VAL_INT(L, 3);
            }

            bool withAlpha = false;
            if (IS_VAL_BOOL(L, 4)) {
                withAlpha = GET_VAL_BOOL(L, 4);
            }

            struct ColorStat {
                m_uint32 color;
                int stat;
            };

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();


            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            int w = (int)pImage->getWidth();
            int h = (int)pImage->getHeight();



            // GET_VAL_BYTE(L, 4)

            for (int p = 0; p < passes; p++) {

                image_t imgOut(pImage->getWidth(), pImage->getHeight());

                for (int y = 0; y < h; y++) {

                    for (int x = 0; x < w; x++) {


                        std::vector<ColorStat> statValues;
                        color_type curPixel = pImage->getPixel({ x, y });
                        auto g = curPixel.getVal(1);
                        // m_uint32 packedCur = packColor4(curPixel->getVal(color_type::channel_r), curPixel->getVal(color_type::channel_g), curPixel->getVal(color_type::channel_b), curPixel->getVal(color_type::channel_a) );
                        {
                            color_type left = pImage->getPixel({ x - 1, y });
                            if (pImage->validPos({ x - 1, y })) {
                                m_uint32 color = packColor4(left.getVal(color_type::channel_r), left.getVal(color_type::channel_g), left.getVal(color_type::channel_b), left.getVal(color_type::channel_a));
                                // if ( color != packedCur ) {
                                std::vector<ColorStat>::iterator i = std::find_if(statValues.begin(), statValues.end(), [color](const ColorStat &v) {return v.color == color; });
                                if (i == statValues.end())
                                    statValues.push_back({ color, 1 });
                                else
                                    i->stat++;
                                // }
                            }
                        }
                        {
                            color_type leftUp = pImage->getPixel({ x - 1, y - 1 });

                            if (pImage->validPos({ x - 1, y - 1 })) {
                                m_uint32 color = packColor4(leftUp.getVal(color_type::channel_r), leftUp.getVal(color_type::channel_g), leftUp.getVal(color_type::channel_b), leftUp.getVal(color_type::channel_a));
                                // if ( color != packedCur ) {
                                std::vector<ColorStat>::iterator i = std::find_if(statValues.begin(), statValues.end(), [color](const ColorStat &v) {return v.color == color; });
                                if (i == statValues.end())
                                    statValues.push_back({ color, 1 });
                                else
                                    i->stat++;
                                //}
                            }
                        }

                        {
                            color_type up = pImage->getPixel({ x, y - 1 });

                            if (pImage->validPos({ x, y - 1 })) {
                                m_uint32 color = packColor4(up.getVal(color_type::channel_r), up.getVal(color_type::channel_g), up.getVal(color_type::channel_b), up.getVal(color_type::channel_a));
                                // if (color != packedCur) {
                                std::vector<ColorStat>::iterator i = std::find_if(statValues.begin(), statValues.end(), [color](const ColorStat &v) {return v.color == color; });
                                if (i == statValues.end())
                                    statValues.push_back({ color, 1 });
                                else
                                    i->stat++;
                                //}
                            }
                        }
                        {
                            color_type rightUp = pImage->getPixel({ x + 1, y - 1 });
                            if (pImage->validPos({ x + 1, y - 1 })) {
                                m_uint32 color = packColor4(rightUp.getVal(color_type::channel_r), rightUp.getVal(color_type::channel_g), rightUp.getVal(color_type::channel_b), rightUp.getVal(color_type::channel_a));
                                //   if ( color != packedCur ) {
                                std::vector<ColorStat>::iterator i = std::find_if(statValues.begin(), statValues.end(), [color](const ColorStat &v) {return v.color == color; });
                                if (i == statValues.end())
                                    statValues.push_back({ color, 1 });
                                else
                                    i->stat++;
                                //  }
                            }
                        }
                        {
                            color_type right = pImage->getPixel({ x + 1, y });
                            if (pImage->validPos({ x + 1, y })) {
                                m_uint32 color = packColor4(right.getVal(color_type::channel_r), right.getVal(color_type::channel_g), right.getVal(color_type::channel_b), right.getVal(color_type::channel_a));
                                // if (color != packedCur) {
                                std::vector<ColorStat>::iterator i = std::find_if(statValues.begin(), statValues.end(), [color](const ColorStat &v) {return v.color == color; });
                                if (i == statValues.end())
                                    statValues.push_back({ color, 1 });
                                else
                                    i->stat++;
                                // }
                            }
                        }
                        {
                            color_type rightDown = pImage->getPixel({ x + 1, y + 1 });
                            if (pImage->validPos({ x + 1, y + 1 })) {
                                m_uint32 color = packColor4(rightDown.getVal(color_type::channel_r), rightDown.getVal(color_type::channel_g), rightDown.getVal(color_type::channel_b), rightDown.getVal(color_type::channel_a));
                                //if (color != packedCur) {
                                std::vector<ColorStat>::iterator i = std::find_if(statValues.begin(), statValues.end(), [color](const ColorStat &v) {return v.color == color; });
                                if (i == statValues.end())
                                    statValues.push_back({ color, 1 });
                                else
                                    i->stat++;
                                // }
                            }
                        }
                        {
                            color_type down = pImage->getPixel({ x, y + 1 });
                            if (pImage->validPos({ x, y + 1 })) {
                                m_uint32 color = packColor4(down.getVal(color_type::channel_r), down.getVal(color_type::channel_g), down.getVal(color_type::channel_b), down.getVal(color_type::channel_a));
                                //if (color != packedCur) {
                                std::vector<ColorStat>::iterator i = std::find_if(statValues.begin(), statValues.end(), [color](const ColorStat &v) {return v.color == color; });
                                if (i == statValues.end())
                                    statValues.push_back({ color, 1 });
                                else
                                    i->stat++;
                                //  }
                            }
                        }
                        {
                            color_type leftDown = pImage->getPixel({ x - 1, y + 1 });
                            if (pImage->validPos({ x - 1, y + 1 })) {
                                m_uint32 color = packColor4(leftDown.getVal(color_type::channel_r), leftDown.getVal(color_type::channel_g), leftDown.getVal(color_type::channel_b), leftDown.getVal(color_type::channel_a));
                                //  if (color != packedCur) {
                                std::vector<ColorStat>::iterator i = std::find_if(statValues.begin(), statValues.end(), [color](const ColorStat &v) {return v.color == color; });
                                if (i == statValues.end())
                                    statValues.push_back({ color, 1 });
                                else
                                    i->stat++;
                                //  }
                            }
                        }

                        color_type setVal = curPixel;

                        if (statValues.size() > 0) {

                            bool equal = true;
                            int statFirst = statValues[0].stat;

                            for (ColorStat statIter : statValues) {
                                if (statFirst != statIter.stat) {
                                    equal = false;
                                    break;
                                }
                            }

                            if (!equal) {

                                std::vector<ColorStat>::iterator maxVal = std::max_element(statValues.begin(), statValues.end(), [](const ColorStat &v1, const ColorStat &v2) { return v1.stat < v2.stat; });

                                m_uint8 rmax;
                                m_uint8 gmax;
                                m_uint8 bmax;
                                m_uint8 amax;

                                unpackColor4(maxVal->color, rmax, gmax, bmax, amax);

                                Tinta::tintaVector4i cur(curPixel.getVal(color_type::channel_r),
                                    curPixel.getVal(color_type::channel_g),
                                    curPixel.getVal(color_type::channel_b),
                                    curPixel.getVal(color_type::channel_a));

                                float dist = byteToFloat((m_uint8)Tinta::distance(cur, tintaVector4i{ (int)rmax, (int)gmax , (int)bmax, (int)amax }));

                                if (dist >= colorFactor && maxVal->stat >= threshold) {
                                    setVal = { rmax, gmax, bmax, amax };

                                }

                            }
                            else {   // all colors has the same count

                                     // take the first
                                if (statValues[0].stat >= threshold) {

                                    m_uint8 rmax;
                                    m_uint8 gmax;
                                    m_uint8 bmax;
                                    m_uint8 amax;

                                    unpackColor4(statValues[0].color, rmax, gmax, bmax, amax);

                                    if (statValues[0].stat >= threshold)
                                        setVal = { rmax, gmax, bmax, amax };

                                }
                            }


                        }


                        imgOut.setPixel({ x,y }, setVal);
                        if (withAlpha)
                            imgOut.setPixelAlpha({ x,y }, setVal.getVal(color_type::channel_a));

                    }
                }
                *pImage = imgOut;

              
            }

            return 0;
        }


        int poster(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            m_uint32 sel = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getIndexImgSel();

            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            m_uint32 pallete = GET_VAL_UINT(L, 1);

            const image_t *pPallete = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImage(pallete);

            if (!pPallete) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong pallete image id!");

            }



            image_t imgOut(pImage->getWidth(), pImage->getHeight());

            //int w = (int)pImage->getWidth();
            //int h = (int)pImage->getHeight();

            size_t q = pImage->getSize();
            size_t pq = pPallete->getSize();

            for (size_t i = 0; i < q; i++) {
                tintaVector3f v1(byteToFloat(pImage->getChannel(i, 0)),
                    byteToFloat(pImage->getChannel(i, 1)),
                    byteToFloat(pImage->getChannel(i, 2)));



                float minLen = 1.0;
                size_t pos = 0;

                for (size_t p = 0; p < pq; p++) {

                    tintaVector3f v2(byteToFloat(pPallete->getChannel(i, 0)),
                        byteToFloat(pPallete->getChannel(i, 1)),
                        byteToFloat(pPallete->getChannel(i, 2)));

                    float dist = Tinta::distance(v1, v2);
                    if (dist < minLen) {
                        minLen = dist;
                        pos = p;
                    }
                }
                color_type finColor(pPallete->getPixel(pos));
                finColor.setVal(3, pImage->getChannel(i, 3));

                imgOut.setPixel(i, finColor);
            }

            *pImage = imgOut;
            Tinta::tintaTexSpringMain::getPtr()->getImageStack()->selectImage(sel);

           

            return 0;
        }

        int setpixelb(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaVector3f vec = tintaVector3f::origin;
            Tinta::coord2dI_t point(GET_VAL_INT(L, 1), GET_VAL_INT(L, 2));


            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            color_type color({ GET_VAL_UBYTE(L, 3), GET_VAL_UBYTE(L, 4), GET_VAL_UBYTE(L, 5) });

            float aVal = 1.f;
            if (IS_VAL_REAL(L, 6))
                aVal = GET_VAL_FLOAT(L, 6);

            color_type pColorOrigin = pImage->getPixel(point);

            if (pImage->validPos(point)) {


                pColorOrigin.setVal(color_type::channel_r, (m_uint8)(color.getVal(color_type::channel_r) * aVal) + (m_uint8)(pColorOrigin.getVal(color_type::channel_r) * (1.f - aVal)));
                pColorOrigin.setVal(color_type::channel_g, (m_uint8)(color.getVal(color_type::channel_g) * aVal) + (m_uint8)(pColorOrigin.getVal(color_type::channel_g) * (1.f - aVal)));
                pColorOrigin.setVal(color_type::channel_b, (m_uint8)(color.getVal(color_type::channel_b) * aVal) + (m_uint8)(pColorOrigin.getVal(color_type::channel_b) * (1.f - aVal)));

                pImage->setPixel(point, pColorOrigin);                
            }
            return 0;
        }


        int setpixelf(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaVector3f vec = tintaVector3f::origin;
            Tinta::coord2dI_t point(GET_VAL_INT(L, 1), GET_VAL_INT(L, 2));


            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");
            }

            color_type color({ floatToByte(GET_VAL_FLOAT(L, 3)), floatToByte(GET_VAL_FLOAT(L, 4)), floatToByte(GET_VAL_FLOAT(L, 5)) });

            float aVal = 1.f;
            if (IS_VAL_REAL(L, 6))
                aVal = GET_VAL_FLOAT(L, 6);

            color_type pColorOrigin = pImage->getPixel(point);

            if (pImage->validPos(point)) {
                /*pColorOrigin->setVal(0, (m_uint8)(color.getVal(color_type::channel_r) * aVal) + (m_uint8)(pColorOrigin->getVal(color_type::channel_r) * (1.f - aVal)) );
                pColorOrigin->setVal(1, (m_uint8)(color.getVal(color_type::channel_g) * aVal) + (m_uint8)(pColorOrigin->getVal(color_type::channel_g) * (1.f - aVal)) );
                pColorOrigin->setVal(2, (m_uint8)(color.getVal(color_type::channel_b) * aVal) + (m_uint8)(pColorOrigin->getVal(color_type::channel_b) * (1.f - aVal)) );
                //color.setVal(color_type::channel_a, pPixelOrigin->getVal(color_type::channel_a) );
                */

                pColorOrigin.setVal(color_type::channel_r, (m_uint8)(color.getVal(color_type::channel_r) * aVal) + (m_uint8)(pColorOrigin.getVal(color_type::channel_r) * (1.f - aVal)));
                pColorOrigin.setVal(color_type::channel_g, (m_uint8)(color.getVal(color_type::channel_g) * aVal) + (m_uint8)(pColorOrigin.getVal(color_type::channel_g) * (1.f - aVal)));
                pColorOrigin.setVal(color_type::channel_b, (m_uint8)(color.getVal(color_type::channel_b) * aVal) + (m_uint8)(pColorOrigin.getVal(color_type::channel_b) * (1.f - aVal)));
                //color.setVal(color_type::channel_a, pPixelOrigin->getVal(color_type::channel_a) );
                pImage->setPixel(point, pColorOrigin);

                
            }

            return 0;

        }

        int channels(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            m_uint32 idImg = GET_VAL_UINT(L, 1);

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImageEx(idImg);

            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong image id!");
            }


            PUSH_INT_ARG(L, (int)pImage->channels()); //		

            return 1;

        }

        int selected(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            PUSH_INT_ARG(L, Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getIndexImgSel()); // return value
            return 1;

        }

        int erase(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            int argc = GET_QUANTITY(L); //1 - int val - image index
            if (argc != 1) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            int index = 1;
            if (!IS_VAL_INTEGER(L, index)) {
                StringStreamBasic msg;
                msg << "Argument num: " << index << ". Wrong argument type!";
                TROW_ERR_BADARGFUNC(L, 1, msg.str().c_str());

            }
            Tinta::tintaTexSpringMain::getPtr()->getImageStack()->delImg(GET_VAL_UINT(L, index));
            return 0;

        }


        int select(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            int argc = GET_QUANTITY(L); //1 - int val - image index

                                        //wrong arg value
            if (argc != 1) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
            }

            bool result = true;
            m_uint32 index = GET_VAL_UINT(L, 1);
            if (!Tinta::tintaTexSpringMain::getPtr()->getImageStack()->selectImage(index)) {
                StringStreamBasic msg;

                msg << "Wrong image id: " << index;
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());


            }
            PUSH_BOOL_ARG(L, result); // return value
            return 1;

        }


        int save(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }



            String extType = GET_VAL_STRING_EX(L, 1);

            Tinta::tintaIImgCodec* mImgFile = NULL_M; //M_NEW tintaPngImgCodec ();

                                                      //if(type_str == "TIFF")				
                                                      // String type(GET_VAL_STRING_EX(extType));
            StringUtil::toLower(extType);
            if (extType == _M("png"))
                mImgFile = M_NEW tintaPngImgCodec();
            else if (extType == _M("jpeg") || extType == _M("jpg"))
                mImgFile = M_NEW tintaJpgImgCodec();
            else {
                StringStream msg;
                msg << _M("Wrong image type: ") << extType;
                TROW_ERR_FUNC_CLEAR_EX(L, msg.str());
                return 0;
            }


            UNIQPTRVIRT(Tinta::tintaIImgCodec, mImgFilePtr, mImgFile);

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }
            StringBasic path_full = GET_VAL_STRING(L, 2); //m_parameter_set.get_string( 0, L );

            if (!Tinta::createFolder(toWideChar(path_full))) {
                StringStreamBasic text_out;
                text_out << "Invalid path: " << path_full.c_str();

                TROW_ERR_FUNC_CLEAR(L, text_out.str().c_str());

            }
            //}

            bool rez = pImage->saveToFile(mImgFilePtr.get(), toWideChar(path_full));

            if (!rez) {
                StringStreamBasic text_out;
                text_out << "Saving image error. Path: " << path_full.c_str();

                TROW_ERR_FUNC_CLEAR(L, text_out.str().c_str());

            }
            return rez;

        }





        int width(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            const image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSel();

            int rez = -1;
            if (pImage) {
                rez = pImage->getWidth();
            }
            else {

                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }


            PUSH_INT_ARG(L, rez); // return value
            return 1;

        }

        int height(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            const image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSel();

            int rez = -1;
            if (pImage) {
                rez = pImage->getHeight();
            }
            else {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            PUSH_INT_ARG(L, rez); // return value
            return 1;

        }


        int getpixelb(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            //const int minParamsCount(2);
            int argc = GET_QUANTITY(L); //1 - int val - image index		

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {

                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            if (argc != 2) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }


            Tinta::coord2dI_t coord(GET_VAL_INT(L, 1), GET_VAL_INT(L, 2));
            color_type color = pImage->getPixel(coord);
            if (!pImage->validPos(coord)) {
                StringStreamBasic logname;
                logname << "Coordinate is out of range! Coordinate:";
                logname << " x " << coord.mValX;
                logname << " y " << coord.mValY;
                TROW_ERR_FUNC_CLEAR(L, logname.str().c_str());


            }

            PUSH_INT_ARG(L, (int)(color.getVal(color_type::channel_r))); // return value
            PUSH_INT_ARG(L, (int)(color.getVal(color_type::channel_g))); // return value
            PUSH_INT_ARG(L, (int)(color.getVal(color_type::channel_b))); // return value
            PUSH_INT_ARG(L, (int)(color.getVal(color_type::channel_a))); // return value

            return 4;
        }

        /*
        Returns pixel value in coordinate
        param x coordinate value
        param y coordinate value
        return r value
        return g value
        return b value
        return alpha value
        */
        int getpixelf(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            //const int minParamsCount(2);
            int argc = GET_QUANTITY(L); //1 - int val - image index


            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {

                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            if (argc != 2) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }



            Tinta::coord2dI_t coord(GET_VAL_INT(L, 1), GET_VAL_INT(L, 2));
            color_type color = pImage->getPixel(coord);

            if (!pImage->validPos(coord)) {
                StringStreamBasic logname;
                logname << "Coordinate is out of range! Coordinate:";
                logname << " x " << coord.mValX;
                logname << " y " << coord.mValY;
                TROW_ERR_FUNC_CLEAR(L, logname.str().c_str());


            }

            PUSH_FLOAT_ARG(L, byteToFloat(color.getVal(color_type::channel_r))); // return value
            PUSH_FLOAT_ARG(L, byteToFloat(color.getVal(color_type::channel_g))); // return value
            PUSH_FLOAT_ARG(L, byteToFloat(color.getVal(color_type::channel_b))); // return value
            PUSH_FLOAT_ARG(L, byteToFloat(color.getVal(color_type::channel_a))); // return value

            return 4;
        }





        int getindpixelb(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            //	const int minParamsCount(2);
            int argc = GET_QUANTITY(L); //1 - int val - image index		

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {

                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            if (argc != 2) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            m_uint32 index = (m_uint32)GET_VAL_INT(L, 1);
            color_type color = pImage->getPixel(index);
            if (!pImage->validPos(index)) {
                StringStreamBasic logname;
                logname << "Image index is out of range! Index:";
                logname << GET_VAL_INT(L, 1);
                TROW_ERR_FUNC_CLEAR(L, logname.str().c_str());


            }

            PUSH_INT_ARG(L, (int)(color.getVal(color_type::channel_r))); // return value
            PUSH_INT_ARG(L, (int)(color.getVal(color_type::channel_g))); // return value
            PUSH_INT_ARG(L, (int)(color.getVal(color_type::channel_b))); // return value
            PUSH_INT_ARG(L, (int)(color.getVal(color_type::channel_a))); // return value

            return 4;


        }



        int getindpixelf(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            //const int minParamsCount(2);
            //int argc = GET_QUANTITY(L); //1 - int val - image index


            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {

                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }

            m_uint32 index = (m_uint32)GET_VAL_INT(L, 1);

            color_type color = pImage->getPixel(index);
            if (!pImage->validPos(index)) {
                StringStreamBasic logname;
                logname << "Image index is out of range! Index:";
                logname << GET_VAL_INT(L, 1);
                TROW_ERR_FUNC_CLEAR(L, logname.str().c_str());
            }

            PUSH_FLOAT_ARG(L, byteToFloat(color.getVal(color_type::channel_r))); // return value
            PUSH_FLOAT_ARG(L, byteToFloat(color.getVal(color_type::channel_g))); // return value
            PUSH_FLOAT_ARG(L, byteToFloat(color.getVal(color_type::channel_b))); // return value
            PUSH_FLOAT_ARG(L, byteToFloat(color.getVal(color_type::channel_a))); // return value

            return 4;

        }



        int fillb(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            const int argCount(3);
            const int argc = GET_QUANTITY(L);
            Tinta::tintaPixel32b color;

            if (argc != argCount) {

                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            m_uint8 val[argCount];
            for (int n = 1, i = 0; n <= argc; ++n, i++) {
                val[i] = GET_VAL_UBYTE(L, n);
            }

            color.setVal(color_type::channel_r, val[0]);
            color.setVal(color_type::channel_g, val[1]);
            color.setVal(color_type::channel_b, val[2]);


            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (pImage) {
                pImage->setPixel(color);               
            }
            else {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }
            return 0;

        }

        int fillf(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            const int argCount(3);
            const int argc = GET_QUANTITY(L);
            Tinta::tintaPixel32b color;

            if (argc != argCount) {

                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");

            }

            float val[argCount];
            for (int n = 1, i = 0; n <= argc; ++n, i++) {
                val[i] = GET_VAL_FLOAT(L, n);
            }

            color.setVal(color_type::channel_r, floatToByte(val[0]));
            color.setVal(color_type::channel_g, floatToByte(val[1]));
            color.setVal(color_type::channel_b, floatToByte(val[2]));


            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (pImage) {
                pImage->setPixel(color);
                
            }
            else {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }
            return 0;

        }

        int create(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            int image_width = GET_VAL_INT(L, 1);//m_parameter_set.get_int(1,L);
            int image_height = GET_VAL_INT(L, 2);//m_parameter_set.get_int(2,L);

            if (image_width <= 0 || image_height <= 0) {
                StringStreamBasic msg;
                msg << "image_width :" << image_width << " image_height: " << image_height << ". Wrong height or width value!";

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            int new_image = -1;

            StringBasic image_type;

            if (IS_VAL_STRING(L, 3))
                image_type = GET_VAL_STRING(L, 3);

            if (image_type.length() == 0) {
                image_type = "basic";
            }
            new_image = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->addImg(image_type); //image_width, image_height);		 			 
            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {

                StringStreamBasic msg;
                msg << "Wrong image type " << image_type;

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            ImgChannels channels = ImgChannels_4;

            if (IS_VAL_INTEGER(L, 4)) {
                int ch = GET_VAL_INT(L, 4);
                if (ch < 1 || ch > 4) {
                    StringStream s;
                    s << _M("Wrond channel value. Not in range 1-4 : ") << ch;
                    TROW_ERR_FUNC_CLEAR_EX(L, s.str().c_str());
                }

                channels = (ImgChannels)ch;
            }

            pImage->createImage(image_width, image_height, channels);

            PUSH_INT_ARG(L, new_image);           

            return 1;

        }

        int read(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            const StringBasic path = IS_VAL_STRING(L, 1) ? GET_VAL_STRING(L, 1) : "";
            const StringBasic extType = IS_VAL_STRING(L, 2) ? GET_VAL_STRING(L, 2) : "";
            StringBasic imageType = IS_VAL_STRING(L, 3) ? GET_VAL_STRING(L, 3) : "";

            Tinta::tintaIImgCodec* mImgFile = NULL_M;
            String type(toWideChar(extType));
            String full = toWideChar(path);
            String typePath(StringUtil::getFileExt(full));

            StringUtil::toLower(type);
            StringUtil::toLower(typePath);
            if (type.length() != 0 && typePath != type) {
                StringStream s;
                s << _M("Codec name and file extansion must be equal: ") << type << _M(" and ") << typePath;
                TROW_ERR_FUNC_CLEAR_EX(L, s.str().c_str());
            }

            type = typePath;

            if (type == _M("png"))
                mImgFile = M_NEW tintaPngImgCodec();
            else if (type == _M("jpeg") || type == _M("jpg"))
                mImgFile = M_NEW tintaJpgImgCodec();
            else
            {
                StringStream s;
                s << _M("Wrong image codec: ") << type;
                TROW_ERR_FUNC_CLEAR_EX(L, s.str().c_str());

            }

            if (!mImgFile) {
                StringStreamBasic msg;
                msg << "Can`t create codec object for: " << extType;

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }
            UNIQPTRVIRT(Tinta::tintaIImgCodec, mImgFilePtr, mImgFile);
            if (imageType.length() == 0) {
                imageType = "basic";
            }

            int newImgIndex = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->addImg(imageType); //image_width, image_height);
            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImageEx(newImgIndex);

            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }


            if (!Tinta::isPathValid(full)) {
                full = Tinta::getRootPath();
                Tinta::StringUtil::addLeaf(full, toWideChar(path));
            }

            if (!Tinta::isPathValid(full)) {

                StringStreamBasic values;
                values << "Wrong path to file: " << path;
                TROW_ERR_FUNC_CLEAR(L, values.str().c_str());
            }

            if (!pImage->readFromFile(mImgFilePtr.get(), full)) {

                Tinta::tintaTexSpringMain::getPtr()->getImageStack()->delImg(newImgIndex);
                StringStreamBasic values;
                values << "Can`t read image from: " << path;
                TROW_ERR_FUNC_CLEAR(L, values.str().c_str());
            }

           
            PUSH_INT_ARG(L, newImgIndex);
            return 1;

        }

        int fillchannelf(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L);

            const int arg_count = 2;
            if (!IS_VAL_REAL(L, 1)) {

                StringStreamBasic msg;
                msg << "Argument num: " << 1 << ". Wrong argument type!";

                TROW_ERR_BADARGFUNC(L, 1, msg.str().c_str());
            }


            m_uint8 ch = GET_VAL_UBYTE(L, 1);

            float v = GET_VAL_FLOAT(L, 2);


            if (argc != arg_count) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
            }

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }
            pImage->setChannel(ch, floatToByte(v));

            return 0;

        }

        int fillchannelb(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L);

            const int arg_count = 2;
            if (!IS_VAL_REAL(L, 1)) {

                StringStreamBasic msg;
                msg << "Argument num: " << 1 << ". Wrong argument type!";

                TROW_ERR_BADARGFUNC(L, 1, msg.str().c_str());
            }


            m_uint8 ch = GET_VAL_UBYTE(L, 1);

            m_uint8 v = GET_VAL_UBYTE(L, 2);


            if (argc != arg_count) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
            }

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();

            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");

            }
            pImage->setChannel(ch, v);

            return 0;

        }




        int setchannelf(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L);

            const int arg_count = 4;

            if (argc != arg_count) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
            }

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");
            }

            coord2dI_t point(GET_VAL_UINT(L, 1), GET_VAL_UINT(L, 2));
            m_uint8 channel = GET_VAL_UBYTE(L, 3);
            float val = GET_VAL_FLOAT(L, 4);

            color_type pColorOrigin = pImage->getPixel(point);

            if (pImage->validPos(point)) {
                pImage->setChannel(point, channel, floatToByte(val));
            }

            return 0;

        }


        int setchannelb(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            int argc = GET_QUANTITY(L);

            const int arg_count = 4;

            if (argc != arg_count) {
                TROW_ERR_FUNC_CLEAR(L, "Wrong arguments quantity!");
            }

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");
            }

            coord2dI_t point(GET_VAL_UINT(L, 1), GET_VAL_UINT(L, 2));
            m_uint8 channel = GET_VAL_UBYTE(L, 3);
            m_uint32 val = GET_VAL_UINT(L, 4);
            color_type pColorOrigin = pImage->getPixel(point);

            if (pImage->validPos(point)) {
                pImage->setChannel(point, channel, (m_uint8)val);
            }

            return 0;
        }


        int mix(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }


            m_uint32 idSrcImage = GET_VAL_UINT(L, 1);

            int x_new_pos = GET_VAL_INT(L, 2);
            int y_new_pos = GET_VAL_INT(L, 3);

            const image_t *pImgSrc = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImage(idSrcImage);

            if (!pImgSrc) {
                TROW_ERR_FUNC_CLEAR(L, "Source image does not exist!");
            }

            image_t *pImgDest = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (!pImgDest) {

                TROW_ERR_FUNC_CLEAR(L, "Destination image does not exist!");

            }
            float reduce = 1.f;
            if (IS_VAL_REAL(L, 4))
                reduce = GET_VAL_FLOAT(L, 4);

            pImgDest->mixImage(pImgSrc, x_new_pos, y_new_pos, reduce);

            return 0;
        }


        int mixchannel(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            //int argc = GET_QUANTITY(L);

            m_uint32 idSrcImage = GET_VAL_UINT(L, 1);
            //m_uint32 idDistImage = GET_VAL_UINT(L, 2);

            int x_new_pos = GET_VAL_INT(L, 2);
            int y_new_pos = GET_VAL_INT(L, 3);

            const image_t *pImageSrc = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImage(idSrcImage);
            if (!pImageSrc) {

                TROW_ERR_FUNC_CLEAR(L, "Source image does not exist!");

            }

            image_t *pImageDist = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (!pImageDist) {

                TROW_ERR_FUNC_CLEAR(L, "Destination image does not exist!");


            }

            float factor = 1.f;
            if (IS_VAL_REAL(L, 4))
                factor = GET_VAL_FLOAT(L, 5);

            pImageDist->mixChannel(pImageSrc, x_new_pos, y_new_pos, color_type::channel_a, factor);

            return 0;
        }





        int eraseall(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            Tinta::tintaTexSpringMain::getPtr()->getImageStack()->delAllImg();

            return 0;

        }

        /*
        Returns images quantity
        */
        int images(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            PUSH_INT_ARG(L, (int)Tinta::tintaTexSpringMain::getPtr()->getImageStack()->size());

            return 1;
        }
    }


    namespace TintaBoxLua {

        int erase(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");
            boxes->delObj(GET_VAL_UINT(L, 1));

            return 0;
        }


        /*
        deletes all boxes in container
        */
        int eraseall(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");
            boxes->clear();

            return 0;
        }

        int getvalf(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            const tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            //bool rez = false;
            m_float32 rezVal = 0.f;
            int xVal = GET_VAL_INT(L, 2);
            int yVal = GET_VAL_INT(L, 3);

            const tintaArray2dFloat_t* arr = box->getFloatBox();
            if (!arr) {
                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            //bool r = false;
            if (box->getType() == tintaArrayBox::enCommonBox) {

                if (arr->inBoundary(xVal, yVal)) {
                    rezVal = arr->getValFast(xVal, yVal);

                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for: ";
                    msg << xVal;
                    msg << " or ";
                    msg << yVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                }
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                size_t pos = (size_t)xVal;

                if (pos < arr->getSize()) {
                    rezVal = arr->getValReal(pos);
                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                }

            }

            PUSH_FLOAT_ARG(L, rezVal); //			 
            return 1;
        }


        int getvali16(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, "tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            const tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            // bool rez = false;
            m_int16 rezVal = 0;
            int xVal = GET_VAL_INT(L, 2);
            int yVal = GET_VAL_INT(L, 3);


            const tintaArray2dInt16_t* arr = box->getInt16Box();
            if (!arr) {
                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            if (box->getType() == tintaArrayBox::enCommonBox) {

                if (arr->inBoundary(xVal, yVal)) {
                    rezVal = arr->getValFast(xVal, yVal);

                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for: ";
                    msg << xVal;
                    msg << " or ";
                    msg << yVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                }
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                size_t pos = (size_t)xVal;

                if (pos < arr->getSize()) {
                    rezVal = arr->getValReal(pos);
                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                }

            }

            PUSH_INT_ARG(L, rezVal); //

            return 1;
        }





        int getvali32(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, "tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            const tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            // bool rez = false;
            int rezVal = 0;
            int xVal = GET_VAL_INT(L, 2);
            int yVal = GET_VAL_INT(L, 3);
            //if( mapval->inBoundary( xVal, yVal) ){

            const tintaArray2dInt_t* arr = box->getIntBox();

            if (!arr) {
                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            if (box->getType() == tintaArrayBox::enCommonBox) {

                if (arr->inBoundary(xVal, yVal)) {
                    rezVal = arr->getValFast(xVal, yVal);

                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for: ";
                    msg << xVal;
                    msg << " or ";
                    msg << yVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                }
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                size_t pos = (size_t)xVal;

                if (pos < arr->getSize()) {
                    rezVal = arr->getValReal(pos);
                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                }

            }

            //}

            PUSH_INT_ARG(L, rezVal); //			
            return 1;
        }

        int getvalui8(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, "tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            const tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            // bool rez = false;
            int rezVal = 0;
            int xVal = GET_VAL_INT(L, 2);
            int yVal = GET_VAL_INT(L, 3);
            //if( mapval->inBoundary( xVal, yVal) ){

            const tintaArray2dByte_t* arr = box->getUInt8Box();

            if (!arr) {
                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            }

            if (box->getType() == tintaArrayBox::enCommonBox) {

                if (arr->inBoundary(xVal, yVal)) {
                    rezVal = arr->getValFast(xVal, yVal);

                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for: ";
                    msg << xVal;
                    msg << " or ";
                    msg << yVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                }
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                size_t pos = (size_t)xVal;

                if (pos < arr->getSize()) {
                    rezVal = arr->getValReal(pos);
                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

                }

            }

            //}

            PUSH_INT_ARG(L, rezVal); //			
            return 1;
        }



        int getval3f(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, "tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            const tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }


            int xVal = GET_VAL_INT(L, 2);
            //             int yVal = GET_VAL_INT(L, 3);
            //if( mapval->inBoundary( xVal, yVal) ){

            const tintaArray2dVec3f_t* arr = box->getVec3fBox();

            if (!arr) {
                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (box->getType() == tintaArrayBox::enVectorBox) {

                size_t pos = (size_t)xVal;

                if (pos < arr->getSize()) {
                    tintaVector3f rezVal = arr->getValReal(pos);

                    PUSH_FLOAT_ARG(L, rezVal.mx); //	
                    PUSH_FLOAT_ARG(L, rezVal.my); //	
                    PUSH_FLOAT_ARG(L, rezVal.mz); //	

                    return 3;
                }
                else {
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                    return 0;
                }

            }

            return 0;
        }


        int getvals(SCRIPT_STATE *L) {


            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            const tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            // bool rez = false;
            StringBasic rezVal("");
            int posVal = GET_VAL_INT(L, 2);

            //if( mapval->inBoundary( xVal, yVal) ){

            const tintaArray2dString_t* arr = box->getStringBox();

            if (!arr || box->getType() != tintaArrayBox::enVectorBox) {
                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }


            size_t pos = (size_t)posVal;

            if (pos < arr->getSize()) {
                rezVal = arr->getValReal(pos);
            }
            else {
                StringStreamBasic msg;
                msg << "Out of boundary for index: ";
                msg << posVal;
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            PUSH_STRING_ARG(L, rezVal.c_str()); //			
            return 1;

        }
        int setvals(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            int posVal = GET_VAL_INT(L, 2);

            tintaArray2dString_t* arr = box->getStringBox();

            if (posVal < 0 || posVal >= (int)arr->getSize()) {
                StringStreamBasic msg;
                msg << "Out of boundary for index: ";
                msg << posVal;
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
            }
            else {
                size_t pos = (size_t)posVal;
                arr->setValReal(pos, toWideChar(GET_VAL_STRING(L, 3)));
            }

            return 0;

        }


        int resize(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }
            if (box->getType() == tintaArrayBox::enCommonBox) {
                box->create(TExtremValues2i(GET_VAL_INT(L, 2), GET_VAL_INT(L, 3), GET_VAL_INT(L, 4), GET_VAL_INT(L, 5)));
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                if (GET_VAL_INT(L, 2) >= 0) {

                    size_t size = (size_t)GET_VAL_INT(L, 2);
                    box->create(size);
                }
                else {
                    StringStreamBasic msg;
                    msg << "For this box type wrong size value: " << GET_VAL_INT(L, 2);
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                    return 0;
                }
            }

            return 0;
        }

        int setvalf(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            int xVal = GET_VAL_INT(L, 2);
            int yVal = GET_VAL_INT(L, 3);

            tintaArray2dFloat_t* arr = box->getFloatBox();

            if (!arr) {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (box->getType() == tintaArrayBox::enCommonBox) {
                if (!arr->setVal(xVal, yVal, GET_VAL_FLOAT(L, 4))) {

                    StringStreamBasic msg;

                    msg << "Out of boundary for x: ";
                    msg << xVal;
                    msg << " y: ";
                    msg << yVal;
                    msg << " extreme values xMin: ";
                    msg << arr->getExtrem().mXMin;
                    msg << " xMax: ";
                    msg << arr->getExtrem().mXMax;
                    msg << " yMin: ";
                    msg << arr->getExtrem().mYMin;
                    msg << " yMax: ";
                    msg << arr->getExtrem().mYMax;

                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                if (xVal < 0 || xVal >= (int)arr->getSize()) { //!arr->setVal( xVal, 0 , (m_int16) ( GET_VAL_INT( L, 4 ) ) ) ){
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
                else {
                    size_t pos = (size_t)xVal;
                    arr->setValReal(pos, GET_VAL_FLOAT(L, 4));
                }
            }

            return 0;
        }

        int setval3f(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            m_uint32 xVal = GET_VAL_UINT(L, 2);
            m_uint32 yVal = GET_VAL_UINT(L, 3);

            tintaArray2dVec3f_t* arr = box->getVec3fBox();

            if (!arr) {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }


            if (box->getType() == tintaArrayBox::enVectorBox) {

                if (xVal >= (m_uint32)arr->getSize()) { //!arr->setVal( xVal, 0 , (m_int16) ( GET_VAL_INT( L, 4 ) ) ) ){
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
                else {
                    size_t pos = (size_t)xVal;

                    arr->setVal(pos, 0, tintaVector3f(GET_VAL_FLOAT(L, 4), GET_VAL_FLOAT(L, 5), GET_VAL_FLOAT(L, 6)));
                }
            }
            else {

                if (xVal >= (m_uint32)arr->getSize() || yVal >= (m_uint32)arr->getSize()) {
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
                else {


                    arr->setVal(xVal, yVal, tintaVector3f(GET_VAL_FLOAT(L, 4), GET_VAL_FLOAT(L, 5), GET_VAL_FLOAT(L, 6)));
                }

            }

            return 0;
        }


        int setvali32(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            int xVal = GET_VAL_INT(L, 2);
            int yVal = GET_VAL_INT(L, 3);

            tintaArray2dInt_t* arr = box->getIntBox();

            if (!arr) {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (box->getType() == tintaArrayBox::enCommonBox) {
                if (!arr->setVal(xVal, yVal, GET_VAL_INT(L, 4))) {

                    StringStreamBasic msg;

                    msg << "Out of boundary for x: ";
                    msg << xVal;
                    msg << " y: ";
                    msg << yVal;
                    msg << " extreme values xMin: ";
                    msg << arr->getExtrem().mXMin;
                    msg << " xMax: ";
                    msg << arr->getExtrem().mXMax;
                    msg << " yMin: ";
                    msg << arr->getExtrem().mYMin;
                    msg << " yMax: ";
                    msg << arr->getExtrem().mYMax;

                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                if (xVal < 0 || xVal >= (int)arr->getSize()) { //!arr->setVal( xVal, 0 , (m_int16) ( GET_VAL_INT( L, 4 ) ) ) ){
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
                else {
                    size_t pos = (size_t)xVal;
                    arr->setValReal(pos, GET_VAL_INT(L, 4));
                }
            }

            return 0;
        }

        int setvalui8(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            int xVal = GET_VAL_INT(L, 2);
            int yVal = GET_VAL_INT(L, 3);

            tintaArray2dByte_t* arr = box->getUInt8Box();

            if (!arr) {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (box->getType() == tintaArrayBox::enCommonBox) {
                if (!arr->setVal(xVal, yVal, GET_VAL_UBYTE(L, 4))) {

                    StringStreamBasic msg;

                    msg << "Out of boundary for x: ";
                    msg << xVal;
                    msg << " y: ";
                    msg << yVal;
                    msg << " extreme values xMin: ";
                    msg << arr->getExtrem().mXMin;
                    msg << " xMax: ";
                    msg << arr->getExtrem().mXMax;
                    msg << " yMin: ";
                    msg << arr->getExtrem().mYMin;
                    msg << " yMax: ";
                    msg << arr->getExtrem().mYMax;

                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                if (xVal < 0 || xVal >= (int)arr->getSize()) { //!arr->setVal( xVal, 0 , (m_int16) ( GET_VAL_INT( L, 4 ) ) ) ){
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
                else {
                    size_t pos = (size_t)xVal;
                    arr->setValReal(pos, GET_VAL_UBYTE(L, 4));
                }
            }

            return 0;
        }




        int pushbacki16(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dInt16_t *arr = box->getInt16Box();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->pushBack((m_int16)GET_VAL_INT(L, 2));
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t push value, box have 'Y' dimension or YMin value not equal 0.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }


        int pushbacks(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dString_t *arr = box->getStringBox();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->pushBack(toWideChar(GET_VAL_STRING(L, 2)));
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t push value, box have 'Y' dimension or YMin value not equal 0.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;
        }



        int pushbacki32(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dInt_t *arr = box->getIntBox();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->pushBack(GET_VAL_INT(L, 2));
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t push value, box have 'Y' dimension or YMin value not equal 0.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }


        int pushbackui8(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dByte_t *arr = box->getUInt8Box();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->pushBack(GET_VAL_UBYTE(L, 2));
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t push value, box have 'Y' dimension or YMin value not equal 0.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }


        int elements(SCRIPT_STATE *L) {


            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            PUSH_UINT_ARG(L, (m_uint32)box->width());
            PUSH_UINT_ARG(L, (m_uint32)box->height());

            return 2;
        }

        int pushbackf(SCRIPT_STATE *L) {



            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dFloat_t *arr = box->getFloatBox();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->pushBack(GET_VAL_FLOAT(L, 2));
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t push value, box have 'Y' dimension or YMin value not equal 0";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }


        int pushback3f(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dVec3f_t *arr = box->getVec3fBox();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->pushBack({ GET_VAL_FLOAT(L, 2), GET_VAL_FLOAT(L, 3), GET_VAL_FLOAT(L, 4) });
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t push value, box have 'Y' dimension or YMin value not equal 0";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }

        int popbackf(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dFloat_t *arr = box->getFloatBox();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->popBack();
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t pop value box size equal 0";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }

        int popback3f(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dVec3f_t *arr = box->getVec3fBox();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->popBack();
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t pop value box size equal 0";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }

        int popbacks(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dString_t *arr = box->getStringBox();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->popBack();
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t pop value box size equal 0";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }

        int popbacki32(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dInt_t *arr = box->getIntBox();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->popBack();
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t pop value box size equal 0";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }

        int popbackui8(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dByte_t *arr = box->getUInt8Box();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->popBack();
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t pop value box size equal 0";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }

        int popbacki16(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");

            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            tintaArray2dInt16_t *arr = box->getInt16Box();
            bool rez = false;
            if (box->getType() == tintaArrayBox::enVectorBox && arr) {
                rez = arr->popBack();
            }
            else {

                StringStreamBasic msg;
                msg << "Not supported function for this box type";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            if (!rez) {
                StringStreamBasic msg;
                msg << "Can`t pop value box size equal 0";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            return 0;

        }

        int setvali16(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            int xVal = GET_VAL_INT(L, 2);
            int yVal = GET_VAL_INT(L, 3);

            tintaArray2dInt16_t* arr = box->getInt16Box();

            if (!arr) {

                StringStreamBasic msg;
                msg << "Not supported function for this box type.";
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }


            if (box->getType() == tintaArrayBox::enCommonBox) {
                if (!arr->setVal(xVal, yVal, (m_int16)(GET_VAL_INT(L, 4)))) {
                    StringStreamBasic msg;

                    msg << "Out of boundary for x: ";
                    msg << xVal;
                    msg << " y: ";
                    msg << yVal;
                    msg << " extreme values xMin: ";
                    msg << arr->getExtrem().mXMin;
                    msg << " xMax: ";
                    msg << arr->getExtrem().mXMax;
                    msg << " yMin: ";
                    msg << arr->getExtrem().mYMin;
                    msg << " yMax: ";
                    msg << arr->getExtrem().mYMax;
                }
            }
            else if (box->getType() == tintaArrayBox::enVectorBox) {

                if (xVal < 0 || xVal >= (int)arr->getSize()) { //!arr->setVal( xVal, 0 , (m_int16) ( GET_VAL_INT( L, 4 ) ) ) ){
                    StringStreamBasic msg;
                    msg << "Out of boundary for index: ";
                    msg << xVal;
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
                else {
                    size_t pos = (size_t)xVal;
                    arr->setValReal(pos, (m_uint16)GET_VAL_INT(L, 4));
                }
            }

            return 0;
        }        

        int create(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();

            CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");
            if (!IS_VAL_STRING(L, 1)) {

                StringStreamBasic msg;
                msg << "Argument num: " << 1 << ". Wrong argument type!";

                TROW_ERR_BADARGFUNC(L, 1, msg.str().c_str());
            }

            m_uint32 id = boxes->addObj(GET_VAL_STRING(L, 1));

            if (id == ZERO_ID) {

                StringStreamBasic msg;
                msg << "Box creating error: " << GET_VAL_STRING(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
            }

            PUSH_INT_ARG(L, id); // return value
            return 1;
        }

        int clear(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();
            CoreAssert(boxes, " tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();");


            tintaArrayBox *box = boxes->getObjEx(GET_VAL_UINT(L, 1));

            if (!box) {

                StringStreamBasic msg;
                msg << "Wrong box id: " << GET_VAL_UINT(L, 1);

                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                return 0;
            }

            box->clear();
            return 0;
        }

        int boxes(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaBoxContainer* boxes = Tinta::tintaTexSpringMain::getPtr()->getBoxContainer();

            CoreAssert(boxes, "tintaBoxContainer* boxes==NULL");


            PUSH_INT_ARG(L, (int)boxes->getSize());

            return 1;

        }

    }


    namespace tintaFontFunctions {          
                     

        int copy(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            m_uint32 id = Tinta::tintaTexSpringMain::getPtr()->cloneFont(GET_VAL_UINT(L, 1));

            if (id == ZERO_ID) {
                StringStreamBasic msg;
                msg << "Wrong source font id: " << GET_VAL_UINT(L, 1);
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
            }

            PUSH_UINT_ARG(L, id);

            return 1;
        }

        int add(SCRIPT_STATE *L) {

            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            String name = GET_VAL_STRING(L, 1);

            float scaleX = 1.f;
            if (IS_VAL_REAL(L, 2))
                scaleX = GET_VAL_FLOAT(L, 2);

            float scaleY = 1.f;
            if (IS_VAL_REAL(L, 3))
                scaleY = GET_VAL_FLOAT(L, 3);

            color_type color = { 0,0,0,0 };
            if (IS_VAL_INTEGER(L, 4) && IS_VAL_INTEGER(L, 5) && IS_VAL_INTEGER(L, 6))
                color = { GET_VAL_UBYTE(L, 4), GET_VAL_UBYTE(L, 5), GET_VAL_UBYTE(L, 6) };

            m_uint32 added = tintaTexSpringMain::getPtr()->addFont(name, scaleX, scaleY, color);
            if (added != ZERO_ID) {
                PUSH_INT_ARG(L, added);
                return 1;
            }

            StringStreamBasic msg;
            msg << "Error while creating font name/path: " << name;
            TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());

            return 0;
        }

        int select(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            if (!tintaTexSpringMain::getPtr()->setFont(GET_VAL_UINT(L, 1))) {
                StringStreamBasic msg;
                msg << "Wrong font id: " << GET_VAL_UINT(L, 1);
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
            }
            return 0;
        }

       
        int erase(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            if (!tintaTexSpringMain::getPtr()->deleteFont(GET_VAL_UINT(L, 1))) {
                StringStreamBasic msg;
                msg << "Wrong font id: " << GET_VAL_UINT(L, 1);
                TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
            }
            return 0;
        }

        int eraseall(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            tintaTexSpringMain::getPtr()->deleteFonts();
            return 0;
        }



        int setcolor(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            m_uint32 id = 0;
            if (IS_VAL_INTEGER(L, 1))
                id = GET_VAL_UINT(L, 1);

            color_type color = { 0,0,0,0 };
            if (IS_VAL_INTEGER(L, 2) && IS_VAL_INTEGER(L, 3) && IS_VAL_INTEGER(L, 4)) {
                color = { GET_VAL_UBYTE(L, 2), GET_VAL_UBYTE(L, 3), GET_VAL_UBYTE(L, 4) , 0 };

                if (!tintaTexSpringMain::getPtr()->setFontParameter(id, 0, 0, color)) {
                    StringStreamBasic msg;
                    msg << "Wrong font id: " << GET_VAL_UINT(L, 1);
                    TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                }
            }
            return 0;
        }

        int render(SCRIPT_STATE *L) {
            if (Tinta::tintaTexSpringMain::getPtr()->isAborted()) { TROW_ERR_FUNC_CLEAR(L, "Command to abort......"); }

            String text;
            if (IS_VAL_STRING(L, 1))
                text = GET_VAL_STRING(L, 1);

            int posX = 0;
            if (IS_VAL_INTEGER(L, 2))
                posX = GET_VAL_INT(L, 2);

            int posY = 0;
            if (IS_VAL_INTEGER(L, 3))
                posY = GET_VAL_INT(L, 3);

            // default
            int xSpace = 15;
            if (IS_VAL_INTEGER(L, 4))
                xSpace = GET_VAL_INT(L, 4);

            int ySpace = xSpace;
            if (IS_VAL_INTEGER(L, 5))
                ySpace = GET_VAL_INT(L, 5);

            // get selected font
            tintaFont *font = tintaTexSpringMain::getPtr()->getFont();
            if (!font) {
                TROW_ERR_FUNC_CLEAR(L, "Font not selected!");

            }

            image_t *pImage = Tinta::tintaTexSpringMain::getPtr()->getImageStack()->getImgSelEx();
            if (!pImage) {
                TROW_ERR_FUNC_CLEAR(L, "Image not selected!");
            }

            typedef utf8::iterator<std::string::iterator> utf8_string_iterator;

            utf8_string_iterator it(text.begin(), text.begin(), text.end());

            int offsetX = posX;

            int offsetY = posY;

            int offsetXLocal = 0;
            int offsetYLocal = 0;

            for (; it.base() != text.end(); ++it) {

                char buf[7] = {};
                utf8::append(*it, buf);

                String find(buf);
                //auto sizeCh = find.length();

                if (find == _M("\n")) {
                    offsetX = posX;
                    offsetY += offsetYLocal + ySpace;
                }

                if (find == _M(" ")) {
                    offsetX += 5 * xSpace;
                }
                else {

                    auto symbol = font->getSymbolData(find);

                    if (symbol) {
                        pImage->mixImage(&(symbol->mImage), offsetX, offsetY + symbol->vertOffset);
                        offsetXLocal = symbol->mImage.getWidth();
                        offsetYLocal = symbol->mImage.getHeight();
                        offsetX += (offsetXLocal + xSpace);
                    }
                }

            }

            return 0;
        }


}

}

