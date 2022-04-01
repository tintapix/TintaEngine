/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaVisibleSet.h"

namespace Graphica {



tintaVisibleSet::tintaVisibleSet (int iMaxQuantity, int iGrowBy)
{
    m_akVisible = 0;
    Resize(iMaxQuantity,iGrowBy);
	
}

tintaVisibleSet::~tintaVisibleSet ()
{
   DELETE_ARRAY_T(m_akVisible,tintaVisibleObject, (unsigned)m_allocated_obj);
   m_allocated_obj = 0;
}

void tintaVisibleSet::Insert(tintaDrawable* pkObject)
{
    // Increase the array storage if necessary.
    if (++m_iQuantity > m_iMaxQuantity)
    {
        int iNewMaxQuantity = m_iMaxQuantity + m_iGrowBy;
        tintaVisibleObject* akNewVisible = NEW_ARRAY_T (tintaVisibleObject,iNewMaxQuantity);
		unsigned allocated_old = (unsigned)m_allocated_obj;
		m_allocated_obj = iNewMaxQuantity;
		
        size_t uiSize = m_iQuantity * sizeof(tintaVisibleObject);
        mlMemcpy(akNewVisible,uiSize,m_akVisible,uiSize);
        DELETE_ARRAY_T(m_akVisible,tintaVisibleObject, allocated_old );
        m_akVisible = akNewVisible;
        m_iMaxQuantity = iNewMaxQuantity;
    }
    
    int iIndex = m_iQuantity-1;
    m_akVisible[iIndex].Object = pkObject;
    
}

void tintaVisibleSet::Resize (int iMaxQuantity, int iGrowBy)
{
    if (iMaxQuantity > 0)
    {
        m_iMaxQuantity = iMaxQuantity;
    }
    else
    {
        m_iMaxQuantity = VS_DEFAULT_MAX_QUANTITY;
    }

    if (iGrowBy > 0)
    {
        m_iGrowBy = iGrowBy;
    }
    else
    {
        m_iGrowBy = VS_DEFAULT_GROWBY;
    }

    DELETE_ARRAY_T( m_akVisible,tintaVisibleObject, (unsigned)m_iQuantity );
    m_iQuantity = 0;
    m_akVisible = NEW_ARRAY_T (tintaVisibleObject,m_iMaxQuantity);
	m_allocated_obj = m_iMaxQuantity;
}

}

