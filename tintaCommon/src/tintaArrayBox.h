/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ARRAY_BOX_H_
#define _TINTA_ARRAY_BOX_H_

#include "tintaArrayBoxFactory.h"
#include "tintaArray2d.h"


namespace Tinta {

/*
	Implements map with float values
*/
	class _CoreExport tintaArrayBox:
		public AllocatedObjectBoxContainer,
        public tintaINamed,
        public tintaISerialisable
{
public:

	enum BoxType {
		enCommonBox,
		enVectorBox,
	};
	tintaArrayBox( void );

	//tintaArrayBox( const tintaArrayBox &rVal );

	virtual ~tintaArrayBox( void );

	
	
	virtual const tintaArray2dFloat_t	   *getFloatBox() const;

	virtual const tintaArray2dInt_t		   *getIntBox() const;

	virtual const tintaArray2dInt16_t	   *getInt16Box() const;

	virtual const tintaArray2dString_t	   *getStringBox() const;

    virtual const tintaArray2dVec3f_t	   *getVec3fBox() const;

    virtual const tintaArray2dByte_t	   *getUInt8Box() const;

	virtual  tintaArray2dFloat_t		   *getFloatBox() ;

	virtual  tintaArray2dInt_t			   *getIntBox() ;

	virtual  tintaArray2dInt16_t		   *getInt16Box();

	virtual  tintaArray2dString_t		   *getStringBox();

    virtual  tintaArray2dVec3f_t		   *getVec3fBox();

    virtual  tintaArray2dByte_t		       *getUInt8Box();

    
// pure virtual functions
	virtual void clear() = 0;
	// resizing, using extrems for common boxes
	virtual bool create( const TExtremValues2i &ex ) = 0;
	// resizing, using new size for vector boxes
	virtual bool create( size_t size ) = 0;

	virtual BoxType getType()const = 0;


	virtual size_t getSize()const = 0;

	virtual size_t height()const = 0;

	virtual size_t width()const = 0;

	virtual bool inBoundary( int x, int y ) const = 0;

    // from tintaISerialisable
    virtual size_t GetBufferSize() const = 0;

    virtual size_t GetDataSize() const = 0;

	virtual void* GetMemPtr() = 0;

    virtual size_t packData(m_int8 *data, size_t offset) const = 0;

    virtual size_t		   unpackData(const m_int8 *data, size_t offset) = 0;


};

	class _CoreExport tintaFloatValBox
		: public tintaArrayBox {
	public:
		tintaFloatValBox();

		~tintaFloatValBox();

		virtual void clear();

		virtual bool create( const TExtremValues2i &ex );

		virtual bool create( size_t size );

		virtual bool inBoundary(  int x, int y ) const;

		virtual bool getFloatVal( int xpos, int ypos,float &v) const;

		virtual bool setFloatVal( int xpos, int ypos, float v);

		virtual BoxType getType()const;

		virtual size_t getSize()const;

		virtual size_t height()const;

		virtual size_t width()const;

		virtual const tintaArray2dFloat_t * getFloatBox() const;

		virtual		  tintaArray2dFloat_t * getFloatBox() ;

        virtual const StringBasic& getName() const;

        // from tintaISerialisable
        virtual size_t GetBufferSize() const;

        virtual size_t GetDataSize() const;

		virtual void* GetMemPtr();

        virtual size_t packData(m_int8 *data, size_t offset) const;

        virtual size_t		   unpackData(const m_int8 *data, size_t offset);
        
        static const char * const str_float32_box;
	private:
		tintaArray2dFloat_t mBox;
        // common box with float values    
        

	};

	class _CoreExport tintaFloatBoxFactory: 
		public tintaArrayBoxObjFactory {
	public:
		tintaFloatBoxFactory(){}
		virtual ~tintaFloatBoxFactory(){}

        const StringBasic& getName(void) const;

		tintaArrayBox *createInstance(  ) const {
			return M_NEW tintaFloatValBox ();
		}	
		void releaseInstance( tintaArrayBox * obj ) {
			M_DELETE obj;
		}	
	};


	class _CoreExport tintaIntValBox
		: public tintaArrayBox {
	public:
		tintaIntValBox();

		~tintaIntValBox();

		virtual void clear();

		virtual bool create( const TExtremValues2i &ex );

		virtual bool create( size_t size );

		virtual bool inBoundary( int x, int y ) const;

		virtual bool getIntVal( int xpos, int ypos, int &v) const;

		virtual bool setIntVal( int xpos, int ypos, int v);

		virtual BoxType getType()const;

		virtual size_t getSize()const;

		virtual size_t height()const;

		virtual size_t width()const;

		virtual const tintaArray2dInt_t * getIntBox() const;

		virtual tintaArray2dInt_t * getIntBox();

        virtual const StringBasic& getName() const;

        // from tintaISerialisable
        virtual size_t GetBufferSize() const;

        virtual size_t GetDataSize() const;

		virtual void* GetMemPtr();

        virtual size_t packData(m_int8 *data, size_t offset) const;

        virtual size_t		   unpackData(const m_int8 *data, size_t offset);

        static const char * const str_int32_box;

	private:
		tintaArray2dInt_t mBox;
	};



class _CoreExport tintaIntBoxFactory: 
	public tintaArrayBoxObjFactory {
public:
	tintaIntBoxFactory(){}
	virtual ~tintaIntBoxFactory(){}

    const StringBasic& getName(void) const;

 	tintaArrayBox *createInstance(  ) const {
 		return M_NEW tintaIntValBox ();
 	}	
	void releaseInstance( tintaArrayBox * obj ) {
		M_DELETE obj;
	}	
};


class _CoreExport tintaInt16ValBox
	: public tintaArrayBox {
public:
	tintaInt16ValBox();

	~tintaInt16ValBox();

	virtual void clear();

	virtual bool create( const TExtremValues2i &ex );

	virtual bool create( size_t size );

	virtual bool inBoundary( int x, int y ) const;

	virtual bool getIntVal( int xpos, int ypos, m_int16 &v) const;

	virtual bool setIntVal( int xpos, int ypos, m_int16 v);

	virtual BoxType getType()const;

	virtual size_t  getSize()const;

	virtual size_t  height()const;

	virtual size_t  width()const;

	virtual const tintaArray2dInt16_t * getInt16Box() const;

	virtual		  tintaArray2dInt16_t * getInt16Box();

    virtual const StringBasic& getName() const;

    // from tintaISerialisable
    virtual size_t GetBufferSize() const;

    virtual size_t GetDataSize() const;

	virtual void* GetMemPtr();

    virtual size_t packData(m_int8 *data, size_t offset) const;

    virtual size_t		   unpackData(const m_int8 *data, size_t offset);

    static const char * const str_int16_box;   

private:
	tintaArray2dInt16_t mBox;
};



class _CoreExport tintaInt16tBoxFactory: 
	public tintaArrayBoxObjFactory {
public:
	tintaInt16tBoxFactory(){}
	virtual ~tintaInt16tBoxFactory(){}

    const StringBasic& getName(void) const;

	tintaArrayBox *createInstance(  ) const {
		return M_NEW tintaInt16ValBox ();
	}	
	void releaseInstance( tintaArrayBox * obj ) {
		M_DELETE obj;
	}	
};


class _CoreExport tintaInt16ValVector
	: public tintaArrayBox {
public:
	tintaInt16ValVector();

	~tintaInt16ValVector();

	virtual void clear();

	virtual bool create( const TExtremValues2i &ex );

	virtual bool create( size_t size );

	virtual bool inBoundary( int x, int y ) const;

	virtual bool getIntVal( int xpos, int ypos, m_int16 &v) const;

	virtual bool setIntVal( int xpos, int ypos, m_int16 v);

	virtual BoxType getType()const;

	virtual size_t getSize()const;

	virtual size_t height()const;

	virtual size_t width()const;

	virtual const tintaArray2dInt16_t * getInt16Box() const;

	virtual		  tintaArray2dInt16_t * getInt16Box();

    virtual const StringBasic& getName() const;

    // from tintaISerialisable
    virtual size_t GetBufferSize() const;

    virtual size_t GetDataSize() const;

	virtual void* GetMemPtr();

    virtual size_t packData(m_int8 *data, size_t offset) const;

    virtual size_t		   unpackData(const m_int8 *data, size_t offset);

    static const char * const str_int16_vector;

private:
	tintaArray2dInt16_t mBox;
};



class _CoreExport tintaInt16tVectorFactory: 
	public tintaArrayBoxObjFactory {
public:
	tintaInt16tVectorFactory(){}
	virtual ~tintaInt16tVectorFactory(){}

    const StringBasic& getName(void) const;

	tintaArrayBox *createInstance(  ) const {
		return M_NEW tintaInt16ValVector ();
	}	
	void releaseInstance( tintaArrayBox * obj ) {
		M_DELETE obj;
	}	
};


//tintaFloatValVector
class _CoreExport tintaFloatValVector
	: public tintaArrayBox {
public:
	tintaFloatValVector();

	~tintaFloatValVector();

	virtual void clear();

	virtual bool create( const TExtremValues2i &ex );

	virtual bool create( size_t size );

	virtual bool inBoundary( int x, int y ) const;

	virtual bool getFloatVal( int xpos, int ypos, m_float32 &v) const;

	virtual bool setFloatVal( int xpos, int ypos, m_float32 v);

	virtual BoxType getType()const;

	virtual size_t getSize()const;

	virtual size_t height()const;

	virtual size_t width()const;   

	virtual const tintaArray2dFloat_t * getFloatBox() const;

	virtual		  tintaArray2dFloat_t * getFloatBox();

    virtual const StringBasic& getName() const;

    // from tintaISerialisable
    virtual size_t GetBufferSize() const;

    virtual size_t GetDataSize() const;

	virtual void* GetMemPtr();

    virtual size_t packData(m_int8 *data, size_t offset) const;

    virtual size_t		   unpackData(const m_int8 *data, size_t offset);

    static const char *const str_floats_vector;

private:
	tintaArray2dFloat_t mBox;
};



class _CoreExport tintaFloatVectorFactory: 
	public tintaArrayBoxObjFactory {
public:
	tintaFloatVectorFactory(){}
	virtual ~tintaFloatVectorFactory(){}

    const StringBasic& getName(void) const;

	tintaArrayBox *createInstance(  ) const {
		return M_NEW tintaFloatValVector ();
	}	
	void releaseInstance( tintaArrayBox * obj ) {
		M_DELETE obj;
	}	
};


//tintaIntValVector
class _CoreExport tintaIntValVector
	: public tintaArrayBox {
public:
	tintaIntValVector();

	~tintaIntValVector();

	virtual void clear();

	virtual bool create( const TExtremValues2i &ex );

	virtual bool create( size_t size );

	virtual bool inBoundary( int x, int y ) const;

	virtual bool getIntVal( int xpos, int ypos, m_int32 &v) const;

	virtual bool setIntVal( int xpos, int ypos, m_int32 v);

	virtual BoxType getType()const;

	virtual size_t getSize()const;

	virtual size_t height()const;

	virtual size_t width()const;

	virtual const tintaArray2dInt_t * getIntBox() const;

	virtual		  tintaArray2dInt_t * getIntBox();

    virtual const StringBasic& getName() const;

    // from tintaISerialisable
    virtual size_t GetBufferSize() const;

    virtual size_t GetDataSize() const;

	virtual void* GetMemPtr();

    virtual size_t packData(m_int8 *data, size_t offset) const;

    virtual size_t		   unpackData(const m_int8 *data, size_t offset);

    static const char * const str_int32_vector;

private:
	tintaArray2dInt_t mBox;
};

//tintaIntValVector
class _CoreExport tintaUInt8ValVector
    : public tintaArrayBox {
public:
    tintaUInt8ValVector();

    ~tintaUInt8ValVector();

    virtual void clear();

    virtual bool create(const TExtremValues2i &ex);

    virtual bool create(size_t size);

    virtual bool inBoundary(int x, int y) const;

    virtual bool getIntVal(int xpos, int ypos, m_int8 &v) const;

    virtual bool setIntVal(int xpos, int ypos, m_int8 v);

    virtual BoxType getType()const;

    virtual size_t getSize()const;

    virtual size_t height()const;

    virtual size_t width()const;

    virtual const tintaArray2dByte_t * getUInt8Box() const;

    virtual		  tintaArray2dByte_t * getUInt8Box();

    virtual const StringBasic& getName() const;

    // from tintaISerialisable
    virtual size_t GetBufferSize() const;

    virtual size_t GetDataSize() const;

    virtual void* GetMemPtr();

    virtual size_t packData(m_int8 *data, size_t offset) const;

    virtual size_t		   unpackData(const m_int8 *data, size_t offset);

    static const char * const str_uint8_vector;

private:
    tintaArray2dByte_t mBox;
};


class _CoreExport tintaInt8VectorFactory :
    public tintaArrayBoxObjFactory {
public:
    tintaInt8VectorFactory() {}
    virtual ~tintaInt8VectorFactory() {}

    const StringBasic& getName(void) const;

    tintaArrayBox *createInstance() const {
        return M_NEW tintaUInt8ValVector();
    }
    void releaseInstance(tintaArrayBox * obj) {
        M_DELETE obj;
    }
};



class _CoreExport tintaIntVectorFactory: 
	public tintaArrayBoxObjFactory {
public:
	tintaIntVectorFactory(){}
	virtual ~tintaIntVectorFactory(){}

	const StringBasic& getName( void ) const;

	tintaArrayBox *createInstance(  ) const {
		return M_NEW tintaIntValVector ();
	}	
	void releaseInstance( tintaArrayBox * obj ) {
		M_DELETE obj;
	}	
};


//tintaIntValVector
class _CoreExport tintaStringValVector
	: public tintaArrayBox { 
public:
	tintaStringValVector();

	~tintaStringValVector();

	virtual void clear();

	virtual bool create( const TExtremValues2i &ex );

	virtual bool create( size_t size );

	virtual bool inBoundary( int x, int y ) const;

	virtual bool getStringVal( int xpos, int ypos, String &v) const;

	virtual bool setStringVal( int xpos, int ypos, const String &v);

	virtual BoxType getType()const;

	virtual size_t getSize()const;

	virtual size_t height()const;

	virtual size_t width()const;

	virtual const tintaArray2dString_t * getStringBox() const;

	virtual		  tintaArray2dString_t * getStringBox();

    virtual const StringBasic& getName() const;

    // from tintaISerialisable
    virtual size_t GetBufferSize() const;

    virtual size_t GetDataSize() const;

	virtual void* GetMemPtr();

    virtual size_t packData(m_int8 *data, size_t offset) const;

    virtual size_t		   unpackData(const m_int8 *data, size_t offset);

    static const char * const str_strs_vector;

private:
	tintaArray2dString_t mBox;
};



class _CoreExport tintaStringVectorFactory: 
	public tintaArrayBoxObjFactory {
public:
	tintaStringVectorFactory(){}
	virtual ~tintaStringVectorFactory(){}

	const StringBasic& getName( void ) const;

	tintaArrayBox *createInstance(  ) const {
		return M_NEW tintaStringValVector ();
	}	
	void releaseInstance( tintaArrayBox * obj ) {
		M_DELETE obj;
	}	
};



//tintaVec3fValVector
class _CoreExport tintaVec3fValVector
    : public tintaArrayBox {
public:
    tintaVec3fValVector();

    ~tintaVec3fValVector();

    virtual void clear();

    virtual bool create(const TExtremValues2i &ex);

    virtual bool create(size_t size);

    virtual bool inBoundary(int x, int y) const;

    virtual tintaVector3f getVal(int xpos, int ypos) const;

    virtual bool setVal(int xpos, int ypos, const tintaVector3f &v);

    virtual BoxType getType()const;

    virtual size_t getSize()const;

    virtual size_t height()const;

    virtual size_t width()const;

    virtual const tintaArray2dVec3f_t * getVec3fBox() const;

    virtual		  tintaArray2dVec3f_t * getVec3fBox();

    virtual const StringBasic& getName() const;

    // from tintaISerialisable
    virtual size_t GetBufferSize() const;

    virtual size_t GetDataSize() const;

    virtual void* GetMemPtr();

    virtual size_t packData(m_int8 *data, size_t offset) const;

    virtual size_t		   unpackData(const m_int8 *data, size_t offset);

    static const char *const str_vec3f_vector;

private:
    tintaArray2dVec3f_t mBox;
};



class _CoreExport tintaVec3fVectorFactory :
    public tintaArrayBoxObjFactory {
public:
    tintaVec3fVectorFactory() {}
    virtual ~tintaVec3fVectorFactory() {}

    const StringBasic& getName(void) const;

    tintaVec3fValVector *createInstance() const {
        return M_NEW tintaVec3fValVector();
    }
    void releaseInstance(tintaArrayBox * obj) {
        M_DELETE obj;
    }
};



}

#endif