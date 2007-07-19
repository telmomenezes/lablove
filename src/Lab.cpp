/*
 * LOVE Lab
 * Copyright (C) 2007 Telmo Menezes.
 * telmo@telmomenezes.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License 
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Lab.h"
#include "functions.h"
#include "random.h"

#if defined(__UNIX)
#include <sys/time.h>
#endif

#if defined(__WIN32)
#include <windows.h>
#include <time.h>
#if !defined( __GNUC__)
#define EPOCHFILETIME (116444736000000000i64)
#else
#define EPOCHFILETIME (116444736000000000LL)
#endif
#endif

Lab Lab::mLab;

Lab::Lab()
{
	mStop = false;
#if defined(__LABLOVE_WITH_GRAPHICS)
	mOgreApp = NULL;
#endif
}

Lab::Lab(lua_State* luaState)
{
	mStop = false;
#if defined (__LABLOVE_WITH_GRAPHICS)
	mOgreApp = NULL;
#endif
}

Lab::~Lab()
{
#if defined (__LABLOVE_WITH_GRAPHICS)
	if (mOgreApp != NULL)
	{
		delete mOgreApp;
		mOgreApp = NULL;
	}
#endif
}

Lab& Lab::getSingleton()
{	
	return mLab;
}

void Lab::setSeedIndex(unsigned int index)
{
	randomSeedIndex(index);
}

void Lab::run()
{
#if defined (__LABLOVE_WITH_GRAPHICS)
	mOgreApp = new OgreApplication();

	try
	{
		mOgreApp->init();
		mSimulation->init();
		mOgreApp->start();
	}
	catch (Ogre::Exception& e)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(NULL,
			e.getFullDescription().c_str(),
			"An exception has occured!",
			MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " <<
			e.getFullDescription().c_str() << std::endl;
#endif
	}
#else
	mSimulation->init();

	while (running())
	{
		cycle();
	}
#endif
}

void Lab::cycle()
{
	mSimulation->cycle();
}

#if defined(__UNIX)
double Lab::realTime()
{
	timeval time;
	gettimeofday(&time, NULL);

	double seconds = (double)time.tv_sec;
	seconds += ((double)time.tv_usec) / 1000000.0f;
	return seconds;
}
#endif

#if defined(__WIN32)
double Lab::realTime()
{
	FILETIME ft;
	LARGE_INTEGER li;
	__int64 t;
	static int tzflag;

	GetSystemTimeAsFileTime(&ft);
        li.LowPart  = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        t  = li.QuadPart;
        t -= EPOCHFILETIME;
        t /= 10;
        return ((double)t) / 1000000.0f;
}
#endif

const char Lab::mClassName[] = "Lab";

Orbit<Lab>::MethodType Lab::mMethods[] = {
	{"setSimulation", &Lab::setSimulation},
        {"setSeedIndex", &Lab::setSeedIndex},
        {0,0}
};

Orbit<Lab>::NumberGlobalType Lab::mNumberGlobals[] = {{0,0}};

int Lab::setSimulation(lua_State* luaState)
{
        Simulation* sim = (Simulation*)Orbit<Lab>::pointer(luaState, 1);
        setSimulation(sim);
        return 0;
}

int Lab::setSeedIndex(lua_State* luaState)
{
        int index = luaL_checkint(luaState, 1);
        setSeedIndex(index);
        return 0;
}

