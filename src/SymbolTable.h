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

#if !defined(__INCLUDE_SYMBOL_TABLE_H)
#define __INCLUDE_SYMBOL_TABLE_H

#include "Symbol.h"
#include "Orbit.h"
#include "RandDistManager.h"
#include "types.h"

#include <map>
#include <string>

using std::map;
using std::string;

class SymbolTable
{
public:
    SymbolTable();
    SymbolTable(lua_State* luaState);
    SymbolTable(Symbol* refSymbol, int id=-1);
    SymbolTable(SymbolTable* table);
    virtual ~SymbolTable();

    SymbolTable* recombine(SymbolTable* table2);

    Symbol* getSymbol(llULINT id);
    llULINT addSymbol(Symbol* sym);
    llULINT addRandomSymbol();
    int getID(){return mID;}
    llULINT getRandomSymbolId();

    map<llULINT, Symbol*>* getSymbolMap(){return &mSymbols;}

    void setDynamic(bool dyn){mDynamic = dyn;}
    bool isDynamic(){return mDynamic;}

    void setName(string name){mName = name;}
    string getName(){return mName;}

    unsigned int getSize(){return mSymbols.size();}

    int addSymbol(lua_State* luaState);
    int getID(lua_State* luaState);
    int setDynamic(lua_State* luaState);
    int setName(lua_State* luaState);

    static const char mClassName[];
    static Orbit<SymbolTable>::MethodType mMethods[];
    static Orbit<SymbolTable>::NumberGlobalType mNumberGlobals[];


protected:
    void create(Symbol* refSymbol, int id=-1);
    Symbol* selectSymbol(Symbol* sym, SymbolTable* table2);

    static int NEXT_SYMBOL_TABLE_ID;
    static mt_distribution* mDistIndex;
    static mt_distribution* mDistRecombine;

    int mID;
    Symbol* mReferenceSymbol;
    map<llULINT, Symbol*> mSymbols;
    bool mDynamic;
    string mName;
    llULINT mNextFixedSymbolID;
};
#endif

