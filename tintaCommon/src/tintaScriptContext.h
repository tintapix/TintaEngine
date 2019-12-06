/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef _TINTA_SCRIPT_CONTEXT_H_
#define _TINTA_SCRIPT_CONTEXT_H_

#include "tintaCommon.h"

extern "C" {

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

}

namespace Tinta
{

// wrapping base types
#define GET_VAL_INT(L, INDEX) lua_tointeger(L, INDEX)
#define GET_VAL_UINT(L, INDEX) ( static_cast<m_uint32> ( (lua_tointeger(L, INDEX)  >= 0) ? lua_tointeger(L, INDEX) : 0) )
#define GET_VAL_STRING(L, INDEX) (lua_tostring(L, INDEX) ? lua_tostring(L, INDEX) : ("" ) )
#define GET_VAL_STRING_EX(L, INDEX) (lua_tostring(L, INDEX) ? (toWideChar(lua_tostring(L, INDEX))) : (_M("")) )
#define GET_VAL_UBYTE(L, INDEX) (static_cast<m_uint8> ((lua_tointeger(L, INDEX) > UCHAR_MAX) ? (UCHAR_MAX) : \
	((lua_tointeger(L, INDEX) < 0) ? 0 : lua_tointeger(L, INDEX))))

#define GET_VAL_FLOAT(L, INDEX) (static_cast<float> (lua_tonumber(L, INDEX)))

#define GET_VAL_DOUBLE(L, INDEX) (static_cast<double> (lua_tonumber(L, INDEX)))

#define GET_VAL_BOOL(L, INDEX) (lua_toboolean(L, INDEX)>0)

#define IS_VAL_REAL(L, INDEX) lua_isnumber(L, INDEX)

#define IS_VAL_INTEGER(L, INDEX) lua_isinteger(L, INDEX)
	
#define IS_VAL_BOOL(L, INDEX) lua_isboolean(L, INDEX)
#define IS_VAL_STRING(L, INDEX) lua_isstring(L, INDEX)


#if defined(LUA_32BITS)	
	#define PUSH_DOUBLE_ARG(L, VALUE) lua_pushnumber(L, (static_cast<float> (VALUE))) 
	#define PUSH_FLOAT_ARG(L, VALUE) lua_pushnumber(L, VALUE) 
#else
	#define PUSH_DOUBLE_ARG(L, VALUE)  lua_pushnumber(L, (static_cast<long> (VALUE))) 
	#define PUSH_FLOAT_ARG(L, VALUE)lua_pushnumber(L, (static_cast<double> (VALUE))) 
#endif


#define PUSH_STRING_ARG(L, VALUE) lua_pushstring( L, VALUE) 
#define PUSH_VAL_STRING_EX(L, VALUE) (lua_pushstring( L, ( toNarrowChar(VALUE).c_str() ) ) )

#define PUSH_INT_ARG(L, VALUE) lua_pushinteger(L, VALUE)
#define PUSH_UINT_ARG(L, VALUE) ( PUSH_INT_ARG(L, ( TintaMath::abs( (int)VALUE)) ) ) 
#define PUSH_BOOL_ARG(L, VALUE) lua_pushboolean(L, VALUE) 
#define GET_QUANTITY(L) lua_gettop(L)

#define TROW_ERR_FUNC(L, TEXT) luaL_error((L), (Tinta::String(TEXT)).c_str()) 	


    #if CORE_PLATFORM  == CORE_PLATFORM_WIN32   
    
        #define TROW_ERR_FUNC_EX(L, TEXT) { std::wcout << StringUtil::createUTF16String(TEXT) << std::endl; luaL_error((L), ("Executing aborted ..."));}
    #else
        #define TROW_ERR_FUNC_EX(L, TEXT) (luaL_error((L), (String(TEXT)).c_str()))
    #endif



#define TROW_ERR_BADARGFUNC(L, ARG, TEXT) luaL_argerror((L), (ARG), (TEXT).c_str()); 	


#define SCRIPT_STATE lua_State

	using namespace std;

	
typedef  std::vector<String>  t_string_array;
typedef  std::vector<StringBasic>  t_bstring_array;
typedef  std::vector<int>  t_int_array;
typedef  std::vector<bool>  t_bool_array;
typedef  std::vector<double>  t_dbl_array;


/*
    Lua wrapper
*/
class  _CoreExport tintaScriptContext
{
protected:
	//template class _CoreExport t_string_array;
public:
		
	 tintaScriptContext(void);	
	 virtual ~tintaScriptContext(void);
  
	void createState( );
	//void createState_threaded(const char* file_path = 0);
	void			 closeState();

	void			 setState(lua_State* new_state);

	const lua_State* getState()const;

	lua_State*		 getState_ex();

	const String*	 getErrors( size_t &error_count ) const ;

	void			 setFile( const String &file_path );	

	bool			 executeFile( const String &file_path = _M("") );
	
	bool			 executeBuffer( const char* buffer, size_t len );

	void			 resetErrors();

	bool			 hasErrors() const ;

private:
	lua_State	   *mpLua;

	t_string_array  mErrors;
	
	String m_str_file_path;

	//bool setFile(lua_State *p_lua_state, const String &file_path);

	bool testTable(lua_State *p_lua_state, const char *table_name, int index_value);

	bool testTable(lua_State *p_lua_state, const char *table_name);

public:
	void getField (int &i_value, const char *key);

	void getField (double &d_value, const char *key);

	void getField (bool &b_value, const char *key);

	void getField (String &str_value, const char *key);


	bool getGlobVar(int &i_value,const char *var_name,const String &file_path= _M(""));
	bool getGlobVar(m_uint32 &ui_value,const char *var_name,const String &file_path= _M(""));

	bool getGlobVar(double &d_value,const char  *var_name, const String &file_path= _M(""));
	bool getGlobVar(bool &b_value, const char *var_name, const String &file_path= _M(""));
	//bool getGlobVar(StringBasic &str_value, const char *var_name, const String &file_path= _M(""));
	bool getGlobVar(String &str_value, const char *var_name, const String &file_path = _M(""));
	template<typename T> 
	inline bool getTabVar(T &table_value, const char *table_name, const char *variable_name, const String &file_path= _M("")) {
		if(!mpLua)
			createState();
		char error_buf[256];

		setFile( file_path);

		lua_getglobal(mpLua, table_name);
		if (!lua_istable(mpLua, -1)){
			//sprintf(error_buf, "%s is not a valid table\n", table_name);

			StringStream os;
			os << table_name;
			os << _M(" is not a valid table");

			mErrors.push_back( os.str() );
			return false;
		}	
		getField(table_value, variable_name);	
		return true;
	}
	bool getArrayField (int &i_value, const char *table_name, int index_value, const String &file_path= _M("")) ;
	bool getArrayField (double &d_value, const char *table_name, int index_value,  const String &file_path= _M("")) ;
	bool getArrayField (bool &b_value, const char *table_name, int index_value,  const String &file_path= _M("")) ;

	bool getArrayField (String &str_value, const char *table_name, int index_value,  const String &file_path= _M("")) ;	
	bool getArray(t_int_array &i_vec_value, const char *table_name,  const String &file_path= _M("")) ;


	bool getArray(std::vector<double> &d_vec_value, const char *table_name,  const String &file_path= _M("")) ;	
	//bool getArray(t_bstring_array &str_vec_value, const char *table_name,  const String &file_path= _M("")) ;

	bool getArray(t_string_array &str_vec_value, const char *table_name,  const String &file_path= _M("")) ;


	
	// gets the fields from array by expression array.nested_array

    bool getArrayExp(t_bstring_array &str_vec_value, const char *expression,  const String &file_path= _M(""));

	bool getArrayExp(t_int_array    &int_vec_value, const char *expression,  const String &file_path= _M(""));
	bool getExp(bool   &bValue, const char *expression,  const String &file_path= _M(""));
	bool getExp(int   &iValue, const char *expression,  const String &file_path= _M(""));
	bool getExp(StringBasic   &strValue, const char *expression,  const String &file_path= _M(""));
	
	
	bool callFunc (const char *func, const char *sig, ...);
	void regFunc(int (*lua_c_func)(lua_State *L),const char *func_name );   

protected:
	//const char* lua_stringexpr( const char* expr,  const char* def );
};

}
#endif
