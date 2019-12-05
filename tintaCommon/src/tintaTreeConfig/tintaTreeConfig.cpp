/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaTreeConfig.h"
#include "tintaCommon.h"


namespace Tinta {

    namespace TreeConfig {


    #define MAX_ERRORS 10

    tintaTreeConfig::tintaTreeConfig(void)
    :mRootNode(NULL ),
     mIsUtf8BOM( false ){
    }


    tintaTreeConfig::~tintaTreeConfig(void)
    {
	    if(mRootNode){
		    DELETE_T  ( mRootNode, tintaConfNode );
		    mRootNode = NULL;
	    }
    }

    /*
	    reads data from the file to buffer
    */
    tintaTreeConfig::tintaTreeConfig( const String &path)
    : mPath( path ),
      mRootNode( NULL ),
      mIsUtf8BOM( false )      
      {}


    tintaTreeConfig::tintaTreeConfig( const char *buffer, m_uint32 length )
        : mPath(_M("")),
        mRootNode(NULL),
        mIsUtf8BOM(false){

        if ( buffer && length > 0) {
            String buff(buffer);
            mBuffer = StringUtil::tokenise(buff, _M("\n"));
           
        }
    }


    /*
	    reads data from the file to Buffer
    */
    bool tintaTreeConfig::readFile( const String &path ){
	    bool r = false;
	    if ( path.length() > 0 ){			

		    mIsUtf8BOM = isUTF8WithBOM( path );
            	    
			r = readUTF8Text( path, mBuffer );

		    if( !r ){
			    StringStreamBasic msg;	
                msg << "EXCEPTION. Reading file: " << path << " error.";
                mLastError = msg.str();
                return false;
		    }		
	    }
	    return r;
    }

      
    


    tintaConfNode::TokenType tintaTreeConfig::validateValue( const String &val ) const {

	    if( val.length() >= 2 && val[0] == _M('"') && val[val.length() -1 ] == _M('"') ){
		    return tintaConfNode::enString;
	    }
	
	    if( val.compare(_M("true")) == 0 || val.compare(_M("false")) == 0 ){
		    return tintaConfNode::enBoolean;
	    }
	
    
	    try {
		
            std::string::size_type s;
            std::stod( val.c_str(), &s );
		    return tintaConfNode::enNumber;
	    }
        catch (std::invalid_argument e){
		    //return enNoType;	
		    ;
	    }
    
	    return tintaConfNode::enNoType;
    }

    // tintaConfNode::TokenType tintaTreeConfig::createMapData( size_t linebeg, size_t beg, size_t lineend, size_t end, String &rezData ) const {
    // 
    // 	Tinta::StringUtil::StrStreamType msg;
    // 	msg<<linebeg;
    // 	msg<<_M(",");
    // 	msg<<beg;
    // 	msg<<_M(",");	
    // 	msg<<lineend;
    // 	msg<<_M(",");
    // 	msg<<end;
    // 	rezData = msg.str();
    // 	return tintaConfNode::enMappos;
    // }

    void tintaTreeConfig::ifErrorAdd( bool rez, int line, const String &text ){

	    if( !rez ){
		    Tinta::StringUtil::StrStreamType msg; 	

		    if( line > -1 )
			    msg << _M("Error in line: ") <<  line << _M(" ") << text;
		    else // common errors
			    msg << _M("Error ") << text;

            mLastError =  msg.str();
		    //tokens.clear();
	    }
    }

    tintaConfNode* tintaTreeConfig::addNode( tintaConfNode * curParentNode, const tintaConfNode::Token &token, int beforeLevel ) {
	
	
	    if(!curParentNode)
		    return NULL;

	    tintaConfNode * parent = NULL;
	    // same level
	    if( beforeLevel == token.mlevel ){
		    assert( token.mlevel >= 0 );
		    parent = curParentNode;
		    tintaConfNode * newNode = NEW_T( tintaConfNode )( parent, token  );

		    if( !newNode->isValid() ){
			    DELETE_T (newNode, tintaConfNode );
			    parent = NULL;
		    }
		    else		
			    parent->addChild( newNode );
	    }
	    else if( beforeLevel > token.mlevel ){ // new node on lower level
		    //assert( token.mlevel - 1  >= 0 );
		    assert(curParentNode);
		    int downto = beforeLevel - token.mlevel; 

		    tintaConfNode * downNode = curParentNode;
		    while(downto > 0 && downNode){
			    downNode = downNode ? downNode->getParent() : NULL;
			    downto--;
		    }

		    parent = downNode; //curParentNode ? curParentNode->getParent() : NULL;
		

		    if( parent ){

			    tintaConfNode * newNode = NEW_T( tintaConfNode )( parent, token  );

			    if( !newNode->isValid() ){
				    DELETE_T ( newNode, tintaConfNode );
				    parent = NULL;
			    }
			    else {
				    if( parent ){
						    parent->addChild( newNode );
				    }
			    }
		    }
	    }
	    else { // beforeLevel < token.mlevel: new node on upper level situation "{ {"
		    assert( token.mlevel + 1  >= 0 );
		    int upto = token.mlevel - beforeLevel; 
		
		    tintaConfNode * lastNode = curParentNode;
		    while(upto > 0 && lastNode ){		
			    size_t chlds = lastNode->childQuantity();		
			    lastNode = lastNode->getChild( chlds - 1 ); // searching node
			    upto--;
		    }		 
		    assert( lastNode );
		    if( lastNode && lastNode->getType() == tintaConfNode::enNode ){ // last node must be not leaf with value
			    parent = lastNode; 
			    tintaConfNode * newNode = NEW_T( tintaConfNode )( parent, token );

			    if( !newNode->isValid() ){
				    DELETE_T ( newNode, tintaConfNode );
				    parent = NULL;
			    }
			    else {
				    if( parent ){
					    parent->addChild( newNode );
				    }
			    }
		    }
		    else 
			    parent = NULL;

	    }
	    return parent;
    }

    void tintaTreeConfig::updateValueToken( tokens_stack_t  &tokens, int level, tintaConfNode::TokenType type, const String &token, size_t endPos )const {
	    if( token.length() == 0 )
		    return;
	
	    size_t pos = tokens.size() - 1;
	    // no value - update 
	    if (   tokens.size() > 0		
		    && tokens.at( pos ).mValue.length() == 0
		    && tokens.at( pos ).mType  != tintaConfNode::enNode ){

				
		    tokens.at( pos ).mType  = type;
		    tokens.at( pos ).mValue = token;
		    tokens.at( pos ).posEnd = endPos;
	    }
	    else  { // have value - insert
		    tokens.push_back( tintaConfNode::Token( level, type, _M(""), token, endPos ) );		
	    }
    }

    String tintaTreeConfig::getError() const {	    
	    return mLastError;
    }
    

    size_t	tintaTreeConfig::childQuantity(const String &request)const {
	    requestChain requests;

        
	    fillReqests( request, requests );

        size_t indexreq = 0;
        const 	tintaConfNode * rezNode{ NULL_M };

	    if( requests.size() == 0 )
            rezNode = findNode(requests, NULL_M, indexreq);
        else    
            rezNode = findNode(requests, mRootNode, indexreq);

	    if( rezNode )
		    return rezNode->childQuantity();
	
	    return 0;	
    }

    const tintaConfNode *tintaTreeConfig::getChild(const String &request, const tintaConfNode *from )const {

        const tintaConfNode *parent = from ? from : mRootNode;
        if( !parent )        
            return NULL_M;

        return parent->getChild( request );

    }

    tintaConfNode  *tintaTreeConfig::getChild(const String &request,  tintaConfNode *from ) {

        tintaConfNode *parent = from ? from : mRootNode;
        if( !parent )
            return NULL_M;

        return parent->getChild(request);

    }

    const tintaConfNode *tintaTreeConfig::getChild(size_t index, const tintaConfNode *from )const {
        const tintaConfNode *parent = from ? from : mRootNode;
        if( !parent )
            return NULL_M;

        return parent->getChild(index);

    }

    tintaConfNode  *tintaTreeConfig::getChild(size_t index,  tintaConfNode *from ) {

        tintaConfNode *parent = from ? from : mRootNode;
        if( !parent )
            return mRootNode;

        return parent->getChild(index);
    }

    size_t tintaTreeConfig::childQuantity( tintaConfNode *from ) const {
        tintaConfNode *parent = from ? from : mRootNode;
        if( !parent )
            return 0;

        return parent->childQuantity();
    }

    const tintaConfNode *tintaTreeConfig::findNode(const tintaTreeConfig::requestChain &requests, const tintaConfNode *from, size_t &errPos)const{
	
	    if( !from )
		    return mRootNode;

        const tintaConfNode * rezNode = from;
	    requestChain::const_iterator it = requests.begin();
	    size_t indexreq = 0;
	    for ( ; it != requests.end() && rezNode; it++, indexreq++ ){
		    if ( it->first >enNoIndex )
			    rezNode = rezNode->getChild(it->first);
		    else
			    rezNode = rezNode->getChild(it->second);
	    }

	    errPos = indexreq;
	

	    return rezNode;
    }

    const tintaConfNode *tintaTreeConfig::findNode(const tintaTreeConfig::requestLink_t &request,  const tintaConfNode *from )const{
        if (!from)
            return mRootNode;
        const tintaConfNode * rezNode = from;
        
        if (request.first > enNoIndex)
            rezNode = from->getChild(request.first);
          else
            rezNode = from->getChild(request.second);
          

        return rezNode;
    }

    tintaConfNode *tintaTreeConfig::findNode(const tintaTreeConfig::requestLink_t &request, tintaConfNode *from ) {

        if ( !from )
            return NULL_M;
        tintaConfNode * rezNode = from;

        if (request.first > enNoIndex)
            rezNode = from->getChild(request.first);
        else
            rezNode = from->getChild(request.second);
    

    return rezNode;
    }


    /*
		    Returns true if there are any values in the tree
	    */
    bool tintaTreeConfig::hasValues() const {
	    return mRootNode && mRootNode->childQuantity() > 0;
    }

    tintaConfNode	   *tintaTreeConfig::findNode(const tintaTreeConfig::requestChain &requests, tintaConfNode *from, size_t &errPos){
	    if(!from)
            return mRootNode;


        tintaConfNode * rezNode = from;

	    requestChain::const_iterator it = requests.begin();
	    size_t indexreq = 0;
	    for ( ; it != requests.end() && rezNode; it++, indexreq++ ){
		    if ( it->first >enNoIndex )
			    rezNode = rezNode->getChild(it->first);
		    else
			    rezNode = rezNode->getChild(it->second);
	    }

	    errPos = indexreq;

	    return rezNode;
    }



    void tintaTreeConfig::fillReqests(const String &request, requestChain  &rez)const {

        if ( request.length() == 0 )
            return;

        StringVector r = StringUtil::tokenise(request, _M("."), _M(""), 10);
	    StringVector::const_iterator rit = r.begin();
	    for( ;rit!=r.end();rit++ ){
		    const String &req = *rit;
		    size_t l = req.length();

		    // trying to find .* - all values on level 			
		    if( req.size() == 1 && req.at(0) == _M('*') ){				
			    rez.push_back( requestLink_t( enAllvalues, _M("") ) );
			    break; // TODO [*] for leafs only
		    }

		    if( l > 2 && req.at(0) == _M('[') 
				      && req.at(l - 1) == _M(']') ){ // number [0] three symbols minimum	
            
			    try {
                    std::string::size_type s;
                    if ( req.length() == 3 && req.at(1) == '*' ){
                        rez.push_back(requestLink_t(enNoIndex, req));
                    }
                    else {

                        int iVal = std::stoi(req.substr(1, l - 2), &s);
                        rez.push_back(requestLink_t(iVal, _M("")));
                    }
				    
			    }
                catch ( std::invalid_argument e ){				    
                    StringStream msg;
                    msg << "Wrong request part: ";                    
                    msg << req;                   

                    mLastError = msg.str();
                    return;
			    }			
                
		    }
		    else {
			    rez.push_back(requestLink_t(enNoIndex, req));
		    }
	    }
    }

    String*	tintaTreeConfig::getTokensDebug( size_t &quantity ) {
	    quantity = tokensDebug.size();
	    if(quantity){
		    quantity = tokensDebug.size();
		    return &tokensDebug[0];
	    }
	    return 0;
	
    }

    

    bool tintaTreeConfig::setValue(const String &request, const double &val, bool discardType)  {
       
        std::vector<double> v{ val };
       return  setAddValues(request, v, tintaConfNode::enNumber, discardType);

    }

    bool tintaTreeConfig::setValue(const String &request, const int &val, bool discardType)  {
       
        std::vector<double> v{ (double)val };
        return setAddValues(request, v, tintaConfNode::enNumber, discardType);

    }

    bool tintaTreeConfig::setValue(const String &request, const float &val, bool discardType)  {
       
        std::vector<double> v{ (double)val };
        return setAddValues(request, v, tintaConfNode::enNumber, discardType);

    }

    bool tintaTreeConfig::setValue(const String &request, const String &val, bool discardType)  {
       
        std::vector<String> v{ val };
        return  setAddValues(request, v, tintaConfNode::enString, discardType);

    }

    bool tintaTreeConfig::setValue(const String &request, const bool &val, bool discardType)  {
        
        std::vector<bool> v{ val };
        return setAddValues(request, v, tintaConfNode::enBoolean, discardType);

    }

    bool tintaTreeConfig::setValue(const String &request, const char_m *val, bool discardType)  {
       
        std::vector<String> v{ val };
        return setAddValues(request, v, tintaConfNode::enString, discardType);

    }

    bool tintaTreeConfig::setValues(const String &request, const std::vector<String> &vals, bool discardType){
       
        return setAddValues(request, vals, tintaConfNode::enString, discardType);
    }

    bool tintaTreeConfig::setValues(const String &request, const std::vector<bool> &vals, bool discardType){
        
        return setAddValues(request, vals, tintaConfNode::enBoolean, discardType);
    }

    bool tintaTreeConfig::setValues(const String &request, const std::vector<double> &vals, bool discardType) {
        
        return setAddValues(request, vals, tintaConfNode::enNumber, discardType);
    }

    bool tintaTreeConfig::getValue(const String &request, double &val){
        return getVal(request, val);
    }

    bool tintaTreeConfig::getValue(const String &request, String &val){
        return getVal(request, val);
    }

    bool tintaTreeConfig::getValue(const String &request, bool &val) {
        return getVal(request, val);
    }

    bool tintaTreeConfig::getValues(const String &request, std::vector<double> &val) {
        return getVals(request, val);
    }

    bool tintaTreeConfig::getValues(const String &request, std::vector<String> &val) {
        return getVals(request, val);
    }

    bool tintaTreeConfig::getValues(const String &request, std::vector<bool> &val){
        return getVals(request, val);
    }


    bool tintaTreeConfig::parse(const char *buffer, m_uint32 length) {

        if ( buffer && length > 0 ) {
            String buff( buffer );
            mBuffer = StringUtil::tokenise( buff, _M("\n") );
        }
        return parse();
    }

    /*
	    reads(if path != "") and parses buffer
    */
    bool tintaTreeConfig::parse( const String &path ){

        clearError();

	    bool r = true;

        tokenStack.clear();
	    tokensDebug.clear();

        if (path.length() > 0) {
            mBuffer.clear();
            mPath = path;
        }

	    mRootNode = NEW_T( tintaConfNode )( NULL, tintaConfNode::Token(0, tintaConfNode::enNode, _M("") , _M("") )  );	
	    // parsing		
	    // first pass: removing all spaces, removing  all comments, testing for the correct brace closing {{ }}		
		    String finalBuff;		
		
            if ( mBuffer.size() == 0 ) {
                r = readFile( mPath );

                if ( !r ) {
                    StringStreamBasic msg;
                    msg << "EXCEPTION. Wrong path.";
                    mLastError = msg.str();
                    return false;
                }
                else 
                    mPath = path;                
            }

		    size_t linePos= 0;
		    size_t globPos= 0;
		    size_t globValPos= 0;
		    //size_t linePosBefore= 0; // updates after value inserted
		    unsigned mode( 0 );			
		
		    int  level = 0;
		    String curToken;

		    // saving '{' '}' ','
		    char_m lastSpecSymbol = _M('%'); // first symbol
		    char_m lastCommonSymbol = _M(' '); //  symbol
		    typedef std::vector<char_m> charStack_t;
		    while( linePos < mBuffer.size() && r )	{	

			    size_t  iPosCur = 0;
			    size_t  iBeforeCur = 0;
			    String  lineText = mBuffer[linePos];
			
			   // if( linePos == 0 && mIsUtf8BOM ) { // first symbol in file utf8 with BOM 
			//	    iPosCur = 1;
				    //globPos = 1;
			  //  }
			
                if (mode & enModeComment) {
                    mode &= ~enModeComment;
                }

                while ( iPosCur < lineText.length() && r ) {
                  
                    if (mode ^ enModeComment) {

                        // comment "//" not in string param or inside "/**/"
                        if (mode ^ enModeStringParam
                            && mode ^ enModeLongComment
                            && iPosCur != iBeforeCur
                            && lineText.at(iPosCur) == _M('/')
                            && lineText.at(iBeforeCur) == _M('/')) { // comment was found												    

                            mode |= enModeComment;

                            //break;
                        }


                        if (mode & enModeLongComment) {

                            if (lineText.at(iPosCur) == _M('/')
                                && lineText.at(iBeforeCur) == _M('*')) { // comment was found	

                                mode &= ~enModeLongComment;
                            }
                        }
                        else {

                            if (mode ^ enModeStringParam                              
                                && iPosCur != iBeforeCur
                                && lineText.at(iPosCur) == _M('*')
                                && lineText.at(iBeforeCur) == _M('/')) { // comment was found					
                                mode |= enModeLongComment;
                                
                            }
                            else {

                                if (lineText.at(iPosCur) == _M('"')) {
                                    // if `\"` - comment not finished
                                    if (mode & enModeStringParam && lineText.at(iBeforeCur) != _M('\\')) {
                                        mode &= ~enModeStringParam;
                                    }
                                    else {
                                        // comment mode
                                        mode |= enModeStringParam;
                                    }
                                }


                                // ignoring some symbols or passing them in the text block
                                if (mode & enModeStringParam) {
                                    curToken.push_back(lineText.at(iPosCur));
                                    lastCommonSymbol = _M(' ');

                                }
                                else if (lineText.at(iPosCur) != _M(' ')
                                    && lineText.at(iPosCur) != _M('/')
                                    && lineText.at(iPosCur) != _M('\\')
                                    && lineText.at(iPosCur) != _M('\n')
                                    && lineText.at(iPosCur) != _M('\t'))
                                {

                                    charStack_t permited;

                                    if (lineText.at(iPosCur) == _M('=')) { 										  //	`=`					


                                        if (curToken.length() == 0) {
                                            r = false;
                                            ifErrorAdd(r, (int)linePos + 1, _M("Wrong name before '='"));
                                            break;
                                        }

                                        // trying add new name token but old value is empty( no delimiter found: ',' '{' '}' )
                                        if (tokenStack.size() > 0
                                            && tokenStack.at(tokenStack.size() - 1).mValue.length() == 0
                                            && tokenStack.at(tokenStack.size() - 1).mType != tintaConfNode::enNode) {
                                            r = false;
                                            Tinta::StringUtil::StrStreamType msg;
                                            msg << _M("Wrong value before: ");
                                            msg << curToken;
                                            ifErrorAdd(r, (int)linePos + 1, msg.str());
                                            break;
                                        }

                                        size_t f = curToken.find(_M('.')); // '.' not allowed in names
                                        if (f != String::npos) {
                                            r = false;
                                            Tinta::StringUtil::StrStreamType msg;
                                            msg << _M("Forbidden symbol in name: ");
                                            msg << lineText.at(iPosCur);
                                            ifErrorAdd(r, (int)linePos + 1, msg.str());
                                            break;
                                        }
                                        // saving name token
                                        tokenStack.push_back(tintaConfNode::Token(level, tintaConfNode::enNoType, curToken, _M("")));
                                        lastSpecSymbol = lineText.at(iPosCur);
                                        curToken.clear();
                                    }
                                    else if (lineText.at(iPosCur) == _M('}')
                                        || lineText.at(iPosCur) == _M(',')) {   //	`}` `,` 	trying save value												
                                        bool needDecr = false;
                                        permited.clear();
                                        permited.push_back(_M('}')); // allows such expressions: 'val1=1000},' or 'val1=1000}}'
                                        if (lineText.at(iPosCur) == _M('}')) {
                                            needDecr = true;
                                            if (curToken.length() == 0)
                                                permited.push_back(_M(',')); // allows such expressions: 'val1=1000,}'
                                        }

                                        // before '}' we must have value token
                                        tintaConfNode::TokenType type = tintaConfNode::enNoType;
                                        type = validateValue(curToken);

                                        if (curToken.length() != 0) { // allow empty tokens '{};
                                            charStack_t::const_iterator it = std::find(permited.begin(), permited.end(), lastSpecSymbol);
                                            if (type == tintaConfNode::enNoType  && it == permited.end()) {
                                                r = false;
                                                Tinta::StringUtil::StrStreamType msg;
                                                msg << _M("Wrong value before ");
                                                msg << lineText.at(iPosCur);
                                                ifErrorAdd(r, (int)linePos + 1, msg.str());
                                                break;
                                            }

                                            // +1 - selecting symbol before
                                            updateValueToken(tokenStack, level, type, curToken, globValPos + 1);
                                        }

                                        if (needDecr)
                                            level--;
                                        curToken.clear();
                                        globValPos = 0;
                                        lastSpecSymbol = lineText.at(iPosCur);
                                    }
                                    else if (lineText.at(iPosCur) == _M('{')) {										 // `{`
                                        permited.clear();
                                        permited.push_back(_M(','));
                                        permited.push_back(_M('{'));// allows  such expressions '{{'
                                        permited.push_back(_M('%'));// ignore ',' and first brace

                                        charStack_t::const_iterator it = std::find(permited.begin(), permited.end(), lastSpecSymbol);

                                        if (it == permited.end()) {
                                            r = false;
                                            ifErrorAdd(r, (int)linePos + 1, _M("Wrong token before '{'"));
                                            break;
                                        }

                                        size_t f = curToken.find(_M('.')); // '.' not allowed in names
                                        if (f != String::npos) {
                                            r = false;
                                            Tinta::StringUtil::StrStreamType msg;
                                            msg << _M("Forbidden symbol in name: ");
                                            msg << lineText.at(iPosCur);
                                            ifErrorAdd(r, (int)linePos + 1, msg.str());
                                            break;
                                        }

                                        // saving name of the namespace if we have it 'namespace{'
                                        tokenStack.push_back(tintaConfNode::Token(level, tintaConfNode::enNode, curToken, _M("")));
                                        curToken.clear();
                                        globValPos = 0;

                                        // increasing levels depth	
                                        level++;
                                        lastSpecSymbol = lineText.at(iPosCur);
                                    }
                                    else { // token data	
                                        permited.clear();
                                        permited.push_back(_M('{'));
                                        permited.push_back(_M('='));
                                        permited.push_back(_M(','));
                                        permited.push_back(_M('%'));
                                        charStack_t::const_iterator it = std::find(permited.begin(), permited.end(), lastSpecSymbol);
                                        // preventing breaking tokens names
                                        if (lastCommonSymbol != _M(' ') && curToken.length() > 0) {
                                            r = false;
                                            Tinta::StringUtil::StrStreamType msg;
                                            msg << _M("No delimeter after ");
                                            msg << curToken;
                                            ifErrorAdd(r, (int)linePos + 1, msg.str());
                                            break;
                                        }

                                        if (it == permited.end()) {
                                            r = false;
                                            ifErrorAdd(r, (int)linePos + 1, _M("No delimeter before "));
                                            break;
                                        }
                                        else {
                                            globValPos = globPos;
                                            curToken.push_back(lineText.at(iPosCur));
                                            lastCommonSymbol = _M(' ');
                                        }
                                    }
                                }
                                else { // if any token began forming we casting error 							
                                    // saving additional symbol					
                                    lastCommonSymbol = lineText.at(iPosCur);
                                }
                            }
                        }
                    }
				    iBeforeCur = iPosCur;
				    iPosCur++;
				    globPos++;
			    }
			    lastCommonSymbol = _M(' ');
			    // new line for sring values in map mode not allowed in this version
			    
			    linePos++;		
                
    //#if CORE_PLATFORM  == CORE_PLATFORM_WIN32			
	//			    globPos += 2;
    //#else			
				    globPos++;				
    //#endif	
            }
		    // in the end trying to find not tokenized value
		    if( curToken.length() != 0 && r ){
 			    charStack_t permited;
 			    permited.clear();			
 			    permited.push_back( _M(',') ); // allows such expressions: ',100'
 			    permited.push_back( _M('=') ); // allows such expressions: ',v1=100'
 
 			    // before '}' we must have value token
			    tintaConfNode::TokenType type = tintaConfNode::enNoType;				
			
			    type = validateValue( curToken );

 			    charStack_t::const_iterator it = std::find(permited.begin(), permited.end(), lastSpecSymbol );
 			    if(  type == tintaConfNode::enNoType  && it == permited.end() ){ 
 				    r = false;
 				    Tinta::StringUtil::StrStreamType msg; 
 				    msg << _M("Wrong value(\" = val\") or delimeter(\",\") after: ");
 				    msg << curToken;
 				    ifErrorAdd( r, (int)linePos + 1, msg.str() );
 				
 			    }
 			    updateValueToken(tokenStack, level, type, curToken , globValPos+1 );
		    }

		    if( r ){

                if ( mode & enModeLongComment ) {
                    r = false;
                    ifErrorAdd(r, -1, _M("Not closed comment '/*' "));
                }

			    if( level > 0  ){
				    r = false;
				    ifErrorAdd( r, -1, _M("Not all opened braces '{' are closed" ) );
			    }
			    else if ( level < 0 ){
				    r = false;
				    ifErrorAdd( r, -1, _M("Too many closed '}' braces" ) );

			    }
		    }
		
	    if( !r )
            tokenStack.clear();

	    if(r){
		    tokens_stack_t::const_iterator rit = tokenStack.begin();
		    tintaConfNode *curParentNode = mRootNode;
		    int curLenvel = 0;
		    for( ;rit!= tokenStack.end() && r ; rit++){			 

			    curParentNode = addNode( curParentNode, *rit , curLenvel );
			    if (!curParentNode){
				    r = false;
				    Tinta::StringUtil::StrStreamType msgErr;
                    msgErr << _M("Error while adding token name: ");
                    msgErr << rit->mName;
                    msgErr << _M(" value: ");
                    msgErr << rit->mValue;
                    msgErr << _M(" level: ");
                    msgErr << rit->mlevel;
				    ifErrorAdd(r, -1, msgErr.str() );
			    }
			    curLenvel = rit->mlevel;
    #if DEBUG_MODE		
               Tinta::StringUtil::StrStreamType msg;
			    //int level = rit->mMult;
			    for(int i = -1; i < curLenvel; i++)
				    msg<<_M("-");

			    msg<<_M("Name: ");
			    msg<<rit->mName;
			    msg<<_M(" Type: ");
			    tintaConfNode::TokenType type = rit->mType;
			    switch( type ){
				    case tintaConfNode::enNoType:
					    msg<<_M(" NoType ");
					    break;
				    case tintaConfNode::enNumber:
					    msg<<_M(" Number ");
					    break;
				    case tintaConfNode::enString:
					    msg<<_M(" String ");
					    break;
				    case tintaConfNode::enBoolean:
					    msg<<_M(" Boolean ");
					    break;
				    case tintaConfNode::enNode:
					    msg<<_M(" enNode ");
					    break;
			    }
			    msg<<_M(" Value: ");
			    msg<<rit->mValue;
			    tokensDebug.push_back(msg.str());

    #endif
		    }		

	    }
	    if( !r ){
		    //assert(r);
		    // destroying  tree
		    if( mRootNode ){
			    DELETE_T  ( mRootNode, tintaConfNode );
			    mRootNode = NULL;
		    }
	    }
	    return r;	
    }


    void tintaTreeConfig::clearError() {
        mLastError = "";
    }
    String tintaTreeConfig::formString( const tintaConfNode * dataNode )const {

	    if( !dataNode )
		    return String();
	    Tinta::StringUtil::StrStreamType rez; 
	    String name = dataNode->getName();

	    int lev = dataNode->getLevel()-1;//first offset ignored
	    for(int t = 0; t < lev;t++	)
		    rez << _M("\t");

	    if( dataNode->getType() == tintaConfNode::enNode ){	
	      rez << name;
	      //rez << _M('{');
	    }
	    else {
		    if( name.length() != 0 )			rez<< name <<_M(" = ");
				
		    switch( dataNode->getType() ){
			    case tintaConfNode::enNumber: 
				    {				
					    m_float64 rezVal = 0.;
					    dataNode->getVal( rezVal );					
					    rez<<rezVal;
				    }
				    break;
			    case tintaConfNode::enString: 
				    {					
					    String rezVal;
					    dataNode->getVal( rezVal );					
					    rez<<_M("\"") << rezVal.c_str() << _M("\"");
				    }
				    break;
			    case tintaConfNode::enBoolean:
				    {					
					    bool rezVal = false;
					    dataNode->getVal( rezVal );
                        rez << std::boolalpha << rezVal;
					    
				    }
				    break;
			    default :
				    break;
		    }				
	    }

	    return  rez.str();	
    }



    void tintaTreeConfig::fillLinesFromNodes( const tintaConfNode * dataNode, StringVector &nodes, bool last){

	    if( !dataNode )
		    return;

	
	    size_t childs = dataNode->childQuantity();
	    if( dataNode->childQuantity() == 0 ){ // leaf
		
		    if( last )
			    nodes.push_back(formString( dataNode ));
		    else {
			    String end = formString( dataNode );
			    end.push_back(_M(','));
			    nodes.push_back(end);
		    }
		    return;
	    }
	    else {
		    if( dataNode->getParent() ) { // ignore for root	
                    //if (dataNode.mName.)
				    nodes.push_back( formString( dataNode ) );
				    Tinta::StringUtil::StrStreamType rez;
				    int lev = dataNode->getLevel()-1;
				    for(int t = 0; t < lev;t++	)
					    rez << _M("\t");
				    rez<<_M("{");
				    nodes.push_back( rez.str() );
		    }
		
		    for( size_t i = 0; i < childs; i++ ){				
			    fillLinesFromNodes( dataNode->getChild(i), nodes, i == (childs-1) );			
		    }
		    if( dataNode->getParent() ) {// ignore for root
			    Tinta::StringUtil::StrStreamType rez; 
			    int lev = dataNode->getLevel()-1; //first offset ignored
			    for(int t = 0; t < lev;t++	)
				    rez << _M("\t");
			    rez << _M("}");
			    if( !last )		rez << _M(',');
			    nodes.push_back(rez.str());
			
		    }
	    }
    }

    /*
	    saves tree in to the file
    */
    bool tintaTreeConfig::save(const String &path){

	    if( path.length() != 0 )
		    mPath = path;

	    if( !mRootNode ){
            StringStreamBasic msg;
		    msg << "Tree have no data.";
           
            mLastError = msg.str();
            return false;
	    }

	    StringVector toSave;
	    fillLinesFromNodes( mRootNode, toSave );

	    bool r = false;
	   // if(mIsUtf8BOM)
        r = writeUTF8toFile(mPath, toSave, false);
	   // else
         //   r = writeUTF8NOBOMtoFile(mPath, toSave, false);

	    if( !r ){
		    StringStreamBasic msg; 		
            msg << "Writing file: " << mPath << " error.";
            mLastError = msg.str();
            return false;
	    }	

        return true;
    // 	typedef std::vector<tintaConfNode *> nodes_t;
    // 	nodes_t toCalculate;
    // 	while( toCalculate.size() > 0 ){
    // 
    // 		const tintaConfNode * data = mRootNode;
    // 		const tintaConfNode * parent = NULL;
    // 		
    // 		size_t index = 0;
    // 		//if( parent == data )
    // 
    // 		//String toWrite = formString( data );
    // 		//if( data->getChild() )
    // 
    // 	}	
	   
    }


}


}
