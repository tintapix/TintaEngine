#include "tintaGraphicaPrecompile.h"
#include "tintaMeshParser.h"

namespace Graphica {

    tintaMeshParser::tintaMeshParser()
    {
    }

    tintaMeshParser::~tintaMeshParser()
    {
    }

    tintaMeshData  tintaMeshParser::parse(const String &path){

        if ( path.length() == 0 || !isPathValid(path) ){
            EXCEPTION(_M("wrong mesh path!"));
        }
        tintaMeshData data;
        data.mMeshName = StringUtil::getGetFilenameFromPath( path );

        mf::tintaTreeConfig conf(path);

        conf.parse();

        
        String e = conf.getError();
        if (e.length() > 0){

            Tinta::StringUtil::StrStreamType msg;
            msg << _M("Error while parsing mesh file: ");
            msg << _M(" file ") << data.mMeshName << _M("\n");            
            msg << e << _M("\n");
            

            EXCEPTION(msg.str());
        }

        size_t q = conf.childQuantity(_M("vertices"));
        data.mVertices.reserve(q);

        for (size_t i = 0; i < q; i++) {
            StringStream s;
            s << _M("vertices.[");
            s << i << _M("].*");
            
            std::vector<double> vVals; // vector data {0,0,0}
            conf.getValues( s.str(), vVals );
            if ( vVals.size() != 3 ) {
                EXCEPTION(_M("wrong mesh vertex data!"));
            }
            data.mVertices.push_back(tintaVector3f((float)vVals.at(0), (float)vVals.at(1), (float)vVals.at(2)));
        }

        q = conf.childQuantity(_M("indices"));
        data.mIndices.reserve( q );
        for (size_t i = 0; i < q; i++) {
            StringStream s;
            s << _M("indices.[");
            s << i << _M("]");
            double ind;
            conf.getValue( s.str(), ind );           
            data.mIndices.push_back( (int)ind );                       
        }
        q = conf.childQuantity(_M("tcoordinates"));
        data.mTcoordinates.reserve(q);

        for (size_t i = 0; i < q; i++) {
            StringStream s;
            s << _M("tcoordinates.[");
            s << i << _M("].*");
            std::vector<double> vVals; // vector data {0,0,0}
            conf.getValues(s.str(), vVals);
            if (vVals.size() != 2) {
                EXCEPTION(_M("wrong mesh vertex data!"));
            }
            data.mTcoordinates.push_back( tintaVector2f( (float)vVals.at(0), (float)vVals.at(1) ) );
        }
        //data.mVertices
        return data;
    }

   
}