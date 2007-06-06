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

#if !defined(__INCLUDE_LOVELAB_H)
#define __INCLUDE_LOVELAB_H

#include "Simulation.h"
#include "KeyboardMouseHandler.h"

#include <list>
using namespace std;

#include "Orbit.h"

#ifdef __LOVELAB_WITH_GRAPHICS
#include "OgreApplication.h"
#endif

class LoveLab : public KeyboardMouseHandler
{
public:
	LoveLab();
	virtual ~LoveLab();

	static LoveLab& getInstance();

	void create();
	void setSimulation(Simulation* simulation){mSimulation = simulation;}
	Simulation* getSimulation(){return mSimulation;}
	void setSeedIndex(unsigned int index);
	bool running(){return !mStop;}
	void run();
	void cycle();
	void addKeyboardMouseHandler(KeyboardMouseHandler* handler);
	void removeKeyboardMouseHandler();

#ifdef __LOVELAB_WITH_GRAPHICS
	//virtual bool onKeyUp(int keycode);
#endif

	static const char mClassName[];
        static Orbit<LoveLab>::MethodType mMethods[];
	static Orbit<LoveLab>::NumberGlobalType mNumberGlobals[];

        LoveLab(lua_State* luaState);
        int create(lua_State* luaState);
	int setSimulation(lua_State* luaState);
	int setSeedIndex(lua_State* luaState);

private:
	static LoveLab* mLove;
	Simulation* mSimulation;
	bool mStop;
	
	list<KeyboardMouseHandler*> mHandlersList;

#ifdef __LOVELAB_WITH_GRAPHICS
	OgreApplication* mOgreApp;
#endif
};
#endif

