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

#ifdef __LOVE_GRAPHICS
#include "GL/gl.h"
#endif

LoveLab* LoveLab::mLove = NULL;

LoveLab::LoveLab()
{
	mStop = false;
	mScreenWidth = 800;
	mScreenHeight = 600;
	mColorDepth = 32;
	mCycleStartTime = 0;
	mLastCycleStartTime = 0;
	mFPS = 0;
	mFPSSum = 0.0f;

        sprintf(mFPSStringBuffer, "");
}

LoveLab::LoveLab(lua_State* luaState)
{
	mStop = false;
	mScreenWidth = 800;
	mScreenHeight = 600;
	mColorDepth = 32;
	mCycleStartTime = 0;
	mLastCycleStartTime = 0;
	mFPS = 0;
	mFPSSum = 0.0f;

        sprintf(mFPSStringBuffer, "");
}

LoveLab::~LoveLab()
{
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

void LoveLab::setScreenDimensions(unsigned int screenWidth,
					unsigned int screenHeight,
					unsigned int colorDepth)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mColorDepth = colorDepth;
}

void LoveLab::setSeedIndex(unsigned int index)
{
	randomSeedIndex(index);
}

void LoveLab::run()
{
#ifdef __LOVE_GRAPHICS
	const SDL_VideoInfo *info;
	Uint8  videoBpp;
	Uint32 videoFlags;
        
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	videoFlags = SDL_OPENGL;

	if ((SDL_SetVideoMode(mScreenWidth, mScreenHeight, mColorDepth, videoFlags)) == NULL )
	{
		fprintf(stderr, "Couldn't set %ix%i video mode: %s\n", mScreenWidth, mScreenHeight, SDL_GetError());
		exit(2);
	}

 	SDL_WM_SetCaption("LOVE Lab", "LOVE Lab");

	glViewport(0, 0, (GLint) mScreenWidth, (GLint) mScreenHeight);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glOrtho(0, mScreenWidth, 0, mScreenHeight, -1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mFont.init();
#endif

	mSimulation->init();

	while (running())
	{
		cycle();
	}
}

void LoveLab::cycle()
{
#ifdef __LOVE_GRAPHICS
	mLastCycleStartTime = mCycleStartTime;
	mCycleStartTime = SDL_GetTicks();

	glClear(GL_COLOR_BUFFER_BIT);
#endif

	mSimulation->cycle();

#ifdef __LOVE_GRAPHICS
	long cycleTicks = mCycleStartTime - mLastCycleStartTime;
	double cycleTime = ((double)cycleTicks) / 1000.0f;
	mFPSSum += 1.0f / cycleTime;

	if ((mSimulation->time() % 100) == 0)
	{
		mFPS = mFPSSum / 100.0f;
		mFPSSum = 0.0f;
		sprintf(mFPSStringBuffer, "FPS: %f", mFPS);
	}

	glColor3f(0.15, 0.15, 0.15);
	mFont.print(10, mScreenHeight - 10, "%s", mFPSStringBuffer);

	glFlush();
	SDL_GL_SwapBuffers();

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

#ifdef __LOVE_GRAPHICS
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

