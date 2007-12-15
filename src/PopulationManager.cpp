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

#include "PopulationManager.h"

PopulationManager::PopulationManager()
{
}

PopulationManager::~PopulationManager()
{
    list<SimulationObject*>::iterator iterObj;
    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        delete *iterObj;
    }
    mObjects.clear();
}

void PopulationManager::addObject(SimulationObject* object, bool init)
{
    mObjects.push_back(object);
    object->onAdd();
}

void PopulationManager::removeObject(SimulationObject* obj)
{
    if (mSelectedObject == obj)
    {
        mSelectedObject = NULL;
    }

    bool stop = false;
    list<SimulationObject*>::iterator iterObj;
    for (iterObj = mObjects.begin(); (iterObj != mObjects.end()) && !stop; ++iterObj)
    {
        if((*iterObj) == obj)
        {
            mObjects.erase(iterObj);
            delete obj;
            return;
        }
    }
}

void PopulationManager::killOrganism(SimulationObject* org)
{
    if (org->mDeleted)
    {
        return;
    }

    org->mDeleted = true;

    mObjectsToKill.push_back(org);
}

void PopulationManager::setSelectedObject(SimulationObject* object)
{
    mSelectedObject = object;
}

float PopulationManager::getFieldValue(SimulationObject* obj, string fieldName)
{
    return obj->getFieldValue(fieldName);
}

