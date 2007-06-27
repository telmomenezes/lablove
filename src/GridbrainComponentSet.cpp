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

void GridbrainComponentSet::addComponent(GridbrainComponent::Type type,
						int subType,
						float parameter,
						int origChemTable,
						int origMoleculeIndex,
						int targetChemTable,
						int targetMoleculeIndex)
{
	GridbrainComponent* comp = new GridbrainComponent();
	comp->mType = type;
	comp->mSubType = subType;
	comp->mParameter = parameter;
	comp->mOrigChemTable = origChemTable;
	comp->mOrigMoleculeIndex = origMoleculeIndex;
	comp->mTargetChemTable = targetChemTable;
	comp->mTargetMoleculeIndex = targetMoleculeIndex;
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
	int origChemTable = luaL_optint(luaState, 4, -1);
	int origMoleculeIndex = luaL_optint(luaState, 5, -1);
	int targetChemTable = luaL_optint(luaState, 6, -1);
	int targetMoleculeIndex = luaL_optint(luaState, 7, -1);

        addComponent((GridbrainComponent::Type)type,
			subType,
			parameter,
			origChemTable,
			origMoleculeIndex,
			targetChemTable,
			targetMoleculeIndex);
        return 0;
}

