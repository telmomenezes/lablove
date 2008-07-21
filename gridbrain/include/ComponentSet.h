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

#include "Component.h"
#include "CompNUL.h"
#include "Orbit.h"
#include "RandDistManager.h"

#include <vector>

using std::vector;

class Gridbrain;

class ComponentSet
{
public:
    ComponentSet();
    ComponentSet(lua_State* luaState);
    ComponentSet(ComponentSet* comp);
    virtual ~ComponentSet();

    void addComponent(Component* component);
    Component* getRandom();

    void update(Gridbrain* gb, unsigned int start, unsigned int end);

    void disable(Component* comp);
    void enable(Component* comp);

    void print();

    static const char mClassName[];
    static Orbit<ComponentSet>::MethodType mMethods[];
    static Orbit<ComponentSet>::NumberGlobalType mNumberGlobals[];

    int addComponent(lua_State* luaState);

    vector<Component*> mComponentVec;
    vector<Component*> mComponentSet;

protected:
    static mt_distribution* mDistComponentSet;

    CompNUL mNullComponent;
};
#endif

