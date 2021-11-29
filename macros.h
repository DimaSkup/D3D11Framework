// last revising at 09.11.21

#pragma once 

/*

template<typename T> void remove(T*& p) { delete p; p = nullptr; }

*/

#define _DELETE(p)			{ if (p) { delete(p); p = nullptr; }}
#define _DELETE_ARRAY(p)	{ if (p) { delete[](p); p = nullptr; }}
#define _RELEASE(p)			{ if (p) { (p)->Release(); p = nullptr; }}
#define _CLOSE(p)			{ if (p) { (p)->Close(); delete(p); p = nullptr; }}

// current version of the framework
#define _VERSION_FRAMEWORK 2