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

#if !defined(__INCLUDE_POPULATION_MANAGER_H)
#define __INCLUDE_POPULATION_MANAGER_H

#include "SimObj.h"

#include <list>
#include <string>

using std::list;
using std::string;

class PopulationManager
{
public:
    PopulationManager();
    virtual ~PopulationManager();

    virtual void addObject(SimObj* object, bool init=false);
    virtual void removeObject(SimObj* obj, bool deleteObj=true);
    list<SimObj*>* getObjectList(){return &mObjects;}

    virtual void placeRandom(SimObj* obj){}
    virtual void placeNear(SimObj* obj, SimObj* ref){}

    virtual void killOrganism(SimObj* org);

    void setSelectedObject(SimObj* object);
    SimObj* getSelectedObject(){return mSelectedObject;}

    virtual bool getFieldValue(SimObj* obj, string fieldName, float& value);

    virtual string getInterfaceName(bool input, int type){return "?";}

protected:
    list<SimObj*> mObjects;
    list<SimObj*> mObjectsToKill;

    SimObj* mSelectedObject;
};
#endif

