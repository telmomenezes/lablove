/*
 * LOVE Simulation
 * Copyright (C) 2007 Telmo Menezes, all rights reserved.
 * telmo@telmomenezes.com
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

GridbrainComponent* GridbrainComponentSet::getRandom(SimulationObject* obj,
                                                        GridbrainComponent* components,
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
        bool tab2sym = false;

        while(!found)
        {
            if (!tab2sym)
            {
                unsigned int pos = mDistComponentSet->iuniform(0, mSize);
                comp = mComponentVec[pos];
            }

            if (comp->mTableLinkType == InterfaceItem::TAB_TO_SYM)
            {
                SymbolTable* table = obj->getSymbolTable(comp->mOrigSymTable);

                if (tab2sym)
                {
                    table->grow();
                }
                else
                {
                    tab2sym = true;
                }

                comp->mOrigSymID = table->getRandomSymbolId();
                //printf("size: %d\n", table->getSize());
                //printf("id: %d\n", comp->mOrigSymID);
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
                    GridbrainComponent* comp2 = &components[pos];

                    if (comp->isEqual(comp2, false))
                    {
                        found = false;
                    }
                }
            }   
        }
    }

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

