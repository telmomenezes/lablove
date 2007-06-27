/*
 * LOVE Simulation
 * Copyright (C) 2007 Telmo Menezes, all rights reserved.
 * telmo@telmomenezes.com
 */

#include "GridbrainComponentSet.h"

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
	return mComponentVec[random() % mSize];
}

void GridbrainComponentSet::addComponent(GridbrainComponent::Type type, float parameter)
{
	GridbrainComponent* comp = new GridbrainComponent();
	comp->mType = type;
	comp->mParameter = parameter;
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
        int parameter = luaL_checkint(luaState, 2);
        addComponent((GridbrainComponent::Type)type, (float)parameter);
        return 0;
}

