/*
 * LabLOVE
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

#include "Simulation.h"
#include "PopulationDynamics.h"
#include "Symbol.h"
#include "SymbolTable.h"
#include "GraphicalObject.h"
//#include "Gridbrain.h"

#include <math.h>
#include <stdexcept>

Simulation::Simulation(lua_State* luaState)
{
    mSimulationTime = 0;
    mSelectedObject = NULL;
    mPopulationDynamics = NULL;

    mStop = false;

    mLastSimulationTime = 0;
    mLastRealTime = 0.0f;
    mCPS = 0.0f;

    mSimulationTimeText = "";
    mRealTimeText = "";
    mCPSText = "";

    mTimeLimit = 0;

    mDrawGraphics = true;

    mLastDrawTime = 0.0f;
    mTargetFPS = 60;

    mFrameDuration = 1.0f / (double)mTargetFPS;
}

Simulation::~Simulation()
{
}

void Simulation::initGraphics(unsigned int width,
                                unsigned int height,
                                bool fullScreen,
                                bool noGraphics)
{
    art_init(width, height, fullScreen, 0, 0, 1, 0);
    art_setBackgroundColor(0, 50, 0);

    mLogo = art_loadImage("media/lablove_small.png");

    art_setWinTitle("LabLOVE");

    if (strcmp(art_systemName(), "Null") == 0)
    {
        mDrawGraphics = false;
    }
}

void Simulation::run()
{
    mInitialRealTime = art_getTime();

    mPopulationDynamics->init(this);

    while (!mStop)
    {
        cycle();
        if ((mTimeLimit != 0) && (mTimeLimit < mSimulationTime))
        {
            mStop = true;
        }
    }
}

void Simulation::cycle()
{
    //if ((mSimulationTime % 1000) == 0) Gridbrain::debugMutationsCount();

    bool drawThisCycle = false;

    double realTime = art_getTime();
    if (realTime - mLastDrawTime >= mFrameDuration)
    {
        mLastDrawTime = realTime;
        drawThisCycle = true;
    }

    bool draw = mDrawGraphics && drawThisCycle;

    list<SimulationObject*>::iterator iterObj;

    for (iterObj = mObjectsToKill.begin();
        iterObj != mObjectsToKill.end();
        iterObj++)
    {
        mPopulationDynamics->onOrganismDeath(*iterObj);
    }
    mObjectsToKill.clear();

    if (draw)
    {
        drawBeforeObjects();
    }

    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        SimulationObject* obj = *iterObj;

        process(obj); 

        if (obj->mType == SimulationObject::TYPE_AGENT)
        {
            Agent* agent = (Agent*)obj;
            perceive(agent);
            agent->compute();
            act(agent);
        }
    
        if (draw)
        {
            obj->draw();
        }
    }

    if (draw)
    {
        drawAfterObjects();
    }

    mPopulationDynamics->onCycle(mSimulationTime, art_getTime());

    art_setColor(255, 255, 255, 200);
    art_drawLayer(mLogo, 0, 0);
    art_fillRectangle(122, 0, art_getWinWidth() - 122, 50);
    drawTimes();

    if (draw)
    {
        art_update();
    }

    mSimulationTime++;

    // Process events
    while (art_nextEvent())
    {
        switch (art_getEventType())
        {
        case ART_EVENT_KEY_DOWN:
            onKeyDown(art_getKeyCode());
            break;
        case ART_EVENT_KEY_UP:
            onKeyUp(art_getKeyCode());
            break;
        case ART_EVENT_MOUSE_BUTTON_DOWN:
            onMouseButtonDown(art_getMouseButton(),
                                art_getMousePosX(),
                                art_getMousePosY());
            break;
        case ART_EVENT_MOUSE_BUTTON_UP:
            onMouseButtonUp(art_getMouseButton(),
                            art_getMousePosX(),
                            art_getMousePosY());
            break;
        case ART_EVENT_MOUSE_MOTION:
            onMouseMove(art_getMousePosX(),
                        art_getMousePosY());
            break;
        case ART_EVENT_MOUSE_WHEEL_UP:
            onMouseWheel(true);
            break;
        case ART_EVENT_MOUSE_WHEEL_DOWN:
            onMouseWheel(false);
            break;
        default:
            break;
        }
    }
}

void Simulation::addObject(SimulationObject* object, bool init)
{
    object->setCreationTime(mSimulationTime);
    PopulationManager::addObject(object);

    if ((object->mType == SimulationObject::TYPE_GRAPHICAL_OBJECT)
        || (object->mType == SimulationObject::TYPE_AGENT))
    {
        GraphicalObject* gObj = (GraphicalObject*)object;
        for (list<Graphic*>::iterator iterGraph = gObj->mGraphics.begin();
                iterGraph != gObj->mGraphics.end();
                iterGraph++)
        {
            (*iterGraph)->init(gObj);
        }
    }
}

void Simulation::setSeedIndex(int index)
{
    gDistManager.setSeedIndex(index);
}

float Simulation::calcSymbolsBinding(SimulationObject* origObj,
                                    SimulationObject* targetObj,
                                    int origSymTable,
                                    int targetSymTable,
                                    llULINT origSymID,
                                    llULINT targetSymID)
{
    SymbolTable* origTable = origObj->getSymbolTable(origSymTable);
    SymbolTable* targetTable = targetObj->getSymbolTable(targetSymTable);

    if ((origTable == NULL) || (targetTable == NULL))
    {
        return 0.0f;
    }

    Symbol* origSym = origTable->getSymbol(origSymID);
    Symbol* targetSym = targetTable->getSymbol(targetSymID);

    if ((origSym == NULL) || (targetSym == NULL))
    {
        return 0.0f;
    }


    float distance = origSym->getBinding(targetSym);

    return distance;
}

void Simulation::drawTimes()
{
    if (mLastRealTime == 0.0f)
    {
        mLastRealTime = art_getTime();
    }
    else if (mSimulationTime % 100 == 0)
    {
        double realTime = art_getTime();
        double deltaRealTime = realTime - mLastRealTime;
        double deltaSimTime = (double)(mSimulationTime - mLastSimulationTime);
        mLastRealTime = realTime;
        mLastSimulationTime = mSimulationTime;

        mCPS = deltaSimTime / deltaRealTime;

        char text[255];

        sprintf(text, "Sim Time: %d Kcycles", mSimulationTime / 1000);
        mSimulationTimeText = text;

        float totalRealTime = realTime - mInitialRealTime;
        float days = 0;
        float hours = 0;
        float minutes = 0;
        float seconds = 0;

        double remainingTime = totalRealTime;

        if (remainingTime > 60 * 60 * 24)
        {
            days = truncf(remainingTime / (60 * 60 * 24));
            remainingTime -= days * 60 * 60 * 24;
        }
        if (remainingTime > 60 * 60)
        {
            hours = truncf(remainingTime / (60 * 60));
            remainingTime -= hours * 60 * 60;
        }
        if (remainingTime > 60)
        {
            minutes = truncf(remainingTime / 60);
            remainingTime -= minutes * 60;
        }
        seconds = truncf(remainingTime);

        sprintf(text, "Real Time: %.0fd %.0fh %.0fm %.0fs", days, hours, minutes, seconds);
        mRealTimeText = text;

        sprintf(text, "CPS: %.0f", mCPS);
        mCPSText = text;
    }

    art_setFont(mFont);
    art_setColor(120, 0, 0, 255);
    art_drawText(140, 16, (char*)mSimulationTimeText.c_str());
    art_setColor(0, 120, 0, 255);
    art_drawText(140, 31, (char*)mRealTimeText.c_str());
    art_setColor(0, 0, 120, 255);
    art_drawText(140, 46, (char*)mCPSText.c_str());
}

bool Simulation::onKeyDown(Art_KeyCode keycode)
{
    switch (keycode)
    {
        case ART_KEY_ESCAPE:
            mStop = true;
            return true;
        default:
            return false;
    }
}

void Simulation::setTimeLimit(llULINT limit)
{
    mTimeLimit = limit * 1000;
}

int Simulation::addObject(lua_State *luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimulationObject>::pointer(luaState, 1);
    addObject(simObj);
    return 0;
}

int Simulation::initGraphics(lua_State* luaState)
{
    int width = luaL_checkint(luaState, 1);
    int height = luaL_checkint(luaState, 2);
    bool fullScreen = luaL_checkbool(luaState, 3);
    bool noGraphics = false;
    if (lua_gettop(luaState) > 3)
    {        
        noGraphics = luaL_checkbool(luaState, 4);
    }

    initGraphics(width, height, fullScreen, noGraphics);

    mFont = art_loadFont("media/vera/Vera.ttf", 8);

    return 0;
}

int Simulation::run(lua_State* luaState)
{
    run();
    return 0;
}

int Simulation::setPopulationDynamics(lua_State *luaState)
{
    PopulationDynamics* popDyn = (PopulationDynamics*)Orbit<SimulationObject>::pointer(luaState, 1);
    setPopulationDynamics(popDyn);
    return 0;
}

int Simulation::setSeedIndex(lua_State* luaState)
{
    int index = luaL_optint(luaState, 1, -1);
    setSeedIndex(index);
    return 0;
}

int Simulation::setTimeLimit(lua_State* luaState)
{
    int limit = luaL_checkint(luaState, 1);
    setTimeLimit(limit);
    return 0;
}

