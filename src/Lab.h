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
#include "Orbit.h"
#include "InputHandler.h"

#include "pyc.h"

#include <list>
using std::list;

class Lab : public InputHandler
{
public:
    virtual ~Lab();

    static Lab& getSingleton();

    void setSimulation(Simulation* simulation){mSimulation = simulation;}
    Simulation* getSimulation(){return mSimulation;}
    void setSeedIndex(unsigned int index);
    bool running(){return !mStop;}
    void run();
    void cycle();
    void addInputHandler(InputHandler* handler);
    void removeInputHandler();
    void processEvents();

    virtual bool onKeyDown(pyc::KeyCode keycode);
    virtual bool onMouseButtonDown(pyc::MouseButton button, int x, int y);
    virtual bool onMouseButtonUp(pyc::MouseButton button, int x, int y);
    virtual bool onMouseMove(int x, int y);

    double realTime();

    pyc::Pycasso* getPycass(){return &mPycasso;}
    pyc::Window* getWindow(){return mWindow;}
    pyc::Layer* getRootLayer(){return mRootLayer;}

    static const char mClassName[];
    static Orbit<Lab>::MethodType mMethods[];
    static Orbit<Lab>::NumberGlobalType mNumberGlobals[];

    Lab(lua_State* luaState);
    int setSimulation(lua_State* luaState);
    int setSeedIndex(lua_State* luaState);

private:
    Lab();

    static Lab mLab;
    Simulation* mSimulation;
    bool mStop;
    
    pyc::Pycasso mPycasso;
    pyc::EventQ* mEventQ;
    pyc::Window* mWindow;
    pyc::Layer* mRootLayer;

    list<InputHandler*> mHandlers;

    bool mDragging;
    int mLastMouseX;
    int mLastMouseY;
};
#endif

