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

#ifndef __INCLUDE_GRIDBRAIN_COMPONENT_SET_H
#define __INCLUDE_GRIDBRAIN_COMPONENT_SET_H

#include "GridbrainComponent.h"
#include "Orbit.h"
#include "RandDistManager.h"
#include "types.h"

#include <vector>

using std::vector;

class GridbrainComponentSet
{
public:
    GridbrainComponentSet(lua_State* luaState=NULL);
    virtual ~GridbrainComponentSet();

    void addComponent(GridbrainComponent* component);
    void addComponent(GridbrainComponent::Type type,
                int subType=-1,
                InterfaceItem::TableLinkType linkType=InterfaceItem::NO_LINK,
                int origSymTable=-1,
                llULINT origSymID=0,
                int targetSymTable=-1,
                llULINT targetSymID=0);
    GridbrainComponent* getRandom(SimulationObject* obj=NULL);

    static const char mClassName[];
    static Orbit<GridbrainComponentSet>::MethodType mMethods[];
    static Orbit<GridbrainComponentSet>::NumberGlobalType mNumberGlobals[];

    int addComponent(lua_State* luaState);

    vector<GridbrainComponent*> mComponentVec;
    unsigned int mSize;

protected:
    static mt_distribution* mDistComponentSet;
};
#endif

