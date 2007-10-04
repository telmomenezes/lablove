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
#include <math.h>

#if defined(__UNIX)
#include <sys/time.h>
#endif

Simulation::Simulation(lua_State* luaState)
{
    mSimulationTime = 0;
    mSelectedObject = NULL;
    mPopulationDynamics = NULL;

    mStop = false;

    mWindow = NULL;
    mRootLayer2D = NULL;
    mEventQ = NULL;
}

Simulation::~Simulation()
{
}

void Simulation::initGraphics(unsigned int width, unsigned int height, bool fullScreen)
{
    mWindow = mPycasso.createWindow(width, height, fullScreen);
    mEventQ = mPycasso.createEventQ();
    mRootLayer2D = mWindow->getRootLayer2D();

    mLogo = mWindow->createPNGLayer("media/lablove.png");

    mWindow->setTitle("LabLOVE");

}

void Simulation::run()
{
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

        obj->mEnergy -= obj->mMetabolism;

        if (obj->mEnergy < 0)
        {
            killOrganism(obj);
        }

        if (obj->mMaxAge > 0)
        {
            if (mSimulationTime - obj->mCreationTime >= obj->mMaxAge)
            {
                killOrganism(obj);
            }
        }

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

    mPopulationDynamics->onCycle(mSimulationTime, getRealTime());

    mRootLayer2D->setColor(255, 255, 255, 200);
    mRootLayer2D->drawLayer(mLogo, 10, 10);

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
        default:
            break;
        }
    }
}

void Simulation::addObject(SimulationObject* object)
{
    object->setCreationTime(mSimulationTime);
    PopulationManager::addObject(object);
}

void Simulation::setSeedIndex(unsigned int index)
{
    Random::setSeedIndex(index);
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

#if defined(__UNIX)
double Simulation::getRealTime()
{
    timeval time;
    gettimeofday(&time, NULL);

    double seconds = (double)time.tv_sec;
    seconds += ((double)time.tv_usec) / 1000000.0f;
    return seconds;
}
#endif

#if defined(__WIN32)
double Simulation::getRealTime()
{
    FILETIME ft;
    LARGE_INTEGER li;
    __int64 t;
    static int tzflag;

    GetSystemTimeAsFileTime(&ft);
        li.LowPart  = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        t  = li.QuadPart;
        t -= EPOCHFILETIME;
        t /= 10;
        return ((double)t) / 1000000.0f;
}
#endif

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

