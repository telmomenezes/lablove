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

#include "Lab.h"
#include "SimSimple2D.h"
#include "PlantSimple2D.h"
#include "AnimatSimple2D.h"
#include "PopDynGenerations.h"
#include "PopDynFixedSpecies.h"
#include "Grid.h"
#include "GridbrainComponent.h"
#include "GridbrainComponentSet.h"
#include "SymbolTable.h"
#include "SymbolRGB.h"
#include "StatMedAvgMinMax.h"
#include "StatTime.h"

#if defined(__LABLOVE_WITH_GRAPHICS)
#if defined(__WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "windows.h"
#endif
#endif

extern "C"
{
#include "lualib.h"
}

int getLab(lua_State* luaState)
{
	Lab* obj = &Lab::getSingleton();
	Orbit<Lab>::push(luaState, obj, false);
	return 1;
}

#if defined(__LABLOVE_WITH_GRAPHICS) && defined(__WIN32)
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char *argv[])
#endif
{
	lua_State* luaState = lua_open();
	
	luaopen_base(luaState);
	luaopen_table(luaState);
	//luaopen_io(L);
	luaopen_string(luaState);
	luaopen_math(luaState);
	luaopen_debug(luaState);
	
	Orbit<Lab>::orbitRegister(luaState);
	Orbit<SimSimple2D>::orbitRegister(luaState);
	Orbit<PlantSimple2D>::orbitRegister(luaState);
	Orbit<AnimatSimple2D>::orbitRegister(luaState);
	Orbit<PopDynGenerations>::orbitRegister(luaState);
	Orbit<PopDynFixedSpecies>::orbitRegister(luaState);
	Orbit<Grid>::orbitRegister(luaState);
	Orbit<GridbrainComponent>::orbitRegister(luaState);
	Orbit<GridbrainComponentSet>::orbitRegister(luaState);
	Orbit<SymbolTable>::orbitRegister(luaState);
	Orbit<SymbolRGB>::orbitRegister(luaState);
	Orbit<StatMedAvgMinMax>::orbitRegister(luaState);
	Orbit<StatTime>::orbitRegister(luaState);

	lua_register(luaState, "getLab", getLab);

	char* scriptFile = "default.lua";
	if (argc == 2)
	{
		scriptFile = argv[1];
	}

	int error = luaL_loadfile(luaState, scriptFile) || lua_pcall (luaState, 0, 0, 0);
	
	if (error)
	{
		printf("%s\n", lua_tostring (luaState, -1));
		lua_pop (luaState, 1);
		return -1;
	}

	Lab::getSingleton().run();

	lua_close(luaState);

	return 0;
}

