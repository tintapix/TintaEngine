#ifndef TINTA_RES_CONTAINER_H_
#define TINTA_RES_CONTAINER_H_

#include "tintaGraphicaPredefine.h"


namespace Graphica {

	template<class T>
	class tintaResContainer
	{

	
	public:
		tintaResContainer(void){}
		~tintaResContainer(void){}

	public:
		bool insertResource(const String &name, T *in_value);
		bool removeResource(const String &name);

        T *getResource(const StringBasic &name);

	protected:

		typedef std::map<String, T*> t_res_map;
		t_res_map m_resources;		
	};

	template <class T>
	bool tintaResContainer<T>::removeResource(const String &name){
        auto it_find = m_resources.find(name);
        if ( it_find == m_resources.end() )
            return false;
        else
            m_resources.erase( it_find->first );
	}

	template <class T>
    T *tintaResContainer<T>::getResource( const  String &name ){

		if( m_resources.size() == 0 )
			return NULL_M;
        auto it_find = m_resources.find( name );

		if( it_find == m_resources.end() )
			return NULL_M;

		return it_find->second;

	}

	template <class T>
	bool tintaResContainer<T>::insertResource(const String &name,  T *in_value){

		assert(in_value);

        auto it_find = m_resources.find(name);

		if( !in_value || it_find != m_resources.end() )
			return false;
        
		m_resources.emplace( t_res_map::value_type( name, in_value ) );	

        return true;		
	}


}

#endif
