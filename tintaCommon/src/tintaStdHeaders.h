/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_STD_HEADERS_H_
#define _TINTA_STD_HEADERS_H_

#include "tintaPlatform.h"

#include <cstring>
#include <cstdarg>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <stdarg.h>
#include <time.h>
#include <typeinfo>
#include <iterator>
#include <random>

#include <algorithm>
#include <functional>
#include <limits>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <regex>
#include <numeric>
#include <signal.h>



#if CORE_PLATFORM == CORE_PLATFORM_WIN32 
    #include <codecvt> // libc++ not support in GCC 4.9 
    #include <direct.h>
#endif
#include <atomic>

extern "C" {
	#   include <sys/types.h>
	#   include <sys/stat.h>
    #   include <sys/timeb.h>
}

#endif

