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

#include "ComponentSet.h"
#include "Gridbrain.h"

int ComponentSet::CURRENT_COMP_TYPE = 0;
mt_distribution* ComponentSet::mDistComponentSet = gDistManager.getNewDistribution();

ComponentSet::ComponentSet()
{
}

ComponentSet::ComponentSet(lua_State* luaState)
{
}

ComponentSet::ComponentSet(ComponentSet* comp)
{
    for (unsigned int i = 0; i < comp->mComponentVec.size(); i++)
    {
        mComponentVec.push_back(comp->mComponentVec[i]);
    }
    for (unsigned int i = 0; i < comp->mComponentSet.size(); i++)
    {
        mComponentSet.push_back(comp->mComponentSet[i]->clone());
    }
}

ComponentSet::~ComponentSet()
{
    for (unsigned int i = 0; i < mComponentSet.size(); i++)
    {
        delete mComponentSet[i];
    }
    mComponentSet.clear();
}

void ComponentSet::addComponent(Component* component)
{
    component->mType = CURRENT_COMP_TYPE++;
    mComponentVec.push_back(component->clone());
}

Component* ComponentSet::getRandom()
{
    Component* comp;

    if (mComponentSet.size() <= 0)
    {
        comp = &mNullComponent;
        return comp;
    }
    else
    {
        unsigned int pos = mDistComponentSet->iuniform(0, mComponentSet.size());
        comp = mComponentSet[pos];
        comp->mParam = mDistComponentSet->uniform(0.0f, 1.0f);

        return comp;
    }
}

void ComponentSet::update(Gridbrain* gb, unsigned int start, unsigned int end)
{
    for (unsigned int pos = start;
            pos < end;
            pos++)
    {
        Component* comp = gb->getComponent(pos);

        if (comp->isUnique())
        {
            disable(comp);
        }
    }
}

void ComponentSet::disable(Component* comp)
{
    if (!comp->isUnique())
    {
        return;
    }

    for (vector<Component*>::iterator iterComp = mComponentSet.begin();
            iterComp != mComponentSet.end();
            iterComp++)
    {
        if ((*iterComp)->isEqual(comp, false))
        {
            delete (*iterComp);
            mComponentSet.erase(iterComp);
            return;
        }
    }
}

void ComponentSet::enable(Component* comp)
{
    if (!comp->isUnique())
    {
        return;
    }

    Component* newComp = comp->clone();
    mComponentSet.push_back(newComp);
}

void ComponentSet::print()
{
    for (unsigned int i = 0; i < mComponentSet.size(); i++)
    {
        Component* comp = mComponentSet[i];
        comp->print();
    }
    printf("\n");
}

const char ComponentSet::mClassName[] = "ComponentSet";

Orbit<ComponentSet>::MethodType ComponentSet::mMethods[] = {
    {"addComponent", &ComponentSet::addComponent},
    {0,0}
};

Orbit<ComponentSet>::NumberGlobalType ComponentSet::mNumberGlobals[] = {{0,0}};

int ComponentSet::addComponent(lua_State* luaState)
{
    Component* comp = (Component*)(Orbit<Component>::pointer(luaState, 1));
    addComponent(comp);
    return 0;
}

