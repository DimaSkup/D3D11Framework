#pragma once

#include <ctime>
#include <cassert>

#include <string>
#include <list>

#define WIN32_MEAN_AND_LEAN
#include <windows.h>

#include <d3d11.h>
#include <d3dx11.h>

#pragma comment (lib, "d3d11.lib")

#ifdef _DEBUG
#		pragma comment (lib, "d3dx11d.lib")
#else
#		pragma comment (lib, "d3dx11.lib")
#endif

#pragma warning (disable : 4996)