/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_VISIBLE_SET_H
#define _TINTA_VISIBLE_SET_H

#include "tintaGraphicaPredefine.h"
#include "tintaVisibleObject.h"

namespace Graphica
{

class  _CoreExport tintaVisibleSet
{
public:
    tintaVisibleSet (int iMaxQuantity = 0, int iGrowBy = 0);
    virtual ~tintaVisibleSet ();

    int GetQuantity () const;
    tintaVisibleObject* GetVisible ();
    tintaVisibleObject& GetVisible (int i);
        
    void Insert(tintaDrawable* pkObject);
    void Clear ();
    void Resize (int iMaxQuantity, int iGrowBy);

private:
    enum
    {
        VS_DEFAULT_MAX_QUANTITY = 32,
        VS_DEFAULT_GROWBY = 32,
    };

    int m_iMaxQuantity, m_iGrowBy, m_iQuantity;
    tintaVisibleObject* m_akVisible;
	unsigned m_allocated_obj;
};

inline int tintaVisibleSet::GetQuantity () const
{
	return m_iQuantity;
}

inline tintaVisibleObject* tintaVisibleSet::GetVisible ()
{
	return m_akVisible;
}

inline tintaVisibleObject& tintaVisibleSet::GetVisible (int i)
{
	CoreAssert(0 <= i && i < m_iQuantity,"0 <= i && i < m_iQuantity");
	return m_akVisible[i];
}

inline void tintaVisibleSet::Clear ()
{
	m_iQuantity = 0;
}

}

#endif
