/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef  _TINTA_COMMON_IMAGE_FACTORY_H_
#define  _TINTA_COMMON_IMAGE_FACTORY_H_

#include <tintaPredefine.h>
#include <tintaImageFactory.h>

namespace Tinta {
	class _CoreExport tintaCommonImageFactory :
		public tintaImageFactory {

	public:
		tintaCommonImageFactory(){}
		virtual ~tintaCommonImageFactory() {}

		const StringBasic& getName(void) const;

		tintaUInt8Image *createInstance() const {
			return M_NEW Tinta::tintaUInt8Image();

		}
		void releaseInstance(Tinta::tintaUInt8Image * obj) {
			M_DELETE obj;
		}
		//virtual void releaseInstance( tintaCompGeom* obj) { M_DELETE obj; }
	};


}

#endif