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

#include "GridbrainComponentSet.h"
#include "Gridbrain.h"

mt_distribution* GridbrainComponentSet::mDistComponentSet = gDistManager.getNewDistribution();

GridbrainComponentSet::GridbrainComponentSet()
{
}

GridbrainComponentSet::GridbrainComponentSet(lua_State* luaState)
{
}

GridbrainComponentSet::GridbrainComponentSet(GridbrainComponentSet* comp)
{
    for (unsigned int i = 0; i < comp->mComponentVec.size(); i++)
    {
        mComponentVec.push_back(comp->mComponentVec[i]);
    }
}

GridbrainComponentSet::~GridbrainComponentSet()
{
    for (unsigned int i = 0; i < mComponentSet.size(); i++)
    {
        delete mComponentSet[i];
    }
    mComponentSet.clear();
}

void GridbrainComponentSet::addComponent(GridbrainComponent* component)
{
    mComponentVec.push_back(component->clone());
}

GridbrainComponent* GridbrainComponentSet::getRandom()
{
    GridbrainComponent* comp;

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

void GridbrainComponentSet::update(SimObj* obj,
                                    vector<GridbrainComponent*>* components,
                                    unsigned int start,
                                    unsigned int end)
{
    for (unsigned int i = 0; i < mComponentSet.size(); i++)
    {
        delete mComponentSet[i];
    }
    mComponentSet.clear();

    for (unsigned int i = 0; i < mComponentVec.size(); i++)
    {
        GridbrainComponent* comp = mComponentVec[i];

        if (comp->mTableLinkType == InterfaceItem::TAB_TO_SYM)
        {
            SymbolTable* table = obj->getSymbolTable(comp->mOrigSymTable);
            comp->mOrigSymID = table->getRandomSymbolId();

            map<llULINT, Symbol*>* symbols = table->getSymbolMap();

            for (map<llULINT, Symbol*>::iterator iterSym = symbols->begin();
                    iterSym != symbols->end();
                    iterSym++)
            {
                llULINT symID = (*iterSym).first;
                GridbrainComponent* newComp = comp->clone();
                newComp->mOrigSymID = symID;
                mComponentSet.push_back(newComp);
            }
        }
        else
        {
            GridbrainComponent* newComp = comp->clone();
            mComponentSet.push_back(newComp);
        }
    }

    for (unsigned int pos = start;
            pos < end;
            pos++)
    {
        GridbrainComponent* comp = (*components)[pos];

        if (comp->isUnique())
        {
            disable(comp);
        }
    }
}

void GridbrainComponentSet::disable(GridbrainComponent* comp)
{
    if (!comp->isUnique())
    {
        return;
    }

    for (vector<GridbrainComponent*>::iterator iterComp = mComponentSet.begin();
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

void GridbrainComponentSet::enable(GridbrainComponent* comp)
{
    if (!comp->isUnique())
    {
        return;
    }

    GridbrainComponent* newComp = comp->clone();
    mComponentSet.push_back(newComp);
}

void GridbrainComponentSet::addComponent(GridbrainComponent::Type type,
                        int subType,
                        InterfaceItem::TableLinkType linkType,
                        int origSymTable,
                        llULINT origSymID,
                        int targetSymTable)
{
    GridbrainComponent* comp = GridbrainComponent::createByType(type);
    comp->mSubType = subType;
    comp->mOrigSymTable = origSymTable;
    comp->mTargetSymTable = targetSymTable;
    comp->mOrigSymID = origSymID;
    comp->mTableLinkType = linkType;
    addComponent(comp);
}

void GridbrainComponentSet::print()
{
    for (unsigned int i = 0; i < mComponentSet.size(); i++)
    {
        GridbrainComponent* comp = mComponentSet[i];
        printf("%s", comp->getName().c_str());
        printf("(%d)[%d] ", comp->mSubType, comp->mOrigSymID);
    }
    printf("\n");
}

const char GridbrainComponentSet::mClassName[] = "GridbrainComponentSet";

Orbit<GridbrainComponentSet>::MethodType GridbrainComponentSet::mMethods[] = {
    {"addComponent", &GridbrainComponentSet::addComponent},
    {0,0}
};

Orbit<GridbrainComponentSet>::NumberGlobalType GridbrainComponentSet::mNumberGlobals[] = {{0,0}};

int GridbrainComponentSet::addComponent(lua_State* luaState)
{
    int type = luaL_checkint(luaState, 1);
    int subType = luaL_optint(luaState, 2, -1);
    InterfaceItem::TableLinkType linkType = (InterfaceItem::TableLinkType)(luaL_optint(luaState, 3, InterfaceItem::NO_LINK));
    int origSymTable = luaL_optint(luaState, 4, -1);
    int origSymIndex = luaL_optint(luaState, 5, -1);
    int targetSymTable = luaL_optint(luaState, 6, -1);

    addComponent((GridbrainComponent::Type)type,
                    subType,
                    linkType,
                    origSymTable,
                    origSymIndex,
                    targetSymTable);

    return 0;
}

