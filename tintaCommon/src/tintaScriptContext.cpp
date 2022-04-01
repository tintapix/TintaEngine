/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaScriptContext.h"
#include "tintaCommon.h"
#include "tintaException.h"
#include "Math/tintaCommonMath.h"

namespace Tinta
{


tintaScriptContext::tintaScriptContext(void)
    :mpLua(NULL_M) {
}

tintaScriptContext::~tintaScriptContext(void){
	if(mpLua){
		lua_close(mpLua);
        mpLua = NULL_M;
	}
}

void tintaScriptContext::createState() {
	if(mpLua){
		lua_close(mpLua);
        mpLua = NULL_M;
	}

	mpLua = luaL_newstate(); /* opens Lua */
	luaL_openlibs(mpLua); /* opens the standard libraries */
    CoreAssert(mpLua, "mpLua==NULL");
    if(!mpLua)
        return;
  

}
void tintaScriptContext::closeState() {
	if(mpLua){
		lua_close(mpLua);
        mpLua = NULL_M;
	}
	

}
void tintaScriptContext::setState(lua_State* new_state){
	if(mpLua)
		lua_close(mpLua);
	mpLua = new_state;
}
const lua_State *tintaScriptContext::getState()const{
	return mpLua;
}
lua_State *tintaScriptContext::getStateEx(){
	return mpLua;
}
const String *tintaScriptContext::getErrors( size_t &error_count )const {
	error_count = mErrors.size();
	if(error_count){
		error_count = mErrors.size();
		return &mErrors[0];
	}
	return 0;
}
bool tintaScriptContext::executeBuffer( const char* buffer , size_t len){
	
	if( !buffer || len ==0 ){		
		return true; // do nothing
	}

	if( !mpLua )
		createState();		

    int error = luaL_loadbuffer(mpLua, buffer, len, "line");// ||	
	if( !error )
		error = lua_pcall(mpLua, 0, LUA_MULTRET, 0);

	if ( error ) {

		StringBasic error_buf = lua_tostring( mpLua, -1 ); //sprintf_s(error_buf, "%s", lua_tostring(mpLua, -1));
        mErrors.push_back(toWideChar(error_buf));
		lua_pop(mpLua, 1); 
		return false;
	}
	return true;

}
bool tintaScriptContext::executeFile( const String &file_path ){
	
	if( !isFile( file_path ) )
		return false;

	if( !mpLua )
		createState();	

    StringBasic buff;
    
    bool rez = readUTF8Text(file_path, buff);
   
    if( !rez ) {
        StringUtil::StrStreamType os;
        os << _M("File read error for: ");
        os << file_path;

        mErrors.push_back(os.str());
   
        return false;
    }

	return executeBuffer( buff.c_str(), buff.length() );	
}


bool tintaScriptContext::getGlobVar(int &i_value, const char* var_name, const String &file_path){
	
	//char error_buf[256];

	if( file_path.length() > 0 )
		setFile( file_path) ;
	
	lua_getglobal(mpLua, var_name);	
	if ( !IS_VAL_INTEGER(mpLua, -1) ){	
		
		StringUtil::StrStreamType os;
		os << var_name;
		os << _M(" should be a integer\n") ;

		mErrors.push_back( os.str() );
		return false;
	}
	i_value = GET_VAL_INT(mpLua, -1);
	lua_pop(mpLua, 1);
	return true;
	
}

bool tintaScriptContext::getGlobVar( m_uint32 &ui_value,const char *var_name,const String &file_path ) {

	if( file_path.length() > 0 )
		setFile( file_path) ;

	lua_getglobal(mpLua, var_name);	
	if (!IS_VAL_INTEGER(mpLua, -1)){

		//sprintf(error_buf, "%s should be a integer\n", var_name);
		StringUtil::StrStreamType os;
		os << var_name;
		os << _M(" should be a integer\n") ;

		mErrors.push_back( os.str() );
		return false;
	}
	ui_value = GET_VAL_UINT(mpLua, -1);
	lua_pop(mpLua, 1);
	return true;

}

bool tintaScriptContext::getGlobVar(bool &b_value,const char* var_name, const String &file_path){
	
	//char error_buf[256];
	
    if( file_path.length() != 0 )
	    setFile( file_path);

	lua_getglobal(mpLua, var_name);	
	if ( !IS_VAL_BOOL(mpLua, -1) ){
		
		StringUtil::StrStreamType os;

		os << var_name;
		os << _M(" should be a boolean\n") ;

		mErrors.push_back( os.str() );

		return false;
	}
	b_value = GET_VAL_BOOL(mpLua, -1);
	lua_pop(mpLua, 1);
	return true;
}

bool tintaScriptContext::getGlobVar(double &d_value,const char* var_name, const String &file_path){
	
//	char error_buf[256];
	if( file_path.length() > 0 )
		setFile(file_path);

	lua_getglobal(mpLua, var_name);	
	if ( !IS_VAL_REAL(mpLua, -1) ){

		StringUtil::StrStreamType os;
		os << var_name;
		os << _M(" should be a double\n") ;

		mErrors.push_back( os.str() );

		return false;
	}
	d_value = GET_VAL_DOUBLE(mpLua, -1);
	lua_pop(mpLua, 1);
	return true;

}


bool tintaScriptContext::getGlobVar(String &str_value, const char* var_name, const String &file_path){

	//char error_buf[256];

	if (file_path.length() > 0)
		setFile(file_path);


	lua_getglobal(mpLua, var_name);

	if (!IS_VAL_STRING(mpLua, -1))
	{

		StringUtil::StrStreamType os;
		os << var_name;
		os << _M(" should be a string\n");

		mErrors.push_back(os.str());
		// 		sprintf(error_buf, "%s should be a double\n", var_name);
		// 		mErrors.push_back(String(error_buf));
		return false;

	}
    str_value = toWideChar(lua_tostring(mpLua, -1));
	lua_pop(mpLua, 1);
	return true;

}

void tintaScriptContext::getField (int &i_value, const char *key) {
	
	//char error_buf[256];
	lua_pushstring(mpLua, key);
	lua_gettable(mpLua, -2);  /* get background[key] */
	if (!IS_VAL_INTEGER(mpLua, -1))
	{
		StringUtil::StrStreamType os;
		os << key;
		os << _M(" should be a integer\n") ;

		mErrors.push_back( os.str() );

// 		sprintf(error_buf, "%s should be a integer\n", key);
// 		mErrors.push_back(String(error_buf));
		return;
	}
	i_value = GET_VAL_INT(mpLua, -1);
	lua_pop(mpLua, 1);  /* remove number */	
}

void tintaScriptContext::getField (double &d_value, const char *key) {

	//char error_buf[256];
	lua_pushstring(mpLua, key);
	lua_gettable(mpLua, -2);  /* get background[key] */
	if (!IS_VAL_REAL(mpLua, -1))
	{
		StringUtil::StrStreamType os;
		os << key;
		os << _M(" should be a double\n") ;

		mErrors.push_back( os.str() );

// 		sprintf(error_buf, "%s should be a double\n", key);
// 		mErrors.push_back(String(error_buf));
		return;
	}
	d_value = GET_VAL_DOUBLE(mpLua, -1);
	lua_pop(mpLua, 1);  /* remove number */	
}

void tintaScriptContext::getField (bool &b_value, const char *key) {

	//char error_buf[256];
	lua_pushstring(mpLua, key);
	lua_gettable(mpLua, -2);  /* get background[key] */
	if (!IS_VAL_BOOL(mpLua, -1))
	{
		StringUtil::StrStreamType os;
		os << key;
		os << _M(" should be a bool\n") ;

		mErrors.push_back( os.str() );

// 		sprintf(error_buf, "%s should be a bool\n", key);
// 		mErrors.push_back(String(error_buf));
		return;
	}
	b_value = GET_VAL_BOOL(mpLua, -1);
	lua_pop(mpLua, 1);  /* remove number */	
}

void tintaScriptContext::getField (String &str_value, const char *key) {

	//char error_buf[256];
	lua_pushstring(mpLua, key);
	lua_gettable(mpLua, -2);  /* get background[key] */
	if (!IS_VAL_STRING(mpLua, -1))
	{
		StringUtil::StrStreamType os;
		os << key;
		os << _M(" should be a string\n") ;

		mErrors.push_back( os.str() );
// 		sprintf(error_buf, "%s should be a string\n", key);
// 		mErrors.push_back(string(error_buf));
		return;
	}
    str_value = toWideChar(lua_tostring(mpLua, -1));
	lua_pop(mpLua, 1);  /* remove number */	
}

// template<typename T> bool tintaScriptContext::getTabVar(T& table_value, const char *table_name, const char *variable_name, const char* file_path)
// {
// 	if(!mpLua)
// 		createState();
// 	char error_buf[256];
// 
// 	if(!setFile(mpLua, file_path)) return false;	
// 
// 	lua_getglobal(mpLua, table_name);
// 	if (!lua_istable(mpLua, -1))
// 	{
// 		sprintf(error_buf, "%s is not a valid table\n", table_name);
// 		mErrors.push_back(string(error_buf));
// 		return false;
// 	}	
// 	getField(table_value, variable_name);	
// 	return true;
// }
// template<typename T>
// bool tintaScriptContext::getTabVar(double &d_value, const char *table_name, const char *variable_name, const char* file_path)
// {
// 	if(!mpLua)
// 		createState();
// 	char error_buf[256];
// 
// 	if(!setFile(mpLua, file_path)) return false;	
// 
// 	lua_getglobal(mpLua, table_name);
// 	if (!lua_istable(mpLua, -1))
// 	{
// 		sprintf(error_buf, "%s is not a valid table\n", table_name);
// 		mErrors.push_back(string(error_buf));
// 		return false;
// 	}
// 
// 	//sprintf("`background' is not a valid color table");
// 
// 
// 	getField(d_value, variable_name);
// 
// 
// 	//str_value = lua_tostring(mpLua, -1);
// 	//lua_pop(mpLua, 1);
// 	return true;
// }
// bool tintaScriptContext::getTabVar(bool &b_value, const char *table_name, const char *variable_name, const char* file_path)
// {
// 	if(!mpLua)
// 		createState();
// 	char error_buf[256];
// 
// 	if(!setFile(mpLua, file_path)) return false;	
// 
// 	lua_getglobal(mpLua, table_name);
// 	if (!lua_istable(mpLua, -1))
// 	{
// 		sprintf(error_buf, "%s is not a valid table\n", table_name);
// 		mErrors.push_back(string(error_buf));
// 		return false;
// 	}
// 
// 	//sprintf("`background' is not a valid color table");
// 
// 
// 	getField(b_value, variable_name);
// 
// 
// 	//str_value = lua_tostring(mpLua, -1);
// 	//lua_pop(mpLua, 1);
// 	return true;
// }
// 
// bool tintaScriptContext::getTabVar(String &str_value, const char *table_name, const char *variable_name, const char* file_path)
// {
// 	if(!mpLua)
// 		createState();
// 	char error_buf[256];
// 
// 	if(!setFile(mpLua, file_path)) return false;	
// 
// 	lua_getglobal(mpLua, table_name);
// 	if (!lua_istable(mpLua, -1))
// 	{
// 		sprintf(error_buf, "%s is not a valid table\n", table_name);
// 		mErrors.push_back(string(error_buf));
// 		return false;
// 	}
// 
// 	//sprintf("`background' is not a valid color table");
// 
// 
// 	getField(str_value, variable_name);
// 
// 
// 	//str_value = lua_tostring(mpLua, -1);
// 	//lua_pop(mpLua, 1);
// 	return true;
// }

void tintaScriptContext::setFile(const String &str_file_path){

	if( str_file_path.length() !=0 ){			
			//error = luaL_loadfile( mpLua, m_str_file_path.c_str() );

        if( !executeFile( str_file_path ) ){
            if( mErrors.size() > 0 ){
                StringUtil::StrStreamType os;
                os << mErrors.at(0);
                CoreAssert( false, os.str().c_str() );
            }
            else {
                CoreAssert( false,"executeFile( str_file_path ) == false" );
            }
        }
	}
}

void tintaScriptContext::resetErrors()
{
	mErrors.clear();
}
bool tintaScriptContext::hasErrors() const
{
	return mErrors.size()>0;
}

bool tintaScriptContext::getArrayField(int &i_value, const char *table_name, int index_value,  const String &file_path ) 
{
	//int result=0;
	//char error_buf[256];
	//assert(index_value > 0);


    if( file_path.length() > 0 )
	    setFile( file_path);

	if(!testTable(mpLua,table_name, index_value))
		return false;
	lua_rawgeti(mpLua, 1, index_value);
	if (!IS_VAL_INTEGER(mpLua, -1))
	{		
		mErrors.push_back(String(_M("error number in table") ));
		return false;
	}

	i_value = GET_VAL_INT(mpLua, -1);

	lua_pop(mpLua, 2); //remove value & table	
	
	return true;
}
// const char* tintaScriptContext::lua_stringexpr( const char* expr,   const char* def ) 
// {  
//    const char* r = def ;  
//    char buf[256];// = _M("") ;
// 
//    /* Assign the Lua expression to a Lua global variable. */  
//    sprintf( buf, "evalExpr=%s", expr );  
//    if ( !luaL_dostring( mpLua, buf ) ) {  
// 	   /* Get the value of the global varibable */  
// 	   lua_getglobal( mpLua, "evalExpr" );  
// 
// 	   bool is_tab = lua_istable(mpLua, -1);
// 	   lua_rawgeti(mpLua, 1, 1);
// 	   const char* val = GET_VAL_STRING(mpLua, -1);
// 		
// 	   /* remove lua_getglobal value and  after lua_rawgeti*/  
// 	   lua_pop( mpLua, 2 );  
//    }  
//    return r ;  
// }

bool tintaScriptContext::getArrayExp(t_int_array    &int_vec_value, const char *expression,  const String &file_path)
{

//	cout<<expression;
	if(file_path.length() > 0)
		setFile( file_path);


	//char buf[BUF_COMMON_SIZE];	

	StringStreamBasic os;
	os << "key=";
	os << expression ;

	//sprintf( buf, "key=%s",expression ); 	
	if( executeBuffer( os.str().c_str(), os.str().length() ) ){
		/* Get the value of the global varibable */  
		lua_getglobal( mpLua, "key" );  		

		if( !lua_istable(mpLua, -1) ){
			lua_pop( mpLua, 1 );  

            StringUtil::StrStreamType oser;
            oser << _M("Wrong table in the expression \n") ;
            oser << expression;
            mErrors.push_back(oser.str() );

			return false;
		}

		size_t array_size = lua_rawlen(mpLua, 1);
		if(array_size == 0)	{
			lua_pop( mpLua, 1 );  
			return false;
		}

		for(size_t count  = 1; count <= array_size; count++){
            lua_rawgeti(mpLua, 1, (int)count);
			int_vec_value.push_back(GET_VAL_INT(mpLua, -1));

		}	
		// remove all values
		lua_pop( mpLua, (int)(array_size) + 1 );  
		return true;
	}  
	return false;

}

bool tintaScriptContext::getExp(bool   &bValue, const char *expression,  const String &file_path) {

	
	if(file_path.length())
		setFile( file_path);


	StringStreamBasic os;
	os << "key=";
	os << expression ;

	//sprintf( buf, "key=%s",expression ); 	
	if( executeBuffer( os.str().c_str(), os.str().length() ) ){
		/* Get the value of the global varibable */  
		lua_getglobal( mpLua, "key" );  		

		if( !IS_VAL_BOOL(mpLua, -1) ){			
			lua_pop( mpLua, 1 );  
			return false;
		}

		bValue = GET_VAL_BOOL(mpLua, -1);
		lua_pop( mpLua, 1 );  

		return true;
	}  
	return false;

}

bool tintaScriptContext::getExp(int   &iValue, const char *expression,  const String &file_path) {

	
	if(file_path.length())
		setFile( file_path);


	StringStreamBasic os;
	os << "key=";
	os << expression ;

	//sprintf( buf, "key=%s",expression ); 	
	if( executeBuffer( os.str().c_str(), os.str().length() ) ){
		/* Get the value of the global varibable */  
		lua_getglobal( mpLua, "key" );  		

		if (!IS_VAL_INTEGER(mpLua, -1)){
			lua_pop( mpLua, 1 );  
			return false;
		}

		iValue = GET_VAL_INT(mpLua, -1);
		lua_pop( mpLua, 1 );  

		return true;
	}  
	return false;

}

bool tintaScriptContext::getExp(StringBasic   &strValue, const char *expression,  const String &file_path) {

	
	if(file_path.length())
		setFile(file_path);


	StringStreamBasic os;
	os << "key=";
	os << expression ;

	//sprintf( buf, "key=%s",expression ); 	
	if( executeBuffer( os.str().c_str(), os.str().length() ) ){
		/* Get the value of the global varibable */  
		lua_getglobal( mpLua, "key" );  		

		if( !IS_VAL_STRING(mpLua, -1) ){			
			lua_pop( mpLua, 1 );  
			return false;
		}

		strValue = GET_VAL_STRING(mpLua, -1);
		lua_pop( mpLua, 1 );  

		return true;
	}  
	return false;

}

bool tintaScriptContext::getArrayExp(t_bstring_array &str_vec_value, const char *expression,  const String &file_path)
{
    if(file_path.length())
        setFile(file_path);


    //sprintf( buf, "key=%s",expression ); 	
    if ( !expression )
        return false;

    /* Get the value of the global varibable */  
    lua_getglobal( mpLua, expression);


    if( !lua_istable(mpLua, -1) ){
        lua_pop( mpLua, 1 );  

        StringUtil::StrStreamType os;            
        os << _M("Wrong table in the expression \n") ;
        os << expression;
        mErrors.push_back( os.str() );

        return false;
    }

    size_t array_size = lua_rawlen(mpLua, 1);
    if(array_size == 0)	{
        lua_pop( mpLua, 1 );  
        return false;
    }

    for(size_t count  = 1; count <= array_size; count++){
        lua_rawgeti(mpLua, 1, count);
        str_vec_value.push_back( GET_VAL_STRING( mpLua, -1 ) );

    }	
    // remove all values
    lua_pop( mpLua, (int)(array_size) + 1 );  
    return true;
    
    return false;
}


bool tintaScriptContext::getArrayField( double &d_value, const char *table_name, int index_value,  const String &file_path ) {
	

	
    if( file_path.length() )
	    setFile( file_path);

	
	if(!testTable(mpLua,table_name, index_value))
		return false;

	lua_rawgeti(mpLua, 1, index_value);
	if (!IS_VAL_REAL(mpLua, -1)){		
		mErrors.push_back( String( _M("error number in table") ) );
		return false;
	}

	d_value = GET_VAL_DOUBLE(mpLua, -1);

	lua_pop(mpLua, 2); //remove value & table	

	return true;
}

bool tintaScriptContext::getArrayField(bool &b_value, const char *table_name, int index_value,  const String &file_path)
{
	//int result=0;
	//char error_buf[256];
	//assert(index_value > 0);	
	
    if( file_path.length() )
	    setFile( file_path );
	
	if(!testTable(mpLua,table_name, index_value))
		return false;

	lua_rawgeti(mpLua, 1, index_value);
	if ( !IS_VAL_BOOL(mpLua, -1) ){		
		mErrors.push_back(String(_M("error boolean in table") ));
		return false;
	}

	b_value = GET_VAL_BOOL(mpLua, -1);

	lua_pop(mpLua, 2); //remove value & table	

	return true;
}
// bool tintaScriptContext::setFile( lua_State *p_lua_state, const String &file_path )
// {
// 	char error_buf[256];
// 
// 	//string path = m_str_file_path()
// 	if( file_path.length() == 0 )
// 		return false;
// 
// 	m_str_file_path = file_path;
// 
// 	String path = m_str_file_path;
// 
// 	assert(path.length() || !"Wrong file name");
// 	int error = luaL_loadfile(p_lua_state, path.c_str());
// 
// 	if ( error ){
// 		sprintf(error_buf, "cannot run file: %s", lua_tostring(p_lua_state, -1));
// 		mErrors.push_back(String(error_buf));
// 
// 		return false;
// 	}
// 
// 	error = lua_pcall(p_lua_state, 0, 0, 0);
// 	if (error){
// 		//sprintf(error_buf, "lua_pcall error"));
// 		mErrors.push_back(String("lua_pcall error"));
// 		return false;
// 	}
// 	if(!m_str_file_path.length()){
// 		setFile(path.c_str());
// 	}
// 	return true;
// 
// }
bool tintaScriptContext::testTable(lua_State *p_lua_state, const char *table_name)
{
	//char error_buf[256];

	lua_State *state = 	p_lua_state ? p_lua_state : mpLua;
		
	lua_getglobal(state, table_name);	 
	
	if (!lua_istable(state, -1)){
		//error(L, "'background' is not a table");
		//sprintf(error_buf, "%s is not a table\n", table_name);
		StringUtil::StrStreamType os;
		os << table_name;
		os << _M(" is not a table\n") ;

		mErrors.push_back( os.str() );
		//mErrors.push_back(String(error_buf));
		lua_pop(state, 1);
		return false;
	}
	return true;
}
bool tintaScriptContext::testTable(lua_State *p_lua_state, const char *table_name, int index_value) {	

	lua_State *state = 	p_lua_state ? p_lua_state : mpLua;
	if( !testTable( state, table_name ) )
		return false;
	int array_size = lua_rawlen(state, 1); /* get size of table */
    CoreAssert(index_value <= array_size && index_value > 0, "index_value > array_size || index_value <= 0");
 	if(index_value > array_size || index_value < 1){
//  		sprintf(error_buf, "%d - wrong index value \n", index_value);
//  		mErrors.push_back(String(error_buf));		
		StringUtil::StrStreamType os;
		os << index_value;
		os << _M(" - wrong index value \n") ;
		mErrors.push_back( os.str() );

 		return false;
 	}
	
	return true;
}
bool tintaScriptContext::getArrayField(String &str_value, const char *table_name, int index_value,  const String &file_path) {
		
    if( file_path.length() >0 )
	    setFile( file_path );
	if(!testTable(mpLua,table_name, index_value)) return false;

	lua_rawgeti(mpLua, 1, index_value);
	if (!IS_VAL_BOOL(mpLua, -1)){		
		mErrors.push_back( String(_M("error string in table") ) );
		return false;
	}
    str_value = toWideChar(lua_tostring(mpLua, -1));

	lua_pop(mpLua, 2); //remove value & table	

	return true;
}
bool tintaScriptContext::getArray(t_int_array &i_vec_value, const char *table_name,  const String &file_path)
{
	//char error_buf[256];

	if(file_path.length())
	    setFile( file_path);
	if(!testTable(mpLua,table_name)) return false;
    
	size_t array_size = lua_rawlen(mpLua, 1);
	for(size_t i = 1; i <= array_size; i++){
		lua_rawgeti(mpLua, 1, i);
		i_vec_value.push_back(GET_VAL_INT(mpLua, -1));
		lua_pop(mpLua, 1);
	}
	lua_pop(mpLua, 1); //pop table
	return true;
}
bool tintaScriptContext::getArray(std::vector<double> &d_vec_value, const char *table_name,  const String &file_path)
{
	
    if( file_path.length() )
        setFile( file_path );
    if(!testTable(mpLua,table_name)) return false;

    size_t array_size = lua_rawlen(mpLua, 1);
    for(size_t i = 1; i <= array_size; i++){
        lua_rawgeti(mpLua, 1, i);
        d_vec_value.push_back(GET_VAL_DOUBLE(mpLua, -1));
        lua_pop(mpLua, 1);
    }
    lua_pop(mpLua, 1); //pop table
    return true;

}

bool tintaScriptContext::getArray(t_string_array &str_vec_value, const char *table_name,  const String &file_path )
{


	if( file_path.length() )
		setFile( file_path );
	if(!testTable(mpLua,table_name)) return false;

	int array_size = lua_rawlen(mpLua, 1);
	for(int i = 1; i <= array_size; i++){
		lua_rawgeti(mpLua, 1, i);
        str_vec_value.push_back(toWideChar(lua_tostring(mpLua, -1)));
		lua_pop(mpLua, 1);
	}
	lua_pop(mpLua, 1); //pop table
	return true;



}
bool tintaScriptContext::callFunc (const char *func, const char *sig, ...) {
	va_list vl;
//	char error_buf[256];


	int narg, nres; /* number of arguments and results */
	va_start(vl, sig);
	lua_getglobal(mpLua, func); /* push function */
	for (narg = 0; *sig; narg++) { /* repeat for each argument */
		/* check stack space */
		luaL_checkstack(mpLua, 1, "too many arguments");
		switch (*sig++) {
			case 'd': /* double argument */
				lua_pushnumber(mpLua, va_arg(vl, float));
				break;
			case 'i': /* int argument */
				lua_pushinteger(mpLua, va_arg(vl, int));
				break;
			case 's': /* string argument */
				lua_pushstring(mpLua, va_arg(vl, char *));
				break;
			case '>': /* end of arguments */
				goto endargs;
			default:
				//error(L, "invalid option (%c)", *(sig - 1));
// 				sprintf(error_buf, "invalid option (%c)", *(sig - 1));
// 				mErrors.push_back(string(error_buf));
                CoreAssert(false,"default case");
				return false;
		}
	}

	endargs:
	nres = strlen(sig); /* number of expected results */
	/* do the call */
	if (lua_pcall(mpLua, narg, nres, 0) != 0) /* do the call */
	{
		//error(L, "error calling '%s': %s", func,		lua_tostring(L, -1));
		//sprintf(error_buf, "error calling '%s': %s", func,		lua_tostring(mpLua, -1));
		StringUtil::StrStreamType os;
		
		os << _M(" error calling \n") ;
		os << func ;
		os << lua_tostring(mpLua, -1);

		mErrors.push_back( os.str() );

		//mErrors.push_back(String(error_buf));
		return false;
	}
	nres = -nres; /* stack index of first result */
	
	while (*sig) { /* repeat for each result */
		switch (*sig++) {
		case 'd': /* double result */
			if (!IS_VAL_REAL(mpLua, nres))
			{
				mErrors.push_back( String(_M("wrong result type") ) );
				return false;
			}
					
			*va_arg(vl, double *)= GET_VAL_DOUBLE(mpLua, nres);
			break;
		case 'i': /* int result */
			if (!IS_VAL_INTEGER(mpLua, nres))
			{
				mErrors.push_back(String(_M("wrong result type")) );			
				return false;
			}					
			*va_arg(vl, int* ) = GET_VAL_INT(mpLua, nres);
			break;
		case 's': /* string result */
			if (!IS_VAL_STRING(mpLua, nres))
			{
				mErrors.push_back(String(_M("wrong result type")) );	
				return false;
			}
//				error(L, "wrong result type");
				*va_arg(vl, const char **) = lua_tostring(mpLua, nres);
			break;
		default:
			{
				//sprintf(error_buf, "invalid option (%c)", *(sig - 1));
				StringUtil::StrStreamType os;
				os << _M("invalid option: ") ;
				os << *(sig - 1);
				mErrors.push_back( os.str() );
				//mErrors.push_back(String(error_buf));
				return false;
			}
			//error(L, "invalid option (%c)", *(sig - 1));
		}
		nres++;
	}
	va_end(vl);
	return true;
}
void tintaScriptContext::regFunc(int (*lua_c_func)(lua_State *L),const char *func_name ){	
	if( mpLua )
        lua_register( mpLua, func_name, lua_c_func );

}


}// namespace Tinta
