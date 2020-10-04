#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#ifndef SAFE_DELETE
    #define SAFE_DELETE(P)				if(P)	{ delete (P);         (P) = NULL; }
    #define SAFE_ARRAY_DELETE(P)		if(P)	{ delete[] (P);       (P) = NULL; }
    #define SAFE_RELEASE(P)				if(P)	{ (P)->Release();     (P) = NULL; }
#endif

#ifndef MNew
    #if defined(_VERSION_WINDOWS)
        #define MNew(MemAddr,ClassName)			(((ClassName *)MemAddr)->ClassName::ClassName(),((ClassName *)MemAddr))
    #else
        #define MNew(MemAddr,ClassName)         ::new (MemAddr) ClassName();
    #endif
#endif

