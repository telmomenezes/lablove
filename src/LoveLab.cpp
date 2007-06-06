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

#include "LoveLab.h"
#include "functions.h"
#include "random.h"

LoveLab* LoveLab::mLove = NULL;

LoveLab::LoveLab()
{
	mStop = false;
#ifdef __LOVELAB_WITH_GRAPHICS
	mOgreApp = NULL;
#endif
}

LoveLab::LoveLab(lua_State* luaState)
{
	mStop = false;
#ifdef __LOVELAB_WITH_GRAPHICS
	mOgreApp = NULL;
#endif
}

LoveLab::~LoveLab()
{
#ifdef __LOVELAB_WITH_GRAPHICS
	if (mOgreApp != NULL)
	{
		delete mOgreApp;
		mOgreApp = NULL;
	}
#endif
}

void LoveLab::create()
{
	mLove = this;
	mLove->addKeyboardMouseHandler(this);
}

LoveLab& LoveLab::getInstance()
{	
	return *mLove;
}

void LoveLab::setSeedIndex(unsigned int index)
{
	randomSeedIndex(index);
}

void LoveLab::run()
{
#ifdef __LOVELAB_WITH_GRAPHICS
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

void LoveLab::cycle()
{
	mSimulation->cycle();

#ifdef __LOVELAB_WITH_GRAPHICS
/*
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		list<KeyboardMouseHandler*>::iterator iterHandler = mHandlersList.begin();
		bool handled = false;
		while ((!handled) && (iterHandler != mHandlersList.end()))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					(*iterHandler)->onKeyDown(event.key.keysym.sym);
					break;
				case SDL_KEYUP:
					(*iterHandler)->onKeyUp(event.key.keysym.sym);
					break;
				case SDL_MOUSEBUTTONDOWN:
					(*iterHandler)->onMouseButtonDown(event.button.button,
										event.button.x,
										mScreenHeight - event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					(*iterHandler)->onMouseButtonUp(event.button.button,
										event.button.x,
										mScreenHeight - event.button.y);
					break;
				case SDL_MOUSEMOTION:
					(*iterHandler)->onMouseMove(event.button.x,
									mScreenHeight - event.button.y);
					break;
				default:
					break;
			}

			iterHandler++;
		}
	}
*/
#endif
}

void LoveLab::addKeyboardMouseHandler(KeyboardMouseHandler* handler)
{
	mHandlersList.push_front(handler);
}

void LoveLab::removeKeyboardMouseHandler()
{
	mHandlersList.pop_front();
}

#ifdef __LOVELAB_WITH_GRAPHICS
/*
bool LoveLab::onKeyUp(int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		mStop = true;
		return true;
	default:
		return false;
	}
}
*/
#endif

const char LoveLab::mClassName[] = "LoveLab";

Orbit<LoveLab>::MethodType LoveLab::mMethods[] = {
        {"create", &LoveLab::create},
	{"setSimulation", &LoveLab::setSimulation},
        {"setSeedIndex", &LoveLab::setSeedIndex},
        {0,0}
};

Orbit<LoveLab>::NumberGlobalType LoveLab::mNumberGlobals[] = {{0,0}};

int LoveLab::create(lua_State* luaState)
{
        create();
        return 0;
}

int LoveLab::setSimulation(lua_State* luaState)
{
        Simulation* sim = (Simulation*)Orbit<LoveLab>::pointer(luaState, 1);
        setSimulation(sim);
        return 0;
}

int LoveLab::setSeedIndex(lua_State* luaState)
{
        int index = luaL_checkint(luaState, 1);
        setSeedIndex(index);
        return 0;
}

