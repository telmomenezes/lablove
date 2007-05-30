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
#include <vector>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "Lunar.h"

using namespace std;

class GridbrainComponentSet
{
public:
	GridbrainComponentSet();
	virtual ~GridbrainComponentSet();

	void add_component(GridbrainComponent* component);
	void add_component(int type, float parameter);
	GridbrainComponent* get_random();

	static const char className[];
        static Lunar<GridbrainComponentSet>::RegType methods[];

        GridbrainComponentSet(lua_State* L);
        int add_component(lua_State* L);

	vector<GridbrainComponent*> _component_vec;
	unsigned int _size;
};
#endif

