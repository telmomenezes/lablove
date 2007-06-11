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

Lab* Lab::mLove = NULL;

Lab::Lab()
{
	mStop = false;
#ifdef __LOVELAB_WITH_GRAPHICS
	mOgreApp = NULL;
#endif
}

Lab::Lab(lua_State* luaState)
{
	mStop = false;
#ifdef __LOVELAB_WITH_GRAPHICS
	mOgreApp = NULL;
#endif
}

Lab::~Lab()
{
#ifdef __LOVELAB_WITH_GRAPHICS
	if (mOgreApp != NULL)
	{
		delete mOgreApp;
		mOgreApp = NULL;
	}
#endif
}

void Lab::create()
{
	mLove = this;
}

Lab& Lab::getInstance()
{	
	return *mLove;
}

void Lab::setSeedIndex(unsigned int index)
{
	randomSeedIndex(index);
}

void Lab::run()
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

void Lab::cycle()
{
	mSimulation->cycle();
}

const char Lab::mClassName[] = "Lab";

Orbit<Lab>::MethodType Lab::mMethods[] = {
        {"create", &Lab::create},
	{"setSimulation", &Lab::setSimulation},
        {"setSeedIndex", &Lab::setSeedIndex},
        {0,0}
};

Orbit<Lab>::NumberGlobalType Lab::mNumberGlobals[] = {{0,0}};

int Lab::create(lua_State* luaState)
{
        create();
        return 0;
}

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

