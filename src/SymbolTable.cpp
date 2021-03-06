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
#include "Random.h"


int SymbolTable::NEXT_SYMBOL_TABLE_ID = 0;


SymbolTable::SymbolTable()
{
}

SymbolTable::SymbolTable(Symbol* refSymbol, int id)
{
    create(refSymbol, id);
}

SymbolTable::SymbolTable(lua_State* luaState)
{
    Symbol* refSymbol = (Symbol*)Orbit<SymbolTable>::pointer(luaState, 1);
    int id = luaL_optinteger(luaState, 2, -1);

    create(refSymbol, id);
}

SymbolTable::SymbolTable(SymbolTable* table)
{
    mReferenceSymbolID = table->mReferenceSymbolID;
    
    mID = table->mID;
    mDynamic = table->mDynamic;
    mName = table->mName;

    map<gbULINT, Symbol*>::iterator iterSymbol;
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
}

SymbolTable::~SymbolTable()
{
    map<gbULINT, Symbol*>::iterator iterSymbol;
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
    mReferenceSymbolID = refSymbol->mID;
    mSymbols[refSymbol->mID] = refSymbol;

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
    mUsedCount = 0;
}

SymbolTable* SymbolTable::recombine(SymbolTable* table2)
{
    SymbolTable* table = new SymbolTable();

    // Scan first parent
    map<gbULINT, Symbol*>::iterator iterSymbol;
    for (iterSymbol = mSymbols.begin();
        iterSymbol != mSymbols.end();
        iterSymbol++)
    {
        Symbol* origSym = (*iterSymbol).second;
        Symbol* sym = selectSymbol(origSym, table2);

        if (sym != NULL)
        {
            if (sym->mAlwaysRandom)
            {
                sym->initRandom();
            }

            table->mSymbols[sym->mID] = sym;
        }
    }

    // Scan second parent
    for (iterSymbol = table2->mSymbols.begin();
        iterSymbol != table2->mSymbols.end();
        iterSymbol++)
    {
        Symbol* origSym = (*iterSymbol).second;

        // Fixed symbols must be the same in both tables
        if (!origSym->mFixed)
        {
            Symbol* sym = table2->selectSymbol(origSym, this);

            if (sym != NULL)
            {
                if (sym->mAlwaysRandom)
                {
                    sym->initRandom();
                }

                table->mSymbols[sym->mID] = sym;
            }
        }
    }

    table->mReferenceSymbolID = mReferenceSymbolID;
    
    table->mID = mID;
    table->mDynamic = mDynamic;
    table->mName = mName;

    return table;
}

Symbol* SymbolTable::selectSymbol(Symbol* sym, SymbolTable* table2)
{
    Symbol* sym1 = sym;
    Symbol* sym2 = table2->getSymbol(sym->mID);

    if (sym2 == NULL)
    {
        if (sym1->mFixed || sym1->mUsed)
        {
            return sym1->clone();
        }
        return NULL;
    }

    if (sym1->mUsed && (!sym2->mUsed))
    {
        return sym1->clone();
    }
    if (sym2->mUsed && (!sym1->mUsed))
    {
        return sym2->clone();
    }
    if ((!sym1->mUsed)
        && (!sym2->mUsed)
        && (!sym1->mFixed))
    {
        return NULL;
    }

    // Prevent these symbols from being selected again
    sym1->mUsed = false;
    sym2->mUsed = false;

    unsigned int select = gRandom.iuniform(0, 2);

    if (select == 0)
    {
        return sym1->clone();
    }
    else
    {
        return sym2->clone();
    }
}

Symbol* SymbolTable::getSymbol(gbULINT id)
{
    for (map<gbULINT, Symbol*>::iterator iterSym = mSymbols.begin();
            iterSym != mSymbols.end();
            iterSym++)
    {
        if ((*iterSym).first == id)
        {
            return (*iterSym).second;
        }
    }

    return NULL;
}

Symbol* SymbolTable::getReferenceSymbol()
{
    return mSymbols[mReferenceSymbolID];
}

gbULINT SymbolTable::addSymbol(Symbol* sym)
{
    gbULINT id = sym->mID;

    if (mSymbols.count(id) == 0)
    {
        mSymbols[id] = sym;
    }
    else
    {
        delete sym;
    }
    return id;
}

gbULINT SymbolTable::addRandomSymbol()
{
    Symbol* sym = mSymbols[mReferenceSymbolID]->clone();
    sym->initRandom();
    sym->mFixed = false;
    sym->generateNewID();
    return addSymbol(sym);
}

gbULINT SymbolTable::getRandomSymbolId()
{
    unsigned int index = gRandom.iuniform(0, mSymbols.size());
    
    map<gbULINT, Symbol*>::iterator iterSymbol = mSymbols.begin();

    for (unsigned int i = 0; i < index; i++)
    {
        iterSymbol++;
    }

    return (*iterSymbol).first;
}

void SymbolTable::grow()
{
    addRandomSymbol(); 
}

void SymbolTable::resetProtections()
{
    map<gbULINT, Symbol*>::iterator iterSymbol;
    for (iterSymbol = mSymbols.begin();
        iterSymbol != mSymbols.end();
        iterSymbol++)
    {
        (*iterSymbol).second->mProtected = false;
    }   
}

void SymbolTable::acquireSymbol(Symbol* sym)
{
    gbULINT id = sym->mID;

    if (mSymbols.count(id) != 0)
    {
        mSymbols[id]->mProtected = true;
        return;
    }

    Symbol* newSym = sym->clone();
    newSym->mProtected = true;
    mSymbols[id] = newSym;
}

int SymbolTable::getSymbolPos(gbULINT symID)
{
    int pos = 0;
    map<gbULINT, Symbol*>::iterator iterSymbol;
    for (iterSymbol = mSymbols.begin();
        iterSymbol != mSymbols.end();
        iterSymbol++)
    {
        if ((*iterSymbol).first == symID)
        {
            return pos;
        }
        pos++;
    }

    return -1;
}

void SymbolTable::printDebug()
{
    printf("Table %d '%s'\n", mID, mName.c_str());

    map<gbULINT, Symbol*>::iterator iterSymbol;
    for (iterSymbol = mSymbols.begin();
        iterSymbol != mSymbols.end();
        iterSymbol++)
    {
        printf("%llu: ", (*iterSymbol).first);
        printf("%s\n", ((*iterSymbol).second)->toString().c_str());
    }
}

int SymbolTable::addSymbol(lua_State* luaState)
{
    Symbol* sym = (Symbol*)Orbit<SymbolTable>::pointer(luaState, 1);
    gbULINT id = addSymbol(sym);
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
    bool dyn = lua_toboolean(luaState, 1);
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

