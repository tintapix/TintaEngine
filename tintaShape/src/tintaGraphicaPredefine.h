/*   Copyright (C) 2011 - 2017 Evdokimov Mikhail
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_GRAPHICA_PREDEFINE_H_
#define TINTA_GRAPHICA_PREDEFINE_H_

#include <tintaConfig.h>
#include <tintaPredefine.h>
#include <Math/tintaQuatern.h>
#include <Math/tintaVector4.h>
#include <tintaCoord2.h>
#include "tintaMemoryAllocator.h"

namespace Graphica {



#ifndef GL_DEBUG
    #define GL_DEBUG
#endif


	using namespace Tinta;

	class tintaRGBAColor;
	class tintaDrawable;
	class tintaGeometry;	    
	class tintaFrameBuffer;
	class tintaCamera;
	class tintaAttributes;	
	class tintaTexture;
	class tintaImage;	
	class tintaVertexBuffer;
	class tintaIndexBuffer;	
	class tintaTriMesh;
	class tintaVisibleSet;	
	class tintaSamplerInformation;
	class tintaGraphicConfig;	
    class imageLayer;
    class tintaBoundingVolume;
    
	// enums	
	enum GeometryType
	{
		GT_POLYPOINT,
		GT_POLYLINE_SEGMENTS,
		GT_POLYLINE_OPEN,
		GT_POLYLINE_CLOSED,
		GT_TRIMESH,
		GT_TRISTRIP,
		GT_TRIFAN,
		GT_MAX_QUANTITY

	};	
    
    enum SBufferDraw {
        STATIC_DRAW,
        DYNAMIC_DRAW,
        MAX_DRAW
    };
    enum FrontMode
	{
		enCCW,  // front faces are counterclockwise ordered
        enCW,   // front faces are clockwise ordered		
	};

    
	enum CullMode
	{
		enFRONT,  // cull front-facing triangles
		enBACK,   // cull back-facing triangles
		enQUANTITY
	};
   
   
	// sampler information
	enum samplerType
	{
		//SAMPLER_1D,       // dimension 1 depricated
		SAMPLER_2D,       // dimension 2
		SAMPLER_3D,       // dimension 3		
		MAX_SAMPLER_TYPES
	};

	enum FilterType
	{
		NEAREST,          // nearest neighbor
		LINEAR,           // linear filtering		
		MAX_FILTER_TYPES
	};

	


	enum imageFormat
	{
        IMG_RGB,
        IMG_RGBA,
        IMG_R,
        IMG_G,
        IMG_B,
        IMG_A,
        IMG_LUMINANCE,
        IMG_FORMAT_QUANTITY
	};

    enum imageComponents {
        COMPONENTS_1 = 1,
        COMPONENTS_2 = 2,
        COMPONENTS_3 = 3,
        COMPONENTS_4 = 4,
    };


    enum imageType
    {
        IMG_UBYTE,
        IMG_UINT,        
        IMG_FLOAT,
        IMG_UNSIGNED_INT_8_8_8_8,
        IMG_TYPE_QUANTITY

    };


    enum EnGeomSide {
        SIDE_BACK = -1,
        SIDE_NOSIDE = 0,
        SIDE_FRONT = 1     
    };  

    enum BVType {
        SphereBV,
        BoxBV
    };

    enum PrimitiveType
    {
        PRIMITIVE_POINT,
        PRIMITIVE_LINE,
        PRIMITIVE_LINE_STRIP,
        PRIMITIVE_POLY,
    };
   
	// structs 
    class  tintaResourceID : public AllocatedMemObject
	{
	public:	

		~tintaResourceID () {}

	protected:
		tintaResourceID () {}
	};

    

    class GLSLProgramID 
        : public tintaResourceID
    {
    public:
        m_uint32 mId;   
    };	

	class  TextureID 
        : public tintaResourceID
	{
	public:
        m_uint32 mId;
		tintaTexture* mTextureObject;
	};

    class  BufferID 
        : public tintaResourceID
    {
    public:
        m_uint32 ID;   
    };

    

    typedef std::list< imageLayer* > imageLayers_t;

    typedef std::list< tintaDrawable* > effectLayers_t;

    typedef effectLayers_t  drawablelist_t;

    enum SceneType{
        Back,      // drawn behind the scene
        Scene,     // scene
        Front,     // drawn in front of the scene
        Camera,    // Camera type
        errorType  // wrong type
    };


    struct tintaTimerScript {
        m_uint32 mIndex;
        m_uint32 mElapsed;
        m_uint32 mLast;
        StringBasic mBuffer;
    };

    struct tintaGLPoint
    {
        tintaVector2f mPoint;
        tintaVector2f mPointWin;
        tintaVector4f mColor;        
    };
    

    typedef std::vector<tintaGLPoint> VecPoints_t;


    struct tintaGLPoints {
        VecPoints_t mPoints;
        float mPointSize;
        PrimitiveType mType;
        String mName;
        bool mVisible;
    };
    

#ifndef GLEW_MX
    //#define GLEW_MX
#endif

    //GLEW_MX
    extern "C" {
        #include "GL/glew.h"
    }


}

#endif