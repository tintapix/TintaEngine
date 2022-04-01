/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaConfNode.h"
#include "tintaException.h"


namespace Tinta {

    namespace TreeConfig {

		tintaConfNode::tintaConfNode()
		:mParent  (NULL),
		 mDataType ( enNoType ),
		 mbValid   ( false ),		 
		 mlevel   (-1)		{
		}

		tintaConfNode::tintaConfNode(  tintaConfNode * parent, const Token &tok) 
			:mParent  ( parent ),
			mDataType( tok.mType ),
			mbValid  ( false ),			
			mName    ( tok.mName )	{
				mlevel   =  parent ? ( parent->mlevel+1 ) : 0,
				initData( tok );			
		}

		tintaConfNode::~tintaConfNode(){
			tintaConfNodesvec_t::iterator dit = mNodesIndices.begin();
			for( dit; dit != mNodesIndices.end(); dit++ )
				DELETE_T  ( *dit, tintaConfNode );
			mNodesIndices.clear();
			//chNodesNames.clear();		
			mParent = NULL;
		}
		String tintaConfNode::getName() const {
			return mName;
		}

		bool tintaConfNode::isValid() const {
			return mbValid;
		} 

		tintaConfNode::TokenType tintaConfNode::getType()const {
			return mDataType;
		}

		int tintaConfNode::getLevel()const {
			return mlevel;
		}
		tintaConfNode * tintaConfNode::getParent(){
			return mParent;
		}	

		const tintaConfNode * tintaConfNode::getParent()const{
			return mParent;
		}	

		void tintaConfNode::addChild( tintaConfNode * chld ){
			String name = chld->getName();
			mNodesIndices.push_back( chld );

		}
		// returns child quantity on this level
		size_t tintaConfNode::childQuantity()const{
			return mNodesIndices.size();
		}

		const m_int8* tintaConfNode::getBufferData()const{
			return mBuffValue.GetBuffer();
		}
		size_t tintaConfNode::getBufferDataLen()const{
			return mBuffValue.GetSize();
		}

        tintaConfNode * tintaConfNode::getByName( const String &name ) {

            tintaConfNode *r = NULL;
            tintaConfNodesvec_t::iterator i = mNodesIndices.begin();

            for( ; i !=mNodesIndices.end() && !r; i++ ){
                if ((*i)->getName() == name || name.length() == 0 ){
                    r = *i;
                    break;
                }
            }
            return r;
        }

        const tintaConfNode * tintaConfNode::getByName( const String &name ) const {

            const tintaConfNode *r = NULL;
            tintaConfNodesvec_t::const_iterator i = mNodesIndices.begin();

            for( ; i !=mNodesIndices.end() && !r; i++ ){
                if ((*i)->getName() == name || name.length() == 0) {  // found or first 
                    r = *i;
                    break;
                }
            }
            return r;
        }

//         const tintaConfNode * tintaConfNode::getByName( const String &name )const {
//             const tintaConfNode *r = NULL;
//             tintaConfNodesvec_t::const_iterator i = mNodesIndices.begin();
// 
//             for( ; i !=mNodesIndices.end() && !r; i++ ){
//                 if( i->getName() == name )
//                     r = *i;                
//             }
//             return r;
//         }



		const m_int8* tintaConfNode::findValue(int  index, const String &name, TokenType type, size_t &datalen )const{
			const m_int8* buffData = NULL;
			if (index > -1 && (size_t)index < mNodesIndices.size() 
                            && mNodesIndices[index]->getType() == type ){
				buffData = mNodesIndices[index]->getBufferData();
				datalen = mNodesIndices[index]->getBufferDataLen();
                CoreAssert(buffData, "buffData");
			}
			else {
				
                const tintaConfNode * node = getByName( name );
				if (node && node->getType() == type ){

					buffData = node->getBufferData();
					datalen = mNodesIndices[index]->getBufferDataLen();
                    CoreAssert(buffData, "buffData");
				}
			}
			return buffData;
			//if (buffData)
			//	ReadFromBuffer<double>(buffData, 0, rez);
		}
		bool tintaConfNode::getVal( double &rez)const{
			//size_t size = 0;
			const m_int8* buffData = mBuffValue.GetBuffer();		

            if ( mDataType == enNumber && buffData && mBuffValue.GetSize() > 1){
				ReadFromBuffer<double>(buffData, 0, rez);
                return true;
            }
			
			return false;

		}

		bool tintaConfNode::getVal(size_t &beg, size_t &end )const{
			const m_int8* buffData = mBuffValue.GetBuffer();
            if ( buffData && mBuffValue.GetSize() == sizeof(size_t) * 2){
				int offset = ReadFromBuffer<size_t>(buffData, 0, beg );
							 ReadFromBuffer<size_t>(buffData, offset, end );
				return true;
			}
			return false;
		}

		bool tintaConfNode::getVal( String &rez)const{
			
			const m_int8* buffData = mBuffValue.GetBuffer();
			size_t size = mBuffValue.GetSize();
            if (mDataType == enString && buffData && size > 0){
				char_m *nameArr = ALLOC_T(char_m, size / sizeof( char_m ) + 1);				

				ReadArrayFromBuffer<char_m>(buffData, 0, nameArr, size / sizeof(char_m) );
				rez = nameArr;

				FREE_T(nameArr);
				
			}
			return buffData != NULL;
		}

		bool tintaConfNode::getVal( bool &rez)const{
			//size_t size = 0;
			const m_int8* buffData = mBuffValue.GetBuffer();//findValue(index, name, enBoolean, size );

            if (mDataType == enBoolean && buffData)
				ReadFromBuffer<bool>( buffData, 0, rez );
			
			return buffData != NULL;

		}


		bool tintaConfNode::setVal(const double &val){
			
            
            mDataType = enNumber;                       

			Tinta::StringUtil::StrStreamType out; 	
			out<<val;
			Token tok(mlevel, mDataType, mName, out.str()); 
			return initData( tok );		
		}

        bool tintaConfNode::setVal(const int &val) {
           return  setVal((double)val);
        }

        bool tintaConfNode::setVal(const float &val){
            return setVal((double)val);
        }

		bool tintaConfNode::setVal(const  String &val){
                       
            mDataType = enString;
            
			Tinta::StringUtil::StrStreamType valstr; 	
			valstr<<_M("\"");
			valstr<<val;
			valstr<<_M("\"");
			Token tok(mlevel, mDataType, mName, valstr.str()); 
			return  initData( tok );		
		}

		bool tintaConfNode::setVal(const  bool &b){
            
            mDataType = enBoolean;

			Tinta::StringUtil::StrStreamType out; 	
            out << std::boolalpha << b;
			
			Token tok(mlevel, mDataType, mName, out.str()); 
			return initData( tok );		
		}


		// returns child on this level by index
		tintaConfNode * tintaConfNode::getChild( size_t index ){
			if( index >= mNodesIndices.size() )
				return NULL;

			return mNodesIndices[index];
		}


		// returns child on this level by name
		tintaConfNode * tintaConfNode::getChild( const String &name){			
            tintaConfNode *r = getByName( name );
			return r;
		}

		const tintaConfNode * tintaConfNode::getChild( size_t index )const{
			if( index >= mNodesIndices.size() )
				return NULL;

			return mNodesIndices[index];
		}



		const tintaConfNode * tintaConfNode::getChild( const String &name)const{
			
            const tintaConfNode *r = getByName( name );
            return r;
		}

		bool tintaConfNode::initData( const Token &tok ){

			mName	  = tok.mName;
			mDataType = tok.mType;
            /* // nice try
			if( bMapMode && tok.mValue.length() > 0 ){
					size_t beg = tok.posEnd - tok.mValue.length();
					if( beg < tok.posEnd ){
						mBuffValue.AllocateBuffer( sizeof(size_t) + sizeof(size_t) );	
						int offset =  WriteToBuffer<size_t>  ( mBuffValue.GetBufferEx(),0,		beg );
									  WriteToBuffer<size_t>  ( mBuffValue.GetBufferEx(),offset, tok.posEnd );
									  //mBuffValue.SetDataSize(mBuffValue.GetSize());
						mbValid = true;
					}
					else
						mbValid = false;

				return mbValid;
			}
            */


			switch( mDataType ){				
				case enNumber:
					try {
						double data = std::stod(tok.mValue.c_str());
						mBuffValue.AllocateBuffer(sizeof(double));
						WriteToBuffer<double>(mBuffValue.GetBufferEx(), 0, data);
						mbValid = true;
					}
					catch( const std::invalid_argument &ia){                        
						mbValid = false;
					}
					catch(const  std::out_of_range &oor){
						mbValid = false;
					}
					break;
				case enString:
					{					
						String val;
						if( tok.mValue.length() > 2  )
							val = tok.mValue.substr( 1, tok.mValue.length() - 2); // cutting ""						

						mBuffValue.AllocateBuffer( ( val.length() + 1 ) * sizeof( char_m ) );
                        WriteToBufferString<char_m>(mBuffValue.GetBufferEx(), 0, val);

						mbValid = true;
					}
					break;
				case enBoolean:					
					{					
						bool bVal = false;
						mbValid = false;
                        String v(tok.mValue);
                        StringUtil::toLower( v );
						if( 
                            v.compare(_M("true") )  == 0 ){
							bVal = true;
							mbValid = true;						
						}
						else if( v.compare(_M("false") )  == 0 ){
							bVal = false;						
							mbValid = true;
						}
						if( mbValid ){
							mBuffValue.AllocateBuffer( sizeof( bool ) );
							WriteToBuffer<bool>  ( mBuffValue.GetBufferEx(),0, bVal );							
						}				
					}

					break;
				case enNode: // Node with no value, situation when: "{ {" or "{ namespace{"
					mbValid = true;
					break;
				case enNoType:								
					mbValid = false;
					break;
				default : 
					mbValid = false;					
			}		
			return mbValid;
		}	

	} //namespace tintaTreeConfig 

}
