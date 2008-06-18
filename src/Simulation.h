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

#if !defined(__INCLUDE_SIMULATION_H)
#define __INCLUDE_SIMULATION_H

#include "SimObj.h"
#include "PopulationDynamics.h"
#include "Orbit.h"
#include "RandDistManager.h"
#include "types.h"

#include <list>
#include <string>

using std::list;
using std::string;

class Simulation
{
public:
    enum BindingType {BINDING_EQUALS, BINDING_PROXIMITY};

    Simulation(lua_State* luaState=NULL);
    virtual ~Simulation();

    virtual void drawBeforeObjects(){}
    virtual void drawAfterObjects(){}

    virtual void run();
    virtual void cycle();

    virtual void addObject(SimObj* obj, bool init=false);
    virtual void removeObject(SimObj* obj, bool deleteObj=true);
    list<SimObj*>* getObjectList(){return &mObjects;}
    SimObj* getObjectByID(llULINT id);

    virtual SimObj* getObjectByScreenPos(int x, int y){return NULL;}

    void setPopulationDynamics(PopulationDynamics* popDyn){mPopulationDynamics = popDyn;}
    PopulationDynamics* getPopulationDynamics(){return mPopulationDynamics;}

    virtual void killOrganism(SimObj* org);

    virtual void placeRandom(SimObj* obj){}
    virtual void placeNear(SimObj* obj, SimObj* ref){}
    virtual void replace(SimObj* obj, SimObj* ref){}

    void setSelectedObject(SimObj* object);
    SimObj* getSelectedObject(){return mSelectedObject;}

    llULINT getTime(){return mSimulationTime;}

    void setSeedIndex(int index=-1);

    virtual bool getFieldValue(SimObj* obj, string fieldName, float& value);

    virtual string getInterfaceName(bool input, int type){return "";}

    float calcSymbolsBinding(SimObj* origObj,
                            SimObj* targetObj,
                            int origSymTable,
                            int targetSymTable,
                            llULINT origSymID,
                            BindingType type);
    float calcSymbolsBinding(SimObj* obj,
                                    int symTable,
                                    llULINT symID,
                                    Symbol* symbol,
                                    BindingType type);

    void setTimeLimit(llULINT limit);

    int addObject(lua_State* luaState);
    int run(lua_State* luaState);
    int setPopulationDynamics(lua_State* luaState);
    int setSeedIndex(lua_State* luaState);
    int setTimeLimit(lua_State* luaState);

protected:
    virtual void onCycle(){}
    
    static RandDistManager mDistManager;

    llULINT mSimulationTime;
    PopulationDynamics* mPopulationDynamics;

    bool mStop;

    list<SimObj*> mObjects;
    list<SimObj*> mObjectsToKill;

    SimObj* mSelectedObject;
    
    llULINT mTimeLimit;

    bool mDrawGraphics;
    bool mDrawThisCycle;
};
#endif

