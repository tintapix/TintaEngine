/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaFrameBuffer.h"
#include "GL/tintaGLRenderer.h"

using namespace Graphica;


#if 0
 tintaFrameBuffer::Creator tintaFrameBuffer::ms_aoCreate[Renderer::MAX_RENDERER_TYPES];
 tintaFrameBuffer::Destroyer tintaFrameBuffer::ms_aoDestroy[Renderer::MAX_RENDERER_TYPES];


tintaFrameBuffer* tintaFrameBuffer::Create (FormatType eFormat, DepthType eDepth,   StencilType eStencil, BufferingType eBuffering,   MultisamplingType eMultisampling, Renderer* pkRenderer, Texture* pkTarget)
{

// impl mev

//     Creator oCreator = ms_aoCreate[pkRenderer->GetType()];
//     assert(oCreator);
//     if (oCreator)
//     {
//         return oCreator(eFormat,eDepth,eStencil,eBuffering,eMultisampling,
//             pkRenderer,pkTarget);
//     }
    return 0;
}

void tintaFrameBuffer::Destroy (tintaFrameBuffer* pkBuffer)
{
	// impl mev
//     Renderer* pkRenderer = pkBuffer->GetRenderer();
//     Destroyer oDestroyer = ms_aoDestroy[pkRenderer->GetType()];
//     assert(oDestroyer);
//     if (oDestroyer)
//     {
//         oDestroyer(pkBuffer);
//     }
}

tintaFrameBuffer::tintaFrameBuffer (FormatType eFormat, DepthType eDepth,
    StencilType eStencil, BufferingType eBuffering,
    MultisamplingType eMultisampling, Renderer* pkRenderer, Texture* pkTarget)
{
    mFormat = eFormat;
    mDepth = eDepth;
    mStencil = eStencil;
    mBuffering = eBuffering;
    mMultisampling = eMultisampling;
    m_pRenderer = pkRenderer;
    m_pkTarget = pkTarget;
}

tintaFrameBuffer::~tintaFrameBuffer ()
{
}

#endif