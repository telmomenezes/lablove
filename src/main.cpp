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

#include "Love.h"
#include "SimSimple2D.h"
#include "PlantSimple2D.h"
#include "AnimatSimple2D.h"
#include "PopDynGenerations.h"
#include "PopDynFixedSpecies.h"
#include "Grid.h"
#include "GridbrainComponent.h"
#include "GridbrainComponentSet.h"
#include "MoleculeRGB.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "luna.h"

int main(int argc, char *argv[])
{
	lua_State *L = lua_open();
	
	luaopen_base(L);
	luaopen_table(L);
	//luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);
	luaopen_debug(L);
	
	Luna<Love>::Register(L);
	Luna<SimSimple2D>::Register(L);
	Luna<PlantSimple2D>::Register(L);
	Luna<AnimatSimple2D>::Register(L);
	Luna<PopDynGenerations>::Register(L);
	Luna<PopDynFixedSpecies>::Register(L);
	Luna<Grid>::Register(L);
	Luna<GridbrainComponentSet>::Register(L);
	Luna<MoleculeRGB>::Register(L);

	GridbrainComponent::init_lua_table(L);

	char* script_file = "default.lua";
	if (argc == 2)
	{
		script_file = argv[1];
	}

	int error = luaL_loadfile(L, script_file) || lua_pcall (L, 0, 0, 0);
	
	if (error)
	{
		printf("%s\n", lua_tostring (L, -1));
		lua_pop (L, 1);
		return -1;
	}

	Love::get_instance().run();

	lua_close(L);

	return 0;
}

