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

#include "Simulation.h"
#include "PopulationDynamics.h"
#include "Random.h"
#include "Symbol.h"
#include "SymbolTable.h"
#include "GraphicalObject.h"
#include <math.h>

Simulation::Simulation(lua_State* luaState)
{
    mSimulationTime = 0;
    mSelectedObject = NULL;
    mPopulationDynamics = NULL;

    mStop = false;

    mWindow = NULL;
    mRootLayer2D = NULL;
    mEventQ = NULL;

    mLastSimulationTime = 0;
    mLastRealTime = 0.0f;
    mFPS = 0.0f;

    mSimulationTimeText = "";
    mRealTimeText = "";
    mFPSText = "";
}

Simulation::~Simulation()
{
}

void Simulation::initGraphics(unsigned int width, unsigned int height, bool fullScreen)
{
    mWindow = mPycasso.createWindow(width, height, fullScreen);
    mEventQ = mPycasso.createEventQ();
    mRootLayer2D = mWindow->getRootLayer2D();

    mLogo = mWindow->loadImage("media/lablove_small.png");

    mWindow->setTitle("LabLOVE");

}

void Simulation::run()
{
    mInitialRealTime = mPycasso.getTime();

    mPopulationDynamics->init(this);

    while (!mStop)
    {
        cycle();
    }
}

void Simulation::cycle()
{
    list<SimulationObject*>::iterator iterObj;

    for (iterObj = mObjectsToKill.begin();
        iterObj != mObjectsToKill.end();
        iterObj++)
    {
        mPopulationDynamics->onOrganismDeath(*iterObj);
    }
    mObjectsToKill.clear();

    drawBeforeObjects();

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

        obj->draw(mRootLayer2D);
    }

    drawAfterObjects();

    mPopulationDynamics->onCycle(mSimulationTime, mPycasso.getTime());

    mRootLayer2D->setColor(255, 255, 255, 200);
    mRootLayer2D->drawLayer(mLogo, 0, 0);
    mRootLayer2D->fillRectangle(122, 0, mWindow->getWidth() - 122, 50);
    drawTimes();

    mWindow->update();

    mSimulationTime++;

    // Process events
    while (mEventQ->next())
    {
        switch (mEventQ->getType())
        {
        case pyc::EVENT_KEY_DOWN:
            onKeyDown(mEventQ->getKeyCode());
            break;
        case pyc::EVENT_KEY_UP:
            onKeyUp(mEventQ->getKeyCode());
            break;
        case pyc::EVENT_MOUSE_BUTTON_DOWN:
            onMouseButtonDown(mEventQ->getMouseButton(),
                                mEventQ->getMousePosX(),
                                mEventQ->getMousePosY());
            break;
        case pyc::EVENT_MOUSE_BUTTON_UP:
            onMouseButtonUp(mEventQ->getMouseButton(),
                            mEventQ->getMousePosX(),
                            mEventQ->getMousePosY());
            break;
        case pyc::EVENT_MOUSE_MOTION:
            onMouseMove(mEventQ->getMousePosX(),
                        mEventQ->getMousePosY());
            break;
        case pyc::EVENT_MOUSE_WHEEL_UP:
            onMouseWheel(true);
            break;
        case pyc::EVENT_MOUSE_WHEEL_DOWN:
            onMouseWheel(false);
            break;
        default:
            break;
        }
    }
}

void Simulation::addObject(SimulationObject* object)
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

void Simulation::setSeedIndex(unsigned int index)
{
    Random::setSeedIndex(index);
}

float Simulation::computeBinding(SimulationObject* sourceObj,
                                    SimulationObject* targetObj,
                                    int symTable,
                                    int origSymIndex,
                                    int targetSymIndex)
{
    SymbolTable* sourceTable = sourceObj->getSymbolTable(symTable);
    SymbolTable* targetTable = targetObj->getSymbolTable(symTable);

    if (!targetTable)
    {
        return 0.0f;
    }

    Symbol* sourceSym = sourceTable->getSymbol(origSymIndex);
    Symbol* targetSym = targetTable->getSymbol(targetSymIndex);

    if (!targetSym)
    {
        return 0.0f;
    }

    return (sourceSym->bind(targetSym));
}

void Simulation::drawTimes()
{
    if (mLastRealTime == 0.0f)
    {
        mLastRealTime = mPycasso.getTime();
    }
    else if (mSimulationTime % 100 == 0)
    {
        double realTime = mPycasso.getTime();
        double deltaRealTime = realTime - mLastRealTime;
        double deltaSimTime = (double)(mSimulationTime - mLastSimulationTime);
        mLastRealTime = realTime;
        mLastSimulationTime = mSimulationTime;

        mFPS = deltaSimTime / deltaRealTime;

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

        sprintf(text, "FPS: %.0f", mFPS);
        mFPSText = text;
    }

    mRootLayer2D->setFont(mFont);
    mRootLayer2D->setColor(120, 0, 0, 255);
    mRootLayer2D->drawText(140, 16, mSimulationTimeText);
    mRootLayer2D->setColor(0, 120, 0, 255);
    mRootLayer2D->drawText(140, 31, mRealTimeText);
    mRootLayer2D->setColor(0, 0, 120, 255);
    mRootLayer2D->drawText(140, 46, mFPSText);
}

bool Simulation::onKeyDown(pyc::KeyCode keycode)
{
    switch (keycode)
    {
        case pyc::KEY_ESCAPE:
            mStop = true;
            return true;
        default:
            return false;
    }
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

    initGraphics(width, height, fullScreen);

    mFont = mWindow->loadFont("media/vera/Vera.ttf", 8);

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
    int index = luaL_checkint(luaState, 1);
    setSeedIndex(index);
    return 0;
}

