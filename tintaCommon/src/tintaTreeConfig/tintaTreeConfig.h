/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_TREE_CONFIG_H_
#define _TINTA_TREE_CONFIG_H_


#include "tintaPredefine.h"
#include "tintaStdHeaders.h"
#include "tintaOSHeaders.h"
#include "tintaConfNode.h"
#include "tintaException.h"



namespace Tinta {

namespace TreeConfig {


	/*class ConfigError
		: public std::exception
	{	
	private :
			StringBasic msg;

	public:
		explicit ConfigError(const StringBasic& _Message)
			: std::exception()
			,msg(_Message)
		{	
		}

		virtual  ~ConfigError() throw()
		{	
		}
		virtual const char* what() const throw() {
			return msg.c_str();
		}
	};
    */

    class tintaTreeConfig
	{	
	protected:
		typedef std::vector<tintaConfNode::Token> tokens_stack_t;
		typedef std::pair<int, String> requestLink_t;
		typedef std::vector<requestLink_t> requestChain; 
		typedef std::vector<requestChain> requestChains;	

	public:

		/*
			Returns true if there are any values in the tree
		*/
		bool hasValues() const ;

		tintaTreeConfig( void );

		/*
			reads data from the file to buffer
		*/
		tintaTreeConfig( const String &path );	

        /*
            reads data from buffer
        */
        tintaTreeConfig(const char *buffer, m_uint32 length );


        virtual ~tintaTreeConfig(void);

		/*
			reads(if path != "") and parses buffer
		*/
		bool parse( const String &path = _M(""));


        /*
            reads data from buffer
        */
        bool parse(const char *buffer, m_uint32 length);

        

		/*
			saves tree in to the file
		*/
        bool save(const String &path = _M(""));

		String*			 getTokensDebug( size_t &quantity );

		String			 getError( ) const ;		

        /*
            Returns child quantity on lower level by reauest
        */
        size_t			 childQuantity( const String &request)const;

        /*
            Returns child quantity on lower level by parrent node
        */
        size_t           childQuantity( tintaConfNode *from = NULL_M) const;

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end
        */
        bool setValue(const String &request, const double &val, bool discardType = false);

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end
        */
        bool setValue(const String &request, const int &val, bool discardType = false);

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end
        */
        bool setValue(const String &request, const float &val, bool discardType = false);

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end
        */
        bool setValue(const String &request, const String &val, bool discardType = false);

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end
        */
        bool setValue(const String &request, const bool &val, bool discardType = false);

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end
        */
        bool setValue(const String &request, const char_m *val, bool discardType = false);

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end, to save all values add "*" to the end of the request e.g. "RGBColor.*"
        */
        bool setValues(const String &request, const std::vector<String> &vals, bool discardType = false);

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end, to save all values add "*" to the end of the request e.g. "RGBColor.*"
        */
        bool setValues(const String &request, const std::vector<bool> &vals, bool discardType = false);

        /*
        discardType - if true rewrites or adds value anyway, if index out of range
        adds value to the end, to save all values add "*" to the end of the request e.g. "RGBColor.*"
        */
        bool setValues(const String &request, const std::vector<double> &vals, bool discardType = false);

        bool getValue(const String &request, double &val);

        bool getValue(const String &request, String &val);
        
        bool getValue(const String &request, bool &val);

        bool getValues(const String &request, std::vector<double> &val);

        bool getValues(const String &request, std::vector<String> &val);

        bool getValues(const String &request, std::vector<bool> &val);
        
		

        /*
            Returns Child node by name 
            from if NUll - from root
        */
        const tintaConfNode *getChild(const String &request, const tintaConfNode *from = NULL_M )const;

         /*
            Returns Child node by name 
        */
        tintaConfNode	    *getChild(const String &request,  tintaConfNode *from = NULL_M);

         /*
            Returns Child node by index
        */
        const tintaConfNode *getChild(size_t index, const tintaConfNode *from = NULL_M)const;

         /*
            Returns Child node by index
        */
        tintaConfNode	    *getChild(size_t index,  tintaConfNode *from = NULL_M);

	protected:
		
        enum {
            // taking value by name
            enNoIndex = -1,
            // taoking all values on level
            enAllvalues = -2,
        };


        enum EnMode {
            //  `"`  - opened and not closed - ignoring all inside of the block until closing `"`
            enModeStringParam = 0x01,                      
            //  `//` - ignoring all inside of the block until the end of the line `\n`
            enModeComment = 0x02, 
            // /* Long comment */
            enModeLongComment = 0x04,
        };
        	
		String			mPath;

		tintaConfNode  *mRootNode;

		bool			mIsUtf8BOM;
        
		mutable String	mLastError;

		StringVector	mBuffer;

		tokens_stack_t  tokenStack;

		StringVector	tokensDebug;
		
        /*
        reads data from the file to Buffer
        */
        bool readFile(const String &path);


        void clearError();

        const tintaConfNode *findNode(const tintaTreeConfig::requestChain &requests, const tintaConfNode *from, size_t &errPos)const;

        tintaConfNode	    *findNode(const tintaTreeConfig::requestChain &requests, tintaConfNode *from, size_t &errPos);

        const tintaConfNode *findNode(const tintaTreeConfig::requestLink_t &request, const tintaConfNode *from)const;

        tintaConfNode	    *findNode(const tintaTreeConfig::requestLink_t &request, tintaConfNode *from);



		// if one of line parameters == -1, the message will not include line prefix
		void ifErrorAdd( bool rez, int line, const String &text );

		// returns new actual parent node
		tintaConfNode * addNode( tintaConfNode * curParentNode, const tintaConfNode::Token &token, int beforeLevel );
        
        /*
		 parsing request string
		 rez - vector with node names or indices
		 quantities - quantity of values in request part
        */
		void fillReqests(const String &req, requestChain  &rez)const;
        	

		/* 
			determine the type of the value : 100, 100.12, .1000, true, false, "text here", returns enNoType if somth wrong
		*/
		tintaConfNode::TokenType validateValue(const String &val) const;

		//tintaConfNode::TokenType createMapData(size_t linebeg, size_t beg, size_t lineend,size_t end, String &rezData) const;

		void updateValueToken(tokens_stack_t  &tokens,int level, tintaConfNode::TokenType type, const String &token, size_t endPos = 0  )const;

		String formString(const tintaConfNode * dataNode )const;

		/*
			Fills vector with nodes description and data in line for saving
		*/
		void fillLinesFromNodes( const tintaConfNode * dataNode, StringVector &nodes, bool last = false );

        template<typename T>
        bool setAddValues(const String &request, const std::vector<T> &vals, tintaConfNode::TokenType type, bool discardType)  {
            clearError();
            requestChain requests;
            fillReqests(request, requests);
            if ( requests.size() == 0 ){
                mLastError = "Wrong request format";
                return false;                
            }

            tintaConfNode *next   = mRootNode;
            tintaConfNode *before = next;
            int level = -1;

            tintaConfNode::TokenType valType = tintaConfNode::enNode;



            for ( requestLink_t pair : requests ){
                level++;

                next = findNode(pair, next);
                bool lastReq = level == (int)(requests.size() - 1);

                // trying set value to the node
                if (next && lastReq
                    && ( next->getType() != type && discardType == false ) ){

                    StringStreamBasic msg;
                    msg << "Can`t set value to the node: ";
                    if (pair.second.length() != 0)
                        msg << pair.second;
                    else
                        msg << pair.first;

                    mLastError = msg.str();
                    return false;
                }

                // trying add node to the leaf
                if (next && !lastReq && next->getType() != tintaConfNode::enNode) {
                    StringStreamBasic msg;
                    msg << "Can`t add the branch to the leaf: ";
                    if (pair.second.length() != 0)
                        msg << pair.second;
                    else
                        msg << pair.first;

                    mLastError = msg.str();
                    return false;
                }

                    if ( lastReq ) 
                        valType = type;
                    
                    if ( lastReq ){ // finishing 
                         
                            size_t idxchild = 0;
                            for (const T &val_ : vals) {

                                tintaConfNode * addSet = NULL_M;

                                // trying to find the existing node
                                if (pair.second.length() > 0)           // String request            
                                    addSet = before->getChild(pair.second);
                                else if (pair.first > -1){             // "[0]" index in request
                                    addSet = before->getChild(pair.first);
                                }
                                else {                                  // "*" - in request
                                    addSet = before->getChild(idxchild);
                                }                             


                                if ( !addSet ) { // parent node have no leavies only adding
                                    String strValue;
                                    tintaConfNode::Token token{ level, valType, pair.second, strValue, 0 };
                                    tintaConfNode *newNode = NEW_T( tintaConfNode )( before, token );
                                    newNode->setVal(val_);
                                    before->addChild(newNode);
                                }
                                else {
                                    if (addSet->getType() != valType && discardType == false ){ // allow only for the same type

                                        StringStreamBasic msg;
                                        msg << "Different types of the request result and value to set in position: ";                           
                                        msg << idxchild;
                                        mLastError = msg.str();
                                        return false;
                                    }

                                    addSet->setVal(val_);
                                    
                                }
                                idxchild++;
                            }                                   
                        // job`s done - breaking loop
                        break;
                    }
                    else if ( !next ) {  // adding node if not exists
                       
                        String strValue;
                        tintaConfNode::Token token{ level, valType, pair.second, strValue, 0 };
                        tintaConfNode *newNode = NEW_T(tintaConfNode)(before, token);                        

                        before->addChild(newNode);
                        next = newNode;
                    }   
                    
                //}   

                before = next;
            }
            return true;
        }


        template<typename T> bool getVal(const String &request, T &rezval ){

            clearError();

            requestChain requests;
            fillReqests(request, requests);
            if ( requests.size() == 0 ){
                mLastError = "Wrong request format";
                return false;
            }

            size_t indexreq = 0;
            tintaConfNode * rezNode = findNode(requests, mRootNode, indexreq);

            if (rezNode){ //last node in request- value found

                //size_t begin = 0, end = 0;
                if (rezNode->getType() == tintaConfNode::enNode) {
                    mLastError = "Result is not the value";
                    return false;
                }
                
                if (!rezNode->getVal(rezval)) {
                    mLastError = "Extracting from the buffer error";
                    return false;
                }

                return true;                
            }

            StringStreamBasic  msg;
            if (indexreq == 0){ // root was not found					
                msg << "Root node was not found";
            }
            else{
                CoreAssert((indexreq - 1) < requests.size(),"(indexreq - 1) >= requests.size()");
                msg << "Wrong request part: ";
                if (requests[indexreq - 1].first > enNoIndex)
                    msg << requests[indexreq - 1].first;
                else
                    msg << requests[indexreq - 1].second;
            }
            mLastError = msg.str();
            return false;            
        }

        template<typename T> bool getVals(const String &request, std::vector<T> &rezval){

            clearError();

            requestChain requests;
            // for this moment multiple requests allowed only on leafs		
            fillReqests(request, requests);
            if (requests.size() == 0){

                //throw ConfigError("Wrong request format");
                mLastError = "Wrong request format";
                return false;
            }

            size_t indexreq = 0;

            bool bAll = (requests.at(requests.size() - 1).first == enAllvalues);
            if (bAll) // request with "*"
                requests.pop_back();

            tintaConfNode * rezNode = findNode(requests, mRootNode, indexreq);
            StringStreamBasic msg;
            if ( rezNode ){ //last node in request- value found
                if ( bAll ){
                    size_t chq = rezNode->childQuantity();
                    rezval.reserve(chq);
                    for (size_t k = 0; k < chq; k++){
                        tintaConfNode *chn = rezNode->getChild(k);

                        CoreAssert(chn,"chn == NULL");

                        if ( chn ){

                            T v = T();
                            if ( chn->getVal(v) )
                                rezval.push_back(v);                                                                                                    
                         }                        
                    }
                }
                else {
                    T v{};
                    if (rezNode->getVal(v))
                        rezval.push_back(v);
                }
            }

            if ( rezval.size() > 0 ){
                return true;
            }

            if (indexreq == 0) // root was not found					
                msg << "Root node was not found";
            else {

                //assert((indexreq - 1) < requests.size());
                CoreAssert((indexreq - 1) < requests.size(), "(indexreq - 1) >= requests.size()");

                if (bAll) // .* wrong request
                    msg << "Wrong request '.*' after: ";
                else
                    msg << "Wrong request part: ";

                if (requests[indexreq - 1].first > enNoIndex) {
                    msg << "[";
                    msg << requests[indexreq - 1].first;
                    msg << "]";
                }
                else
                    msg << requests[indexreq - 1].second;
            } 
            mLastError = msg.str();
            return false;
            //throw ConfigError( msg.str() );				
        }

		
	};

} // 

}

#endif
