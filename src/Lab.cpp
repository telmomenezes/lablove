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
	mTranslateVector = Vector3::ZERO;
	mNumScreenShots = 0;
	mMoveScale = 0.0f;
	mRotScale = 0.0f;
	mMoveSpeed = 100;
	mRotateSpeed = 36;
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
	mLove->addInputHandler(this);
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

#ifdef __LOVELAB_WITH_GRAPHICS
	// Update camera position
 	mOgreApp->getCamera()->yaw(mRotX);
	mOgreApp->getCamera()->pitch(mRotY);
	mOgreApp->getCamera()->moveRelative(mTranslateVector);

/*
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		list<InputHandler*>::iterator iterHandler = mHandlersList.begin();
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

void Lab::addInputHandler(InputHandler* handler)
{
	mHandlersList.push_front(handler);
}

void Lab::removeInputHandler()
{
	mHandlersList.pop_front();
}

#ifdef __LOVELAB_WITH_GRAPHICS
bool Lab::onKeyDown(int key)
{
	switch (key)
	{
	case KC_A:
		mTranslateVector.x = -mMoveScale;	// Move camera left
		return true;
	case KC_D:
		mTranslateVector.x = mMoveScale;	// Move camera RIGHT
		return true;
	case KC_UP:
	case KC_W:
		mTranslateVector.z = -mMoveScale;	// Move camera forward
		return true;
	case KC_DOWN:
	case KC_S:
		mTranslateVector.z = mMoveScale;	// Move camera backward
		return true;
	case KC_PGUP:
		mTranslateVector.y = mMoveScale;	// Move camera up
		return true;
	case KC_PGDOWN:
		mTranslateVector.y = -mMoveScale;	// Move camera down
		return true;
	case KC_RIGHT:
		mOgreApp->getCamera()->yaw(-mRotScale);
		return true;
	case KC_LEFT:
		mOgreApp->getCamera()->yaw(mRotScale);
		return true;
	case KC_ESCAPE:
	case KC_Q:
		mStop = true;
		return true;
	}

	return false;
}

bool Lab::onMouseMove(int x, int y)
{
	/*if (button pressed)
	{
		mTranslateVector.x += ms.X.rel * 0.13;
		mTranslateVector.y -= ms.Y.rel * 0.13;

		return true;
	}*/
	
	mRotX = Degree(-x * 0.13);
	mRotY = Degree(-y * 0.13);

	return true;
}

void Lab::beforeCycle(float timeSinceLastCycle)
{
	if (timeSinceLastCycle == 0)
	{
		mMoveScale = 1;
		mRotScale = 0.1;
	}
	else
	{
		mMoveScale = mMoveSpeed * timeSinceLastCycle;
		mRotScale = mRotateSpeed * timeSinceLastCycle;
	}

	mRotX = 0;
	mRotY = 0;
	mTranslateVector = Ogre::Vector3::ZERO;
}
#endif

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

