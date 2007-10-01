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

#if !defined(__INCLUDE_SIMULATION_H)
#define __INCLUDE_SIMULATION_H

#include "PopulationManager.h"
#include "SimulationObject.h"
#include "Agent.h"
#include "PopulationDynamics.h"

#include "pyc.h"

#include "Orbit.h"

#include <list>

using std::list;

class Simulation : public PopulationManager
{
public:
    Simulation(lua_State* luaState=NULL);
    virtual ~Simulation();

    virtual void init();
    
    virtual void drawBeforeObjects(){}

    void run();
    void cycle();

    virtual void processObjects(){};

    virtual void addObject(SimulationObject* object);

    virtual SimulationObject* getObjectByScreenPos(int x, int y){return NULL;}

    void setPopulationDynamics(PopulationDynamics* popDyn){mPopulationDynamics = popDyn;}
    PopulationDynamics* getPopulationDynamics(){return mPopulationDynamics;}

    unsigned long getTime(){return mSimulationTime;}
    double getRealTime();

    void setSeedIndex(unsigned int index);

    virtual bool onKeyDown(pyc::KeyCode keycode);
    virtual bool onKeyUp(pyc::KeyCode keycode){return false;}
    virtual bool onMouseButtonDown(pyc::MouseButton button, int x, int y){return false;}
    virtual bool onMouseButtonUp(pyc::MouseButton button, int x, int y){return false;}
    virtual bool onMouseMove(int x, int y){return false;}

    int setPopulationDynamics(lua_State* luaState);
    int setSeedIndex(lua_State* luaState);
    int run(lua_State* luaState);

protected:
    virtual void perceive(Agent* agent){}
    virtual void act(Agent* agent){}
    unsigned long mSimulationTime;
    PopulationDynamics* mPopulationDynamics;

    bool mStop;
    
    pyc::Pycasso mPycasso;
    pyc::EventQ* mEventQ;
    pyc::Window* mWindow;
    pyc::Layer2D* mRootLayer2D;
    pyc::Layer* mLogo;
};
#endif

