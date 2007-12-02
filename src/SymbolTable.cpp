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

#include "SymbolTable.h"
#include "Simulation.h"

int SymbolTable::NEXT_SYMBOL_TABLE_ID = 0;
mt_distribution* SymbolTable::mDistIndex = gDistManager.getNewDistribution();
mt_distribution* SymbolTable::mDistMutationsProb = gDistManager.getNewDistribution();

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
    mReferenceSymbol = table->mReferenceSymbol->clone();

    vector<Symbol*>::iterator iterSymbol;

    for (iterSymbol = table->mSymbols.begin();
        iterSymbol != table->mSymbols.end();
        iterSymbol++)
    {
        Symbol* sym = (*iterSymbol)->clone();

        if ((*iterSymbol)->mAlwaysRandom)
        {
            sym->initRandom();
        }
        mSymbols.push_back(sym);
    }

    mMutateSymbolProb = table->mMutateSymbolProb;
}

SymbolTable::~SymbolTable()
{
    vector<Symbol*>::iterator iterSymbol;

    for (iterSymbol = mSymbols.begin();
        iterSymbol != mSymbols.end();
        iterSymbol++)
    {
        delete (*iterSymbol);
    }

    mSymbols.clear();
}

void SymbolTable::create(Symbol* refSymbol, int id)
{
    mReferenceSymbol = refSymbol;
    mSymbols.push_back(mReferenceSymbol);

    if (id < 0)
    {
        mID = NEXT_SYMBOL_TABLE_ID++;
    }
    else
    {
        mID = id;
    }

    mMutateSymbolProb = 0.0f;
}

Symbol* SymbolTable::getSymbol(unsigned int index)
{
    return mSymbols[index];
}

unsigned int SymbolTable::addSymbol(Symbol* sym)
{
    unsigned int pos = mSymbols.size();
    mSymbols.push_back(sym);
    return pos;
}

void SymbolTable::mutate()
{
    float nonSelectionProb = 1.0f - mMutateSymbolProb;
    if (nonSelectionProb == 1.0f)
    {
        return;
    }
    float prob = mDistMutationsProb->uniform(0.0f, 1.0f);
    double nextPos = trunc(log(prob) / log(nonSelectionProb));
    
    unsigned int symbolPos = 0;
    unsigned int symbolCount = mSymbols.size();

    while (nextPos < symbolCount) 
    {
        mSymbols[nextPos]->mutate();
        prob = mDistMutationsProb->uniform(0.0f, 1.0f);
        nextPos += trunc(log(prob) / log(nonSelectionProb));
    }
}

int SymbolTable::addSymbol(lua_State* luaState)
{
    Symbol* sym = (Symbol*)Orbit<SymbolTable>::pointer(luaState, 1);
    unsigned int index = addSymbol(sym);
    lua_pushnumber(luaState, index);
    return 1;
}

int SymbolTable::getID(lua_State* luaState)
{
    lua_pushnumber(luaState, mID);
    return 1;
}

int SymbolTable::setMutateSymbolProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateSymbolProb(prob);
    return 0;
}

const char SymbolTable::mClassName[] = "SymbolTable";

Orbit<SymbolTable>::MethodType SymbolTable::mMethods[] = {
    {"addSymbol", &SymbolTable::addSymbol},
    {"getID", &SymbolTable::getID},
    {"setMutateSymbolProb", &SymbolTable::setMutateSymbolProb},
    {0,0}
};

Orbit<SymbolTable>::NumberGlobalType SymbolTable::mNumberGlobals[] = {{0,0}};

