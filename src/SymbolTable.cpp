/*
 * LabLOVE
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

#include "SymbolTable.h"
#include "Simulation.h"

int SymbolTable::NEXT_SYMBOL_TABLE_ID = 0;
mt_distribution* SymbolTable::mDistIndex = gDistManager.getNewDistribution();

SymbolTable::SymbolTable(Symbol* refSymbol, int id)
{
    create(refSymbol, id);
}

SymbolTable::SymbolTable(lua_State* luaState)
{
    Symbol* refSymbol = (Symbol*)Orbit<SymbolTable>::pointer(luaState, 1);
    int id = luaL_optint(luaState, 2, -1);

    create(refSymbol, id);
}

SymbolTable::SymbolTable(SymbolTable* table)
{

    map<unsigned long, Symbol*>::iterator iterSymbol;
    for (iterSymbol = table->mSymbols.begin();
        iterSymbol != table->mSymbols.end();
        iterSymbol++)
    {
        Symbol* origSym = (*iterSymbol).second;
        Symbol* sym = origSym->clone();

        if (sym->mAlwaysRandom)
        {
            sym->initRandom();
        }
        mSymbols[(*iterSymbol).first] = sym;
    }

    mReferenceSymbol = mSymbols[0];
    
    mID = table->mID;
    mLastSymbolID = table->mLastSymbolID;
    mDynamic = table->mDynamic;
    mName = table->mName;
}

SymbolTable::~SymbolTable()
{
    map<unsigned long, Symbol*>::iterator iterSymbol;
    for (iterSymbol = mSymbols.begin();
        iterSymbol != mSymbols.end();
        iterSymbol++)
    {
        delete (*iterSymbol).second;
    }

    mSymbols.clear();
}

void SymbolTable::create(Symbol* refSymbol, int id)
{
    mReferenceSymbol = refSymbol;
    mLastSymbolID = 0;
    mSymbols[mLastSymbolID] = mReferenceSymbol;

    if (id < 0)
    {
        mID = NEXT_SYMBOL_TABLE_ID++;
    }
    else
    {
        mID = id;
    }

    mDynamic = false;
    mName = "";
}

Symbol* SymbolTable::getSymbol(unsigned long id)
{
    return mSymbols[id];
}

unsigned long SymbolTable::addSymbol(Symbol* sym)
{
    mLastSymbolID++;
    mSymbols[mLastSymbolID] = sym;
    return mLastSymbolID;
}

unsigned long SymbolTable::addRandomSymbol()
{
    Symbol* sym = mReferenceSymbol->clone();
    sym->initRandom();
    sym->mFixed = false;
    return addSymbol(sym);
}

unsigned long SymbolTable::getRandomSymbolID()
{
    unsigned int index = mDistIndex->iuniform(0, mSymbols.size());
    
    map<unsigned long, Symbol*>::iterator iterSymbol = mSymbols.begin();

    for (unsigned int i = 0; i < index; i++)
    {
        iterSymbol++;
    }

    return (*iterSymbol).first;
}

int SymbolTable::addSymbol(lua_State* luaState)
{
    Symbol* sym = (Symbol*)Orbit<SymbolTable>::pointer(luaState, 1);
    unsigned long id = addSymbol(sym);
    lua_pushnumber(luaState, id);
    return 1;
}

int SymbolTable::getID(lua_State* luaState)
{
    lua_pushnumber(luaState, mID);
    return 1;
}

int SymbolTable::setDynamic(lua_State* luaState)
{
    bool dyn = luaL_checkbool(luaState, 1);
    setDynamic(dyn);
    return 0;
}

int SymbolTable::setName(lua_State* luaState)
{
    string name = luaL_checkstring(luaState, 1);
    setName(name);
    return 0;
}

const char SymbolTable::mClassName[] = "SymbolTable";

Orbit<SymbolTable>::MethodType SymbolTable::mMethods[] = {
    {"addSymbol", &SymbolTable::addSymbol},
    {"getID", &SymbolTable::getID},
    {"setDynamic", &SymbolTable::setDynamic},
    {"setName", &SymbolTable::setName},
    {0,0}
};

Orbit<SymbolTable>::NumberGlobalType SymbolTable::mNumberGlobals[] = {{0,0}};

