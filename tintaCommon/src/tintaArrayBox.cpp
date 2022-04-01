/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaArrayBox.h"


namespace Tinta {



	tintaArrayBox::tintaArrayBox( void ){
	}

	

	tintaArrayBox::~tintaArrayBox( void ){
	}

	const tintaArray2dFloat_t * tintaArrayBox::getFloatBox() const {
		//assert(false);
		return NULL;
	}

	const tintaArray2dInt_t * tintaArrayBox::getIntBox() const{
		//assert(false);
		return NULL;
	}

	const tintaArray2dInt16_t * tintaArrayBox::getInt16Box() const{
		//assert(false);
		return NULL;
	}

	const tintaArray2dString_t	   * tintaArrayBox::getStringBox()const{
		return NULL;
	}

	tintaArray2dString_t	   * tintaArrayBox::getStringBox(){
		return NULL;
	}

	 tintaArray2dFloat_t * tintaArrayBox::getFloatBox()  {
		//assert(false);
		return NULL;
	}

	 tintaArray2dInt_t * tintaArrayBox::getIntBox() {
		//assert(false);
		return NULL;
	}

	 tintaArray2dInt16_t * tintaArrayBox::getInt16Box() {
		// assert(false);
		 return NULL;
	 }

     tintaArray2dVec3f_t * tintaArrayBox::getVec3fBox() {
         //assert(false);
         return NULL;
     }

     tintaArray2dByte_t * tintaArrayBox::getUInt8Box() {
         //assert(false);
         return NULL;
     }

     const tintaArray2dByte_t * tintaArrayBox::getUInt8Box() const {
         //assert(false);
         return NULL;
     }

     const tintaArray2dVec3f_t * tintaArrayBox::getVec3fBox() const {
         // assert(false);
         return NULL;
     }

     
     const StringBasic& tintaFloatValBox::getName() const {
         static StringBasic name = tintaFloatValBox::str_float32_box;
         return name;

     }

    const char * const tintaFloatValBox::str_float32_box = "float32_box";

	tintaFloatValBox::tintaFloatValBox(){
	}

	tintaFloatValBox::~tintaFloatValBox(){
	}

	void tintaFloatValBox::clear(){
		mBox.clear();
	}

	bool tintaFloatValBox::create( const TExtremValues2i &ex ){
		return mBox.prepare(ex, false);
	}
	bool tintaFloatValBox::create( size_t size ){
		return false; //not supported
	}

	bool tintaFloatValBox::inBoundary( int x, int y) const {
		return mBox.inBoundary(x,y);
	}
	

	bool tintaFloatValBox::getFloatVal(int xpos, int ypos, float &v ) const{
		v = mBox.getVal(xpos,ypos);
		return true;
	}


	bool tintaFloatValBox::setFloatVal( int xpos, int ypos, float v){
		mBox.setVal(xpos,ypos, v);

		return true;
	}

    const StringBasic& tintaFloatBoxFactory::getName(void) const {
        static StringBasic name = tintaFloatValBox::str_float32_box;
		return name;	
	}	

	const tintaArray2dFloat_t * tintaFloatValBox::getFloatBox() const {
		return &mBox;
	}

	size_t tintaFloatValBox::getSize()const {
		return mBox.getSize();
	}

	size_t tintaFloatValBox::height()const{
		return mBox.height();
	}

	size_t tintaFloatValBox::width()const{
		return mBox.width();
	
	}

	tintaArray2dFloat_t * tintaFloatValBox::getFloatBox() {
		return &mBox;
	}

	tintaArrayBox::BoxType tintaFloatValBox::getType()const{
		return enCommonBox;
	}

    size_t tintaFloatValBox::GetBufferSize() const{
       // return mBox.
        return getBufferSize(mBox);
    }

    size_t tintaFloatValBox::GetDataSize() const {
        return mBox.getDataSize();
    }

	void* tintaFloatValBox::GetMemPtr() {
		return mBox.getMemPtrEx();
	}

    size_t tintaFloatValBox::packData(m_int8 *data, size_t offset) const{
                       
        return packArray2d(mBox, data, offset);
    }

    size_t tintaFloatValBox::unpackData(const m_int8 *data, size_t offset) {

        return unpackArray2d(mBox, data, offset);
    }
    

    const StringBasic& tintaIntValBox::getName() const {        
        static StringBasic name = tintaIntValBox::str_int32_box;
        return name;
    }


    const char * const tintaIntValBox::str_int32_box = "int32_box";

	tintaIntValBox::tintaIntValBox(){
	}

	tintaIntValBox::~tintaIntValBox(){
	}

	void tintaIntValBox::clear(){
		mBox.clear();
	}

	bool tintaIntValBox::create( const TExtremValues2i &ex ){
		return mBox.prepare(ex, false);
	}

	bool tintaIntValBox::create( size_t size ){
		return false; //not supported
	}

	bool tintaIntValBox::inBoundary( int x, int y) const {
		return mBox.inBoundary(x,y);
	}


	bool tintaIntValBox::getIntVal(int xpos, int ypos, int &v ) const{
		v = mBox.getVal(xpos,ypos);
		return true;
	}

	size_t tintaIntValBox::getSize()const {
		return mBox.getSize();
	}
	size_t tintaIntValBox::height()const{
		return mBox.height();
	}

	size_t tintaIntValBox::width()const{
		return mBox.width();
	}

	bool tintaIntValBox::setIntVal( int xpos, int ypos, int v){
		mBox.setVal(xpos,ypos, v);

		return true;
	}

	const tintaArray2dInt_t * tintaIntValBox::getIntBox() const{
		return &mBox;
	}

	 tintaArray2dInt_t * tintaIntValBox::getIntBox() {
		return &mBox;
	}


	 tintaArrayBox::BoxType tintaIntValBox::getType()const{
		 return enCommonBox;
	 }


     size_t tintaIntValBox::GetBufferSize() const{
         // return mBox.
         return getBufferSize(mBox);
     }

	 void* tintaIntValBox::GetMemPtr() {
		 return mBox.getMemPtrEx();
	 }

     size_t tintaIntValBox::GetDataSize() const {
         return mBox.getDataSize();
     }

     size_t tintaIntValBox::packData(m_int8 *data, size_t offset) const{
         return packArray2d(mBox, data, offset);
     }

     size_t tintaIntValBox::unpackData(const m_int8 *data, size_t offset) {

         return unpackArray2d(mBox, data, offset);
     }

     const StringBasic& tintaIntBoxFactory::getName(void) const {
        static StringBasic name = tintaIntValBox::str_int32_box;
		return name;	
	}	

    

    const StringBasic& tintaInt16ValBox::getName() const {
        static StringBasic name = tintaInt16ValBox::str_int16_box;
        return name;
    }

    const char * const tintaInt16ValBox::str_int16_box = "int16_box";

	tintaInt16ValBox::tintaInt16ValBox(){
	}

	tintaInt16ValBox::~tintaInt16ValBox(){
	}

	void tintaInt16ValBox::clear(){
		mBox.clear();
	}

	bool tintaInt16ValBox::create( const TExtremValues2i &ex ){
		return mBox.prepare(ex, false);
	}

	bool tintaInt16ValBox::create( size_t size ){
		return false; //not supported
	}

	bool tintaInt16ValBox::inBoundary( int x, int y) const {
		return mBox.inBoundary(x,y);
	}


	bool tintaInt16ValBox::getIntVal(int xpos, int ypos, m_int16 &v ) const{
		v = mBox.getVal(xpos,ypos);
		return true;
	}


	bool tintaInt16ValBox::setIntVal( int xpos, int ypos, m_int16 v){
		mBox.setVal(xpos,ypos, v);

		return true;
	}

	size_t tintaInt16ValBox::getSize()const {
		return mBox.getSize();
	}

	size_t tintaInt16ValBox::height()const{
		return mBox.height();
	}

	size_t tintaInt16ValBox::width()const{
		return mBox.width();
	}

	const tintaArray2dInt16_t * tintaInt16ValBox::getInt16Box() const{
		return &mBox;
	}

	tintaArray2dInt16_t * tintaInt16ValBox::getInt16Box() {
		return &mBox;
	}

	tintaArrayBox::BoxType tintaInt16ValBox::getType()const{
		return enCommonBox;
	}

    size_t tintaInt16ValBox::GetBufferSize() const{
        // return mBox.
        return getBufferSize(mBox);
    }

    size_t tintaInt16ValBox::GetDataSize() const {
        return mBox.getDataSize();

    }
	void* tintaInt16ValBox::GetMemPtr() {
		return mBox.getMemPtrEx();
	}

    size_t tintaInt16ValBox::packData(m_int8 *data, size_t offset) const{
        return packArray2d(mBox, data, offset);

    }

    size_t tintaInt16ValBox::unpackData(const m_int8 *data, size_t offset) {

        return unpackArray2d(mBox, data, offset);
    }




    const StringBasic& tintaInt16tBoxFactory::getName(void) const {
        static StringBasic name = tintaInt16ValBox::str_int16_box;
		return name;	
	}	


	//tintaInt16ValVector

    const StringBasic& tintaInt16ValVector::getName() const {        
        static StringBasic name = tintaInt16ValVector::str_int16_vector;
        return name;
    }
	
    const char *const tintaInt16ValVector::str_int16_vector = "int16_vector";

	tintaInt16ValVector::tintaInt16ValVector(){
	}

	tintaInt16ValVector::~tintaInt16ValVector(){
	}

	void tintaInt16ValVector::clear(){
		mBox.clear();
	}

	bool tintaInt16ValVector::create( const TExtremValues2i &ex ){
		return false; // not supported
	}

	bool tintaInt16ValVector::create( size_t size ){
		if( size == 0 )
			return false;

		return mBox.prepare(0, size - 1, 0, 0, false);
	}

	bool tintaInt16ValVector::inBoundary( int x, int y ) const {
		return mBox.inBoundary(x,0);
	}


	bool tintaInt16ValVector::getIntVal(int xpos, int ypos, m_int16 &v ) const{
		v = mBox.getVal( xpos, 0 );
		return true;
	}

	size_t tintaInt16ValVector::getSize()const {
		return mBox.getSize();
	}

	size_t tintaInt16ValVector::height()const{
		return mBox.height();
	}

	size_t tintaInt16ValVector::width()const{
		return mBox.width();
	}


	bool tintaInt16ValVector::setIntVal( int xpos, int ypos, m_int16 v){
		mBox.setVal(xpos,0, v);

		return true;
	}

	tintaArrayBox::BoxType tintaInt16ValVector::getType()const{
		return enVectorBox;
	}

	const tintaArray2dInt16_t * tintaInt16ValVector::getInt16Box() const{
		return &mBox;
	}

	tintaArray2dInt16_t * tintaInt16ValVector::getInt16Box() {
		return &mBox;
	}

    size_t tintaInt16ValVector::GetBufferSize() const{
        // return mBox.
        return getBufferSize(mBox);
    }

    size_t tintaInt16ValVector::GetDataSize() const {
        return mBox.getDataSize();

    }

	void* tintaInt16ValVector::GetMemPtr() {
		return mBox.getMemPtrEx();
	}

    size_t tintaInt16ValVector::packData(m_int8 *data, size_t offset) const{
        return packArray2d(mBox, data, offset);

    }

    size_t tintaInt16ValVector::unpackData(const m_int8 *data, size_t offset) {

        return unpackArray2d(mBox, data, offset);
    }


    const StringBasic& tintaInt16tVectorFactory::getName(void) const {
        static StringBasic name = tintaInt16ValVector::str_int16_vector;
		return name;	
	}



	//tintaFloatValVector

    const StringBasic& tintaFloatValVector::getName() const {        
        static StringBasic name = tintaFloatValVector::str_floats_vector;
        return name;
    }

    const char *const tintaFloatValVector::str_floats_vector = "float32_vector";

	tintaFloatValVector::tintaFloatValVector(){
	}

	tintaFloatValVector::~tintaFloatValVector(){
	}

	void tintaFloatValVector::clear(){
		mBox.clear();
	}

	bool tintaFloatValVector::create( const TExtremValues2i &ex ){
		return false; // not supported
	}
	   
	bool tintaFloatValVector::create( size_t size ){
		if( size == 0 ){
			mBox.clear();
			return true;
		}

        return mBox.resizeBox(0, size - 1, 0, 0);
	}

	bool tintaFloatValVector::inBoundary( int x, int y ) const {
		return mBox.inBoundary(x,0);
	}

	size_t tintaFloatValVector::getSize()const {
		return mBox.getSize();
	}

	size_t tintaFloatValVector::height()const{
		return mBox.height();
	}

	size_t tintaFloatValVector::width()const{
		return mBox.width();
	}

    

	bool tintaFloatValVector::getFloatVal(int xpos, int ypos, m_float32 &v ) const{
		v = mBox.getVal( xpos, 0 );
		return true;
	}


	bool tintaFloatValVector::setFloatVal( int xpos, int ypos, m_float32 v ){
		mBox.setVal(xpos,0, v);

		return true;
	}

	tintaArrayBox::BoxType tintaFloatValVector::getType()const{
		return enVectorBox;
	}

	const tintaArray2dFloat_t * tintaFloatValVector::getFloatBox() const{
		return &mBox;
	}

	tintaArray2dFloat_t * tintaFloatValVector::getFloatBox() {
		return &mBox;
	}


    const StringBasic& tintaFloatVectorFactory::getName(void) const {
        static StringBasic name = tintaFloatValVector::str_floats_vector;
		return name;	
	}

    size_t tintaFloatValVector::GetBufferSize() const{
        // return mBox.
        return getBufferSize(mBox);
    }

    size_t tintaFloatValVector::GetDataSize() const {
        return mBox.getDataSize();

    }

	void* tintaFloatValVector::GetMemPtr() {
		return mBox.getMemPtrEx();
	}

    size_t tintaFloatValVector::packData(m_int8 *data, size_t offset) const{
        return packArray2d(mBox, data, offset);

    }

    size_t tintaFloatValVector::unpackData(const m_int8 *data, size_t offset) {

        return unpackArray2d(mBox, data, offset);
    }

	//tintaIntValVector

    const StringBasic& tintaIntValVector::getName() const {        
        static StringBasic name = tintaIntValVector::str_int32_vector;
        return name;
    }

    const char * const tintaIntValVector::str_int32_vector = "int32_vector";

    

	tintaIntValVector::tintaIntValVector(){
	}

	tintaIntValVector::~tintaIntValVector(){
	}

	void tintaIntValVector::clear(){
		mBox.clear();
	}

	bool tintaIntValVector::create( const TExtremValues2i &ex ){
		return false; // not supported
	}

	bool tintaIntValVector::create( size_t size ){
		if( size == 0 )
			return false;

		return mBox.prepare(0, size - 1, 0, 0, false);
	}

	bool tintaIntValVector::inBoundary( int x, int y ) const {
		return mBox.inBoundary(x,0);
	}

	size_t tintaIntValVector::getSize()const {
		return mBox.getSize();
	}


	size_t tintaIntValVector::height()const{
		return mBox.height();
	}

	size_t tintaIntValVector::width()const{
		return mBox.width();
	}

	bool tintaIntValVector::getIntVal(int xpos, int ypos, m_int32 &v ) const{
		v = mBox.getVal( xpos, 0 );
		return true;
	}


	bool tintaIntValVector::setIntVal( int xpos, int ypos, m_int32 v){
		mBox.setVal(xpos,0, v);

		return true;
	}

    size_t tintaIntValVector::GetBufferSize() const{
        // return mBox.
        return getBufferSize(mBox);
    }

    size_t tintaIntValVector::GetDataSize() const {
        return mBox.getDataSize();

    }

	void* tintaIntValVector::GetMemPtr() {
		return mBox.getMemPtrEx();
	}

    size_t tintaIntValVector::packData(m_int8 *data, size_t offset) const{
        return packArray2d(mBox, data, offset);

    }

    size_t tintaIntValVector::unpackData(const m_int8 *data, size_t offset) {

        return unpackArray2d(mBox, data, offset);
    }

	tintaArrayBox::BoxType tintaIntValVector::getType()const{
		return enVectorBox;
	}

	const tintaArray2dInt_t * tintaIntValVector::getIntBox() const{
		return &mBox;
	}

	tintaArray2dInt_t * tintaIntValVector::getIntBox() {
		return &mBox;
	}


    const StringBasic& tintaIntVectorFactory::getName(void) const {
        static StringBasic name = tintaIntValVector::str_int32_vector;
		return name;	
	}



   


    ////////////////////// tintaUInt8ValVector

    const char * const tintaUInt8ValVector::str_uint8_vector = "uint8_vector";

    tintaUInt8ValVector::tintaUInt8ValVector() {
    }

    tintaUInt8ValVector::~tintaUInt8ValVector() {
    }

    void tintaUInt8ValVector::clear() {
        mBox.clear();
    }

    bool tintaUInt8ValVector::create(const TExtremValues2i &ex) {
        return false; // not supported
    }

    bool tintaUInt8ValVector::create(size_t size) {
        if (size == 0)
            return false;

        return mBox.prepare(0, size - 1, 0, 0, false);
    }

    bool tintaUInt8ValVector::inBoundary(int x, int y) const {
        return mBox.inBoundary(x, 0);
    }

    size_t tintaUInt8ValVector::getSize()const {
        return mBox.getSize();
    }


    size_t tintaUInt8ValVector::height()const {
        return mBox.height();
    }

    size_t tintaUInt8ValVector::width()const {
        return mBox.width();
    }

    bool tintaUInt8ValVector::getIntVal(int xpos, int ypos, m_int8 &v) const {
        v = mBox.getVal(xpos, 0);
        return true;
    }


    bool tintaUInt8ValVector::setIntVal(int xpos, int ypos, m_int8 v) {
        mBox.setVal(xpos, 0, v);

        return true;
    }

    size_t tintaUInt8ValVector::GetBufferSize() const {
        // return mBox.
        return getBufferSize(mBox);
    }

    size_t tintaUInt8ValVector::GetDataSize() const {
        return mBox.getDataSize();

    }

    void* tintaUInt8ValVector::GetMemPtr() {
        return mBox.getMemPtrEx();
    }

    size_t tintaUInt8ValVector::packData(m_int8 *data, size_t offset) const {
        return packArray2d(mBox, data, offset);

    }

    size_t tintaUInt8ValVector::unpackData(const m_int8 *data, size_t offset) {

        return unpackArray2d(mBox, data, offset);
    }

    tintaArrayBox::BoxType tintaUInt8ValVector::getType()const {
        return enVectorBox;
    }

    const tintaArray2dByte_t * tintaUInt8ValVector::getUInt8Box() const {
        return &mBox;
    }

    tintaArray2dByte_t * tintaUInt8ValVector::getUInt8Box() {
        return &mBox;
    }

    const StringBasic& tintaUInt8ValVector::getName() const {
        static StringBasic name = tintaUInt8ValVector::str_uint8_vector;
        return name;
    }

    const StringBasic& tintaInt8VectorFactory::getName(void) const {
        static StringBasic name = tintaUInt8ValVector::str_uint8_vector;
        return name;
    }

///////////////////////////////////////

	//tintaStringValVector
    const StringBasic& tintaStringValVector::getName() const {
        static StringBasic name = tintaStringValVector::str_strs_vector;
        return name;        
    }

    const char * const tintaStringValVector::str_strs_vector = "str_vector";

	tintaStringValVector::tintaStringValVector(){
	}

	tintaStringValVector::~tintaStringValVector(){
	}

	void tintaStringValVector::clear(){
		mBox.clear();
	}

	bool tintaStringValVector::create( const TExtremValues2i &ex ){
		return false; // not supported
	}

	bool tintaStringValVector::create( size_t size ){
		if( size == 0 )
			return false;

		return mBox.prepare(0, size - 1, 0, 0, false);
	}

	bool tintaStringValVector::inBoundary( int x, int y ) const {
		return mBox.inBoundary(x,0);
	}

	size_t tintaStringValVector::getSize()const {
		return mBox.getSize();
	}

	size_t tintaStringValVector::height()const{
		return mBox.height();
	}

	size_t tintaStringValVector::width()const{
		return mBox.width();
	}

	bool tintaStringValVector::getStringVal(int xpos, int ypos, String &v ) const{
		v = mBox.getVal( xpos, 0 );
		return true;
	}


	bool tintaStringValVector::setStringVal( int xpos, int ypos, const String &v){
		mBox.setVal(xpos,0, v);

		return true;
	}

	tintaArrayBox::BoxType tintaStringValVector::getType()const{
		return enVectorBox;
	}

	const tintaArray2dString_t * tintaStringValVector::getStringBox() const{
		return &mBox;
	}

	tintaArray2dString_t * tintaStringValVector::getStringBox() {
		return &mBox;
	}

    size_t tintaStringValVector::GetBufferSize() const{
        // return mBox.
        return getBufferSizeArray2dStr(mBox);
    }

    size_t tintaStringValVector::GetDataSize() const {

        size_t elements = mBox.getSize();
        size_t size = 0;
        for (size_t i = 0; i < elements; i++)
            size += mBox.getVal(i, 0).size();

        size *= elements;
        
        return size;
    }

	void* tintaStringValVector::GetMemPtr() {
		return mBox.getMemPtrEx();
	}
	

    size_t tintaStringValVector::packData(m_int8 *data, size_t offset) const{
        return packArray2dStr(mBox, data, offset);

    }

    size_t tintaStringValVector::unpackData(const m_int8 *data, size_t offset) {

        return unpackArray2dStr(mBox, data, offset);
    }


    const StringBasic& tintaStringVectorFactory::getName(void) const {
        static StringBasic name = tintaStringValVector::str_strs_vector;
		return name;	
	}

    //---------------------


    const StringBasic& tintaVec3fValVector::getName() const {
        static StringBasic name = tintaVec3fValVector::str_vec3f_vector;
        return name;
    }

    const char *const tintaVec3fValVector::str_vec3f_vector = "vec3f_vector";

    tintaVec3fValVector::tintaVec3fValVector() {
    }

    tintaVec3fValVector::~tintaVec3fValVector() {
    }

    void tintaVec3fValVector::clear() {
        mBox.clear();
    }

    bool tintaVec3fValVector::create(const TExtremValues2i &ex) {
        return false; // not supported
    }

    bool tintaVec3fValVector::create(size_t size) {
        if (size == 0) {
            mBox.clear();
            return true;
        }

        return mBox.resizeBox(0, size - 1, 0, 0);
    }

    bool tintaVec3fValVector::inBoundary(int x, int y) const {
        return mBox.inBoundary(x, 0);
    }

    size_t tintaVec3fValVector::getSize()const {
        return mBox.getSize();
    }

    size_t tintaVec3fValVector::height()const {
        return mBox.height();
    }

    size_t tintaVec3fValVector::width()const {
        return mBox.width();
    }

    tintaVector3f tintaVec3fValVector::getVal(int xpos, int ypos ) const {
        return mBox.getVal(xpos, 0);        
    }


    bool tintaVec3fValVector::setVal(int xpos, int ypos, const tintaVector3f &v) {
        mBox.setVal(xpos, 0, v);

        return true;
    }

    tintaArrayBox::BoxType tintaVec3fValVector::getType()const {
        return enVectorBox;
    }

    const tintaArray2dVec3f_t * tintaVec3fValVector::getVec3fBox() const {
        return &mBox;
    }

    tintaArray2dVec3f_t * tintaVec3fValVector::getVec3fBox() {
        return &mBox;
    }


    size_t tintaVec3fValVector::GetBufferSize() const {
        // return mBox.
        return getBufferSize(mBox);
    }
    size_t tintaVec3fValVector::GetDataSize() const {
        return mBox.getSize() * sizeof(float) * 3;

    }

    void* tintaVec3fValVector::GetMemPtr() {
        return mBox.getMemPtrEx();
    }

    size_t tintaVec3fValVector::packData(m_int8 *data, size_t offset) const {
        return packArray2d(mBox, data, offset);

    }

    size_t tintaVec3fValVector::unpackData(const m_int8 *data, size_t offset) {

        return unpackArray2d(mBox, data, offset);
    }


    const StringBasic& tintaVec3fVectorFactory::getName(void) const {
        static StringBasic name = tintaVec3fValVector::str_vec3f_vector;
        return name;
    }

	
}