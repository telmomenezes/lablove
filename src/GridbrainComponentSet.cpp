/*
 * LOVE Simulation
 * Copyright (C) 2007 Telmo Menezes, all rights reserved.
 * telmo@telmomenezes.com
 */

#include "GridbrainComponentSet.h"

GridbrainComponentSet::GridbrainComponentSet()
{
	_size = 0;
}


GridbrainComponentSet::~GridbrainComponentSet()
{
	for (unsigned int i = 0; i < _size; i++)
	{
		delete _component_vec[i];
	}
	_component_vec.clear();
}

void GridbrainComponentSet::add_component(GridbrainComponent* component)
{
	_component_vec.push_back(component);
	_size++;
}

GridbrainComponent* GridbrainComponentSet::get_random()
{
	return _component_vec[random() % _size];
}

void GridbrainComponentSet::add_component(int type, float parameter)
{
	GridbrainComponent* comp = new GridbrainComponent();
	comp->_type = type;
	comp->_parameter = parameter;
	add_component(comp);
}

const char GridbrainComponentSet::className[] = "GridbrainComponentSet";

Luna<GridbrainComponentSet>::RegType GridbrainComponentSet::methods[] = {
	{"add_component", &GridbrainComponentSet::add_component},
        {0,0}
};

GridbrainComponentSet::GridbrainComponentSet(lua_State* L)
{
	_size = 0;
}

int GridbrainComponentSet::add_component(lua_State *L)
{
	int type = luaL_checkint(L, 1);
        int parameter = luaL_checkint(L, 2);
        add_component(type, (float)parameter);
        return 0;
}

