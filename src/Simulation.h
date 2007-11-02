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
#include "RandDistManager.h"

#include <list>
#include <string>

using std::list;
using std::string;

class Simulation : public PopulationManager
{
public:
    Simulation(lua_State* luaState=NULL);
    virtual ~Simulation();

    virtual void initGraphics(unsigned int width,
                                unsigned int height,
                                bool fullScreen,
                                bool noGraphics=false);
    
    virtual void drawBeforeObjects(){}
    virtual void drawAfterObjects(){}

    void run();
    void cycle();

    virtual void initializeData(SimulationObject* obj){}
    virtual void addObject(SimulationObject* object);

    virtual SimulationObject* getObjectByScreenPos(int x, int y){return NULL;}

    void setPopulationDynamics(PopulationDynamics* popDyn){mPopulationDynamics = popDyn;}
    PopulationDynamics* getPopulationDynamics(){return mPopulationDynamics;}

    unsigned long getTime(){return mSimulationTime;}

    void setSeedIndex(int index=-1);
    static mt_distribution* getNewDistribution();

    pyc::Pycasso& getPycasso(){return mPycasso;}

    virtual bool onKeyDown(pyc::KeyCode keycode);
    virtual bool onKeyUp(pyc::KeyCode keycode){return false;}
    virtual bool onMouseButtonDown(pyc::MouseButton button, int x, int y){return false;}
    virtual bool onMouseButtonUp(pyc::MouseButton button, int x, int y){return false;}
    virtual bool onMouseMove(int x, int y){return false;}
    virtual bool onMouseWheel(bool up){return false;}

    void setTimeLimit(unsigned long limit);

    int addObject(lua_State* luaState);
    int run(lua_State* luaState);
    int initGraphics(lua_State* luaState);
    int setPopulationDynamics(lua_State* luaState);
    int setSeedIndex(lua_State* luaState);
    int setTimeLimit(lua_State* luaState);

protected:
    virtual void process(SimulationObject* obj){}
    virtual void perceive(Agent* agent){}
    virtual void act(Agent* agent){}
    float computeBinding(SimulationObject* sourceObj,
                            SimulationObject* targetObj,
                            int symTable,
                            int origSymIndex,
                            int targetSymIndex);
    void drawTimes();

    static RandDistManager mDistManager;

    unsigned long mSimulationTime;
    PopulationDynamics* mPopulationDynamics;

    bool mStop;
    
    pyc::Pycasso mPycasso;
    pyc::EventQ* mEventQ;
    pyc::Window* mWindow;
    pyc::Layer2D* mRootLayer2D;
    pyc::Layer* mLogo;

    double mInitialRealTime;
    unsigned long mLastSimulationTime;
    double mLastRealTime;
    double mFPS;
    string mSimulationTimeText;
    string mRealTimeText;
    string mFPSText;

    pyc::Font* mFont;

    unsigned long mTimeLimit;

    bool mDrawGraphics;

    double mLastDrawTime;
    unsigned int mTargetFPS;
    double mFrameDuration;
};
#endif

