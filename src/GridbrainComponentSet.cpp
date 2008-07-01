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

GridbrainComponentSet::GridbrainComponentSet(lua_State* luaState)
{
    mSize = 0;
}

GridbrainComponentSet::~GridbrainComponentSet()
{
    for (unsigned int i = 0; i < mSize; i++)
    {
        delete mComponentVec[i];
    }
    mComponentVec.clear();
}

void GridbrainComponentSet::addComponent(GridbrainComponent* component)
{
    mComponentVec.push_back(component);
    mSize++;
}

GridbrainComponent* GridbrainComponentSet::getRandom(SimObj* obj,
                                                        vector<GridbrainComponent>* components,
                                                        map<llULINT, GridbrainMemCell>* memory,
                                                        unsigned int startPos,
                                                        unsigned int endPos)
{
    GridbrainComponent* comp;

    if (mSize <= 0)
    {
        comp = GridbrainComponent::getNullComponent();
    }
    else
    {
        bool found = false;

        while(!found)
        {
            unsigned int pos = mDistComponentSet->iuniform(0, mSize);
            comp = mComponentVec[pos];

            if (comp->mTableLinkType == InterfaceItem::TAB_TO_SYM)
            {
                SymbolTable* table = obj->getSymbolTable(comp->mOrigSymTable);
                comp->mOrigSymID = table->getRandomSymbolId();
                //printf("size: %d\n", table->getSize());
                //printf("rand symID: %d", comp->mOrigSymID);
                //printf(" objID: %d\n", obj->getID());
            }
            else if (comp->isMemory())
            {
                unsigned int pos = mDistComponentSet->iuniform(0, memory->size());

                map<llULINT, GridbrainMemCell>::iterator iterMem = memory->begin();
                for (unsigned int i = 0; i < pos; i++)
                {
                    iterMem++;
                }

                comp->mOrigSymID = (*iterMem).first;
                //printf("size: %d\n", memory->size());
                //printf("id: %d\n", comp->mOrigSymID);
            }

            found = true;

            if (components && (comp->isUnique()))
            {
                for (unsigned int pos = startPos;
                        found && (pos < endPos);
                        pos++)
                {
                    GridbrainComponent* comp2 = &((*components)[pos]);

                    if (comp->isEqual(comp2, false))
                    {
                        found = false;
                    }
                }
            }   
        }
    }

    comp->mParam = mDistComponentSet->uniform(0.0f, 1.0f);

    return comp;
}

void GridbrainComponentSet::addComponent(GridbrainComponent::Type type,
                        int subType,
                        InterfaceItem::TableLinkType linkType,
                        int origSymTable,
                        llULINT origSymID,
                        int targetSymTable)
{
    GridbrainComponent* comp = new GridbrainComponent();
    comp->mType = type;
    comp->mSubType = subType;
    comp->mOrigSymTable = origSymTable;
    comp->mTargetSymTable = targetSymTable;
    comp->mOrigSymID = origSymID;
    comp->mTableLinkType = linkType;
    addComponent(comp);
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

