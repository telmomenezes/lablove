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

#include "LoveLab.h"
#include "SimSimple2D.h"
#include "PlantSimple2D.h"
#include "AnimatSimple2D.h"
#include "PopDynGenerations.h"
#include "PopDynFixedSpecies.h"
#include "Grid.h"
#include "GridbrainComponent.h"
#include "GridbrainComponentSet.h"
#include "MoleculeRGB.h"

#include "Orbit.h"
extern "C"
{
#include "lualib.h"
}

int main(int argc, char *argv[])
{
	lua_State *L = lua_open();
	
	luaopen_base(L);
	luaopen_table(L);
	//luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);
	luaopen_debug(L);
	
	Orbit<LoveLab>::orbit_register(L);
	Orbit<SimSimple2D>::orbit_register(L);
	Orbit<PlantSimple2D>::orbit_register(L);
	Orbit<AnimatSimple2D>::orbit_register(L);
	Orbit<PopDynGenerations>::orbit_register(L);
	Orbit<PopDynFixedSpecies>::orbit_register(L);
	Orbit<Grid>::orbit_register(L);
	Orbit<GridbrainComponent>::orbit_register(L);
	Orbit<GridbrainComponentSet>::orbit_register(L);
	Orbit<MoleculeRGB>::orbit_register(L);


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

	LoveLab::get_instance().run();

	lua_close(L);

	return 0;
}

