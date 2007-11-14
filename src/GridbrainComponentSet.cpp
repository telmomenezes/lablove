/*
 * LOVE Simulation
 * Copyright (C) 2007 Telmo Menezes, all rights reserved.
 * telmo@telmomenezes.com
 */

#include "GridbrainComponentSet.h"

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

GridbrainComponent* GridbrainComponentSet::getRandom()
{
    if (mSize <= 0)
    {
        return GridbrainComponent::getNullComponent();
    }

    unsigned int pos = mDistComponentSet->iuniform(0, mSize);
    return mComponentVec[pos];
}

void GridbrainComponentSet::addComponent(GridbrainComponent::Type type,
                        int subType,
                        float parameter,
                        int symTable,
                        int origSymIndex,
                        int targetSymIndex)
{
    GridbrainComponent* comp = new GridbrainComponent();
    comp->mType = type;
    comp->mSubType = subType;
    comp->mParameter = parameter;
    comp->mSymTable = symTable;
    comp->mOrigSymIndex = origSymIndex;
    comp->mTargetSymIndex = targetSymIndex;
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
    float parameter = luaL_optnumber(luaState, 3, 0.0f);
    int symTable = luaL_optint(luaState, 4, -1);
    int origSymIndex = luaL_optint(luaState, 5, -1);
    int targetSymIndex = luaL_optint(luaState, 6, -1);

    addComponent((GridbrainComponent::Type)type,
                    subType,
                    parameter,
                    symTable,
                    origSymIndex,
                    targetSymIndex);

    return 0;
}

