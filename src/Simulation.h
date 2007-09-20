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

#include "SimulationObject.h"
#include "PopulationDynamics.h"
#include "InputHandler.h"

#include "pyc.h"

#include "Orbit.h"

#include <list>

using std::list;

class Simulation : public InputHandler
{
public:
    Simulation();
    virtual ~Simulation();

    virtual void init();
    
    void setWorldDimensions(float worldWidth,
                            float worldLength,
                            float cellSide);

    float getWorldWidth(){return mWorldWidth;}
    float getWorldLength(){return mWorldLength;}
    float getCellSide(){return mCellSide;}
    unsigned int getWorldCellWidth(){return mWorldCellWidth;}
    unsigned int getWorldCellLength(){return mWorldCellLength;}
    SimulationObject** getCellGrid(){return mCellGrid;}

    virtual void drawBeforeObjects(){}

    virtual void addObject(SimulationObject* object);
    virtual void removeObject(SimulationObject* object);

    void cycle();
    virtual void processObjects()=0;

    virtual SimulationObject* getObjectByScreenPos(int x, int y);
    void setSelectedObject(SimulationObject* object);
    SimulationObject* getSelectedObject(){return mSelectedObject;}
    void moveView(float x, float y);
    float getViewX(){return mViewX;}
    float getViewY(){return mViewY;}

    void setPopulationDynamics(PopulationDynamics* popDyn){mPopulationDynamics = popDyn;}
    PopulationDynamics* getPopulationDynamics(){return mPopulationDynamics;}

    virtual void killOrganism(SimulationObject* org);
    unsigned long time(){return mSimulationTime;}

    int setPopulationDynamics(lua_State* luaState);
    int setWorldDimensions(lua_State* luaState);

protected:
    float mWorldWidth;
    float mWorldLength;

    float mCellSide;
    unsigned int mWorldCellWidth;
    unsigned int mWorldCellLength;

    SimulationObject** mCellGrid;

    list<SimulationObject*> mObjects;
    list<SimulationObject*> mObjectsToKill;

    unsigned long mSimulationTime;
    SimulationObject* mSelectedObject;
    PopulationDynamics* mPopulationDynamics;

    float mViewX;
    float mViewY;

    pyc::Layer* mLogo;
};
#endif

