/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CONF_NODE_H_
#define _TINTA_CONF_NODE_H_

#include "tintaPredefine.h"
#include "tintaBufferIO.h"


namespace Tinta {

    namespace TreeConfig {   

    struct _CoreExport tintaConfNode {

		enum TokenType {
			enNoType = 0,
			enNumber,
			enString,
			enBoolean,
			/*not supported yet*/
			//enMappos,  // position in file lineBeg,posBeg,lineEnd,posEnd
			enNode	     // node with nested nodes or values
		};		

		struct Token {

			Token( int level, tintaConfNode::TokenType type,
				const String &name, const String &val, 
				size_t pos = 0 )
				:mlevel(level)
				,mType( type )
				,mName( name )
				,mValue( val )
				,posEnd(pos) {
				// comment always attaching later				
			}

			// multiplicity of the node 0 - first level
			int	  mlevel = 0;
			tintaConfNode::TokenType mType = tintaConfNode::enNoType;
			String	  mName; // token name like `val1` in exp.  val1 = 100
			// global pos  of the last symbol in token
			String    mValue; // token value like `100` in exp.  val1 = 100
			size_t posEnd = 0;
			String	  mComment;
		};


		typedef std::vector<tintaConfNode*>			tintaConfNodesvec_t;	

		tintaConfNode();

		tintaConfNode(  tintaConfNode * parent, const Token &tok );// tintaConfNode * parent, const String &name , const String &strValue, TokenType type, bool mapMode = false );		

		~tintaConfNode();	

		bool initData( const Token &tok );		

		String getName() const ;

		bool isValid() const;

		TokenType getType()const;

		int getLevel()const;
		
		tintaConfNode * getParent();

		const tintaConfNode * getParent()const;
		
		void addChild( tintaConfNode * chld );

		// returns child quantity on this level
		size_t childQuantity()const;
		
		// returns child on this level by index
		tintaConfNode * getChild( size_t index );

		// returns child on this level by name
		tintaConfNode * getChild(const String &name);

		// returns const child on this level by index
		const tintaConfNode * getChild( size_t index )const;

		// returns const child on this level by name
		const tintaConfNode * getChild(const String &name)const;

		
		bool getVal(size_t &beg, size_t &end )const;

		bool getVal(double &rez)const;

		bool getVal( String &rez)const;

		bool getVal( bool &rez)const;

		bool setVal(const double &val);

        bool setVal(const int &val);

        bool setVal(const float &val);

		bool setVal(const String &val);

		bool setVal(const bool &val);

		const m_int8* getBufferData()const;

		size_t getBufferDataLen()const;

	private:

		// root has NULL
		tintaConfNode     *mParent;

		TokenType		   mDataType;

		bool			   mbValid;
        		
		int				   mlevel;

		String 			   mName;		
        		
		// keeping in buffer
		tintaBufferIO	   mBuffValue;

		// keeping by index
		tintaConfNodesvec_t  mNodesIndices;
		// by name if it exists		

		String			mNameValue;	

        // Node Name from name token
        const m_int8* findValue(int  index, const String &name, TokenType type, size_t &datalen)const;

        tintaConfNode * getByName(const String &name);

        const tintaConfNode * getByName(const String &name)const;
	};



}

}

#endif
