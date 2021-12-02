#ifndef PCH_H
#define PCH_H

#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <stdlib.h>
#include <memory>
#include <iostream>
#include <stack>
#include <queue>
#include <math.h>

// SFML
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// LUA
#include <lua.hpp>

// GLOBAL FUNCTIONS
namespace ss
{
	template<class T> void deletePointers(T& i)
	{
		delete i;
		i = nullptr;
		std::cout << "PCH::TEMPLATE::Deleted a pointer" << std::endl;
	}
}

#endif