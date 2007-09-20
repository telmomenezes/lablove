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

#if !defined(__INCLUDE_SYMBOL_TABLE_H)
#define __INCLUDE_SYMBOL_TABLE_H

#include "Symbol.h"
#include "Orbit.h"

#include <vector>

using std::vector;

class SymbolTable
{
public:
    SymbolTable(lua_State* luaState);
    SymbolTable(Symbol* refSymbol);
    SymbolTable(SymbolTable* table);
    virtual ~SymbolTable();

    Symbol* getSymbol(unsigned int index);
    unsigned int addSymbol(Symbol* sym);
    void mutate();

    int addSymbol(lua_State* luaState);

    static const char mClassName[];
    static Orbit<SymbolTable>::MethodType mMethods[];
    static Orbit<SymbolTable>::NumberGlobalType mNumberGlobals[];


protected:
    Symbol* mReferenceSymbol;
    vector<Symbol*> mSymbols;
};
#endif

