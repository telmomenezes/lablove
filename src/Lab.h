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

#if !defined(__INCLUDE_LAB_H)
#define __INCLUDE_LAB_H

#include "Simulation.h"
#include "InputHandler.h"

#include <list>
using namespace std;

#include "Orbit.h"

#ifdef __LOVELAB_WITH_GRAPHICS
#include "OgreApplication.h"
#endif

class Lab : public InputHandler
{
public:
	Lab();
	virtual ~Lab();

	static Lab& getInstance();

	void create();
	void setSimulation(Simulation* simulation){mSimulation = simulation;}
	Simulation* getSimulation(){return mSimulation;}
	void setSeedIndex(unsigned int index);
	bool running(){return !mStop;}
	void run();
	void cycle();
	void addInputHandler(InputHandler* handler);
	void removeInputHandler();

#ifdef __LOVELAB_WITH_GRAPHICS
	virtual bool onKeyDown(int key);
	virtual bool onMouseMove(int x, int y);
	OgreApplication* getOgreApplication(){return mOgreApp;}
	void beforeCycle(float timeSinceLastCycle);
#endif

	static const char mClassName[];
        static Orbit<Lab>::MethodType mMethods[];
	static Orbit<Lab>::NumberGlobalType mNumberGlobals[];

        Lab(lua_State* luaState);
        int create(lua_State* luaState);
	int setSimulation(lua_State* luaState);
	int setSeedIndex(lua_State* luaState);

private:
	static Lab* mLove;
	Simulation* mSimulation;
	bool mStop;
	
	list<InputHandler*> mHandlersList;

#ifdef __LOVELAB_WITH_GRAPHICS
	OgreApplication* mOgreApp;

	Vector3 mTranslateVector;
	unsigned int mNumScreenShots;
	float mMoveScale;
	Degree mRotScale;
	Radian mRotX;
	Radian mRotY;
	Real mMoveSpeed;
	Degree mRotateSpeed;
#endif
};
#endif

