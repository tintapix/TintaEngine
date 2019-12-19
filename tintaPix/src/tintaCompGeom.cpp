/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaCompGeom.h"
#include "tintaUInt8Image.h"


namespace Tinta {


    //nsNameStrings::
    const char * const tintaCompGeom::str_prop_angle = "angle";
    const char * const tintaCompGeom::str_prop_circle_radius = "circle_radius";
    // minimal point step in random figure
    const char * const tintaCompGeom::str_prop_min_step = "min_step";
    // maximal point step in random figure
    const char * const tintaCompGeom::str_prop_max_step = "max_step";
    // nodes in graph
    const char * const tintaCompGeom::str_prop_node_quantity = "node_quantity";
    // max branches on node  (3 as default)
    const char * const tintaCompGeom::str_prop_branches_in_node = "branches_in_node";

    // graph node
    const char * const tintaCompGeom::str_prop_node = "node";
    // graph max size in pixels
    const char * const tintaCompGeom::str_prop_graph_max_size = "graph_max_size";
    const char * const tintaCompGeom::str_prop_point = "point";
    // fill method
    const char * const tintaCompGeom::str_prop_geom_fill = "fill";
    const char * const tintaCompGeom::str_prop_center_point = "center";
    const char * const tintaCompGeom::str_prop_spline_node = "spline_node";
    // property for fill_random_spread - for geometry   
    // pixel steps where drawing any not cycled figure
    const char * const tintaCompGeom::str_outline_pixel_step = "outline_pixel_step";


    const char * const tintaCompGeom::str_prop_buffer_id = "buffer_id";
    // minimal distance to find nearest point
    const char *const tintaCompGeom::str_prop_min_distance = "min_distance";
    // minimal points quantity must be found around point on minimal distance
    const char *const tintaCompGeom::str_prop_around_points = "around_points";
    const char *const tintaCompGeom::str_fill_method = "fill_method";
    

    tintaCompGeom::tintaCompGeom(bool selectable)
        :m_prop_changed(true)
        , mExtreme(MAX_INT32, MIN_INT32, MAX_INT32, MIN_INT32){

    }
    tintaCompGeom::tintaCompGeom()
        : m_prop_changed(true)
        , mExtreme(MAX_INT32, MIN_INT32, MAX_INT32, MIN_INT32)
    {
    }

    tintaCompGeom::~tintaCompGeom(void)
    {
    }



    bool tintaCompGeom::addProperty(const tintaCompGeomProp &prop)
    {
        std::pair<TProperSet::iterator, bool> rezult = m_properties.insert(TProperSet::value_type(prop.GetName(), prop));

        return rezult.second;
    }
    void tintaCompGeom::clearProperties(){
        TProperSetIter it = m_properties.begin();
        for (; it != m_properties.end(); it++){
            it->second.clear();
        }
    }
    bool tintaCompGeom::setPropVal(const tintaCompGeomProp &prop)
    {
        StringBasic name = prop.GetName();
        TProperSetIter iter_set = m_properties.find(prop.GetName());
        //assert(iter_set != m_properties.end());
        if (iter_set == m_properties.end())
            return false;
        iter_set->second.setVals(prop.getVal());
        // guess that value was changed
        iter_set->second.setAssigned(true);
        m_prop_changed = true;
        //*iter_set = prop;
        return true;
    }
    bool tintaCompGeom::addPropVal(const tintaCompGeomProp &prop)
    {
        TProperSetIter iter_set = m_properties.find(prop.GetName());
        //assert(iter_set != m_properties.end());
        if (iter_set == m_properties.end())
            return false;
        iter_set->second.addVals(prop.getVal());
        // guess that value was changed
        iter_set->second.setAssigned(true);
        m_prop_changed = true;
        return true;
    }
    bool tintaCompGeom::getPropVal(const char *propNme, tintaCompGeomProp &prop)const
    {
        tintaCompGeomProp prop_find(propNme);
        TProperSetConstIter iter_set = m_properties.find(prop_find.GetName());

        if (iter_set == m_properties.end() || !iter_set->second.getAssigned())
            return false;
        // guess that value was changed

        //tintaCompGeomProp *prop = iter_set->second;
        prop = iter_set->second;

        return true;
    }

    tintaCompGeomProp *tintaCompGeom::getPropValPtr(const StringBasic &propNme)
    {
        //tintaCompGeomProp prop_find(propNme);
        TProperSetIter iter_set = m_properties.find(propNme);

        if (iter_set == m_properties.end())
            return 0;
        iter_set->second.setAssigned(true);
        m_prop_changed = true;
        return &(iter_set->second);
    }

    const tintaCompGeomProp *tintaCompGeom::getPropValPtr(const StringBasic &propNme) const
    {
        tintaCompGeomProp prop_find(propNme);
        TProperSetConstIter iter_set = m_properties.find(prop_find.GetName());

        if (iter_set == m_properties.end())
            return 0;
        // guess that value was changed

        return &(iter_set->second);
    }


    bool tintaCompGeom::executeObj(float factor, ExecuteRez_t &rez) {

        rez.mTextError = "The function for this object is not supported!";
        rez.bRes = false;

        assert(false);
        return 0.f;
    }

    bool tintaCompGeom::executeObj(ExecuteRez_t &rez)
    {
        // void
        rez.mTextError = "The function for this object is not supported!";
        rez.bRes = false;

        assert(false);
        return false;
    }


    void tintaCompGeom::fillPoints(const GeomPoint *src, size_t size, const TExtremValues2i *extremes) {

        if (size == 0)
            return;

        if (extremes){
            mExtreme = *extremes;
            m_pointsRes.resize(size);
            copy(src, src + size, m_pointsRes.begin());
        }
        else {
            m_pointsRes.reserve(size);
            for (size_t i = 0; i < size; i++){
                mExtreme.extend(src[i]);
                m_pointsRes.push_back(src[i]);
            }
        }
    }

    TExtremValues2i tintaCompGeom::getExtreme() const {
        return mExtreme;
    }

    StringBasic tintaCompGeom::formPropWrongErr(const char *propName)  {

        StringStreamBasic msg;
        msg << "Property: ";
        msg << propName;
        msg << " was not assigned.";

        return msg.str();
    }


    void tintaCompGeom::clear() {
        m_pointsRes.clear();
    }
    // clear all data include result
    void tintaCompGeom::clearData() {
        // do nothing override it if you need
    }

    ///returns points quantity
    size_t tintaCompGeom::pointsQuantity() const {
        return m_pointsRes.size();
    }

    GeomPoint *tintaCompGeom::getPoints(size_t &out){
        out = m_pointsRes.size();
        if (out == 0)
            return 0;
        return &m_pointsRes[0];
    }
}
