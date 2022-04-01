/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_FRAME_BUFFER_H_
#define _TINTA_FRAME_BUFFER_H_

namespace Graphica
{

class Renderer;
class Texture;

class _CoreExport tintaFrameBuffer
{
public:
    enum FormatType
    {
        FT_FORMAT_NONE,
        FT_FORMAT_RGB,
        FT_FORMAT_RGBA,
        FT_FORMAT_DEPTH
    };

    enum DepthType
    {
        DT_DEPTH_NONE,
        DT_DEPTH_16,
        DT_DEPTH_24,
        DT_DEPTH_32
    };

    enum StencilType
    {
        ST_STENCIL_NONE,
        ST_STENCIL_8
    };

    enum BufferingType
    {
        BT_BUFFERED_SINGLE,
        BT_BUFFERED_DOUBLE
    };

    enum MultisamplingType
    {
        MT_SAMPLING_NONE,
        MT_SAMPLING_2,
        MT_SAMPLING_4
    };

   
    static tintaFrameBuffer* Create (FormatType eFormat, DepthType eDepth,
        StencilType eStencil, BufferingType eBuffering,
        MultisamplingType eMultisampling, Renderer* pkRenderer,
        Texture* pkTarget);

    static void Destroy (tintaFrameBuffer* pkBuffer);

    tintaFrameBuffer (FormatType eFormat, DepthType eDepth, StencilType eStencil,
        BufferingType eBuffering, MultisamplingType eSampling,
        Renderer* pkRenderer, Texture* pkTarget);

    virtual ~tintaFrameBuffer ();

    // Member access.
    FormatType GetFormatType () const;
    DepthType GetDepthType () const;
    StencilType GetStencilType () const;
    BufferingType GetBufferingType () const;
    MultisamplingType GetMultisamplingType () const;
    Renderer* GetRenderer ();
    Texture* GetTarget ();

    virtual void Enable () = 0;
    virtual void Disable () = 0;
    virtual void CopyToTexture (bool bFillImage) = 0;

protected:
    FormatType mFormat;
    DepthType mDepth;
    StencilType mStencil;
    BufferingType mBuffering;
    MultisamplingType mMultisampling;
    Renderer* m_pRenderer;
    Texture* m_pkTarget;
   
    typedef tintaFrameBuffer* (*Creator)(FormatType, DepthType, StencilType,
        BufferingType, MultisamplingType, Renderer*, Texture*);
    typedef void (*Destroyer)(tintaFrameBuffer*);
    static Creator ms_aoCreate[];
    static Destroyer ms_aoDestroy[];
};



inline tintaFrameBuffer::FormatType tintaFrameBuffer::GetFormatType () const
{
	return mFormat;
}

inline tintaFrameBuffer::DepthType tintaFrameBuffer::GetDepthType () const
{
	return mDepth;
}

inline tintaFrameBuffer::StencilType tintaFrameBuffer::GetStencilType () const
{
	return mStencil;
}

inline tintaFrameBuffer::BufferingType tintaFrameBuffer::GetBufferingType () const
{
	return mBuffering;
}

inline tintaFrameBuffer::MultisamplingType tintaFrameBuffer::GetMultisamplingType ()
const
{
	return mMultisampling;
}

inline Renderer* tintaFrameBuffer::GetRenderer ()
{
	return m_pRenderer;
}

inline Texture* tintaFrameBuffer::GetTarget ()
{
	return m_pkTarget;
}



}

#endif
