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

#include "SimCont2D.h"
#include "SimulationObject.h"
#include "GraphicalObject.h"
#include "Agent.h"
#include "PopDynGenerations.h"
#include "PopDynFixedSpecies.h"
#include "PopDynSpeciesBuffers.h"
#include "InterfaceItem.h"
#include "DummyBrain.h"
#include "Gridbrain.h"
#include "Grid.h"
#include "GridbrainComponent.h"
#include "GridbrainComponentSet.h"
#include "SymbolTable.h"
#include "SymbolBitString.h"
#include "SymbolRGB.h"
#include "SymbolFixedString.h"
#include "SymbolFloat.h"
#include "SymbolFloatVector.h"
#include "SymbolUL.h"
#include "LogBestBrain.h"
#include "StatCommon.h"
#include "StatTime.h"
#include "GraphicTriangle.h"
#include "GraphicSquare.h"
#include "GraphicGradient.h"
#include "GraphicBiomorph.h"

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

#include <stdexcept>
#include <string>
#include <map>

using std::string;
using std::map;

map<string, string> gCommandParameters;
string gScriptFile;

int parseCommandLine(int argc, char *argv[])
{
    gScriptFile = "default.lua";
    string paramName = "";

    for (unsigned int i = 1; i < argc; i++)
    {
        string argStr = argv[i];
        if (argStr[0] == '-')
        {
            paramName = argStr.erase(0, 1);
        }
        else
        {
            if (paramName != "")
            {
                gCommandParameters[paramName] = argStr;
                paramName = "";
            }
            else
            {
                gScriptFile = argStr;
            }
        }
    }
}

int getCommandLineParameter(lua_State *luaState)
{
    if (lua_gettop(luaState) != 1)
    {
        lua_pushstring(luaState, "incorrect number of arguments");
        lua_error(luaState);
    }
    
    if (!lua_isstring(luaState, 1))
    {
        lua_pushstring(luaState, "incorrect argument");
        lua_error(luaState);
    }

    string paramName = lua_tostring(luaState, 1);
    string paramValue = "";

    if (gCommandParameters.count(paramName) != 0)
    {
        paramValue = gCommandParameters[paramName];
    }
    

    lua_pushstring(luaState, paramValue.c_str());
    return 1;
}

int main(int argc, char *argv[])
{
    parseCommandLine(argc, argv);
    lua_State* luaState = lua_open();

    // Disable LUA's garbage collector
    lua_gc(luaState, LUA_GCSTOP, 0);    

    luaopen_base(luaState);
    luaopen_table(luaState);
    //luaopen_io(L);
    luaopen_string(luaState);
    luaopen_math(luaState);
    luaopen_debug(luaState);
    
    Orbit<SimCont2D>::orbitRegister(luaState);
    Orbit<SimulationObject>::orbitRegister(luaState);
    Orbit<GraphicalObject>::orbitRegister(luaState);
    Orbit<Agent>::orbitRegister(luaState);
    Orbit<PopDynGenerations>::orbitRegister(luaState);
    Orbit<PopDynFixedSpecies>::orbitRegister(luaState);
    Orbit<PopDynSpeciesBuffers>::orbitRegister(luaState);
    Orbit<InterfaceItem>::orbitRegister(luaState);
    Orbit<DummyBrain>::orbitRegister(luaState);
    Orbit<Gridbrain>::orbitRegister(luaState);
    Orbit<Grid>::orbitRegister(luaState);
    Orbit<GridbrainComponent>::orbitRegister(luaState);
    Orbit<GridbrainComponentSet>::orbitRegister(luaState);
    Orbit<SymbolTable>::orbitRegister(luaState);
    Orbit<SymbolBitString>::orbitRegister(luaState);
    Orbit<SymbolRGB>::orbitRegister(luaState);
    Orbit<SymbolFixedString>::orbitRegister(luaState);
    Orbit<SymbolFloat>::orbitRegister(luaState);
    Orbit<SymbolFloatVector>::orbitRegister(luaState);
    Orbit<SymbolUL>::orbitRegister(luaState);
    Orbit<LogBestBrain>::orbitRegister(luaState);
    Orbit<StatCommon>::orbitRegister(luaState);
    Orbit<StatTime>::orbitRegister(luaState);
    Orbit<GraphicTriangle>::orbitRegister(luaState);
    Orbit<GraphicSquare>::orbitRegister(luaState);
    Orbit<GraphicGradient>::orbitRegister(luaState);
    Orbit<GraphicBiomorph>::orbitRegister(luaState);

    lua_register(luaState, "getCommandLineParameter", getCommandLineParameter);

    int error = luaL_loadfile(luaState, gScriptFile.c_str()) || lua_pcall (luaState, 0, 0, 0);
    
    if (error)
    {
        printf("%s\n", lua_tostring (luaState, -1));
        lua_pop(luaState, 1);
        return -1;
    }

    // TODO: This causes a segmentation fault
    //lua_close(luaState);

    return 0;
}

