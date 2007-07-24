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

#if defined(__LABLOVE_WITH_GRAPHICS)
#include "SDL.h"
#endif

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
	mScreen = NULL;
}

Lab::Lab(lua_State* luaState)
{
	mStop = false;
	mScreen = NULL;
}

Lab::~Lab()
{
	if (mScreen != NULL)
	{
		delete mScreen;
		mScreen = NULL;
	}
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
	addInputHandler(this);
	addInputHandler(mSimulation);

	mScreen = layer::ScreenManager::getSingleton().createScreen(layer::SDLOpenGL, 800, 600);
	mScreen->setCaption("LabLOVE");
	mScreen->setBackgroundColor(1.0f, 1.0f, 1.0f);

	mSimulation->init();

	while (running())
	{
		cycle();
	}
}

void Lab::cycle()
{
	mSimulation->cycle();
#if defined(__LABLOVE_WITH_GRAPHICS)
	processEvents();
#endif
}

void Lab::processEvents()
{
#if defined(__LABLOVE_WITH_GRAPHICS)
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		std::list<InputHandler*>::iterator iterHandler = mHandlers.begin();
                bool handled = false;
                while ((!handled) && (iterHandler != mHandlers.end()))
                {
                        switch (event.type)
                        {
				case SDL_KEYDOWN:
					handled = (*iterHandler)->onKeyDown(event.key.keysym.sym);
					break;
				case SDL_KEYUP:
					handled = (*iterHandler)->onKeyUp(event.key.keysym.sym);
					break;
				case SDL_MOUSEBUTTONDOWN:
					handled = (*iterHandler)->onMouseButtonDown(event.button.button, 
											event.button.x,
											mScreen->getHeight() - event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					handled = (*iterHandler)->onMouseButtonUp(event.button.button,
											event.button.x,
											mScreen->getHeight() - event.button.y);
					break;
				case SDL_MOUSEMOTION:
					handled = (*iterHandler)->onMouseMove(event.button.x,
										mScreen->getHeight() - event.button.y);
					break;
				default:
					break;
			}

			iterHandler++;
		}
	}
#endif
}

void Lab::addInputHandler(InputHandler* handler) 
{
        mHandlers.push_back(handler);
}

void Lab::removeInputHandler()
{
        mHandlers.pop_front();
}

bool Lab::onKeyDown(int keycode)
{
	switch (keycode)
	{
		case SDLK_ESCAPE:
			mStop = true;
			return true;
		default:
			return false;
	}
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

