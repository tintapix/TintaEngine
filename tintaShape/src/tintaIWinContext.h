#ifndef _TINTA_WIN_CONTEXT_H_
#define _TINTA_WIN_CONTEXT_H_

namespace Graphica {

/// Platform specific functions
struct tintaIWinContext {     

    virtual void displayBackBuffer () = 0;

    virtual void SaveContext ()       = 0;

    virtual void RestoreContext ()    = 0;

    virtual bool IsActive () const    = 0;

    virtual void activate ()          = 0;
};

}
#endif